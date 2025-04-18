#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "uthash.h"
#include "process.h"

// the maximum number of characters we expect to be in a line
const unsigned int MAX_LINE_LENGTH = 1024;
// these are the tokens we'll use to split words in a line
// don't include "'" since it could be a contraction
const char * TOKENS = " .;:,+\"`!?@#$%^&*()={}[]|~<>\\/-_\n";

/**
 Handles adding the word to the concordance information
 
 O(W) where W is the length of the word
 
 @param word the word to add, this function may modify the buffer
 @param wordLength the number of characters in word
 @param lineNumber the line number the word appeared on
 @param state the map of words to lines
 @return 0 on success, other on error
 */
static int processWord(char * word,
                       size_t wordLength,
                       unsigned int lineNumber,
                       ConcordanceState_t * state)
{
    if (!word || !state) {
        printf("- processWord: null word or state\n");
        return -2;
    }
    
    ConcordanceEntry_t * wordEntry = NULL;
    char * wordCopy = NULL;

    // we don't care about case, let's get rid of it
    // O(W)
    for (size_t i = 0; i < wordLength; ++i) {
        word[i] = tolower(word[i]);
    }
    
    // we need to check to see if the word is already present
    // adding it if it isn't
    // then we need to add the line number
    // this should be O(1), though really O(W) since it needs to strcmp the word
    HASH_FIND_STR(state->table, word, wordEntry);
    if (!wordEntry) {
        // no such entry exists yet, add it
        // O(W)
        wordEntry = stateAddWord(word, wordLength, state);
        if (!wordEntry) {
            printf("- processWord: couldn't add word\n");
            return -1;
        }
    }
    
    // now we have an entry, we just have to add the line number
    // O(1)
    int result = stateAddLineNumberToEntry(wordEntry, lineNumber);
    if (result) {
        printf("- processWord: could not add line number (%d)\n", result);
        return result;
    }
    return 0;
}

/**
 Replaces any unprintable characters in 'line' with a ' '
 
 O(P) where P is the length of the line (capped at 1024 here,
      so could say O(1), but if we went to unlimited lines...)
 
 (which winds up making the unprintable characters tokens)
 @param line the line to clean up
 */
static void preprocessLine(char * line)
{
    if (!line) {
        printf("- preprocessLine: line is NULL\n");
        return;
    }
    
    // O(L)
    for (size_t i = 0; line[i] != '\0'; ++i) {
        // O(1)
        if (!isprint(line[i])) {
            line[i] = ' ';
        }
    }
}

/**
 Processes the line adding any concordance information to the state.
 
 O(P) where P is the length of the line
 
 @param line a null terminated string that may get modified
 @param lineNumber the line number the line appears on
 @param state the concordance state that will be updated
 @return 0 on success, other on error
 */
static int processLine(char * line,
                       unsigned int lineNumber,
                       ConcordanceState_t * state)
{
    if (!line || !state) {
        printf("- processLine: line or state is null\n");
        return -1;
    }
    
    // the index at which the current word ends
    size_t wordEndIndex = 0;
    // the index at which the current word starts
    size_t wordStartIndex = 0;
    // a pointer to the what has yet to be processed in the line
    char * remainingLine = line;

    // preprocess the line to get rid of any bad characters
    // O(L)
    preprocessLine(line);

    // go through each word in the line
    // O(L) since we have to look at each character
    bool done = false;
    while (!done) {
        // this will give us the index of the first token
        // that occurs within the line, we actually don't
        // care which token it is
        //
        // The O cost here can be amortized away, it takes
        // time, but it reduces the number of times through
        // the loop.  So it's really O(T), but the number
        // of tokens is constant, so we can ignore it.
        wordEndIndex = strcspn(remainingLine, TOKENS);
        
        if (remainingLine[wordEndIndex] == '\0') {
            // didn't find any tokens
            // so we're done
            done = true;
        }
        
        // we're going to add a null terminator at the location
        // of the token so that we can use the bit ahead of it
        // as the word without allocating anything
        // This is OK if we've hit the end, we'll just be
        // overwriting an existing null terminator
        remainingLine[wordEndIndex] = '\0';
        char * word = &remainingLine[wordStartIndex];
        
        // if the word is zero length that just means
        // we probably had two tokens next to each other
        size_t wordLength = wordEndIndex - wordStartIndex;
        assert(strlen(word) == wordLength);
        if (wordLength > 0) {
            // handle the word
            // O(W), which is really just a part of O(L) so again, we can ignore it as
            // we're already counting O(L)
            int result = processWord(word, wordLength, lineNumber, state);
            if (result) {
                printf("- processLine could not process word [%s] on line %d\n", word, lineNumber);
                return result;
            }
        }
        
        if (!done) {
            // we don't want to do this if we're one since we'll
            // access memory that isn't ours (potentially)
            
            // this is now the rest of the line after we performed surgery above
            remainingLine = &remainingLine[wordEndIndex + 1];
        }
    }
    return 0;
}

/**
 Processes the file and returns a mapping of words to the lines
 each word appears on.
 
 The file is assumed to be ASCII and have lines no longer than
 MAX_LINE_LENGTH
 
 O(L * P) where L is the number of lines and P is the average line length
 
 @param filename The file to process
 @param state the concordance state that will be updated
 @return 0 on success, other on error
 */
int processFile(const char * filename,
                ConcordanceState_t * state)
{
    if (!filename || !state) {
        printf("- processFile: filename or state is null\n");
        return -3;
    }
    
    FILE * file = fopen(filename, "r");
    if (!file) {
        printf("- processFile: Could not open file\n");
        return -1;
    }
    
    // we'll store each line here as we're processing it
    char lineBuffer[MAX_LINE_LENGTH];
    // people usually expect lines to start at 1
    unsigned int lineNumber = 1;
    
    // grab each line in the file,
    // NOTE: we won't detect a line that exceeds the MAX_LINE_LENGTH
    // but we could with some extra work.
    // O(L) where L is the number of lines
    while (fgets(lineBuffer, MAX_LINE_LENGTH, file) != NULL) {
        // O(P)
        int processResult = processLine(lineBuffer, lineNumber, state);
        if (processResult) {
            printf("- processFile: error processing line (%d)\n", processResult);
            return -2;
        }
        ++lineNumber;
    }
    
    fclose(file);
    
    return 0;
}