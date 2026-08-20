
// Filename: test/TestPointers.c
#include "unity.h"
#include "../src/Pointers.h"  // getint(int16_t *pn)
#include "../src/Input.h"     // getch, ungetch, putchars
#include <string.h>           // strlen

void setUp(void) {}
void tearDown(void) {}

static void feed_input(const char *s)
{
    putchars(s, strlen(s));
}

static void consume_char(int expected)
{
    int c = getch();
    TEST_ASSERT_EQUAL_INT(expected, c);
}

/* Simple positive */
void test_getint_positive_simple(void)
{
    int16_t n;
    int ret;

    feed_input("123");
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_EQUAL_INT16(123, n);
}

/* Simple negative */
void test_getint_negative_simple(void)
{
    int16_t n;
    int ret;

    feed_input("-456");
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_EQUAL_INT16(-456, n);
}

/* Run getint multiple times on "a123b456" */
void test_getint_multiple_on_mixed_string(void)
{
    int16_t n;
    int ret;

    feed_input("aa123bb456");

    /* 1st getint: sees 'a' (non-digit/non-sign), returns 0, leaves 'a' pushed back */
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(0, ret);
    consume_char('a');

    /* 2nd getint: sees 'a' again, returns 0, 'a' still pushed back */
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(0, ret);
    consume_char('a');

    /* 3rd getint: now at '1', reads 123, stops at 'b', pushes 'b' back, returns 'b' */
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT('b', ret);
    TEST_ASSERT_EQUAL_INT16(123, n);

    /* 4th getint: sees 'b' (non-digit), returns 0, pushes 'b' back */
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(0, ret);
    consume_char('b');

    /* 5th getint: sees 'b' again, returns 0, pushes 'b' back */
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(0, ret);
    consume_char('b');

    /* 6th getint: now at '4', reads 456, stops at EOF, returns EOF, no pushback */
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_EQUAL_INT16(456, n);
}

void test_getint_pushes_back_first_non_digit(void)
{
    int16_t n;
    int ret;

    feed_input("123abc");

    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT('a', ret);    // loop exits when c = 'a'; then ungetch('a'); return 'a'
    TEST_ASSERT_EQUAL_INT16(123, n);

    /* Next getch should see the pushed-back 'a' */
    int c = getch();
    TEST_ASSERT_EQUAL_INT('a', c);
}

/* EOF immediately */
void test_getint_immediate_eof(void)
{
    int16_t n = 0;
    int ret;

    feed_input("");
    ret = getint(&n);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
}

/* 1. Simple positive integer-like float ("123" -> 123.0f) */
void test_getfloat_positive_simple(void)
{
    float f;
    int ret;

    feed_input("123");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 123.0f, f);
}

/* 2. Simple negative ("-456" -> -456.0f) */
void test_getfloat_negative_simple(void)
{
    float f;
    int ret;

    feed_input("-456");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -456.0f, f);
}

/* 3. With fractional part ("123.45") */
void test_getfloat_with_fraction(void)
{
    float f;
    int ret;

    feed_input("123.45");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 123.45f, f);
}

/* 4. Leading whitespace */
void test_getfloat_with_leading_whitespace(void)
{
    float f;
    int ret;

    feed_input("   789.5");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 789.5f, f);
}

/* 5. Plus sign */
void test_getfloat_with_plus_sign(void)
{
    float f;
    int ret;

    feed_input("+42.0");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 42.0f, f);
}

/* 6. Non-numeric start: "abc123" -> getfloat returns 0, pf unchanged */
void test_getfloat_non_numeric_start(void)
{
    float f = 999.0f;
    int ret;

    feed_input("abc123");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 999.0f, f);
}

/* 7. Mixed string: multiple calls, like we did for getint ("a123.5b456") */
void test_getfloat_multiple_on_mixed_string(void)
{
    float f;
    int ret;

    feed_input("a123.5b456");

    /* 1st getfloat: sees 'a' (non-digit/non-sign), returns 0, pushes 'a' back */
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(0, ret);
    consume_char('a');     // handle the 'a'

    /* 2nd getfloat: now at '1', reads 123.5, stops at 'b', pushes 'b', returns nonzero */
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT('b', ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 123.5f, f);
    consume_char('b');     // handle the 'b'

    /* 3rd getfloat: now at '4', reads 456.0, stops at EOF */
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 456.0f, f);
}

/* 8. EOF immediately */
void test_getfloat_immediate_eof(void)
{
    float f = 0.0f;
    int ret;

    feed_input("");
    ret = getfloat(&f);
    TEST_ASSERT_EQUAL_INT(EOF, ret);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_getint_positive_simple);
    RUN_TEST(test_getint_negative_simple);
    RUN_TEST(test_getint_multiple_on_mixed_string);
    RUN_TEST(test_getint_pushes_back_first_non_digit);
    RUN_TEST(test_getint_immediate_eof);

    RUN_TEST(test_getfloat_positive_simple);
    RUN_TEST(test_getfloat_negative_simple);
    RUN_TEST(test_getfloat_with_fraction);
    RUN_TEST(test_getfloat_with_leading_whitespace);
    RUN_TEST(test_getfloat_with_plus_sign);
    RUN_TEST(test_getfloat_non_numeric_start);
    RUN_TEST(test_getfloat_multiple_on_mixed_string);
    RUN_TEST(test_getfloat_immediate_eof);

    return UNITY_END();
}
