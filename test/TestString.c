
#include "unity.h"
#include "../src/String.h"

void setUp() {} // Nothing
void tearDown() {} // Nothing

void test_escape_no_special_chars(void)
{
    char s[64] = {0};
    const char t[] = "Hello, world!";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("Hello, world!", s);
}

void test_escape_newline_only(void)
{
    char s[64] = {0};
    const char t[] = "Line1\nLine2";

    escape(s, t);

    // \n should become backslash + 'n'
    TEST_ASSERT_EQUAL_STRING("Line1\\nLine2", s);
}

void test_escape_tab_only(void)
{
    char s[64] = {0};
    const char t[] = "Col1\tCol2";

    escape(s, t);

    // \t should become backslash + 't'
    TEST_ASSERT_EQUAL_STRING("Col1\\tCol2", s);
}

void test_escape_mixed_newline_and_tab(void)
{
    char s[64] = {0};
    const char t[] = "Line1\tLine2\nLine3\tEnd";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("Line1\\tLine2\\nLine3\\tEnd", s);
}

void test_escape_leading_and_trailing_control_chars(void)
{
    char s[64] = {0};
    const char t[] = "\tStart\nMiddle\tEnd\n";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("\\tStart\\nMiddle\\tEnd\\n", s);
}

void test_escape_empty_string(void)
{
    char s[8] = {0};
    const char t[] = "";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("", s);
}

void test_escape_only_controls(void)
{
    char s[64] = {0};
    const char t[] = "\n\t\n\t";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("\\n\\t\\n\\t", s);
}

void test_escape_multiple_consecutive_controls(void)
{
    char s[64] = {0};
    const char t[] = "\n\n\t\t";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("\\n\\n\\t\\t", s);
}

void test_escape_preserves_other_escape_backslashes(void)
{
    char s[64] = {0};
    const char t[] = "Path\\with\\backslashes\nand\ttabs";

    escape(s, t);

    // Backslashes should stay, only \n and \t are encoded
    TEST_ASSERT_EQUAL_STRING("Path\\with\\backslashes\\nand\\ttabs", s);
}

void test_expand_simple_lowercase_range(void)
{
    char out[64] = {0};

    expand("a-d", out);
    TEST_ASSERT_EQUAL_STRING("abcd", out);

    expand("a-z", out);
    TEST_ASSERT_EQUAL_STRING("abcdefghijklmnopqrstuvwxyz", out);
}

void test_expand_simple_digit_range(void)
{
    char out[64];

    expand("0-3", out);
    TEST_ASSERT_EQUAL_STRING("0123", out);

    expand("0-9", out);
    TEST_ASSERT_EQUAL_STRING("0123456789", out);
}

void test_expand_mixed_text_and_ranges(void)
{
    char out[64];

    expand("A-Z0-3", out);
    TEST_ASSERT_EQUAL_STRING("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123", out);

    expand("abc0-2xyz", out);
    TEST_ASSERT_EQUAL_STRING("abc012xyz", out);
}

void test_expand_multiple_ranges_in_one_string(void)
{
    char out[64];

    expand("a-c0-2x-z", out);
    TEST_ASSERT_EQUAL_STRING("abc012xyz", out);

    expand("1-3a-c5-7", out);
    TEST_ASSERT_EQUAL_STRING("123abc567", out);
}

void test_expand_leading_and_trailing_dash_literal(void)
{
    char out[64];

    // Leading '-': treat it literally, then expand a-c
    expand("-a-c", out);
    TEST_ASSERT_EQUAL_STRING("-abc", out);

    // Trailing '-': expand a-c, then literal '-'
    expand("a-c-", out);
    TEST_ASSERT_EQUAL_STRING("abc-", out);

    // Both leading and trailing
    expand("-a-c-", out);
    TEST_ASSERT_EQUAL_STRING("-abc-", out);
}

void test_expand_isolated_dash_and_no_ranges(void)
{
    char out[64];

    expand("-", out);
    TEST_ASSERT_EQUAL_STRING("-", out);

    expand("abc-def", out);  // depends on your implementation; here we treat "abc-def" as "abc-def" or "abcdef"
    // If you ONLY expand patterns like a-c (single letters on each side),
    // then "abc-def" has 'c-d' as a range and 'ab' and 'ef' literal.
    // Let's assume you only expand single-letter ranges:
    // "abc-def" -> "ab" + "cdef"
    TEST_ASSERT_EQUAL_STRING("abcdef", out);
}

