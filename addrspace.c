#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <vm.h>
#include <synch.h>
#include <mips/tlb.h>
#include <spl.h>
#include <proc.h>

/*
 * Note! If OPT_DUMBVM is set, as is the case until you start the VM
 * assignment, this file is not compiled or linked or in any way
 * used. The cheesy hack versions in dumbvm.c are used instead.
 */

struct addrspace *
as_create(void)
{
    struct addrspace *new_space = kmalloc(sizeof(struct addrspace));
    if (new_space == NULL) {
        return NULL;
    }

    new_space->page_table_base = NULL;
    new_space->region_base = NULL;
    new_space->heap_start = 0;
    new_space->heap_end = 0;

    return new_space;
}

int
as_copy(struct addrspace *old, struct addrspace **ret)
{
    struct addrspace *copy_space = as_create();
    if (copy_space == NULL) {
        return ENOMEM;
    }

    struct page_table_entry *source_pte = old->page_table_base;
    struct page_table_entry *copy_pte = NULL;

    struct region *source_region = old->region_base;
    struct region *copy_region = NULL;

    while (source_pte != NULL) {
        if (copy_space->page_table_base == NULL) {
            copy_pte = kmalloc(sizeof(struct page_table_entry));
            if (copy_pte == NULL) {
                return ENOMEM;
            }
            copy_space->page_table_base = copy_pte;
        } else {
            copy_pte->next = kmalloc(sizeof(struct page_table_entry));
            if (copy_pte->next == NULL) {
                return ENOMEM;
            }
            copy_pte = copy_pte->next;
        }

        copy_pte->lock = lock_create("pte_lock");
        if (copy_pte->lock == NULL) {
            kfree(copy_pte);
            return ENOMEM;
        }

        lock_acquire(copy_pte->lock);
        copy_pte->virtual_page = source_pte->virtual_page;
        copy_pte->permissions = source_pte->permissions;
        copy_pte->state = UNMAPPED;
        copy_pte->next = NULL;

        paddr_t new_physical_page = getuserpage(1, copy_space, copy_pte->virtual_page, true);
        if (new_physical_page == 0) {
            return ENOMEM;
        }

        lock_acquire(source_pte->lock);
        if (source_pte->state == SWAPPED) {
            if (diskblock_read(new_physical_page, source_pte->diskpage_location, false)) {
                panic("Cannot read from disk to memory");
            }
        } else {
            memmove((void *)PADDR_TO_KVADDR(new_physical_page),
                    (const void *)PADDR_TO_KVADDR(source_pte->physical_page),
                    PAGE_SIZE);
        }
        lock_release(source_pte->lock);

        copy_pte->state = MAPPED;
        copy_pte->physical_page = new_physical_page;
        lock_release(copy_pte->lock);
        source_pte = source_pte->next;
    }

    while (source_region != NULL) {
        if (copy_space->region_base == NULL) {
            copy_region = kmalloc(sizeof(struct region));
            if (copy_region == NULL) {
                return ENOMEM;
            }
            copy_space->region_base = copy_region;
        } else {
            copy_region->next = kmalloc(sizeof(struct region));
            if (copy_region->next == NULL) {
                return ENOMEM;
            }
            copy_region = copy_region->next;
        }

        copy_region->start = source_region->start;
        copy_region->size = source_region->size;
        copy_region->npages = source_region->npages;
        copy_region->read = source_region->read;
        copy_region->write = source_region->write;
        copy_region->execute = source_region->execute;
        copy_region->next = NULL;

        source_region = source_region->next;
    }

    copy_space->heap_start = old->heap_start;
    copy_space->heap_end = old->heap_end;

    *ret = copy_space;
    return 0;
}

void
as_destroy(struct addras *as)
{
    KASSERT(as != NULL);

    struct page_table_entry *pte_ptr = as->page_table_base;
    struct region *region_ptr = as->region_base;

    while (pte_ptr != NULL) {
        struct page_table_entry *temp_pte = pte_ptr;
        pte_ptr = pte_ptr->next;

        lock_acquire(temp_pte->lock);
        if (temp_pte->state == SWAPPED) {
            bitmap_unmark_wrapper(temp_pte->diskpage_location);
        } else {
            free_ppages(temp_pte->physical_page);
        }
        lock_release(temp_pte->lock);
        lock_destroy(temp_pte->lock);

        kfree(temp_pte);
    }

    while (region_ptr != NULL) {
        struct region *temp_region = region_ptr;
        region_ptr = region_ptr->next;
        kfree(temp_region);
    }

    kfree(as);
}

void
as_activate(void)
{
    int i, spl_level;
    struct addrspace *curr_space = proc_getas();

    if (curr_space == NULL) {
        return;
    }

    spl_level = splhigh();
    for (i = 0; i < NUM_TLB; i++) {
        tlb_write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
    }
    splx(spl_level);
}

int
as_define_region(struct addrspace *as, vaddr_t vaddr, size_t memsize,
                 int readable, int writeable, int executable)
{
    KASSERT(as != NULL);

    memsize += vaddr & ~(vaddr_t)PAGE_FRAME;
    vaddr &= PAGE_FRAME;
    memsize = (memsize + PAGE_SIZE - 1) & PAGE_FRAME;

    struct region *curr_region = as->region_base;
    while (curr_region && curr_region->next) {
        curr_region = curr_region->next;
    }

    struct region *new_region = kmalloc(sizeof(struct region));
    if (new_region == NULL) {
        return ENOMEM;
    }

    new_region->start = vaddr;
    new_region->size = memsize;
    new_region->npages = memsize / PAGE_SIZE;
    new_region->readable = (readable == 4);
    new_region->writeable = (writeable == 2);
    new_region->execute = (executable == 1);
    new_region->next = NULL;

    if (curr_region) {
        curr_region->next = new_region;
    } else {
        as->region_base = new_region;
    }

    return 0;
}

int
as_prepare_load(struct addrspace *as)
{
    vaddr_t heap_base = 0;
    struct region *region_iter = as->region_base;

    while (region_iter != NULL) {
        vaddr_t region_end = region_iter->start + region_iter->size;
        if (region_end > heap_base) {
            heap_base = region_end;
        }
        region_iter = region_iter->next;
    }

    KASSERT(heap_base % 4 == 0);
    as->heap_start = heap_base;
    as->heap_end = heap_base;
    return 0;
}

int
as_complete_load(struct addrspace *as)
{
    (void)as;
    return 0;
}

int
as_define_stack(struct addrspace *as, vaddr_t *stackptr)
{
    (void)as;
    *stackptr = USERSTACK;
    return 0;
}
