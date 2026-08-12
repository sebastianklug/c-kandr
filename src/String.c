#include "String.h"

void escape(char s[], const char t[])
{
    int i=0;
    for(int j=0; t[j] != '\0'; j++)
    {
        switch (t[j])
        {
        case '\n':
            s[i++] = '\\';
            s[i++] = 'n';
            break;
        case '\t':
            s[i++] = '\\';
            s[i++] = 't';
            break;
        default:
            s[i++] = t[j];
            break;
        }
        //printf("j: %d, i: %d, t[j]: %c, s: %s\n", j, i, t[j], s);
    }
    s[i] = '\0';
}

void expand(const char s1[], char s2[])
{
    char next = '\0', prev = '\0';
    for(int i = 0, j = 0; i < strlen(s1) ; i++)
    {
        next = s1[i+1];
        if ((s1[i] == '-') && (((prev >= 'a' && prev <= 'z') && (next >= 'a' && next <= 'z')) ||
                               ((prev >= 'A' && prev <= 'Z') && (next >= 'A' && next <= 'Z')) ||
                               ((prev >= '0' && prev <= '9') && (next >= '0' && next <= '9')))
            )
        {

            if (prev < next)
            {
                for (char k = prev + 1; k <= next; j++, k++)
                {
                    s2[j] = k;
                }
            }
            else
            {
                for (char k = prev - 1; k >= next; j++, k--)
                {
                    s2[j] = k;
                }            }
            
            i++;
        }
        else
        {
            s2[j++] = s1[i];
        }
        s2[j] = '\0';
        printf("s1: %s, s2: %s\n", s1, s2);
        prev = s1[i];
    }
}