#pragma once
#include <stddef.h>
#include "uthash.h"

static const size_t DEFAULT_NUMBER_OF_LINES = 10;

typedef struct LineNumberEntry {
    unsigned int lineNumbers[DEFAULT_NUMBER_OF_LINES];
    size_t numberOfLines;
    struct LineNumberEntry * next;
} LineNumberEntry_t;

typedef struct ConcordanceEntry {
    char * word;
    LineNumberEntry_t * lines;
    UT_hash_handle hh;
} ConcordanceEntry_t;

typedef struct ConcordanceState {
    ConcordanceEntry_t * table;
} ConcordanceState_t;

ConcordanceState_t * stateCreate();
void stateDelete(ConcordanceState_t * state);
ConcordanceEntry_t * stateAddWord(const char * word, size_t wordLength, ConcordanceState_t * state);
int stateAddLineNumberToEntry(ConcordanceEntry_t * entry, unsigned int lineNumber);

