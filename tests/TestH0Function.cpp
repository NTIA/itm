#include "TestUtils.h"

// Duplicate implementation of H0Curve constants for comparisons
const double H0_a[] = {25.0, 80.0, 177.0, 395.0, 705.0};
const double H0_b[] = {24.0, 45.0, 68.0, 80.0, 105.0};

TEST(TestH0Function, TestH0FunctionNoInterpolation) {
    // No interpolation when eta_s is an integer.
    // Try a few values: min, max, and in the middle
    double eta_s = 1.0;
    const double r = 1.0;  // used for all cases

    double expected = H0Curve(static_cast<int>(eta_s) - 1, r);
    EXPECT_DOUBLE_EQ(H0Function(r, eta_s), expected);

    eta_s = 3.0;
    expected = H0Curve(static_cast<int>(eta_s) - 1, r);
    EXPECT_DOUBLE_EQ(H0Function(r, eta_s), expected);

    eta_s = 5.0;
    expected = H0Curve(static_cast<int>(eta_s) - 1, r);
    EXPECT_DOUBLE_EQ(H0Function(r, eta_s), expected);
}

TEST(TestH0Function, TestH0FunctionEtaOutOfRange) {
    // eta_s too small or too large should clip the value of eta_s
    // to the range 1 <= eta_s <= 5
    const double r = 1.0;
    EXPECT_DOUBLE_EQ(H0Function(r, 1.0), H0Function(r, -100.0));
    EXPECT_DOUBLE_EQ(H0Function(r, 1.0), H0Function(r, 0.0));
    EXPECT_DOUBLE_EQ(H0Function(r, 1.0), H0Function(r, 0.9));
    EXPECT_DOUBLE_EQ(H0Function(r, 5.0), H0Function(r, 5.1));
    EXPECT_DOUBLE_EQ(H0Function(r, 5.0), H0Function(r, 500.0));
}

TEST(TestH0Function, TestH0FunctionInterpolation) {
    // Interplation occurs when eta_s is not an integer
    const double r = 1.0;
    const double eta_s = 3.5;
    const int i = static_cast<int>(eta_s);
    double result = H0Curve(i - 1, r);
    result *= 1.0 - (eta_s - i);
    result += (eta_s - i) * H0Curve(i, r);
    EXPECT_DOUBLE_EQ(H0Function(r, eta_s), result);
}

TEST(TestH0Function, TestH0CurveREquals1) {
    // Test case: r=1, try all valid values of j
    for (int j = 0; j < 5; j++) {
        // Expected value from simplified equation with r=1
        double expected = 10 * log10(1 + H0_a[j] + H0_b[j]);
        EXPECT_DOUBLE_EQ(H0Curve(j, 1.0), expected);
    }
}

TEST(TestH0Function, TestH0CurveJEquals1) {
    // Test case: j=1, try a handful of values for r
    for (double r = 1.0; r < 5; r++) {
        // Expected value from equivalent equation with j=1
        double expected
            = 10 * log10(1 + pow(r, -4) * H0_a[1] + pow(r, -2) * H0_b[1]);
        EXPECT_DOUBLE_EQ(H0Curve(1, r), expected);
    }
}