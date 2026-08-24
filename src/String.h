#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void itoa_helper(uint16_t n, char *s_p, size_t *index_p);

void escape(char s[], const char t[]);
void expand(const char s1[], char s2[]);
int16_t strrindex(char s[], char t[]);
void itoa(int16_t n, char *s_p);
void krstrcat(char *s, char *t);
void krstrcat_p(char *s, char *t);

#endif