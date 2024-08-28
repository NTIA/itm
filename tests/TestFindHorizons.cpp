#include "TestUtils.h"


TEST(TestFindHorizons, TestSymmetricHorizons) {
    // Define test parameters: symmetric terrain profile along path
    double pfl[7] = {4.0, 10.0, 0.0, 10.0, 20.0, 10.0, 0.0};  // Simplified symmetric terrain
    double h__meter[2] = {5.0, 5.0};  // Example terminal heights AGL
    IntermediateValues iv;
    const double a_e__meter = 6371000.0;
    iv.d__km = pfl[1] * pfl[0] * 1e-3;

    FindHorizons(pfl, a_e__meter, h__meter, iv.theta_hzn, iv.d_hzn__meter);

    // Expected equalities for symmetric profile:
    EXPECT_DOUBLE_EQ(iv.theta_hzn[0], iv.theta_hzn[1]);
    EXPECT_DOUBLE_EQ(iv.d_hzn__meter[0], iv.d_hzn__meter[1]);
}

TEST(TestFindHorizons, CheckHorizonInitialization) {
    // Use a terrain profile so small that the for loop never runs.
    // This lets us test the pre-loop function behavior where horizons
    // are initialized.
    double pfl[] = {1.0, 10.0, 2.0, 4.0};
    double h__meter[2] = {1.0, 5.0};
    IntermediateValues iv;
    iv.d__km = pfl[1] * pfl[0] * 1e-3;
    const double a_e__meter = 6371000.0;

    // Duplicate internal FindHorizons initialization
    const double z_tx = pfl[2] + h__meter[0];
    const double z_rx = pfl[3] + h__meter[1];
    const double d_e = 2 * a_e__meter;
    const double theta_hzn_tx = (z_rx - z_tx) / (iv.d__km * 1e3) - (iv.d__km * 1e3) / d_e;
    const double theta_hzn_rx = -(z_rx - z_tx) / (iv.d__km * 1e3) - (iv.d__km * 1e3) / d_e;

    FindHorizons(pfl, a_e__meter, h__meter, iv.theta_hzn, iv.d_hzn__meter);

    // Check initializations
    EXPECT_DOUBLE_EQ(iv.theta_hzn[0], theta_hzn_tx);
    EXPECT_DOUBLE_EQ(iv.theta_hzn[1], theta_hzn_rx);
    EXPECT_EQ(iv.d_hzn__meter[0], iv.d__km * 1e3);
    EXPECT_EQ(iv.d_hzn__meter[1], iv.d__km * 1e3);
}

TEST(TestFindHorizons, TestSmallProfile) {
    // Let the loop run once; check expected values.
    // Inputs are set so that both conditions in the loop are true when it runs.
    double pfl[] = {2.0, 10.0, 2.0, 10.0, 4.0};
    double h__meter[2] = {1.0, 5.0};
    IntermediateValues iv;
    iv.d__km = pfl[0] * pfl[1] * 1e-3;
    const double a_e__meter = 1000000.0;

    const int num_loops = static_cast<int>(pfl[0]) - 1;

    FindHorizons(pfl, a_e__meter, h__meter, iv.theta_hzn, iv.d_hzn__meter);

    // Expected values. Simplified (non-iterative) calculation of 
    // horizon angles requires that the condition in the original loop
    // is true for all iterations.
    double z_tx = pfl[2] + h__meter[0];
    double z_rx = pfl[4] + h__meter[1];
    double d_e = a_e__meter * 2;
    double d_tx = 0.0 + (num_loops * pfl[1]);
    double d_rx = (iv.d__km * 1e3) - (num_loops * pfl[1]);
    double theta_tx = (pfl[num_loops + 2] - z_tx) / d_tx - d_tx / d_e;
    double theta_rx = -(z_rx - pfl[num_loops + 2]) / d_rx - d_rx / d_e;

    EXPECT_DOUBLE_EQ(iv.theta_hzn[0], theta_tx);
    EXPECT_DOUBLE_EQ(iv.theta_hzn[1], theta_rx);
    EXPECT_DOUBLE_EQ(iv.d_hzn__meter[0], d_tx);
    EXPECT_DOUBLE_EQ(iv.d_hzn__meter[1], d_rx);
}