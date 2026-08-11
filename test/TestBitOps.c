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

void test_rightrot_basic(void)
{
    uint16_t x;
    int16_t n;
    uint16_t res;

    // 1) Simple rotation by 1: 0000...0001 -> 1000...0000
    x = 0b0000000000000001;
    n = 1;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b1000000000000000, res);

    // 2) Rotate by 4: 0x1234
    // 0001 0010 0011 0100 (0x1234)
    // >> 4 → 0000 0001 0010 0011 (0x0123)
    // low 4 bits (0100) move to high: 0100 0001 0010 0011 (0x4123)
    x = 0b0001001000110100;  // 0x1234
    n = 4;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b0100000100100011, res); // 0x4123

    // 3) Rotate by 8: 0xFF00
    // 1111 1111 0000 0000 → 0000 0000 1111 1111 (0x00FF)
    x = 0b1111111100000000;  // 0xFF00
    n = 8;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b0000000011111111, res); // 0x00FF

    // 4) Rotate pattern 0xAAAA by 3: gives 0x5555
    x = 0b1010101010101010;  // 0xAAAA
    n = 3;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b0101010101010101, res); // 0x5555
}

void test_rightrot_modulo_16(void)
{
    uint16_t x;
    int16_t n;
    uint16_t res;

    // 5) Rotate by 16 → unchanged if n%16 == 0
    x = 0b0011000000111001;  // 0x3039
    n = 16;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b0011000000111001, res);

    // 6) Rotate by 17 → same as rotate by 1
    x = 0b0011000000111001;  // 0x3039
    n = 17;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b1001100000011100, res);

    // 7) Large n, e.g. 33 → same as rotate by 1 (33 % 16 == 1)
    x = 0b1111000000001111;  // 0xF00F
    n = 33;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b1111100000000111, res); // 0xF807
}

void test_rightrot_zero_and_all_ones(void)
{
    uint16_t x;
    int16_t n;
    uint16_t res;

    // 8) Rotate zero → always zero
    x = 0b0000000000000000;
    n = 5;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b0000000000000000, res);

    // 9) Rotate all ones → always all ones
    x = 0b1111111111111111;  // 0xFFFF
    n = 7;
    res = rightrot(x, n);
    TEST_ASSERT_EQUAL_UINT16(0b1111111111111111, res);
}

int main(void) 
{
    UNITY_BEGIN();
    RUN_TEST(test_htoi);
    RUN_TEST(test_squeeze);
    RUN_TEST(test_any);
    RUN_TEST(test_setbits);
    RUN_TEST(test_invert);
    RUN_TEST(test_rightrot_basic);
    RUN_TEST(test_rightrot_modulo_16);
    RUN_TEST(test_rightrot_zero_and_all_ones);
    
    return UNITY_END();
}