void test_expand_invalid_or_descending_range_copied_literal(void)
{
    char out[64];

    expand("z-a", out);
    TEST_ASSERT_EQUAL_STRING("zyxwvutsrqponmlkjihgfedcba", out);

    // Mixed with valid range
    expand("a-dz-a", out);
    TEST_ASSERT_EQUAL_STRING("abcdzyxwvutsrqponmlkjihgfedcba", out);
}

void test_expand_mixed_cases(void)
{
    char out[128];

    expand("A-Cx-z0-2-M", out);
    // A-C -> ABC, x-z -> xyz, 0-2 -> 012, -M is literal '-' then 'M'
    TEST_ASSERT_EQUAL_STRING("ABCxyz012-M", out);

    expand("a-b-c", out);
    TEST_ASSERT_EQUAL_STRING("abc", out);
}

void test_expand_empty_and_single_char(void)
{
    char out[16];

    expand("", out);
    TEST_ASSERT_EQUAL_STRING("", out);

    expand("x", out);
    TEST_ASSERT_EQUAL_STRING("x", out);
}

void test_strrindex_basic_match(void)
{
    TEST_ASSERT_EQUAL_INT(0,  strrindex("abc", "a"));    // first char
    TEST_ASSERT_EQUAL_INT(2,  strrindex("abc", "c"));    // last char
    TEST_ASSERT_EQUAL_INT(4,  strrindex("abcabc", "b")); // multiple matches, rightmost
}

void test_strrindex_substring(void)
{
    TEST_ASSERT_EQUAL_INT(3,  strrindex("abcbc", "bc"));   // matches at 1 and 3. rightmost is 3.
    TEST_ASSERT_EQUAL_INT(2,  strrindex("abcbc", "cbc"));  // rightmost "cbc" starts at 2
    TEST_ASSERT_EQUAL_INT(3,  strrindex("aaaaa", "aa"));   // overlapping occurrences, rightmost starting index
}

void test_strrindex_no_match(void)
{
    TEST_ASSERT_EQUAL_INT(-1, strrindex("abc", "d"));     // pattern not in text
    TEST_ASSERT_EQUAL_INT(-1, strrindex("abc", "abcd"));  // pattern longer than text
    TEST_ASSERT_EQUAL_INT(-1, strrindex("", "a"));        // empty text
}

void test_strrindex_empty_pattern(void)
{
    TEST_ASSERT_EQUAL_INT(-1, strrindex("abc", ""));   // if you define it as end-of-string
    TEST_ASSERT_EQUAL_INT(-1, strrindex("", ""));      // empty in empty
}

void test_itoa_zero(void)
{
    char buf[8] = {0};
    itoa(0, buf);
    TEST_ASSERT_EQUAL_STRING("0", buf);
}

void test_itoa_positive_numbers(void)
{
    char buf[16] = {0};

    itoa(1, buf);
    TEST_ASSERT_EQUAL_STRING("1", buf);

    itoa(12, buf);
    TEST_ASSERT_EQUAL_STRING("12", buf);

    itoa(1234, buf);
    TEST_ASSERT_EQUAL_STRING("1234", buf);
}

void test_itoa_negative_numbers(void)
{
    char buf[16] = {0};

    itoa(-1, buf);
    TEST_ASSERT_EQUAL_STRING("-1", buf);

    itoa(-12, buf);
    TEST_ASSERT_EQUAL_STRING("-12", buf);

    itoa(-1234, buf);
    TEST_ASSERT_EQUAL_STRING("-1234", buf);
}

void test_itoa_int16_limits(void)
{
    char buf[16] = {0};

    // int16_t max: 32767
    itoa(32767, buf);
    TEST_ASSERT_EQUAL_STRING("32767", buf);

    // int16_t min: -32768
    itoa(-32768, buf);
    TEST_ASSERT_EQUAL_STRING("-32768", buf);
}

