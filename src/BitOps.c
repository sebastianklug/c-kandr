#include "BitOps.h"

int16_t htoi(char s[])
{
    int i = 0;
    bool neg = false;
    uint16_t res = 0;
    if (s[i] == '-')
    {
        neg = true;
        i = 1;
    }
    if (s[i] == '0' && (s[i+1] == 'x' || s[i+1] == 'X'))
    {
        i = i + 2;
    }

    for (; 
         (s[i] >= '0' && s[i] <= '9') ||
         (s[i] >= 'a' && s[i] <= 'f') ||
         (s[i] >= 'A' && s[i] <= 'F');
         ++i)
    {
        res = (res << 4) | hex_value_of_char(s[i]);
    }

    if (neg)
    {
        res = -1 * res;
    }
    return res;
}

void squeeze(char s1[], char s2[])
{
    uint16_t i, j;
    
    for (i = j = 0; s1[i] != '\0'; i++)
    {
        if (strchr(s2, s1[i]) == NULL)
        {
            s1[j] = s1[i];
            j++;
        }
    }
    s1[j] = '\0';
}

int16_t any(char s1[], char s2[])
{
    for (int i=0; s1[i] != '\0'; i++)
    {
        if (strchr(s2, s1[i]) != NULL)
        {
            return i;
        }
    }
    return -1;
}

uint16_t setbits(uint16_t x, int16_t p, int16_t n, uint16_t y)
{
    int16_t insertPos = p-n+1;
    uint16_t mask = (y & ~(~0 << n));
    if (insertPos > 0)
    {
        mask = mask << insertPos;
    }
    else
    {
        mask = mask >> -(insertPos);
    }
    
    return x | mask;
}

uint16_t invert(uint16_t x, int16_t p, int16_t n)
{
    int16_t insertPos = p-n+1;
    uint16_t mask = ~(~0 << n);
    if (insertPos > 0)
    {
        mask = mask << insertPos;
    }
    else
    {
        mask = mask >> -(insertPos);
    }
    printf("mask: %b\n", mask);
    
    return x ^ mask;
}

uint16_t rightrot(uint16_t x, int16_t n)
{
    if ((n % BIT_WIDTH_UINT16) == 0)
    {
        // Special handling of no rotation
        return x;
    }
    n = n % BIT_WIDTH_UINT16;
    uint16_t rightBits = x << (BIT_WIDTH_UINT16 - n);
    
    x = x >> n;
    return x | rightBits;
}

/************************************************/

static int16_t hex_value_of_char(char c) 
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
}