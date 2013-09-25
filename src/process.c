#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "process.h"

const unsigned int MAX_LINE_LENGTH = 1024;
// these are the tokens we'll use to split words in a line
const char * TOKENS = " .;:,+\"'!?@#$%^&*()={}|~<>\\/-";

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
    printf("+ processLine line [%4d]: %s\n", lineNumber, line);
    
    // go through each word in the line
    size_t wordEndIndex = 0;
    size_t wordStartIndex = 0;
    char * remainingLine = line;
    size_t lineLength = strlen(line);

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
        printf("+ processLine word: %s\n", word);

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
    printf("+ Processing %s\n", filename);
    
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