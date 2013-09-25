
#include <stdio.h>
#include "uthash.h"
#include "state.h"
#include "process.h"
#include "display.h"

int main(int argc, char* argv[])
{
    int result = 0;
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return -1;
    }
    ConcordanceState_t * state = stateCreate();
    if (!state) {
        printf("- Could not create state\n");
        return -2;
    }
    
    int processResult = processFile(argv[1], state);
    if (processResult) {
        printf("- Error processing file (%d)\n", processResult);
        result = -3;
        goto cleanup;
    }
    displayState(state);
    
cleanup:
    stateDelete(state);
    return 0;
}