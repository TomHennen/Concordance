#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "uthash.h"
#include "process.h"

const unsigned int MAX_LINE_LENGTH = 1024;
// these are the tokens we'll use to split words in a line
const char * TOKENS = " .;:,+\"'!?@#$%^&*()={}[]|~<>\\/-_\n";

/**
 Handles adding the word to the concordance information
 @param word the word to add, this function may modify the buffer
 @param lineNumber the line number the word appeared on
 @param state the map of words to lines
 @return 0 on success, other on error
 */
static int processWord(char * word,
                       unsigned int lineNumber,
                       ConcordanceState_t * state)
{
    ConcordanceEntry_t * wordEntry = NULL;
    char * wordCopy = NULL;

    // we don't care about case, let's get rid of it
    size_t wordLength = strlen(word);
    for (size_t i = 0; i < wordLength; ++i) {
        if (!isprint(word[i])) {
            printf("Could not print 0x%x for %s\n", (int)word[i], word);
            printf("!!!!\n");
            word[i] = '*';
            //return -1;
        }
        word[i] = tolower(word[i]);
    }
    
    // we need to check to see if the word is already present
    // adding it if it isn't
    // then we need to add the line number
    HASH_FIND_STR(state->table, word, wordEntry);
    if (!wordEntry) {
        // no such entry exists yet, add it
        wordEntry = stateAddWord(word, wordLength, state);
        if (!wordEntry) {
            printf("- processWord couldn't add word\n");
            return -1;
        }
    }
    
    // now we have an entry, we just have to add the line number
    int result = stateAddLineNumberToEntry(wordEntry, lineNumber);
    if (result) {
        printf("- processWord could not add line number (%d)\n", result);
        return result;
    }
    return 0;
}

/**
 Replaces any unprintable characters in 'line' with a ' '
 (which winds up making the unprintable characters tokens)
 @param line the line to clean up
 @param lineLength the length of the line
 */
static void preprocessLine(char * line, size_t lineLength)
{
    for (size_t i = 0; i < lineLength; ++i) {
        if (!isprint(line[i])) {
            line[i] = ' ';
        }
    }
}

/**
 Processes the line adding any concordance information to the state.
 
 @param line a null terminated string that may get modified
 @param lineNumber the line number the line appears on
 @param state the concordance state that will be updated
 @return 0 on success, other on error
 */
static int processLine(char * line,
                        unsigned int lineNumber,
                        ConcordanceState_t * state)
{
    // go through each word in the line
    size_t wordEndIndex = 0;
    size_t wordStartIndex = 0;
    char * remainingLine = line;
    size_t lineLength = strlen(line);

    // preprocess the line to get rid of any bad characters
    preprocessLine(line, lineLength);
    
    bool done = false;
    while (!done) {
        // this will give us the index of the first token
        // that occurs within the line, we actually don't
        // care which token it is
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
        if (strlen(word) > 0) {
            // handle the word
            int result = processWord(word, lineNumber, state);
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
 @param filename The file to process
 @param state the concordance state that will be updated
 @return 0 on success, other on error
 */
int processFile(const char * filename,
                                 ConcordanceState_t * state)
{
    FILE * file = fopen(filename, "r");
    if (!file) {
        printf("- Could not open file\n");
        return -1;
    }
    
    // we'll store each line here as we're processing it
    char lineBuffer[MAX_LINE_LENGTH];
    // people usually expect lines to start at 1
    unsigned int lineNumber = 1;
    
    // grab each line in the file,
    // NOTE: we won't detect a line that exceeds the MAX_LINE_LENGTH
    // but we could with some extra work.
    while (fgets(lineBuffer, MAX_LINE_LENGTH, file) != NULL) {
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