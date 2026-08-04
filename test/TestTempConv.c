#include "../unity/src/unity.h"
#include "../src/TempConv.h"

void setUp() {} // Nothing
void tearDown() {} // Nothing

void test_temperatureConvert_should_lowToHigh() 
{
    char output_buffer[1024];

    int i = temperatureConvert(output_buffer, sizeof(output_buffer), 60, 100, 20);
    TEST_ASSERT_EQUAL_STRING(
        "F   C\n 60   15.6\n 80   26.7\n100   37.8\n", output_buffer
    );
    TEST_ASSERT_EQUAL_INT16(0, i);
}

void test_temperatureConvert_should_highToLow()
{
    char output_buffer[1024];

    int i = temperatureConvert(output_buffer, sizeof(output_buffer), 60, 100, -20);
    TEST_ASSERT_EQUAL_STRING(
        "F   C\n100   37.8\n 80   26.7\n 60   15.6\n", output_buffer
    );
    TEST_ASSERT_EQUAL_INT16(0, i);
}

void test_temperatureConvert_invalidStep()
{
    char output_buffer[1024];

    int i = temperatureConvert(output_buffer, sizeof(output_buffer), 1, 1, 0);
    TEST_ASSERT_EQUAL_INT16(1, i);
}

// Main runner
int main(void) 
{
    UNITY_BEGIN();
    RUN_TEST(test_temperatureConvert_should_lowToHigh);
    RUN_TEST(test_temperatureConvert_should_highToLow);
    RUN_TEST(test_temperatureConvert_invalidStep);
    
    return UNITY_END();
}
