#include "Text.h"

uint64_t charCount(char charToCount, FILE *in)
{
    char c;
    long count = 0;
    while((c = fgetc(in)) != EOF)
    {
        if(c == charToCount)
        {
            ++count;
        }
    }
    return count;
}

void removeDuplicateConsecutiveChars(char remove, FILE *in, FILE *out)
{
    char prev;
    char c;

    while((c = fgetc(in)) != EOF)
    {
        if(!((prev == remove) && (c == remove)))
        {
            fputc(c, out);
        }
        prev = c;
    }
}
