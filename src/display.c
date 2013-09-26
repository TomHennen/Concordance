
#include <string.h>
#include <stdio.h>
#include "utlist.h"
#include "display.h"

int word_sort(ConcordanceEntry_t * a, ConcordanceEntry_t * b)
{
    // the interface doesn't really allow for a good option
    // if either a or b is NULL, so let's cover our eyes
    // and hope the rest of our code is good enough that
    // that doesn't happen
    return strcmp(a->word, b->word);
}

void displayState(ConcordanceState_t * state)
{
    if (!state) {
        printf("- displayState: state is null\n");
        return;
    }
    HASH_SORT(state->table, word_sort);
    
    for(ConcordanceEntry_t * entry = state->table;
        entry != NULL;
        entry = (ConcordanceEntry_t*)(entry->hh.next)) {
        
        printf("%s", entry->word);
        LineNumberEntry_t * lineEntry;
        DL_FOREACH(entry->lines,lineEntry) {
            printf(" %d", lineEntry->lineNumber);
        }
        printf("\n");
    }
    
    return;
}