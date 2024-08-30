#ifndef __ITS_PROPAGATION_ITM_TEST_UTILS_H__
#define __ITS_PROPAGATION_ITM_TEST_UTILS_H__

#include "ITS.Propagation.ITM/ITM.h"

#include <algorithm>  // For all_of, max, min, nth_element
#include <cmath>      // For exp, sqrt
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>  // For accumulate
#include <sstream>  // For istringstream
#include <string>   // For getline, stod
#include <vector>

using namespace ITS::Propagation::ITM;

using std::exp;
using std::max;
using std::sqrt;

#define ABSTOL__DB 0.1;

// Typedefs for loading functions from prior-version DLL
typedef int(__stdcall *itm_p2p_tls_func)(
    double h_tx__meter,
    double h_rx__meter,
    double pfl[],
    int climate,
    double N_0,
    double f__mhz,
    int pol,
    double epsilon,
    double sigma,
    int mdvar,
    double time,
    double location,
    double situation,
    double *A__db,
    long *warnings
);

struct TestParameters {
    int profile_idx;
    double tx_h__m;
    double rx_h__m;
    double epsilon;
    double sigma;
    double en0;
    double f__mhz;
    int polar;
    int climate;
    double time;
    double loc;
    double sit;
    int mdvar;
};

std::string getDataDirectory();
std::vector<TestParameters> readTestParameters(const std::string &filename);
std::vector<std::vector<double>> readProfiles(const std::string &filename);
double calculateMean(const std::vector<double> &values);
double calculateMedian(const std::vector<double> values);
double calculateStdDev(const std::vector<double> &values, double mean);
double calculateMeanDifference(
    const std::vector<double> &values1, const std::vector<double> &values2
);
double calculateStdDevDifference(
    const std::vector<double> &values1,
    const std::vector<double> &values2,
    double mean_diff
);

#endif