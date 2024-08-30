#include "TestUtils.h"

void appendDirectorySep(std::string &str) {
#ifdef _WIN32
    str += "\\";
#else
    str += "/";
#endif
}

std::string getDataDirectory() {
    std::string dataDir(__FILE__);
    dataDir.resize(dataDir.find_last_of("/\\"));
    appendDirectorySep(dataDir);
    dataDir += "data";
    appendDirectorySep(dataDir);
    return dataDir;
}

std::vector<TestParameters> readTestParameters(const std::string &filename) {
    std::vector<TestParameters> testParams;
    std::string dataDir = getDataDirectory();
    std::ifstream file(dataDir + filename);
    std::string line;
    TestParameters d;  // struct to store data from a single line of CSV
    char c;            // single-character representing the comma (delimiter)
    std::getline(file, line);  // Skip first line of CSV
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (iss >> d.profile_idx >> c >> d.tx_h__m >> c >> d.rx_h__m >> c
            >> d.epsilon >> c >> d.sigma >> c >> d.en0 >> c >> d.f__mhz >> c
            >> d.polar >> c >> d.climate >> c >> d.time >> c >> d.loc >> c
            >> d.sit >> c >> d.mdvar) {
            testParams.push_back(d);
        }
    }
    return testParams;
}

std::vector<std::vector<double>> readProfiles(const std::string &filename) {
    std::vector<std::vector<double>> profiles;
    std::string dataDir = getDataDirectory();
    std::ifstream file(dataDir + filename);
    std::string line;

    while (std::getline(file, line)) {
        // Skip blank lines
        if (line.empty() || std::all_of(line.begin(), line.end(), isspace)) {
            continue;
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<double> profile;

        // Read profile values
        while (std::getline(iss, token, ',')) {
            profile.push_back(std::stod(token));
        }

        profiles.push_back(profile);
    }

    return profiles;
}

double calculateMean(const std::vector<double> &values) {
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double calculateMedian(std::vector<double> values) {
    std::nth_element(
        values.begin(), values.begin() + values.size() / 2, values.end()
    );
    return values[values.size() / 2];
}

double calculateStdDev(const std::vector<double> &values, double mean) {
    double sum = 0.0;
    for (const auto &value : values) {
        sum += (value - mean) * (value - mean);
    }
    return std::sqrt(sum / values.size());
}

// Function to calculate mean of differences
double calculateMeanDifference(
    const std::vector<double> &values1, const std::vector<double> &values2
) {
    double sum = 0.0;
    for (size_t i = 0; i < values1.size(); ++i) {
        sum += (values1[i] - values2[i]);
    }
    return sum / values1.size();
}

// Function to calculate standard deviation of differences
double calculateStdDevDifference(
    const std::vector<double> &values1,
    const std::vector<double> &values2,
    double mean_diff
) {
    double sum = 0.0;
    for (size_t i = 0; i < values1.size(); ++i) {
        double diff = values1[i] - values2[i];
        sum += (diff - mean_diff) * (diff - mean_diff);
    }
    return std::sqrt(sum / values1.size());
}