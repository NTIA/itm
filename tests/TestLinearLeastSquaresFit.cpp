#include "TestUtils.h"

class TestLinearLeastSquaresFit: public testing::Test {
    // Provide simple linear "terrain" input
protected:
    double pfl[7];
    TestLinearLeastSquaresFit() : pfl{4.0, 1.0, 1.0, 2.0, 3.0, 4.0, 5.0} {}
    double d_start = 0.0;
    double d_stop = 4.0;
    double y1;
    double y2;
    double expected_y1 = pfl[2];
    double expected_y2 = pfl[6];
};

TEST_F(TestLinearLeastSquaresFit, BasicTest) {
    // Simple input data is linear
    LinearLeastSquaresFit(pfl, d_start, d_stop, &y1, &y2);
    EXPECT_DOUBLE_EQ(y1, expected_y1);
    EXPECT_DOUBLE_EQ(y2, expected_y2);
}

TEST_F(TestLinearLeastSquaresFit, ZeroRangeAtStart) {
    // Test in which i_start == i_end == 0
    d_start = 0.0;
    d_stop = 0.0;
    double expected_fit_y1 = 2.5;
    double expected_fit_y2 = 2.0;
    LinearLeastSquaresFit(pfl, d_start, d_stop, &y1, &y2);
    EXPECT_DOUBLE_EQ(y1, expected_y1);
    EXPECT_DOUBLE_EQ(y2, expected_y2);
}

TEST_F(TestLinearLeastSquaresFit, ZeroRangeInMiddle) {
    // Test in which i_start == i_end > 0
    d_start = 2.0;
    d_stop = 2.0;
    double expected_fit_y1 = 2.5;
    double expected_fit_y2 = 2.0;
    LinearLeastSquaresFit(pfl, d_start, d_stop, &y1, &y2);
    EXPECT_DOUBLE_EQ(y1, expected_y1);
    EXPECT_DOUBLE_EQ(y2, expected_y2);
}

TEST_F(TestLinearLeastSquaresFit, ZeroRangeAtEnd) {
    // Test in which i_start == i_end == np
    d_start = 4.0;
    d_stop = 4.0;
    double expected_fit_y1 = 2.5;
    double expected_fit_y2 = 2.0;
    LinearLeastSquaresFit(pfl, d_start, d_stop, &y1, &y2);
    EXPECT_DOUBLE_EQ(y1, expected_y1);
    EXPECT_DOUBLE_EQ(y2, expected_y2);
}

TEST_F(TestLinearLeastSquaresFit, NegativeRangeAtStart) {
    // Test in which i_start == i_end + 1 == 1
    d_start = 1.0;
    d_stop = 0.0;
    double expected_fit_y1 = 2.5;
    double expected_fit_y2 = 2.0;
    LinearLeastSquaresFit(pfl, d_start, d_stop, &y1, &y2);
    EXPECT_DOUBLE_EQ(y1, expected_y1);
    EXPECT_DOUBLE_EQ(y2, expected_y2);
}

TEST_F(TestLinearLeastSquaresFit, NegativeRangeAtEnd) {
    // Test in which i_start == i_end + 1  == np
    d_start = 4.0;
    d_stop = 3.0;
    double expected_fit_y1 = 2.5;
    double expected_fit_y2 = 2.0;
    LinearLeastSquaresFit(pfl, d_start, d_stop, &y1, &y2);
    EXPECT_DOUBLE_EQ(y1, expected_y1);
    EXPECT_DOUBLE_EQ(y2, expected_y2);
}
