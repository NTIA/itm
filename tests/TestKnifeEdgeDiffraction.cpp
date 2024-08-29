#include "TestUtils.h"

TEST(TestKnifeEdgeDiffraction, SimpleTest) {
    // Configure inputs to simplify numbers and cancel terms
    // This lets us easily evaluate a simple expression below for "v"
    const double d__meter = 6.0;
    const double f__mhz = PI * 47.7;
    const double a_e__meter = 3.0;
    const double theta_los__rad = 1.0;
    double d_hzn__meter[2] = {1.0, 1.0};
    double A_k = KnifeEdgeDiffraction(
        d__meter, f__mhz, a_e__meter, theta_los__rad, d_hzn__meter
    );
    const double v = 0.0795775 * PI * 4.0 / 5.0;
    const double expected = 2 * FresnelIntegral(v);
    EXPECT_DOUBLE_EQ(A_k, expected);
}