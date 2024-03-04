#include "gtest/gtest.h"
#include "cpulib.hpp"

using namespace CPULib;

TEST(CPULibTest, Stack)
{
    stack<int> stack_1;
    EXPECT_EQ(stack_1.empty(), 1);

    stack_1.push(1);
    EXPECT_EQ(stack_1.empty(), 0);

    stack_1.pop();
    EXPECT_EQ(stack_1.empty(), 1);

    stack_1.pop();
    stack_1.top();

    stack_1.push(2);
    stack_1.push(3);
    EXPECT_EQ(stack_1.top(), 3);

    stack stack_2(stack_1);
    EXPECT_EQ(stack_2.top(), 3);

    stack_1.pop();
    EXPECT_EQ(stack_1.top(), 2);
    EXPECT_EQ(stack_2.top(), 3);

    stack_2 = stack_1;
    EXPECT_EQ(stack_2.top(), 2);

    stack_2 = std::move(stack_1);
    EXPECT_EQ(stack_2.top(), 2);
    EXPECT_EQ(stack_1.empty(), 1);

    stack stack_3(7412);
    EXPECT_EQ(stack_3.top(), 7412);

    int num = 123;
    stack stack_4(num);
    EXPECT_EQ(stack_4.top(), 123);

    stack stack_5(std::move(stack_4));
    EXPECT_EQ(stack_5.top(), 123);
    EXPECT_EQ(stack_4.empty(), 1);
}