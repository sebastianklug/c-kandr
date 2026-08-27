#include "String.h"

static void itoa_helper(uint16_t n, char *s_p, size_t *index_p)
{
    if ((n / 10) > 0)
    {
        itoa_helper(n / 10, s_p, index_p);
    }
    s_p[(*index_p)++] = (char)(n % 10 + '0');
}

static char *last_char_in_string(char *s)
{
    while(*s)
    {
        s++;
    }
    return s-1;
}
/************************************************************* */

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
        prev = s1[i];
    }
}

int16_t strrindex(char s[], char t[])
{
    int i, j, k;
    int16_t res = -1;

    for (i=0; s[i] != '\0'; i++)
    {
        for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
        {
            ;
        }
        if (k > 0 && t[k] == '\0')
        {
            res = i;
        }
    }
    return res;
}

void itoa(int16_t n, char *s_p)
{
    uint16_t nu = n;
    size_t index = 0;
    if (n < 0)
    {
        s_p[index++] = '-';
        nu = n * -1;
    }
    itoa_helper(nu, s_p, &index);
    s_p[index] = '\0';   
}

void krstrcat(char *s, char *t)
{
    int i, j;
    char *temp = s;
    i = j = 0;

    while (s[i] != '\0') /* find end of s */
    {
        i++;
    }
    while ((s[i++] = t[j++]) != '\0') /* copy t */
    {
        ;
    }
    printf("%s\n", temp);
}

void krstrcat_p(char *s, char *t)
{
    s = last_char_in_string(s) + 1;
    while(*s++ = *t++)
    {
       ;
    }
}

bool strend(char *s, char *t)
{
    size_t lent = strlen(t);
    
    if (lent > strlen(s))
    {
        return false;
    }

    printf("s: %s, t: %s\n", s, t);
    
    s = last_char_in_string(s);
    t = last_char_in_string(t);

    for (int i=0; (i < lent); *s--, *t--, i++)
    {
        printf("s: %c, t: %c\n", *s, *t);

        if (*s!=*t)
        {
            return false;
        }
    }
    return true;
}

void krstrncpy(char *s, const char *t, size_t n)
{
    size_t len = strlen(t);

    for (int i = 0;  (n > i) && (*s++ = *t++); i++)
        ;
    for (;len < n; len++)
    {
        *s++ = '\0';
    }
}

void krstrncat(char *s, const char *t, size_t n)
{
    s = last_char_in_string(s) + 1;
    for(int i=0; i < n && (*s++ = *t++); i++)
        ;
}

int16_t krstrncmp(const char *s, const char *t, size_t n)
{
    size_t tmpn = strlen(s) > strlen(t) ? strlen(t) : strlen(s);
    n = n > tmpn ? tmpn : n;

    for(int i = 0; i < n; i++)
    {
        if (s[i] > t[i])
        {
            return 1;
        }
        else if (s[i] < t[i])
        {
            return -1;
        }
    }
    return 0;
}
