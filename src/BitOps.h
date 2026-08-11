#ifndef BITOPS_H
#define BITOPS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int16_t htoi(char s[]);
void squeeze(char s1[], char s2[]);
int16_t any(char s1[], char s2[]);
uint16_t setbits(uint16_t x, int16_t p, int16_t n, uint16_t y);
uint16_t invert(uint16_t x, int16_t p, int16_t n);

static int16_t hex_value_of_char(char c);

#endif