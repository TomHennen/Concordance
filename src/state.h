#pragma once
#include <stddef.h>
#include "uthash.h"

typedef struct ConcordanceEntry {
    char * word;
    int placeholder;
    UT_hash_handle hh;
} ConcordanceEntry_t;

typedef struct ConcordanceState {
    ConcordanceEntry_t * table;
} ConcordanceState_t;

ConcordanceState_t * stateCreate();
void stateDelete(ConcordanceState_t * state);
ConcordanceEntry_t * stateAddWord(const char * word, size_t wordLength, ConcordanceState_t * state);