#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "utlist.h"
#include "state.h"

/**
 Add the line number to the provided entry
 
 O(1) (assuming malloc is O(1) also)
 
 @param entry the entry to add the line number to
 @param lineNumber the line number to add to the entry
 @return 0 on success, other on error
 */
int stateAddLineNumberToEntry(ConcordanceEntry_t * entry, unsigned int lineNumber)
{
    if (!entry) {
        printf("- stateAddLineNumberToEntry: entry is null\n");
        return -2;
    }
    LineNumberEntry_t * lineEntry = malloc(sizeof(LineNumberEntry_t));
    if (!lineEntry) {
        printf("- stateAddLineNumberToEntry could not allocate LineNumberEntry\n");
        return -1;
    }
    memset(lineEntry, 0, sizeof(LineNumberEntry_t));
    lineEntry->lineNumber = lineNumber;
    DL_APPEND(entry->lines, lineEntry);
    return 0;
}

/**
 Adds the specified word to the state, returning it's entry.
 
 O(W) (where W is the length of the word)
 
 @param word the word to add
 @param wordLength the length of the word
 @param state the map to add the word to
 @return the entry added for that word in the map
 */
ConcordanceEntry_t * stateAddWord(const char * word, size_t wordLength, ConcordanceState_t * state)
{
    if (!word || !state) {
        printf("- stateAddLineNumberToEntry: entry is null\n");
        return NULL;
    }
    
    // first allocate space for our entry
    // both the word and the entry for it
    char * wordCopy = malloc(wordLength + 1);
    if (!wordCopy) {
        printf("- stateAddWord could not allocate word copy\n");
        goto error;
    }
    strncpy(wordCopy, word, wordLength);
    
    ConcordanceEntry_t * wordEntry = malloc(sizeof(ConcordanceEntry_t));
    if (!wordEntry) {
        printf("- stateAddWord could not allocate word entry\n");
        goto error;
    }
    memset(wordEntry, 0, sizeof(ConcordanceEntry_t));
    
    // now that it's all set up, add it to the table
    wordEntry->word = wordCopy;
    wordEntry->lines = NULL;
    
    // this should be O(1)
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
 Creates the state we're going to store all our information in.
 
 O(1) assuming malloc is O(1)
 
 @return the state, must be destroyed with stateDelete
 */
ConcordanceState_t * stateCreate()
{
    ConcordanceState_t * state = (ConcordanceState_t*)malloc(sizeof(ConcordanceState_t));
    if (!state) {
        printf("- stateCreate: could not allocate state\n");
        return NULL;
    }
    
    memset(state, 0, sizeof(ConcordanceState_t));
    return state;
}

/**
 Deletes the provided state, freeing anything it points to
 
 O(K * L) (where K is the number of unique words and L is the number of lines)
 
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
        // O(L) since worst case each word will appear on each line
        DL_FOREACH_SAFE(curEntry->lines, curLineEntry, tmpLineEntry) {
            DL_DELETE(curEntry->lines, curLineEntry);
            free(curLineEntry);
        }
        
        // and free the entry
        free(curEntry->word);
        free(curEntry);
    }
    
    free(state);
}