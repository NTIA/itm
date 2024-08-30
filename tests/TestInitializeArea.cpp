#include "ITS.Propagation.ITM/Enums.h"
#include "TestUtils.h"

class TestInitializeArea: public testing::Test {
protected:
    // Basic set of inputs to preset tests
    int site_criteria[2];
    const double gamma_e = 157e-9 * (1.0 - 0.04665 * exp(301.0 / 179.3));
    ;
    const double delta_h__meter = 1.0;
    double h__meter[2] = {10.0, 4.0};  // Low RX triggers a condition
    // Outputs
    double h_e__meter[2];
    double d_hzn__meter[2];
    double theta_hzn[2];
    // internal testing variables (e.g., for comparison)
    double d_Ls;
    double expected_h_e;
    double expected_d_hzn;
    double expected_theta;
};

// Helper function to calculate d_Ls
double get_d_Ls(const double h_e, const double gamma_e) {
    return sqrt(2.0 * h_e / gamma_e);
}

// Helper function to get h_e for careful or very careful siting
double get_h_e(const double h, const double B, const double delta_h) {
    return h + (1.0 + B) * exp(-min(20.0, 2.0 * h / max(1e-3, delta_h)));
}

// Helper function to calculate horizon distance. Hard-codes H_3
double get_d_hzn(const double h_e, const double d_Ls, const double delta_h) {
    return d_Ls * exp(-0.07 * sqrt(delta_h / max(h_e, 5.0)));
}

// Helper function duplicating the calculation of theta_hzn
double get_theta_hzn(
    const double delta_h,
    const double d_Ls,
    const double d_hzn,
    const double h_e
) {
    // Note: EXPECT_NEAR is used to compare these results with ITM's
    // due to small differences due to floating point arithmetic.
    return (0.65 * delta_h * (d_Ls / d_hzn - 1.0) - 2.0 * h_e) / d_Ls;
}

TEST_F(TestInitializeArea, TestRandomSiting) {
    site_criteria[0] = SITING_CRITERIA__RANDOM;
    site_criteria[1] = SITING_CRITERIA__RANDOM;
    InitializeArea(
        site_criteria,
        gamma_e,
        delta_h__meter,
        h__meter,
        h_e__meter,
        d_hzn__meter,
        theta_hzn
    );
    for (int i = 0; i < 2; i++) {
        EXPECT_DOUBLE_EQ(
            h_e__meter[i], h__meter[i]
        );  // Effective heights are actual heights for random siting
        d_Ls = get_d_Ls(h_e__meter[i], gamma_e);
        expected_d_hzn = get_d_hzn(h_e__meter[i], d_Ls, delta_h__meter);
        EXPECT_DOUBLE_EQ(d_hzn__meter[i], expected_d_hzn);
        expected_theta
            = get_theta_hzn(delta_h__meter, d_Ls, expected_d_hzn, h__meter[i]);
        EXPECT_NEAR(theta_hzn[i], expected_theta, 0.000001);
    }
}

TEST_F(TestInitializeArea, TestCarefulSiting) {
    site_criteria[0] = SITING_CRITERIA__CAREFUL;
    site_criteria[1] = SITING_CRITERIA__CAREFUL;
    InitializeArea(
        site_criteria,
        gamma_e,
        delta_h__meter,
        h__meter,
        h_e__meter,
        d_hzn__meter,
        theta_hzn
    );
    double B[2] = {4.0, 4.0 * sin(0.1 * PI * h__meter[1])};
    for (int i = 0; i < 2; i++) {
        expected_h_e = get_h_e(h__meter[i], B[i], delta_h__meter);
        EXPECT_DOUBLE_EQ(h_e__meter[i], expected_h_e);
        d_Ls = get_d_Ls(h_e__meter[i], gamma_e);
        expected_d_hzn = get_d_hzn(h_e__meter[i], d_Ls, delta_h__meter);
        EXPECT_DOUBLE_EQ(d_hzn__meter[i], expected_d_hzn);
        expected_theta
            = get_theta_hzn(delta_h__meter, d_Ls, expected_d_hzn, h__meter[i]);
        EXPECT_NEAR(theta_hzn[i], expected_theta, 0.000001);
    }
}

TEST_F(TestInitializeArea, TestVeryCarefulSiting) {
    site_criteria[0] = SITING_CRITERIA__VERY_CAREFUL;
    site_criteria[1] = SITING_CRITERIA__VERY_CAREFUL;
    InitializeArea(
        site_criteria,
        gamma_e,
        delta_h__meter,
        h__meter,
        h_e__meter,
        d_hzn__meter,
        theta_hzn
    );
    double B[2] = {9.0, 9.0 * sin(0.1 * PI * h__meter[1])};
    for (int i = 0; i < 2; i++) {
        expected_h_e = get_h_e(h__meter[i], B[i], delta_h__meter);
        EXPECT_DOUBLE_EQ(h_e__meter[i], expected_h_e);
        d_Ls = get_d_Ls(h_e__meter[i], gamma_e);
        expected_d_hzn = get_d_hzn(h_e__meter[i], d_Ls, delta_h__meter);
        EXPECT_DOUBLE_EQ(d_hzn__meter[i], expected_d_hzn);
        expected_theta
            = get_theta_hzn(delta_h__meter, d_Ls, expected_d_hzn, h__meter[i]);
        EXPECT_NEAR(theta_hzn[i], expected_theta, 0.000001);
    }
}