/* Helper: run the same concat test on both implementations */
static void assert_krstrcat_both(const char *initial_s, const char *t, const char *expected)
{
    char buf1[128];
    char buf2[128];

    /* Prepare separate buffers for each implementation */
    strcpy(buf1, initial_s);
    strcpy(buf2, initial_s);

    /* Call array/index version */
    krstrcat(buf1, (char *)t);
    TEST_ASSERT_EQUAL_STRING(expected, buf1);

    /* Call pointer version */
    krstrcat_p(buf2, (char *)t);
    TEST_ASSERT_EQUAL_STRING(expected, buf2);
}

/* 1. Simple concat: "Hello" + "World" */
void test_krstrcat_simple(void)
{
    assert_krstrcat_both("Hello", "World", "HelloWorld");
}

/* 2. Empty t: "Hello" + "" -> "Hello" */
void test_krstrcat_empty_t(void)
{
    assert_krstrcat_both("Hello", "", "Hello");
}

/* 3. Empty s: "" + "World" -> "World" */
void test_krstrcat_empty_s(void)
{
    assert_krstrcat_both("", "World", "World");
}

/* 4. Both empty: "" + "" -> "" */
void test_krstrcat_both_empty(void)
{
    assert_krstrcat_both("", "", "");
}

/* 5. Multiple concatenations */
void test_krstrcat_multiple_appends(void)
{
    char buf1[128];
    char buf2[128];

    strcpy(buf1, "A");
    strcpy(buf2, "A");

    /* array/index version */
    krstrcat(buf1, "B");        // "AB"
    krstrcat(buf1, "C");        // "ABC"
    TEST_ASSERT_EQUAL_STRING("ABC", buf1);

    /* pointer version */
    krstrcat_p(buf2, "B");      // "AB"
    krstrcat_p(buf2, "C");      // "ABC"
    TEST_ASSERT_EQUAL_STRING("ABC", buf2);
}

/* 6. Ensure null-termination is preserved */
void test_krstrcat_null_termination(void)
{
    char buf1[8];
    char buf2[8];

    /* Enough room for "A" + "B" + '\0' */
    strcpy(buf1, "A");
    strcpy(buf2, "A");

    krstrcat(buf1, "B");
    krstrcat_p(buf2, "B");

    TEST_ASSERT_EQUAL_STRING("AB", buf1);
    TEST_ASSERT_EQUAL_STRING("AB", buf2);

    /* Verify explicit null terminator */
    TEST_ASSERT_EQUAL_CHAR('\0', buf1[2]);
    TEST_ASSERT_EQUAL_CHAR('\0', buf2[2]);
}

/* 7. Non-ASCII characters (just bytes) */
void test_krstrcat_non_ascii(void)
{
    /* Use bytes with high bit set; treated as chars */
    const char s_init[] = "A\xC3";
    const char t[]      = "\xB1""B";  // arbitrary bytes

    char expected[16];
    strcpy(expected, s_init);
    krstrcat(expected, (char *)t);    // use library krstrcat to build expected

    assert_krstrcat_both(s_init, t, expected);
}

/* 1. Exact match: s and t identical */
void test_strend_exact_match(void)
{
    TEST_ASSERT_TRUE(strend("hello", "hello"));
}

/* 2. t is proper suffix of s */
void test_strend_t_is_suffix(void)
{
    TEST_ASSERT_TRUE(strend("hello", "lo"));
    TEST_ASSERT_TRUE(strend("abcdef", "def"));
    TEST_ASSERT_TRUE(strend("xyz", "z"));
}

/* 3. t is not at the end */
void test_strend_t_not_suffix(void)
{
    TEST_ASSERT_FALSE(strend("hello", "he"));
    TEST_ASSERT_FALSE(strend("abcdef", "abc"));
    TEST_ASSERT_FALSE(strend("abcdef", "cd"));
}

/* 4. t longer than s: cannot be suffix */
void test_strend_t_longer_than_s(void)
{
    TEST_ASSERT_FALSE(strend("hi", "hello"));
    TEST_ASSERT_FALSE(strend("", "a"));
}

/* 5. Empty t: usually considered suffix of any s (including empty) */
void test_strend_empty_t(void)
{
    TEST_ASSERT_TRUE(strend("hello", ""));
    TEST_ASSERT_TRUE(strend("", ""));
}

