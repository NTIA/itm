#include "TestUtils.h"

TEST(TestDiffractionLoss, TestDiffractionLoss) {
    // Construct a test case which satisfies:
    // - small angle approximation for theta_hzn
    // - horizon distances >= 1/10 smooth earth horizon distance
    // - horizon distances <= 3x smooth earth horizon distance
    const double d__meter = 1000.0;
    double d_hzn__meter[2] = {2500.0, 3000.0};
    double h_e__meter[2] = {8.0, 5.0};
    const std::complex<double> Z_g = {1.0, 0.1};
    const double a_e__meter = 6371000.0;
    const double delta_h__meter = 10.0;
    double h__meter[2] = {8.0, 5.0};
    const int mode = MODE__P2P;
    const double f__mhz = 3500.0;
    double theta_hzn[2] = {-0.19, -0.08};

    // Construct other inputs using Longley Rice approach
    double d_hzn_s[2];
    for (int i = 0; i < 2; i++)
        d_hzn_s[i] = sqrt(2.0 * h_e__meter[i] * a_e__meter);
    const double d_sML__meter = d_hzn_s[0] + d_hzn_s[1];
    const double d_ML__meter = d_hzn__meter[0] + d_hzn__meter[1];
    const double theta_los__rad
        = -max(theta_hzn[0] + theta_hzn[1], -d_ML__meter / a_e__meter);

    double result = DiffractionLoss(
        d__meter,
        d_hzn__meter,
        h_e__meter,
        Z_g,
        a_e__meter,
        delta_h__meter,
        h__meter,
        mode,
        theta_los__rad,
        d_sML__meter,
        f__mhz
    );

    EXPECT_DOUBLE_EQ(result, 14.728185720658667);
}
