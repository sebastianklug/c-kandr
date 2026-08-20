
// TestInput.c
#include "unity.h"
#include "../src/Input.h"

void setUp() {}
void tearDown() {}

void test_putchars_and_getch_sequence(void)
{
    const char *s = "ABC";
    putchars(s, 3);

    int c;
    c = getch(); TEST_ASSERT_EQUAL_INT('A', c);
    c = getch(); TEST_ASSERT_EQUAL_INT('B', c);
    c = getch(); TEST_ASSERT_EQUAL_INT('C', c);
    c = getch(); TEST_ASSERT_EQUAL_INT(EOF, c);
}

void test_ungetch_lifo_with_putchars(void)
{
    const char *s = "XY";
    putchars(s, 2);

    /* We now have X, Y, EOF in buffer. Let’s push another 'Z'. */
    ungetch('Z');

    int c;
    c = getch(); TEST_ASSERT_EQUAL_INT('Z', c);  // from ungetch
    c = getch(); TEST_ASSERT_EQUAL_INT('X', c);  // start of original
    c = getch(); TEST_ASSERT_EQUAL_INT('Y', c);
    c = getch(); TEST_ASSERT_EQUAL_INT(EOF, c);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_putchars_and_getch_sequence);
    RUN_TEST(test_ungetch_lifo_with_putchars);

    return UNITY_END();
}
