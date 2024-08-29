#include "TestUtils.h"

TEST(TestQuickPfl, SimpleTest) {
    double h__meter[2] = {6.0, 5.0};
    double pfl[]= {8.0, 100.0, 1.0, 2.0, 3.0, 5.0, 3.0, 5.0, 3.0, 2.0, 1.0};
    double gamma_e = 1.0 / 7860e3;
    double theta_hzn[2];
    double d_hzn__meter[2];
    double h_e__meter[2];
    double delta_h__meter;
    double d__meter;

    QuickPfl(pfl, gamma_e, h__meter, theta_hzn, d_hzn__meter, h_e__meter, &delta_h__meter, &d__meter);
    EXPECT_DOUBLE_EQ(theta_hzn[0], -0.0011422640014581357);
    EXPECT_DOUBLE_EQ(theta_hzn[1], -0.0010153790362535343);
    EXPECT_DOUBLE_EQ(delta_h__meter, 12.901387666337202);
    EXPECT_DOUBLE_EQ(d_hzn__meter[0], 8764.4253963572828);
    EXPECT_DOUBLE_EQ(d_hzn__meter[1], 7922.7879168398858);
    EXPECT_DOUBLE_EQ(h_e__meter[0], 6.0);
    EXPECT_DOUBLE_EQ(h_e__meter[1], 5.0);
    EXPECT_DOUBLE_EQ(d__meter, pfl[0] * pfl[1]);
}