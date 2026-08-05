#include "../unity/src/unity.h"
#include "../src/Text.h"

void setUp() {} // Nothing
void tearDown() {} // Nothing


void test_charCount_blanks(void)
{
    FILE *in = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char charToCount = ' ';
    uint64_t numberOfBlanks = charCount(charToCount, in);

    TEST_ASSERT_EQUAL_UINT64(13, numberOfBlanks);
}

void test_charCount_tabs(void)
{
    FILE *in = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char charToCount = '\t';
    uint64_t numberOfTabs = charCount(charToCount, in);

    TEST_ASSERT_EQUAL_UINT64(3, numberOfTabs);
}

void test_charCount_newlines(void)
{
    FILE *in = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char charToCount = '\n';
    uint64_t numberOfNewlines = charCount(charToCount, in);

    TEST_ASSERT_EQUAL_UINT64(2, numberOfNewlines);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_charCount_blanks);
    RUN_TEST(test_charCount_tabs);
    RUN_TEST(test_charCount_newlines);
    return UNITY_END();
}