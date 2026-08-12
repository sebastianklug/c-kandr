
#include "unity.h"
#include "../src/String.h"

void setUp() {} // Nothing
void tearDown() {} // Nothing

void test_escape_no_special_chars(void)
{
    char s[64];
    const char t[] = "Hello, world!";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("Hello, world!", s);
}

void test_escape_newline_only(void)
{
    char s[64];
    const char t[] = "Line1\nLine2";

    escape(s, t);

    // \n should become backslash + 'n'
    TEST_ASSERT_EQUAL_STRING("Line1\\nLine2", s);
}

void test_escape_tab_only(void)
{
    char s[64];
    const char t[] = "Col1\tCol2";

    escape(s, t);

    // \t should become backslash + 't'
    TEST_ASSERT_EQUAL_STRING("Col1\\tCol2", s);
}

void test_escape_mixed_newline_and_tab(void)
{
    char s[64];
    const char t[] = "Line1\tLine2\nLine3\tEnd";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("Line1\\tLine2\\nLine3\\tEnd", s);
}

void test_escape_leading_and_trailing_control_chars(void)
{
    char s[64];
    const char t[] = "\tStart\nMiddle\tEnd\n";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("\\tStart\\nMiddle\\tEnd\\n", s);
}

void test_escape_empty_string(void)
{
    char s[8];
    const char t[] = "";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("", s);
}

void test_escape_only_controls(void)
{
    char s[64];
    const char t[] = "\n\t\n\t";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("\\n\\t\\n\\t", s);
}

void test_escape_multiple_consecutive_controls(void)
{
    char s[64];
    const char t[] = "\n\n\t\t";

    escape(s, t);

    TEST_ASSERT_EQUAL_STRING("\\n\\n\\t\\t", s);
}

void test_escape_preserves_other_escape_backslashes(void)
{
    char s[64];
    const char t[] = "Path\\with\\backslashes\nand\ttabs";

    escape(s, t);

    // Backslashes should stay, only \n and \t are encoded
    TEST_ASSERT_EQUAL_STRING("Path\\with\\backslashes\\nand\\ttabs", s);
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
    
    return UNITY_END();
}