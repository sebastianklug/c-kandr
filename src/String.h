#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

void escape(char s[], const char t[]);
void expand(const char s1[], char s2[]);
int16_t strrindex(char s[], char t[]);

#endif