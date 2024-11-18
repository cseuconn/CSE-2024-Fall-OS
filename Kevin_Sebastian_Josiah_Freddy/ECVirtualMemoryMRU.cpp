#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "ECVirtualMemoryMRU.h"
using namespace std;


//*****************************************************************************
// Virtual memory: consists of memory pages and a main memory with limited capacity
// Page: represented by an integer; main memory can hold up to K pages
// Page replacement: when the main memory reaches its limit (i.e., having K pages) 
// and a new page (not currently in memory) is to be add, 
// then need to swap out one current page to make room for this new page
// This class: use OPT or MRU algorithm

ECVirtualMemory::ECVirtualMemory(int capacity)
    : max_num_pages(capacity), faults(0), index(0) {
    page_set = set<int>(); //used for quick lookups
    page_list = vector<int>(); //used for moving things around and knowing which page to take out
    page_tracker = vector<int>(); //used to make our own list of all the pages and when they get added
    page_indices = map<int,int>(); //used to keep track of page indices for page replacements
}

// MRU
void ECVirtualMemory::AccessPage(int page) {
    // Track this page in page_tracker for OPT use
    page_tracker.push_back(page);

    // If the page is already in memory, we don't need to do anything
    if (page_set.count(page) == 0) {
        // Page fault occurs
        faults += 1;

        // Check if there's space in memory
        if (page_set.size() < max_num_pages) {
            // If there's space, add the new page
            page_list.push_back(page);
            page_set.insert(page);
            page_indices[page] = index;
            index++;
        } else {
            index = page_indices[prev_page];  // Save index value of MRU page

            // No space, remove the MRU page
            page_set.erase(prev_page);        // Remove it from the set
            page_indices.erase(prev_page);    // Remove MRU page from map

            // Add the new page to memory
            page_set.insert(page);            // Add page to set for fast lookup
            page_indices[page] = index;       // Add page:index entry to map
            page_list[index] = page;          // Call index of MRU page and replace it with current page
        }
    }
    prev_page = page;   // The next iteration will keep track of the MRU page with the variable prev_page
}

int ECVirtualMemory::RunOpt(){
    //based on the .h we are recycling what we used to keep track of pages
    // Caution: this would erase previous run of MRU; you need to re-initialize to run
    // to get proper results
    faults = 0;
    page_set.clear();
    page_list.clear();

    //feeding through the page list we made in access data 
    for (int i = 0; i<page_tracker.size(); i++){
        int page = page_tracker[i];
        if (page_set.count(page) == 0){

            faults += 1;

            //before adding we have to check that we have space
            if(page_set.size() < max_num_pages){    //in the case where there is space
                page_list.insert(page_list.begin(), page);
                page_set.insert(page);//add to the front 
            }
            else{//in the case where there isn't space 
                int page_to_replace = -1;   
                int max = -1; //used to keep track of the farthest page 

                // Check each page in current vector
                for (int j = 0; j < page_list.size(); j++) {
                    int current_page = page_list[j];
                    int next_use = -1;

                    // Find the next use of the page starting at the end + 1 (because if it's full it
                    //means we have all unique ones so no point in starting to look there)
                    for (int k = i + 1; k < page_tracker.size(); k++) {
                        //once we find a match update 
                        if (page_tracker[k] == current_page) {
                            next_use = k;
                            break; // Found the next use
                        }
                    }

                    // If we find one that is never used again then we just switch with that one
                    if (next_use == -1) {
                        page_to_replace = current_page;
                        break; 
                    }
                    //updating the max if we find another further one
                    if (next_use > max) {
                        max = next_use;
                        page_to_replace = current_page;
                    }
                }

                // same as LRU 
                page_set.erase(page_to_replace);
                page_list.erase(find(page_list.begin(), page_list.end(), page_to_replace));

                // Add the new page
                page_list.push_back(page);
                page_set.insert(page);
            }
        }// 2/2 case where the page is in main memory 
        else{
            // Page is already in memory, move it to the front (most recently used)
            //do a for loop to find the other instance and delete it 
            auto it = find(page_list.begin(), page_list.end(), page);
            page_list.erase(it); // Remove the existing page
            page_list.insert(page_list.begin(), page);
        }
        




    }
    return faults;
}

int ECVirtualMemory::GetNumPagesInMainMemory() const{
    return page_set.size();
}

int ECVirtualMemory::GetNumPageFaults() const{
    return faults;
}

