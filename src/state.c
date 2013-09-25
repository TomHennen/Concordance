#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "utlist.h"
#include "state.h"

/**
 @param entry the entry to add the line number to
 @param lineNumber the line number to add to the entry
 @return 0 on success, other on error
 */
int stateAddLineNumberToEntry(ConcordanceEntry_t * entry, unsigned int lineNumber)
{
    LineNumberEntry_t * lineEntry = malloc(sizeof(LineNumberEntry_t));
    if (!lineEntry) {
        printf("- stateAddLineNumberToEntry could not allocate LineNumberEntry\n");
        return -1;
    }
    memset(lineEntry, 0, sizeof(LineNumberEntry_t));
    lineEntry->lineNumber = lineNumber;
    LL_APPEND(entry->lines, lineEntry);
    return 0;
}

/**
 @param word the word to add
 @param wordLength the length of the word
 @param state the map to add the word to
 @return the entry added for that word in the map
 */
ConcordanceEntry_t * stateAddWord(const char * word, size_t wordLength, ConcordanceState_t * state)
{
    int result = 0;
    
    // first allocate space for our entry
    // both the word and the entry for it
    char * wordCopy = malloc(wordLength + 1);
    if (!wordCopy) {
        printf("- stateAddWord could not allocate word copy\n");
        result = -1;
        goto error;
    }
    strncpy(wordCopy, word, wordLength);
    ConcordanceEntry_t * wordEntry = malloc(sizeof(ConcordanceEntry_t));
    if (!wordEntry) {
        printf("- stateAddWord could not allocate word entry\n");
        result =  -2;
        goto error;
    }
    memset(wordEntry, 0, sizeof(ConcordanceEntry_t));
    
    // now that it's all set up, add it to the table
    wordEntry->word = wordCopy;
    wordEntry->lines = NULL;
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

/**
 @return the state, must be destroyed with stateDelete
 */
ConcordanceState_t * stateCreate()
{
    ConcordanceState_t * state = (ConcordanceState_t*)malloc(sizeof(ConcordanceState_t));
    memset(state, 0, sizeof(ConcordanceState_t));
    return state;
}

/**
 @param state the state to delete
 */
void stateDelete(ConcordanceState_t * state)
{
    if (!state) {
        printf("- stateDelete: state is NULL\n");
        return;
    }
    
    // iterate through all the entries and free them
    ConcordanceEntry_t * tmpEntry = NULL;
    ConcordanceEntry_t * curEntry = NULL;
    LineNumberEntry_t * curLineEntry = NULL;
    LineNumberEntry_t * tmpLineEntry = NULL;
    HASH_ITER(hh, state->table, curEntry, tmpEntry) {
        HASH_DEL(state->table, curEntry);
        
        // now delete the line info
        LL_FOREACH_SAFE(curEntry->lines, curLineEntry, tmpLineEntry) {
            LL_DELETE(curEntry->lines, curLineEntry);
            free(curLineEntry);
        }
        
        // and free the entry
        free(curEntry->word);
        free(curEntry);
    }
    
    free(state);
}