#include "../unity/src/unity.h"
#include "../src/BitOps.h"

void setUp() {} // Nothing
void tearDown() {} // Nothing

void test_htoi(void)
{
    char *h = "1A2B3C4F567890";
    int16_t d = 0;
    d = htoi(h);
    TEST_ASSERT_EQUAL_INT16(0x1A2B3C4F567890, d);

    h = "0xA";
    d = htoi(h);
    TEST_ASSERT_EQUAL_INT16(0xa, d);

    h = "0XB";
    d = htoi(h);
    TEST_ASSERT_EQUAL_INT16(0xB, d);

    h = "-0xC";
    d = htoi(h);
    TEST_ASSERT_EQUAL_INT16(-0xC, d);

    h = "-d";
    d = htoi(h);
    TEST_ASSERT_EQUAL_INT16(-0xd, d);

}

void test_squeeze(void)
{
    char s1[64], s2[64];

    strcpy(s1, "asdfghjk\0");
    strcpy(s2, "zxcvcvbn\0");
    squeeze(s1, s2);
    TEST_ASSERT_EQUAL_STRING("asdfghjk", s1);

    strcpy(s1, "zxcvcvbn\0");
    strcpy(s2, "zxcvcvbn\0");
    squeeze(s1, s2);
    TEST_ASSERT_EQUAL_STRING("", s1);

    strcpy(s1, "asdftyughjkuyiylasdfasdfghjk\0");
    strcpy(s2, "asdfghjk\0");
    squeeze(s1, s2);
    TEST_ASSERT_EQUAL_STRING("tyuuyiyl", s1);
    
}

void test_any(void)
{
    char s1[64], s2[64];
    int16_t index = -1;

    strcpy(s1, "asdftyughjkuyiylasdfasdfghjk\0");
    strcpy(s2, "asdfghjk\0");
    index = any(s1, s2);
    TEST_ASSERT_EQUAL_INT16(0, index);

    strcpy(s1, "asdftyughjkuyiylasdfasdfghjk\0");
    strcpy(s2, "hjk\0");
    index = any(s1, s2);
    TEST_ASSERT_EQUAL_INT16(8, index);

    strcpy(s1, "asdftyughjkuyiylasdfasdfghjk\0");
    strcpy(s2, "zxv\0");
    index = any(s1, s2);
    TEST_ASSERT_EQUAL_INT16(-1, index);

    strcpy(s1, "asd\0");
    strcpy(s2, "d\0");
    index = any(s1, s2);
    TEST_ASSERT_EQUAL_INT16(2, index);
}

void test_setbits(void)
{
    uint16_t x = 0b0000000011000000111001;
    uint16_t y = 0b0000000011000000111101;
    int16_t p = 2;
    int16_t n = 3;

    uint16_t res = setbits(x, p, n, y);

    TEST_ASSERT_EQUAL_UINT16(0b0000000011000000111101, res);

    x = 0b0000000011000000111001;
    y = 0b0000000011000000111101;
    p = 20;
    n = 6;

    res = setbits(x, p, n, y);

    TEST_ASSERT_EQUAL_UINT16(0b1011000000111001, res);

    x = 0b0000000011000000111001;
    y = 0b0000000011000000111101;
    p = 2;
    n = 6;

    res = setbits(x, p, n, y);

    TEST_ASSERT_EQUAL_UINT16(0b0000000011000000111111, res);
}

void test_invert(void)
{
    uint16_t x = 0b0011000000111001;
    
    int16_t p = 14;
    int16_t n = 6;

    uint16_t res = invert(x, p, n);

    TEST_ASSERT_EQUAL_UINT16(0b0100111000111001, res);

    p = 20;
    n = 6;

    res = invert(x, p, n);

    TEST_ASSERT_EQUAL_UINT16(0b1011000000111001, res);

    p = 2;
    n = 6;

    res = invert(x, p, n);

    TEST_ASSERT_EQUAL_UINT16(0b0011000000111110, res);
}

int main(void) 
{
    UNITY_BEGIN();
    RUN_TEST(test_htoi);
    RUN_TEST(test_squeeze);
    RUN_TEST(test_any);
    RUN_TEST(test_setbits);
    RUN_TEST(test_invert);
    
    return UNITY_END();
}