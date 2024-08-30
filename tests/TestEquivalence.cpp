#include "ITS.Propagation.ITM/Errors.h"
#include "TestUtils.h"

// This file allows for testing of equivalence and comparison of inequivalence
// betweeen the current (linked) version of ITM and some previous version of
// ITM available as "itm.dll" in {REPO_ROOT}/tests/data/
// Currently this is only supported on Windows.

#include <windows.h>  // For FreeLibrary, LoadLibrary, GetProcAddress

// TODO : Add area mode tests

#define PRINT << std::endl << "[          ] " <<

class TestEquivalence: public ::testing::Test {
protected:
    void SetUp() override {
        parameters = readTestParameters("real_medians.csv");
        profiles = readProfiles("combined_profiles.csv");

        // Ensure test data was loaded
        EXPECT_NE(parameters.size(), 0);
        EXPECT_NE(profiles.size(), 0);

        // Load ITM DLL
        std::string dataDir = getDataDirectory();
        hLib = LoadLibrary((dataDir + "itm.dll").c_str());
        ASSERT_NE(hLib, nullptr) << "Failed to load DLL";
        ITM_PREV_P2P_TLS = reinterpret_cast<itm_p2p_tls_func>(
            GetProcAddress(hLib, "ITM_P2P_TLS")
        );
        ASSERT_NE(ITM_PREV_P2P_TLS, nullptr)
            << "Failed to get P2P Mode function address";
    }

    void TearDown() override {
        // Unload the DLL
        if (hLib) {
            FreeLibrary(hLib);
        }
    }

    // Vectors to hold test data
    std::vector<TestParameters> parameters;
    std::vector<std::vector<double>> profiles;

    // Model inputs and outputs
    double h_tx__meter;
    double h_rx__meter;
    int climate;
    double N_0;
    double f__mhz;
    int pol;
    double epsilon;
    double sigma;
    int mdvar;
    double time;
    double A__db = 0.0;
    long warnings = 0;
    int rtn;

    // DLL Loading
    HMODULE hLib;
    itm_p2p_tls_func ITM_PREV_P2P_TLS;
};

TEST_F(TestEquivalence, DoesItRunItm) {
    TestParameters p = parameters[0];
    rtn = ITM_P2P_TLS(
        p.tx_h__m,
        p.rx_h__m,
        profiles[p.profile_idx].data(),
        p.climate,
        p.en0,
        p.f__mhz,
        p.polar,
        p.epsilon,
        p.sigma,
        p.mdvar,
        p.time,
        p.loc,
        p.sit,
        &A__db,
        &warnings
    );
    EXPECT_EQ(rtn, SUCCESS_WITH_WARNINGS);
}

TEST_F(TestEquivalence, DoesItRunItmPrevious) {
    TestParameters p = parameters[0];
    rtn = ITM_PREV_P2P_TLS(
        p.tx_h__m,
        p.rx_h__m,
        profiles[p.profile_idx].data(),
        p.climate,
        p.en0,
        p.f__mhz,
        p.polar,
        p.epsilon,
        p.sigma,
        p.mdvar,
        p.time,
        p.loc,
        p.sit,
        &A__db,
        &warnings
    );
    EXPECT_EQ(rtn, SUCCESS_WITH_WARNINGS);
}

// Run all test cases and get stats on results.
// This does not perform any assertions, and therefore never fails.
TEST_F(TestEquivalence, CompareImplementations) {
    // New variables to store ITM results
    double prev_A__db = 0.0;
    int prev_rtn;
    long prev_warnings = 0;
    // Vectors to store all results
    std::vector<double> itm_predictions;
    std::vector<double> prev_predictions;
    for (const auto &p : parameters) {
        // Call the models
        rtn = ITM_P2P_TLS(
            p.tx_h__m,
            p.rx_h__m,
            profiles[p.profile_idx].data(),
            p.climate,
            p.en0,
            p.f__mhz,
            p.polar,
            p.epsilon,
            p.sigma,
            p.mdvar,
            p.time,
            p.loc,
            p.sit,
            &A__db,
            &warnings
        );
        itm_predictions.push_back(A__db);

        prev_rtn = ITM_PREV_P2P_TLS(
            p.tx_h__m,
            p.rx_h__m,
            profiles[p.profile_idx].data(),
            p.climate,
            p.en0,
            p.f__mhz,
            p.polar,
            p.epsilon,
            p.sigma,
            p.mdvar,
            p.time,
            p.loc,
            p.sit,
            &prev_A__db,
            &prev_warnings
        );
        prev_predictions.push_back(prev_A__db);
    }

    // Collect statistics on ITM results
    double itm_mean = calculateMean(itm_predictions);
    double itm_median = calculateMedian(itm_predictions);
    double itm_stddev = calculateStdDev(itm_predictions, itm_mean);

    // Collect statistics on ITM (Previous .DLL) results
    double prev_mean = calculateMean(prev_predictions);
    double prev_median = calculateMedian(prev_predictions);
    double prev_stddev = calculateStdDev(prev_predictions, prev_mean);

    // Collect stats on differences
    double diff_mean
        = calculateMeanDifference(prev_predictions, itm_predictions);
    double diff_stddev = calculateStdDevDifference(
        prev_predictions, itm_predictions, diff_mean
    );

    std::cerr << "[          ] Statistics:" PRINT "ITM UPDATE Loss (dB)" PRINT
                 "  N: "
              << itm_predictions.size() PRINT "  Mean: "
              << itm_mean PRINT "  Median: "
              << itm_median PRINT "  StdDev: "
              << itm_stddev PRINT "ITM v1.4.0 Loss (dB)" PRINT "  N: "
              << prev_predictions.size() PRINT "  Mean: "
              << prev_mean PRINT "  Median: " << prev_median PRINT "  StdDev: "
              << prev_stddev PRINT
        "Differences (ITM v1.4.0 Loss - ITM-UPDATE Loss)" PRINT "  Mean: "
              << diff_mean PRINT "  StdDev: " << diff_stddev << std::endl;
}