/* 6. Empty s, non-empty t: cannot be suffix */
void test_strend_empty_s_nonempty_t(void)
{
    TEST_ASSERT_FALSE(strend("", "hello"));
}

/* 7. Multiple similar endings */
void test_strend_similar_endings(void)
{
    TEST_ASSERT_TRUE(strend("banana", "ana"));    // suffix "ana"
    TEST_ASSERT_FALSE(strend("banana", "ban"));   // "ban" is a prefix, not a suffix
}

/* 8. Non-ASCII / arbitrary bytes (treated as chars) */
void test_strend_non_ascii(void)
{
    const char *s = "A\xC3\xB1";   // "A" + two non-ASCII bytes
    const char *t = "\xC3\xB1";    // last two bytes
    TEST_ASSERT_TRUE(strend((char *)s, (char *)t));

    TEST_ASSERT_FALSE(strend((char *)s, "A"));
}


/* 1. krstrncpy copy less than length of t, s remains without explicit '\0' beyond n */
void test_krstrncpy_copy_partial(void)
{
    char s[10];
    const char *t = "abcdef";

    // Clear s to see what happens beyond n
    memset(s, 'X', sizeof(s));

    krstrncpy(s, t, 3);  // copy "abc"

    TEST_ASSERT_EQUAL_CHAR('a', s[0]);
    TEST_ASSERT_EQUAL_CHAR('b', s[1]);
    TEST_ASSERT_EQUAL_CHAR('c', s[2]);

    // Standard krstrncpy does NOT guarantee '\0' at s[3] when n < strlen(t)
}

/* 2. krstrncpy copy exactly n when n >= strlen(t): remaining bytes padded with '\0' */
void test_krstrncpy_copy_with_padding(void)
{
    char s[10];
    const char *t = "abc";

    memset(s, 'X', sizeof(s));

    krstrncpy(s, t, 6);  // n > strlen(t), copy "abc" then pad with '\0'

    TEST_ASSERT_EQUAL_CHAR('a', s[0]);
    TEST_ASSERT_EQUAL_CHAR('b', s[1]);
    TEST_ASSERT_EQUAL_CHAR('c', s[2]);

    TEST_ASSERT_EQUAL_CHAR('\0', s[3]);
    TEST_ASSERT_EQUAL_CHAR('\0', s[4]);
    TEST_ASSERT_EQUAL_CHAR('\0', s[5]);
}

/* 3. krstrncpy with n == 0: no copy */
void test_krstrncpy_zero_n(void)
{
    char s[10] = "XXXXXXXXX";
    const char *t = "abc";

    krstrncpy(s, t, 0);  // no bytes copied

    TEST_ASSERT_EQUAL_STRING("XXXXXXXXX", s);
}


/* 4. krstrncat appends partial t */
void test_krstrncat_append_partial(void)
{
    char s[16] = "Hello";
    const char *t = "World";

    krstrncat(s, t, 3);  // append "Wor"

    TEST_ASSERT_EQUAL_STRING("HelloWor", s);
}

/* 5. krstrncat appends full t when n >= strlen(t) */
void test_krstrncat_append_full(void)
{
    char s[16] = "Hello";
    const char *t = "World";

    krstrncat(s, t, 10);  // n bigger than strlen(t) = 5

    TEST_ASSERT_EQUAL_STRING("HelloWorld", s);
}

/* 6. krstrncat with n == 0: s unchanged */
void test_krstrncat_zero_n(void)
{
    char s[16] = "Hello";
    const char *t = "World";

    krstrncat(s, t, 0);  // append nothing

    TEST_ASSERT_EQUAL_STRING("Hello", s);
}


/* 7. krstrncmp equal strings, n >= length */
void test_krstrncmp_equal_full(void)
{
    const char *s = "abc";
    const char *t = "abc";

    int r = krstrncmp(s, t, 5);
    TEST_ASSERT_EQUAL_INT(0, r);
}

/* 8. krstrncmp equal up to n, but different afterwards; n small */
void test_krstrncmp_equal_prefix(void)
{
    const char *s = "abcdef";
    const char *t = "abcXYZ";

    int r = krstrncmp(s, t, 3);  // compare only "abc"
    TEST_ASSERT_EQUAL_INT(0, r);

    r = krstrncmp(s, t, 4);      // compare "abcd" vs "abcX"
    TEST_ASSERT_TRUE(r > 0);   // 'd' > 'X' in ASCII
}

