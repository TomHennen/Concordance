
#include <stdio.h>
#include "uthash.h"
#include "state.h"
#include "process.h"
#include "display.h"

/**
 Runs concordance
 
 O(P * L + 2(K * L) + K * log K)
 where:
        P is the average length of a line
        L is the number of lines
        K is the number of unique words
 */
int main(int argc, char* argv[])
{
    int result = 0;
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return -1;
    }
    
    // O(1)
    ConcordanceState_t * state = stateCreate();
    if (!state) {
        printf("- Could not create state\n");
        return -2;
    }
    
    // O(P * L)
    int processResult = processFile(argv[1], state);
    if (processResult) {
        printf("- Error processing file (%d)\n", processResult);
        result = -3;
        goto cleanup;
    }
    // O(K * (L + log K)
    displayState(state);
    
cleanup:
    // O(K * L)
    // though we could technically just exit and let the OS deal with this
    // it'll free the memory for us
    stateDelete(state);
    return 0;
}