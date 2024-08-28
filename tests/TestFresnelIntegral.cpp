#include "TestUtils.h"

TEST(TestFresnelIntegral, TestVBelowCutoff) {
    // Negative values of v2 are actually incorrect inputs, but
    // the function itself does not handle this. Don't test for
    // v2<0 cases here.
    const std::vector<double> v2Vals = {0.0, 3.0, 5.75};
    double expected;
    for (double const &v2 : v2Vals) {
        expected = 6.02 + 9.11 * sqrt(v2) - 1.27 * v2;
        EXPECT_DOUBLE_EQ(FresnelIntegral(v2), expected);
    }
}

TEST(TestFresnelIntegral, TestVAboveCutoff) {
    // Include cutoff value in test cases. Behavior is the same
    const std::vector<double> v2Vals = {5.76, 5.77, 6.0, 100.0};
    double expected;
    for (double const &v2 : v2Vals) {
        expected = 12.953 + 10 * log10(v2);
        EXPECT_DOUBLE_EQ(FresnelIntegral(v2), expected);
    }
}