#include "String.h"

void escape(char s[], const char t[])
{
    int i=0;
    for(int j=0; t[i] != '\0'; j++)
    {
        switch (t[i])
        {
        case '\n':
            s[i] = '\\';
            i++;
            s[i] = 'n';
            i++;
        case '\t':
            s[i] = '\\';
            i++;
            s[i] = 't';
            i++;
        default:
            s[i] = t[j];
            i++;
            break;
        }
    }
    s[i] = '\0';
}
