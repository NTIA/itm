#include "ITS.Propagation.ITM/Enums.h"
#include "ITS.Propagation.ITM/Errors.h"
#include "ITS.Propagation.ITM/Warnings.h"
#include "TestUtils.h"

// TODO Add area mode tests
// todo ADD checks for h_sys calculation, d__meter calculation, etc. which was migrated to InitializeP2P in Midband
class TestLongleyRice: public testing::Test {
protected:
    void SetUp() override {
        A_ref__db = 0.0;
        warnings = 0x0000;
        propMode = MODE__NOT_SET;
        rtn = -1;
    }
    double theta_hzn__rad[2] = {-1.9, -0.8};
    const double f__mhz = 3500.0;
    const complex<double> Z_g = complex<double>(0.2, 0.0);
    double d_hzn__meter[2] = {15e3, 7e3};
    double h_e__meter[2] = {15.0, 5.0};
    const double gamma_e = 1.0 / 7860e3;
    const double N_s = 252.0;
    const double delta_h__meter = 3.5;
    double h__meter[2] = {15.0, 5.0};
    double d__meter = 10000.0;
    int mode = MODE__P2P;
    double A_ref__db;
    long warnings;
    int propMode;
    int rtn;
};


TEST_F(TestLongleyRice, TestLineOfSightPath1) {
    // Arbitrary test case in the LOS region
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(propMode, MODE__LINE_OF_SIGHT);
    EXPECT_DOUBLE_EQ(A_ref__db, 4.2910433148599818);
}

TEST_F(TestLongleyRice, TestDiffractionPath1) {
    // Arbitrary test case in the diffraction region
    d__meter = 25e3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(propMode, MODE__DIFFRACTION);
    EXPECT_DOUBLE_EQ(A_ref__db, 23.008174136481173);
}

TEST_F(TestLongleyRice, TestTroposcatterPath1) {
    // Abitrary test case in the troposcatter region
    d__meter = 10e6 + 1;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(propMode, MODE__TROPOSCATTER);
    EXPECT_DOUBLE_EQ(A_ref__db, 10257.44380116946);
}

TEST_F(TestLongleyRice, TestTxHorizonAngleWarning) {
    theta_hzn__rad[0] = 201e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__TX_HORIZON_ANGLE);
}

TEST_F(TestLongleyRice, TestRxHorizonAngleWarning) {
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 201e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__RX_HORIZON_ANGLE);
}

TEST_F(TestLongleyRice, TestTxHorizonDistanceWarning1) {
    double d_hzn_s = sqrt(2.0 * h_e__meter[0] / gamma_e);
    d_hzn__meter[0] = (0.1 * d_hzn_s) - 1;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__TX_HORIZON_DISTANCE_1);
}

TEST_F(TestLongleyRice, TestRxHorizonDistanceWarning1) {
    double d_hzn_s = sqrt(2.0 * h_e__meter[1] / gamma_e);
    d_hzn__meter[1] = (0.1 * d_hzn_s) - 1;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__RX_HORIZON_DISTANCE_1);
}

TEST_F(TestLongleyRice, TestTxHorizonDistanceWarning2) {
    double d_hzn_s = sqrt(2.0 * h_e__meter[0] / gamma_e);
    d_hzn__meter[0] = (3.0 * d_hzn_s) + 1;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__TX_HORIZON_DISTANCE_2);
}

TEST_F(TestLongleyRice, TestRxHorizonDistanceWarning2) {
    double d_hzn_s = sqrt(2.0 * h_e__meter[1] / gamma_e);
    d_hzn__meter[1] = (3.0 * d_hzn_s) + 1;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__RX_HORIZON_DISTANCE_2);
}

TEST_F(TestLongleyRice, TestPathDistanceWarning1) {
    // Test a path distance which is below d_min and below 1km
    double d_min = abs(h_e__meter[0] - h_e__meter[1]) / 200e-3; // = 50
    d__meter = d_min - 1;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    long expected = WARN__PATH_DISTANCE_TOO_SMALL_1;
    expected |= WARN__PATH_DISTANCE_TOO_SMALL_2;
    EXPECT_EQ(warnings, expected);
}

TEST_F(TestLongleyRice, TestPathDistanceWarning2) {
    // Test a path distance which is below d_min but above 1km
    h_e__meter[0] = 206;
    double d_min = abs(h_e__meter[0] - h_e__meter[1]) / 200e-3; // = 1005
    d__meter = d_min - 1;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__PATH_DISTANCE_TOO_SMALL_1);
}

TEST_F(TestLongleyRice, TestPathDistanceWarning3) {
    // Test a path distance which is greater than 1000 km
    d__meter = 1001e3;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, WARN__PATH_DISTANCE_TOO_BIG_1);
}

TEST_F(TestLongleyRice, TestPathDistanceWarning4) {
    // Test a path distance which is greater than 2000 km
    d__meter = 2001e3;
    theta_hzn__rad[0] = 199e-3;
    theta_hzn__rad[1] = 199e-3;
    rtn = LongleyRice(
        theta_hzn__rad,
        f__mhz,
        Z_g,
        d_hzn__meter,
        h_e__meter,
        gamma_e,
        N_s,
        delta_h__meter,
        h__meter,
        d__meter,
        mode,
        &A_ref__db,
        &warnings,
        &propMode
    );
    EXPECT_EQ(rtn, SUCCESS);
    long expected = WARN__PATH_DISTANCE_TOO_BIG_1;
    expected |= WARN__PATH_DISTANCE_TOO_BIG_2;
    EXPECT_EQ(warnings, expected);
}
