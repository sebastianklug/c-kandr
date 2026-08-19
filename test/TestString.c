
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
   
    return UNITY_END();
}