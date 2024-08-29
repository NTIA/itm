#include "TestUtils.h"

TEST(TestSmoothEarthDiffraction, TestSmoothEarthDiffraction) {
    const double d__meter = 18e3;
    const double f__mhz = 3500.0;
    const double a_e__meter = 7850e3;
    const double theta_los__rad = 0.002;
    double d_hzn__meter[2] = {8e3, 8e3};
    double h_e__meter[2] = {15.0, 3.0};
    const std::complex<double> Z_g(0.2, 0.0);
    double result = SmoothEarthDiffraction(
        d__meter,
        f__mhz,
        a_e__meter,
        theta_los__rad,
        d_hzn__meter,
        h_e__meter,
        Z_g
    );
    EXPECT_DOUBLE_EQ(result, 25.355962945991564);
}
TEST(TestSmoothEarthDiffraction, TestHeightFunction) {
    double x;
    double K;
    double result;
    // Test case matrix to exercise each internal condition:
    // x < 200 && K >= 1e-5 && x*(-log(K)^3) <= 5495
    x = 150.0;
    K = 1.0;
    result = 2.5e-5 * pow(x, 2) / K - 8.686 * -log(K) - 15.0;
    EXPECT_DOUBLE_EQ(HeightFunction(x, K), result);
    // 1 < x < 200 && K < 1e-5
    K = 1e-6;
    result = 17.372 * log(x) - 117.0;
    EXPECT_DOUBLE_EQ(HeightFunction(x, K), result);
    // x <= 1 && K < 1e-5
    x = 0.9;
    result = -117.0;
    EXPECT_DOUBLE_EQ(HeightFunction(x, K), result);
    // 200 <= x < 2000
    x = 250.0;
    result = 0.05751 * x - 4.343 * log(x);
    double w = 0.0134 * x * exp(-0.005 * x);
    result = (1.0 - w) * result + w * (17.372 * log(x) - 117.0);
    EXPECT_DOUBLE_EQ(HeightFunction(x, K), result);
    EXPECT_DOUBLE_EQ(
        HeightFunction(x, K * 100.0), result
    );  // K irrelevant for x >= 200
    // x >= 2000
    x = 2001.0;
    result = 0.05751 * x - 4.343 * log(x);
    EXPECT_DOUBLE_EQ(
        HeightFunction(x, K), result
    );  // K irrelevant for x >= 200
}