
#include <stdio.h>
#include "uthash.h"
#include "state.h"
#include "process.h"
#include "display.h"

/**
 Runs concordance
 
 Time Complexity
 O(P * L + 2(K * L) + K * log K)
 O(P * L + 3K(L + log K))
 O(P * L + K(L + log K))
 
 Space complexity
 O(K * L * W)
    Our only allocated space is in the ConcordanceState_t structure
    The rest is stack. We don't have any recursion, so that's a finite
    size.
 
 where:
     P is the average length of a line
     L is the number of lines
     K is the number of unique words
     W is the average length of a word
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