#include "Sample.cpp"
#include <gtest/gtest.h>
 
TEST(SumTest, PositiveAndNegativeNumbers) { 
    ASSERT_EQ(6.0 + 4.0, sum(6.0, 4.0));
    ASSERT_EQ(sum(6.0, 4.0), sum(4.0, 6.0));
    ASSERT_EQ(sum(-6.0, 4.0), sum(4.0, -6.0));
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
