#include "TestUtils.h"

#include <cmath>  // For isnan

TEST(TestSigmaHFunction, ZeroTest) {
    EXPECT_DOUBLE_EQ(SigmaHFunction(0.0), 0.0);
}

TEST(TestSigmaHFunction, NegativeInputTest) {
    EXPECT_TRUE(std::isnan(SigmaHFunction(-1.0)));
}

TEST(TestSigmaHFunction, SimpleTest) {
    // Compare to result of simplified expression
    EXPECT_DOUBLE_EQ(SigmaHFunction(16.0), 0.78 * 16.0 * exp(-1.0));
}
