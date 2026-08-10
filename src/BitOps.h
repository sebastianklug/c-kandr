#ifndef BITOPS_H
#define BITOPS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int16_t htoi(char s[]);
void squeeze(char s1[], char s2[]);
int16_t any(char s1[], char s2[]);

static int16_t hex_value_of_char(char c);

#endif