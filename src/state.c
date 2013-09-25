#include <stdlib.h>
#include "state.h"

ConcordanceState_t * stateCreate()
{
    return (ConcordanceState_t*)malloc(sizeof(ConcordanceState_t));
}

void stateDelete(ConcordanceState_t * state)
{
    if (!state) {
        printf("- stateDelete: state is NULL\n");
        return;
    }
    free(state);
}