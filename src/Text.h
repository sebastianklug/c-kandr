#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdint.h>

uint64_t charCount(char charToCount, FILE *in);
void removeDuplicateConsecutiveChars(char remove, FILE *in, FILE *out);
void wordLengthStats(uint16_t *arr, size_t size, FILE *in);
void allCharCount(uint16_t counts[256], FILE *in);
void tabsToSpaces(size_t noOfSpacesInTab, FILE *in, FILE *out);

#endif