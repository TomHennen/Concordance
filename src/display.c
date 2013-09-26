
#include <string.h>
#include <stdio.h>
#include "utlist.h"
#include "display.h"

/**
 Compares a and b and orders them by word
 Used by uthash for sorting
 
 O(W) where W is the length of the shorter word
 
 @param a the first paramter to compare
 @param b the second parameter to compare
 @return -1 if (a < b)
          0 if (a == b)
          1 if (a > b)
 */
int word_sort(ConcordanceEntry_t * a, ConcordanceEntry_t * b)
{
    // the interface doesn't really allow for a good option
    // if either a or b is NULL, so let's cover our eyes
    // and hope the rest of our code is good enough that
    // that doesn't happen
    return strcmp(a->word, b->word);
}

/**
 Displays the words in sorted order, which each word followed
 by a list of the lines it occurs on
 
 O(K * L) where K is the number of unique words in the document
          and L is the number of lines in the document.
 
 @param state the state that contains the mapping
 */
void displayState(ConcordanceState_t * state)
{
    if (!state) {
        printf("- displayState: state is null\n");
        return;
    }
    // the implementation uses MergeSort, so O(K log K)
    // if K is the number of unique words in the document
    HASH_SORT(state->table, word_sort);
    
    // O(K * L)
    for(ConcordanceEntry_t * entry = state->table;
        entry != NULL;
        entry = (ConcordanceEntry_t*)(entry->hh.next)) {
        
        printf("%s", entry->word);
        LineNumberEntry_t * lineEntry;
        // O(L) since worst case, every word appears on every line
        DL_FOREACH(entry->lines,lineEntry) {
            printf(" %d", lineEntry->lineNumber);
        }
        printf("\n");
    }
    
    return;
}