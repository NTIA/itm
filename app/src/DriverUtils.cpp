/** @file DriverUtils.cpp
 * Implements various model-agnostic utility functions for the driver
 */
#include "Driver.h"

#ifdef _WIN32
    // Ensure localtime_s is available on Windows
    #ifndef __STDC_LIB_EXT1__
        #define __STDC_LIB_EXT1__
    #endif
    #ifndef __STDC_WANT_LIB_EXT1__
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
#endif

#include <algorithm>  // for std::transform
#include <cctype>     // for std::tolower
#include <cstddef>    // for std::size_t
#include <ctime>      // for localtime_{s,r}, std::{time, time_t, tm, strftime}
#include <iomanip>    // for std::setfill, std::setw
#include <iostream>   // for std::cerr, std::endl
#include <ostream>    // for std::ostream
#include <string>     // for std::stod, std::stoi, std::string

/******************************************************************************
 * Get a string containing the current date and time information.
 * 
 * @return  A localized standard date and time string (locale dependent)
 ******************************************************************************/
std::string GetDatetimeString() {
    std::time_t now = std::time(nullptr);
    struct std::tm localTime;

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    if (localtime_r(&now, &localTime) == nullptr) {
        return "Date and time unknown";
    }
#endif
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%a %b %d %H:%M:%S %Y", &localTime)
        == 0) {
        return "Could not format datetime string";
    }
    return std::string(mbstr);
}

/*******************************************************************************
 * Parse a boolean value read from the input parameter file.
 * 
 * Supports either "true" or "false" (case-insensitive) or "0" or "1"
 * 
 * @param[in]  str    Input file value as string
 * @param[out] value  Input file value converted to bool
 * @return            Return code
 ******************************************************************************/
DrvrReturnCode ParseBoolean(const std::string &str, bool &value) {
    try {
        std::string str_lower = str;
        StringToLower(str_lower);
        if (str_lower == "0" || str_lower == "false") {
            value = false;
        } else if (str_lower == "1" || str_lower == "true") {
            value = true;
        } else {
            return DRVRERR__PARSE;
        }
    } catch (...) {
        return DRVRERR__PARSE;
    }
    return DRVR__SUCCESS;
}

/*******************************************************************************
 * Parse a double value read from the input parameter file
 * 
 * @param[in]  str    Input file value as string
 * @param[out] value  Input file value converted to double
 * @return            Return code
 ******************************************************************************/
DrvrReturnCode ParseDouble(const std::string &str, double &value) {
    try {
        value = std::stod(str);
    } catch (...) {
        // error parsing the input string value
        return DRVRERR__PARSE;
    }

    return DRVR__SUCCESS;
}

/*******************************************************************************
 * Parse an integer value read from the input parameter file
 * 
 * @param[in]  str    Input file value as string
 * @param[out] value  Input file value converted to int
 * @return            Return code
 ******************************************************************************/
DrvrReturnCode ParseInteger(const std::string &str, int &value) {
    try {
        std::size_t pos;
        value = std::stoi(str, &pos, 10);

        // Verify the entire string was parsed
        if (pos != str.size()) {
            return DRVRERR__PARSE;
        }
    } catch (...) {
        // error parsing the input string value
        return DRVRERR__PARSE;
    };

    return DRVR__SUCCESS;
}

/*******************************************************************************
 * Helper function to standardize printing of text labels to file
 * 
 * @param[in] os   Output stream for writing
 * @param[in] lbl  Text message
 ******************************************************************************/
void PrintLabel(std::ostream &os, const std::string &lbl) {
    os << "[" << lbl << "]";
}


/******************************************************************************
 * Convert a string to lowercase.
 * 
 * @param[in, out] str  The string to convert
 ******************************************************************************/
void StringToLower(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), [](const char c) {
        return static_cast<char>(std::tolower(c));
    });
}

/*******************************************************************************
 * Print version information to the specified output stream 
 * 
 * @param[in] os  Output stream for writing; defaults to `std::cout`
 ******************************************************************************/
void Version(std::ostream &os) {
    os << std::setfill('*') << std::setw(55) << "" << std::endl;
    os << "Institute for Telecommunication Sciences - Boulder, CO" << std::endl;
    os << "\tDriver Version: " << DRIVER_VERSION << std::endl;
    os << "\t" << LIBRARY_NAME << " Version: " << LIBRARY_VERSION << std::endl;
    os << "Time: " << GetDatetimeString() << std::endl;
    os << std::setfill('*') << std::setw(55) << "" << std::endl;
}
