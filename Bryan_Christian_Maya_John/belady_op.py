"""
Implementation of Belady's optimal page replacement algorithm.
This implementation was coded by John Medeiros.
"""
def beladyOpPageRep(pages, cacheSize, stepThru = False):
    #Creating set to represent cache, and well as counts for hits and misses
    cache = set()
    hitCount = 0
    missCount = 0
    
    for i in range(len(pages)):
        page = pages[i]
        
        #If page is in cache, no work needs to be done
        if page in cache:
            hitCount += 1

            if (stepThru):
                print("Hit!")
                print("Request Page: " + str(page) +" Cache: " + str(cache))
                input("Press Enter to Continue")
            
        else:
            missCount += 1

            if(stepThru):
                print("Miss!")
                print("Request Page: " + str(page) +" Cache: " + str(cache))
            #If the cache is not full, add the page to cache
            if len(cache) < cacheSize:
                cache.add(page)

                if(stepThru):
                    print("Cache below size, add without removal. New Cache: " + str(cache))
                    input("Press enter to continue")
            else:
                #Full cache -- tricky part: finding furthest used page
                farthestIndex = -1
                pageToRemove = None

                #For each page in the cache, check its next occurrence
                for cachedPage in cache:
                    if cachedPage in pages[i+1:]:
                        nextUse = pages[i+1:].index(cachedPage)
                    else:
                        #If not used again, easily replacable
                        nextUse = float('inf')
                    
                    #If current page being looked at is further than current furthest, replace this instead
                    if nextUse > farthestIndex:
                        farthestIndex = nextUse
                        pageToRemove = cachedPage

                # Replace the farthest-used page with the current page
                cache.remove(pageToRemove)
                cache.add(page)

                if(stepThru):
                    print("Removed Page: " + str(pageToRemove) +" New Cache: " + str(cache))
                    input("Press Enter to Continue")
                
    
 
    print("\nTotals:")
    print(f"Total Requests: {len(pages)}")
    print(f"Cache Hit Count: {hitCount}")
    print(f"Cache Miss Count: {missCount}")

    
    return hitCount

if __name__ == "__main__":
    pages = [7, 6, 0, 0, 4, 6, 4, 4, 4, 1, 3, 5, 8, 1, 5, 3, 8, 3, 9, 7, 6, 0, 2, 2, 2, 3, 2, 0, 8, 8, 8, 1, 7, 8, 8, 1, 9, 0, 7, 0, 3, 3, 7, 4, 3, 1, 3, 0, 0, 4]
    cacheSize = 5
    beladyOpPageRep(pages, cacheSize, False)


