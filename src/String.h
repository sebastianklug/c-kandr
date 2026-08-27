#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void itoa_helper(uint16_t n, char *s_p, size_t *index_p);
static char *last_char_in_string(char *s);

void escape(char s[], const char t[]);
void expand(const char s1[], char s2[]);
int16_t strrindex(char s[], char t[]);
void itoa(int16_t n, char *s_p);
void krstrcat(char *s, char *t);
void krstrcat_p(char *s, char *t);
bool strend(char *s, char *t);
void krstrncpy(char *s, const char *t, size_t n);
void krstrncat(char *s, const char *t, size_t n);
int16_t krstrncmp(const char *s, const char *t, size_t n);

#endif