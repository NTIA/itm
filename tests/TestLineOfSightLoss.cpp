#include "TestUtils.h"

class TestLineOfSightLoss: public testing::Test {
protected:
    double d__meter = 5e3;
    double h_e__meter[2] = {1.0, 2.0};
    const std::complex<double> Z_g {0.2, 0.0};
    const double delta_h__meter = 10.0;
    const double M_d = 0.05;
    const double A_d0 = 0.0;
    const double d_sML__meter = 100e3;
    double f__mhz = 47.7;  // sets wn to 1
    double A_los;
};

TEST_F(TestLineOfSightLoss, TestSmallInternalQ) {
    // Sets the inputs such that the internal conditions are:
    // (q < 0.25 || q < sin_psi) = TRUE
    // (delta_phi > PI / 2) = TRUE
    f__mhz = 47.7 * 1e12;  // forces exponent in R_e to be -10
    A_los = LineOfSightLoss(
        d__meter,
        h_e__meter,
        Z_g,
        delta_h__meter,
        M_d,
        A_d0,
        d_sML__meter,
        f__mhz
    );
    EXPECT_DOUBLE_EQ(A_los, 249.99999994754504);
}

TEST_F(TestLineOfSightLoss, TestLargeInternalQ) {
    // Sets the inputs such that the internal conditions are:
    // (q < 0.25 || q < sin_psi) = FALSE
    // (delta_phi > PI / 2) = FALSE
    A_los = LineOfSightLoss(
        d__meter,
        h_e__meter,
        Z_g,
        delta_h__meter,
        M_d,
        A_d0,
        d_sML__meter,
        f__mhz
    );
    EXPECT_DOUBLE_EQ(A_los, 44.454186433741803);
}
