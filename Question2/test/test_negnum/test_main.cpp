
#include <Arduino.h>
#include <unity.h>
#include <limits.h>

extern "C" bool negnum(int x);
extern "C" int add(int x, int y);

void setUp(void)
{
}

void tearDown(void)
{
}

void test_negnum(void)
{
    TEST_ASSERT_TRUE(negnum(-5));
    TEST_ASSERT_TRUE(negnum(-10));
    TEST_ASSERT_FALSE(negnum(8));
    TEST_ASSERT_FALSE(negnum(0));
}

void test_add(void)
{
    TEST_ASSERT_TRUE(sizeof(int) * CHAR_BIT >= 16);
    TEST_ASSERT_EQUAL_INT(300, add(150, 150));
}

void setup()
{
    Serial.begin(9600);
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_negnum);
    RUN_TEST(test_add);

    UNITY_END();
}

void loop()
{
}
