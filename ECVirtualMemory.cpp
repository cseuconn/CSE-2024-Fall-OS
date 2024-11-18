//
//  ECVirtualMemory.cpp
//  
//
//  Created by Yufeng Wu on 8/30/23.
//  Implement popular page replacement algorithms

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "ECVirtualMemory.h"
using namespace std;


//*****************************************************************************
// Virtual memory: consists of memory pages and a main memory with limited capacity
// Page: represented by an integer; main memory can hold up to K pages
// Page replacement: when the main memory reaches its limit (i.e., having K pages) 
// and a new page (not currently in memory) is to be add, 
// then need to swap out one current page to make room for this new page
// This class: use OPT or LRU algorithm




//  idea behind the code    //


//idea is to use a set to get those fast look ups
//vector to easily move things around 
//get the best of both fast look ups and actually be able to move things around for later use 

//so imagining a list we just check to see if a certain id is in it
//if it is not then page fault

//idea for LRU is using a vector to move any pages that are used to the front
//after we reach the capacity lets say 10 pages remove the one at the end 
//and add the new one to the front



ECVirtualMemory::ECVirtualMemory(int capacity)
    : max_num_pages(capacity), faults(0) {
    page_set = set<int>(); //used for quick lookups
    page_list = vector<int>(); //used for moving things around and knowing which page to take out
    page_tracker = vector<int>(); //used to make our own list of all the pages and when they get added
}

void ECVirtualMemory::AccessPage(int page){

    // 1/2 cases either the page is not in main memory 
    page_tracker.push_back(page);   //adding to our own list to use later 
    if (page_set.count(page) == 0){

        //add to the front since there are no instances of the page 
        faults += 1;

        //before adding we have to check that we have space
        if(page_set.size() < max_num_pages){                //in the case where there is space
            page_list.insert(page_list.begin(), page);      //add to the front 
            page_set.insert(page);
        }
        else{                                               //in the case where there isn't space 

            int lru_page = page_list.back(); //get the actual value so we can erase from the set
            page_list.pop_back();            //take it out of the vector 
            page_set.erase(lru_page);        //erasing from the set using the value we got earlier 

            //adding it into our vector and set 
            page_list.insert(page_list.begin(), page);
            page_set.insert(page);
        }

    }// 2/2 case where the page is in main memory 
    else{
        // Page is already in memory, move it to the front (most recently used)
        // do a for loop to find the other instance and delete it 
        auto it = find(page_list.begin(), page_list.end(), page);
        //checking that we didn't go to the end of the vector meaning we didn't find it 
        page_list.erase(it); // Remove the existing page
        page_list.insert(page_list.begin(), page);
        
    }

}

//from Piazza 
//"You should keep track what pages are to be accessed. 
//Then use the OPT algorithm to find the page replacement."

//should be similar to LRU only change should just be the part when we get a full vector that's 
//when we do different things which in turn results in different faults. 

int ECVirtualMemory::RunOpt(){
    //based on the .h we are recycling what we used to keep track of pages
    // Caution: this would erase previous run of LRU; you need to re-initialize to run
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

