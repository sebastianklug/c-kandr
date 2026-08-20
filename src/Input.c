#include "Input.h"

char buf[BUFSIZE];    /* buffer for ungetch */
int bufp = 0;         /* next free position in buf */

int getch(void)  /* get a (possibly pushed-back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)   /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}


void putchars(const char *s, size_t len)
{
    if (len + 1 > BUFSIZE) 
    {
        printf("putchars: too many characters (len=%zu)\n", len);
        return;
    }

    bufp = 0;
    ungetch(EOF);

    for (size_t i = 0; i < len; ++i) 
    {
        ungetch((unsigned char)s[len - 1 - i]);
    }
}
