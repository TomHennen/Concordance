
#include <string.h>
#include <stdio.h>
#include "utlist.h"
#include "display.h"

int word_sort(ConcordanceEntry_t * a, ConcordanceEntry_t * b)
{
    return strcmp(a->word, b->word);
}

void displayState(ConcordanceState_t * state)
{
    HASH_SORT(state->table, word_sort);
    
    for(ConcordanceEntry_t * entry = state->table;
        entry != NULL;
        entry = (ConcordanceEntry_t*)(entry->hh.next)) {
        
        printf("%s", entry->word);
        LineNumberEntry_t * lineEntry;
        LL_FOREACH(entry->lines,lineEntry) {
            for (size_t lineNumIndex = 0;
                 lineNumIndex < lineEntry->numberOfLines;
                 ++lineNumIndex) {
                printf(" %d", lineEntry->lineNumbers[lineNumIndex]);
            }
        }
        printf("\n");
    }
    
    return;
}