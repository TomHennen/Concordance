
#include <stdio.h>
#include "uthash.h"
#include "state.h"
#include "process.h"
#include "display.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return -1;
    }
    ConcordanceState_t * state = stateCreate();
    if (!state) {
        printf("- Could not create state\n");
        return -2;
    }
    
    int result = processFile(argv[1], state);
    if (result) {
        printf("- Error processing file (%d)\n", result);
        return -3;
    }
    displayState(state);
    
    stateDelete(state);
    printf("+ done\n");
    return 0;
}