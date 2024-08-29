#include "TestUtils.h"

class TestInitializePointToPoint: public testing::Test {
protected:
    // Basic set of inputs to preset tests
    double f__mhz = 3500.0;
    double h_sys__meter = 0.0;
    double N_0 = 301.0;
    int pol = 1;
    double epsilon = 15.0;
    double sigma = 0.005;
    // Outputs
    complex<double> Z_g;
    double gamma_e;
    double N_s;
};

TEST_F(TestInitializePointToPoint, TestSetEffectiveEarthCurvature) {
    InitializePointToPoint(
        f__mhz, h_sys__meter, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s
    );
    const double expected = 157e-9 * (1.0 - 0.04665 * exp(N_0 / 179.3));
    EXPECT_DOUBLE_EQ(gamma_e, expected);
}

TEST_F(TestInitializePointToPoint, TestSetRefractivity) {
    // Case 1: h_sys is zero
    InitializePointToPoint(
        f__mhz, h_sys__meter, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s
    );
    EXPECT_DOUBLE_EQ(N_s, N_0);
    // Case 2: h_sys is non-zero
    h_sys__meter = 1.5;
    InitializePointToPoint(
        f__mhz, h_sys__meter, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s
    );
    const double expected = N_0 * exp(-h_sys__meter / 9460.0);
    EXPECT_DOUBLE_EQ(N_s, expected);
}

TEST_F(TestInitializePointToPoint, TestSetGroundImpedance) {
    // Case 1: horizontal polarization
    pol = 0;
    sigma = f__mhz / 18000;  // Simplifies expression; sets ep_r.imag to 1
    InitializePointToPoint(
        f__mhz, h_sys__meter, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s
    );
    const complex<double> ep_r(epsilon, 1.0);
    complex<double> expected = sqrt(ep_r - 1.0);
    EXPECT_DOUBLE_EQ(Z_g.real(), expected.real());
    EXPECT_DOUBLE_EQ(Z_g.imag(), expected.imag());
    // Case 2: horizontal polarization
    pol = 1;
    expected /= ep_r;
    InitializePointToPoint(
        f__mhz, h_sys__meter, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s
    );
    EXPECT_DOUBLE_EQ(Z_g.real(), expected.real());
    EXPECT_DOUBLE_EQ(Z_g.imag(), expected.imag());
}
