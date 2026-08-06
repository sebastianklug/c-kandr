#include <string.h>

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

void test_removeDuplicateConsecutiveChars(void)
{
    FILE *in = tmpfile();
    FILE *out = tmpfile();

    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);

    char remove = ' ';
    removeDuplicateConsecutiveChars(remove, in, out);
    rewind(out);

    char buffer[256];
    size_t n = fread(buffer, 1, sizeof(buffer), out);
    buffer[n] = '\0';

    TEST_ASSERT_EQUAL_STRING(" asfas \n\t \t \n \t", buffer);
}

void test_wordLengthStats(void)
{
    FILE *in = tmpfile();
    fputs(
        "Lorem ipsum dolor sit amet, consectetur adip\niscing elit. \tFusce et tincidunt massa, id \nmaximus odio."
        " Pellentesque mattis libero vel \tenim rutrum, a sodales mi eleifend. asdkfjaskldfjaskdlfjaskldfjasdklfjaskldfj",
        in
    );
    rewind(in);

    size_t size = 10;
    uint16_t result[size];
    memset(result, 0, sizeof(result));
    
    wordLengthStats(result, size, in);

    uint16_t expected[] = {1, 3, 2, 5, 5, 4, 2, 1, 1, 3};
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected, result, 10);
}

void test_tabsToSpaces(void)
{
    FILE *in = tmpfile();
    FILE *out = tmpfile();
    size_t noOfSpacesInTab = 4;
    
    fputs("     asfas  \n\t  \t  \n  \t", in);
    rewind(in);
    
    tabsToSpaces(noOfSpacesInTab, in, out);

    rewind(out);
    char buffer[256];
    size_t n = fread(buffer, 1, sizeof(buffer), out);
    buffer[n] = '\0';

    TEST_ASSERT_EQUAL_STRING("     asfas  \n            \n      ", buffer);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_charCount_blanks);
    RUN_TEST(test_charCount_tabs);
    RUN_TEST(test_charCount_newlines);
    RUN_TEST(test_removeDuplicateConsecutiveChars);
    RUN_TEST(test_wordLengthStats);
    RUN_TEST(test_tabsToSpaces);
    return UNITY_END();
}