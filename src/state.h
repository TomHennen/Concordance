#pragma once
#include <stddef.h>
#include "uthash.h"

/**
 Used to represent one line number that a word appears on.
 Meant to be used in a utlist.
 */
typedef struct LineNumberEntry {
    /** the line number itself */
    unsigned int lineNumber;
    /** points to the next number in the list */
    struct LineNumberEntry * next;
    /** points to the previous number in the list */
    struct LineNumberEntry * prev;
} LineNumberEntry_t;

/**
 Represents one word in our mapping.
 Meant to be used in a uthash.
 */
typedef struct ConcordanceEntry {
    /** A pointer to the word this entry represents, must be free'd */
    char * word;
    /** 
     A pointer to the list of lines this entry appears on,
     must be iterated on and deleted 
     */
    LineNumberEntry_t * lines;
    /** Used by uthash to manage our hash table */
    UT_hash_handle hh;
} ConcordanceEntry_t;

/**
 Wrapping data structure that manages our state.
 Space complexity should be:
 O(K * L * W)
 Where
     L is the number of lines
     K is the number of unique words
     W is the average length of a word
 */
typedef struct ConcordanceState {
    /** The base of our hash table */
    ConcordanceEntry_t * table;
} ConcordanceState_t;

ConcordanceState_t * stateCreate();
void stateDelete(ConcordanceState_t * state);
ConcordanceEntry_t * stateAddWord(const char * word, size_t wordLength, ConcordanceState_t * state);
int stateAddLineNumberToEntry(ConcordanceEntry_t * entry, unsigned int lineNumber);

