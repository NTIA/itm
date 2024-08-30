#include "ITS.Propagation.ITM/Enums.h"
#include "ITS.Propagation.ITM/Errors.h"
#include "ITS.Propagation.ITM/Warnings.h"
#include "TestUtils.h"


class TestValidateInputs: public testing::Test {
    // Initialize valid inputs
protected:
    double h_tx__meter = 15.0;
    double h_rx__meter = 15.0;
    int climate = CLIMATE__DESERT;
    double time = 50.0;
    double location = 50.0;
    double situation = 50.0;
    double N_0 = 300.0;
    double f__mhz = 1000.0;
    int pol = POLARIZATION__VERTICAL;
    double epsilon = 15;
    double sigma = 0.005;
    int mdvar = MDVAR__BROADCAST_MODE;
    long warnings = 0;
    int rtn = 0;
};

TEST_F(TestValidateInputs, TestValidInputs) {
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, SUCCESS);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestValidateInputs, TestWarnTxHeight) {
    h_tx__meter = 0.9;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(warnings, WARN__TX_TERMINAL_HEIGHT);
    warnings = 0;

    h_tx__meter = 1000.1;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(warnings, WARN__TX_TERMINAL_HEIGHT);
}

TEST_F(TestValidateInputs, TestWarnRxHeight) {
    h_rx__meter = 0.9;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(warnings, WARN__RX_TERMINAL_HEIGHT);
    warnings = 0;

    h_rx__meter = 1000.1;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(warnings, WARN__RX_TERMINAL_HEIGHT);
}

TEST_F(TestValidateInputs, TestTerminalHeightErrors) {
    h_tx__meter = 0.49;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__TX_TERMINAL_HEIGHT);

    h_tx__meter = 3000.1;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__TX_TERMINAL_HEIGHT);

    h_tx__meter = 10.0;
    h_rx__meter = 0.49;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__RX_TERMINAL_HEIGHT);

    h_rx__meter = 3000.1;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__RX_TERMINAL_HEIGHT);
}

TEST_F(TestValidateInputs, TestRadioClimateErrors) {
    // Valid values are integers 1 to 7 inclusive
    climate = 0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_RADIO_CLIMATE);

    climate = 8;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_RADIO_CLIMATE);
}

TEST_F(TestValidateInputs, TestEnvironmentErrors) {
    // Checks errors returned for invalid N_0, epsilon, and sigma
    N_0 = 249.9;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__REFRACTIVITY);

    N_0 = 400.1;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__REFRACTIVITY);

    N_0 = 300.0;
    epsilon = 0.9;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__EPSILON);

    epsilon = 15.0;
    sigma = 0.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__SIGMA);
}

TEST_F(TestValidateInputs, TestRfErrors) {
    // Checks errors for invalid frequency and polarization
    f__mhz = 19.9;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__FREQUENCY);
    f__mhz = 20000.1;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__FREQUENCY);
    f__mhz = 1000.0;
    // Valid polarizations are 0 and 1
    pol = 2;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__POLARIZATION);
}

TEST_F(TestValidateInputs, TestMdvarErrors) {
    // Error should be raised for multiple ranges of mdvar values
    int bad_mdvars[8] = {-1, 4, 9, 14, 19, 24, 29, 34};
    for (int i = 0; i < 8; i++) {
        rtn = ValidateInputs(
            h_tx__meter,
            h_rx__meter,
            climate,
            time,
            location,
            situation,
            N_0,
            f__mhz,
            pol,
            epsilon,
            sigma,
            bad_mdvars[i],
            &warnings
        );
        EXPECT_EQ(rtn, ERROR__MDVAR);
    }
}

TEST_F(TestValidateInputs, TestVariabilityErrors) {
    time = 0.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_TIME);
    time = 100.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_TIME);
    time = 50.0;

    location = 0.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_LOCATION);
    location = 100.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_LOCATION);
    location = 50.0;

    situation = 0.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    EXPECT_EQ(rtn, ERROR__INVALID_SITUATION);
    situation = 100.0;
    rtn = ValidateInputs(
        h_tx__meter,
        h_rx__meter,
        climate,
        time,
        location,
        situation,
        N_0,
        f__mhz,
        pol,
        epsilon,
        sigma,
        mdvar,
        &warnings
    );
    ;
    EXPECT_EQ(rtn, ERROR__INVALID_SITUATION);
}