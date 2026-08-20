
#include "Pointers.h"

int16_t getint(int16_t *pn)
{
    int16_t c, sign;

    while (isspace(c = getch()))
        ;
    
    if (!isdigit(c) && c != EOF && c != '+' && c != '-')
    {
        ungetch(c);
        return 0;
    }
    
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
    {
        c = getch();
        if (!isdigit(c))
        {
            ungetch(c);
            return 0;
        }
    }
    for (*pn = 0; isdigit(c); c = getch())
    {
        *pn = 10 * *pn + (c - '0');
    }
    *pn *= sign;
    if (c != EOF)
    {
        ungetch(c);
    }
    return c;
}

int16_t getfloat(float *pf)
{
    int16_t c, sign;

    while (isspace(c = getch()))
        ;
    
    if (!isdigit(c) && c != EOF && c != '+' && c != '-')
    {
        ungetch(c);
        return 0;
    }
    
    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')
    {
        c = getch();
        if (!isdigit(c))
        {
            ungetch(c);
            return 0;
        }
    }
    
    for (*pf = 0.0; isdigit(c); c = getch())
    {
        *pf = 10 * *pf + (c - '0');
    }
    
    if (c == '.')
    {
        c = getch();
        if (!isdigit(c))
        {
            ungetch(c);
            return 0;
        }
        for (float f = 10.0; isdigit(c); c = getch(), f *= 10.0)
        {
            *pf = *pf + (c - '0') / f;
        }
    }
    *pf *= sign;

    if (c != EOF)
    {
        ungetch(c);
    }
    return c;
}
