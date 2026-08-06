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

void wordLengthStats(uint16_t *arr, size_t size, FILE *in)
{
    int len = 0;
    char c;

    while((c = fgetc(in)) != EOF)
    {
        printf("%c %d\n", c, len);
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            ++len;
        }
        else if (len > 0)
        {
            if (len > size)
            {
                len = size;
            }
            ++arr[len - 1];
            len = 0;
        }
    }
    if (len > 0)
    {
        if (len > size)
        {
            len = size;
        }
        ++arr[len - 1];
    }

    for (int i=0; size > i; ++i)
    {
        printf("%d\n", arr[i]);
    }
}

void allCharCount(uint16_t counts[256], FILE *in)
{
    char c;
    while ((c = fgetc(in)) != EOF)
    {
        ++counts[c];
    }
}

void tabsToSpaces(size_t noOfSpacesInTab, FILE *in, FILE *out)
{
    char c;
    while ((c = fgetc(in)) != EOF)
    {
        if (c == '\t')
        {
            for (int i=0; i < noOfSpacesInTab; ++i)
            {
                fputc(' ', out);
            }
        }
        else
        {
            fputc(c, out);
        }
    }
}
