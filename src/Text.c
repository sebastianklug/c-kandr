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

void spacesToTabs(size_t noOfSpacesInTab, FILE *in, FILE *out)
{
    size_t consecutiveSpaces = 0;
    char c;

    while ((c = fgetc(in)) != EOF)
    {
        if (c == ' ')
        {
            ++consecutiveSpaces;
            if (consecutiveSpaces == noOfSpacesInTab)
            {
                fputc('\t', out);
                consecutiveSpaces = 0;
            }
        }
        else if (consecutiveSpaces > 0)
        {
            for(int i=0; i < consecutiveSpaces; ++i)
            {
                fputc(' ', out);
            }
            consecutiveSpaces = 0;

            fputc(c, out);
        }
        else 
        {
            fputc(c, out);
        }
    }
}

void fold(size_t noOfColumns, FILE *in, FILE *out)
{
    size_t posInLine = 0;
    size_t posStartOfThisWord = 0;
    size_t lenOfThisWord = 0;
    char buffer[256] = {0};
    char c;

    while ((c = fgetc(in)) != EOF)
    {
        char nextc = fgetc(in);
        bool finalc = false;
        if (nextc == EOF)
        {
            finalc = true;
            buffer[lenOfThisWord] = c;
        }
        ungetc(nextc, in);

        if (c == ' ' || finalc)
        {
            if (posInLine > noOfColumns)
            {
                if (lenOfThisWord < noOfColumns)
                {
                    buffer[lenOfThisWord] = '\0';
                    fputc('\n', out);
                    fputs(buffer, out);
                    fputc(' ', out);

                    memset(buffer, 0, sizeof(buffer));
                    posInLine = lenOfThisWord;
                    lenOfThisWord = 0;
                    posStartOfThisWord = (size_t)SIZE_MAX;
                }
                else
                {
                    char *b = buffer;
                    uint16_t colLeftInLine = noOfColumns - (posStartOfThisWord + LONGWORDDELIMITER);
                    for(int i = 1; i < colLeftInLine; ++i)
                    {
                        fputc(b[0], out);
                        b = b + 1;
                    }
                    fputs("-\n", out);
                    fputs(b, out);
                    memset(buffer, 0, sizeof(buffer));

                    posInLine = colLeftInLine - lenOfThisWord;
                    posStartOfThisWord = (size_t)SIZE_MAX;
                    lenOfThisWord = 0;
                }
            }
            else
            {
                fputs(buffer, out);
                fputc(' ', out);
                memset(buffer, 0, sizeof(buffer));
                ++posInLine;
                posStartOfThisWord = (size_t)SIZE_MAX;
                lenOfThisWord = 0;
            }
        }
        else if (c == '\n')
        {
            posInLine = 0;
            posStartOfThisWord = (size_t)SIZE_MAX;
            lenOfThisWord = 0;
            fputs(buffer, out);
            memset(buffer, 0, sizeof(buffer));
            fputc('\n', out);
        }
        else
        {
            buffer[lenOfThisWord] = c;
            if (posStartOfThisWord == (size_t)SIZE_MAX)
            {
                posStartOfThisWord = posInLine;
            }
            ++posInLine;
            ++lenOfThisWord;
        }
    }
}
