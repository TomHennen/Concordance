#pragma once
#include <stddef.h>
typedef struct ConcordanceState {
    char * name;
} ConcordanceState_t;

ConcordanceState_t * stateCreate();
void stateDelete(ConcordanceState_t * state);