#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#define BUFSIZE 100

int getch(void);
void ungetch(int c);
void putchars(const char *s, size_t len);

#endif