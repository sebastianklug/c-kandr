#include "../unity/src/unity.h"
#include "../src/Text.h"

void setUp() {} // Nothing
void tearDown() {} // Nothing

void test_charCount_blanks(void)
{
    FILE *in = tmpfile();
    FILE *out = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char charToCount = ' ';
    int numberOfBlanks = charCount(charToCount, in, out);

    rewind(out);

    char buffer[32];
    fgets(buffer, sizeof(buffer), out);

    TEST_ASSERT_EQUAL_INT(10, numberOfBlanks);
}

void test_charCount_tabs(void)
{
    FILE *in = tmpfile();
    FILE *out = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char charToCount = '\t';
    int numberOfTabs = charCount(charToCount, in, out);

    rewind(out);

    char buffer[32];
    fgets(buffer, sizeof(buffer), out);

     TEST_ASSERT_EQUAL_INT(3, numberOfTabs);
}

void test_charCount_newlines(void)
{
    FILE *in = tmpfile();
    FILE *out = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char charToCount = '\n';
    int numberOfNewlines = charCount(charToCount, in, out);

    rewind(out);

    char buffer[32];
    fgets(buffer, sizeof(buffer), out);

    TEST_ASSERT_EQUAL_INT(2, numberOfNewlines);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_charCount_blanks);
    RUN_TEST(test_charCount_tabs);
    RUN_TEST(test_charCount_newlines);
    return UNITY_END();
}