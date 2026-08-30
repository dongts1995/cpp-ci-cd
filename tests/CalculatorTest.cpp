#include <gtest/gtest.h>

#include "Calculator.h"

TEST(CalculatorTest, Add)
{
    Calculator calculator;

    EXPECT_EQ(calculator.add(2, 3), 999);
}

TEST(CalculatorTest, Subtract)
{
    Calculator calculator;

    EXPECT_EQ(calculator.subtract(5, 3), 2);
}