#include "TestUtils.h"

TEST(TestTroposcatterLoss, TestFFunction) {
    double D;
    double expected;
    // D <= 10 km
    D = 9e3;
    expected = 133.4 + 0.332e-3 * D - 10 * log10(D);
    EXPECT_DOUBLE_EQ(FFunction(D), expected);
    // 10 km < D <= 70 km
    D = 30e3;
    expected = 104.6 + 0.212e-3 * D - 2.5 * log10(D);
    EXPECT_DOUBLE_EQ(FFunction(D), expected);
    // D > 70 km
    D = 75e3;
    expected = 71.8 + 0.157e-3 * D + 5 * log10(D);
    EXPECT_DOUBLE_EQ(FFunction(D), expected);
}

TEST(TestTroposcatterLoss, TestTroposcatterLoss) {
    const double d__meter = 100e3;
    double theta_hzn__rad[2] = {-0.001, -0.001};
    double d_hzn__meter[2] = {8e3, 8e3};
    double h_e__meter[2] = {10.0, 5.0};
    const double a_e__meter = 7850e3;
    const double N_s = 301.0;
    const double f__mhz = 3500.0;
    const double theta_los__rad = -0.02;
    double h0 = -1.0;
    double result = TroposcatterLoss(
        d__meter,
        theta_hzn__rad,
        d_hzn__meter,
        h_e__meter,
        a_e__meter,
        N_s,
        f__mhz,
        theta_los__rad,
        &h0
    );
    EXPECT_DOUBLE_EQ(result, 76.969449309403771);

    // Test short circuit (when h0 > 15)
    double expected;
    h0 = 16.0;
    double th = d__meter / a_e__meter - theta_los__rad;
    expected = FFunction(th * d__meter) + 10 * log10(f__mhz * pow(th, 4)) + h0;
    result = TroposcatterLoss(
        d__meter,
        theta_hzn__rad,
        d_hzn__meter,
        h_e__meter,
        a_e__meter,
        N_s,
        f__mhz,
        theta_los__rad,
        &h0
    );
    EXPECT_DOUBLE_EQ(result, expected);
}