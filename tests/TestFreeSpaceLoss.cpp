#include "TestUtils.h"

TEST(TestFreeSpaceLoss, BasicTest) {
    const double d_meter = 1000.0;
    const double f_mhz = 1000.0;
    const double expected_loss
        = 20.0 * (log10(f_mhz) + log10(d_meter)) - 27.5522;
    ASSERT_DOUBLE_EQ(FreeSpaceLoss(d_meter, f_mhz), expected_loss);
}

TEST(TestFreeSpaceLoss, ZeroDistanceTest) {
    // Should return -inf (undefined log term)
    const double d_meter = 0.0;
    const double f_mhz = 1000.0;
    const double loss = FreeSpaceLoss(d_meter, f_mhz);
    ASSERT_TRUE(std::isinf(loss));
    ASSERT_TRUE(std::isless(loss, 0));
}

TEST(TestFreeSpaceLoss, ZeroFrequencyTest) {
    // Should return -inf (undefined log term)
    const double d_meter = 1000.0;
    const double f_mhz = 0.0;
    const double loss = FreeSpaceLoss(d_meter, f_mhz);
    ASSERT_TRUE(std::isinf(loss));
    ASSERT_TRUE(std::isless(loss, 0));
}

TEST(TestFreeSpaceLoss, NegativeDistanceTest) {
    // Should return NaN
    const double d_meter = -1000.0;
    const double f_mhz = 1000.0;
    const double loss = FreeSpaceLoss(d_meter, f_mhz);
    ASSERT_TRUE(std::isnan(loss));
}

TEST(TestFreeSpaceLoss, NegativeFrequencyTest) {
    // Should return NaN
    const double d_meter = 1000.0;
    const double f_mhz = -1000.0;
    const double loss = FreeSpaceLoss(d_meter, f_mhz);
    ASSERT_TRUE(std::isnan(loss));
}

TEST(TestFreeSpaceLoss, LargeValuesTest) {
    const double d_meter = 1e9;
    const double f_mhz = 1e9;
    const double expected_loss
        = 20.0 * (log10(f_mhz) + log10(d_meter)) - 27.5522;
    ASSERT_DOUBLE_EQ(FreeSpaceLoss(d_meter, f_mhz), expected_loss);
}
