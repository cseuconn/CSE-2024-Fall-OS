from collections import deque

#Initialize cache & parameters
cache_size = 5
cache = deque()   # FIFO queue for cache
pages_string = "76004644413581538397602223208881788190703374313004"
pages = [int(digit) for digit in pages_string]
page_faults = 0 
hits = 0

for page in pages:
    if page not in cache: 
        page_faults += 1 #Page fault occurs when page is not in cache
        if len(cache) >= cache_size:
            cache.popleft()  #Remove the oldest page 
        cache.append(page)  #Adds new page to the cache
        print(f"Page {page} added, causing a page fault.")
        print("Current cache:", list(cache))
    else: #Page already in cache -- No fault 
        hits+=1 
        print(f"Page {page} accessed, no page fault.") 
        print("Current cache:", list(cache))
print("Total page faults:", page_faults)
print("Total hits:", hits)
print("Frames:", cache_size)
    



