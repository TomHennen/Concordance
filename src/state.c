#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "state.h"

ConcordanceEntry_t * stateAddWord(const char * word, size_t wordLength, ConcordanceState_t * state)
{
    int result = 0;
    
    // first allocate space for our entry
    // both the word and the entry for it
    char * wordCopy = malloc(wordLength);
    if (!wordCopy) {
        printf("- processWord could not allocate word copy\n");
        result = -1;
        goto error;
    }
    strncpy(wordCopy, word, wordLength);
    ConcordanceEntry_t * wordEntry = malloc(sizeof(ConcordanceEntry_t));
    if (!wordEntry) {
        printf("- processWord could not allocate word entry\n");
        result =  -2;
        goto error;
    }
    
    // now that it's all set up, add it to the table
    wordEntry->word = wordCopy;
    wordEntry->placeholder = 1;
    HASH_ADD_KEYPTR(hh, state->table, wordCopy, wordLength, wordEntry);

    return wordEntry;
    
    // used in an error case to make sure we clean everything up
error:
    if (wordEntry) {
        free(wordEntry);
    }
    if (wordCopy) {
        free(wordCopy);
    }
    return NULL;
}

ConcordanceState_t * stateCreate()
{
    ConcordanceState_t * state = (ConcordanceState_t*)malloc(sizeof(ConcordanceState_t));
    memset(state, 0, sizeof(state));
    return state;
}

void stateDelete(ConcordanceState_t * state)
{
    if (!state) {
        printf("- stateDelete: state is NULL\n");
        return;
    }
    free(state);
}