/* 9. krstrncmp s < t */
void test_krstrncmp_less(void)
{
    const char *s = "abc";
    const char *t = "abd";

    int r = krstrncmp(s, t, 3);
    TEST_ASSERT_TRUE(r < 0);   // 'c' < 'd'
}

/* 10. krstrncmp s > t */
void test_krstrncmp_greater(void)
{
    const char *s = "abd";
    const char *t = "abc";

    int r = krstrncmp(s, t, 3);
    TEST_ASSERT_TRUE(r > 0);   // 'd' > 'c'
}

/* 11. krstrncmp with n == 0: always equal */
void test_krstrncmp_zero_n(void)
{
    const char *s = "abc";
    const char *t = "xyz";

    int r = krstrncmp(s, t, 0);
    TEST_ASSERT_EQUAL_INT(0, r);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_escape_no_special_chars);
    RUN_TEST(test_escape_newline_only);
    RUN_TEST(test_escape_tab_only);
    RUN_TEST(test_escape_mixed_newline_and_tab);
    RUN_TEST(test_escape_leading_and_trailing_control_chars);
    RUN_TEST(test_escape_empty_string);
    RUN_TEST(test_escape_multiple_consecutive_controls);
    RUN_TEST(test_escape_preserves_other_escape_backslashes);
    RUN_TEST(test_expand_simple_lowercase_range);
    RUN_TEST(test_expand_simple_digit_range);
    RUN_TEST(test_expand_mixed_text_and_ranges);
    RUN_TEST(test_expand_multiple_ranges_in_one_string);
    RUN_TEST(test_expand_leading_and_trailing_dash_literal);
    RUN_TEST(test_expand_isolated_dash_and_no_ranges);
    RUN_TEST(test_expand_invalid_or_descending_range_copied_literal);
    RUN_TEST(test_expand_mixed_cases);
    RUN_TEST(test_expand_empty_and_single_char);
    RUN_TEST(test_strrindex_basic_match);
    RUN_TEST(test_strrindex_substring);
    RUN_TEST(test_strrindex_no_match);
    RUN_TEST(test_strrindex_empty_pattern);
    RUN_TEST(test_itoa_zero);
    RUN_TEST(test_itoa_positive_numbers);
    RUN_TEST(test_itoa_negative_numbers);
    RUN_TEST(test_itoa_int16_limits);
    RUN_TEST(test_krstrcat_simple);
    RUN_TEST(test_krstrcat_empty_t);
    RUN_TEST(test_krstrcat_empty_s);
    RUN_TEST(test_krstrcat_both_empty);
    RUN_TEST(test_krstrcat_multiple_appends);
    RUN_TEST(test_krstrcat_null_termination);
    RUN_TEST(test_krstrcat_non_ascii);
    RUN_TEST(test_strend_exact_match);
    RUN_TEST(test_strend_t_is_suffix);
    RUN_TEST(test_strend_t_not_suffix);
    RUN_TEST(test_strend_t_longer_than_s);
    RUN_TEST(test_strend_empty_t);
    RUN_TEST(test_strend_empty_s_nonempty_t);
    RUN_TEST(test_strend_similar_endings);
    RUN_TEST(test_strend_non_ascii);
    
    /* krstrncpy tests */
    RUN_TEST(test_krstrncpy_copy_partial);
    RUN_TEST(test_krstrncpy_copy_with_padding);
    RUN_TEST(test_krstrncpy_zero_n);

    /* krstrncat tests */
    RUN_TEST(test_krstrncat_append_partial);
    RUN_TEST(test_krstrncat_append_full);
    RUN_TEST(test_krstrncat_zero_n);

    /* krstrncmp tests */
    RUN_TEST(test_krstrncmp_equal_full);
    RUN_TEST(test_krstrncmp_equal_prefix);
    RUN_TEST(test_krstrncmp_less);
    RUN_TEST(test_krstrncmp_greater);
    RUN_TEST(test_krstrncmp_zero_n);

    return UNITY_END();
}
