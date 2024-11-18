#Make our list of pages
pages = []
page_str = '76004644413581538397602223208881788190703374313004'
for digit in page_str:
    pages.append(int(digit))

#Create cache set and indexes of pages
cache = set()
indexes_of_pages = {}


cache_size = 5
cache_misses = 0
cache_hits = 0

#Iterate through the pages
for index, page in enumerate(pages):
    print(f"Processing page {page} at index {index}")
    
    if page not in cache:
        print("Page fault occurred.")

        #Page fault!
        
        #Are we at capacity?
        if len(cache) == cache_size:
            
            #Take the min index, remove it
            lru = min(cache, key=lambda x: indexes_of_pages[x])
            print(f"Cache is full. Evicting page {lru} which was least recently used.")
            cache.remove(lru)
            del indexes_of_pages[lru]
        
        #Add the page and increment page faults
        cache.add(page)
        cache_misses += 1
    
    else:
        cache_hits += 1
        print("Cache hit.")
    
    #Update the index of the page
    indexes_of_pages[page] = index
    print(f"Current cache: {cache}")
    print(f"Current page indexes: {indexes_of_pages}")
    print()


#Print results!         
print("Cache:", cache)
print("Page Faults:", cache_misses)
print("Cache Hits:", cache_hits)


    


