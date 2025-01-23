/** @file ReturnCodes.cpp
 * Maps status messages to library return codes
 */

#include "ITM.h"

#ifdef _WIN32
    // Ensure strcpy_s is available on Windows
    #ifndef __STDC_LIB_EXT1__
        #define __STDC_LIB_EXT1__
    #endif
    #ifndef __STDC_WANT_LIB_EXT1__
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
#endif

#include <cstring>        // for strcpy_s
#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map

namespace ITS {
namespace Propagation {
namespace ITM {
/*******************************************************************************
 * Get an error message string from a return code.
 * 
 * @param[in] code  Integer return code.
 * @return          A status message corresponding to the input code.
 ******************************************************************************/
std::string GetReturnStatus(int code) {
    static const std::unordered_map<ReturnCode, std::string> messages
        = {{SUCCESS, "Successful execution without warnings"},
           {SUCCESS_WITH_WARNINGS, "Successful execution with warnings"},
           {ERROR__TX_TERMINAL_HEIGHT, "TX terminal height is out of range"},
           {ERROR__RX_TERMINAL_HEIGHT, "RX terminal height is out of range"},
           {ERROR__INVALID_RADIO_CLIMATE, "Invalid value for radio climate"},
           {ERROR__INVALID_TIME, "Time percentage is out of range"},
           {ERROR__INVALID_LOCATION, "Location percentage is out of range"},
           {ERROR__INVALID_SITUATION, "Situation percentage is out of range"},
           {ERROR__INVALID_CONFIDENCE, "Confidence percentage is out of range"},
           {ERROR__INVALID_RELIABILITY, "Reliability percentage is out of range"
           },
           {ERROR__REFRACTIVITY, "Refractivity is out of range"},
           {ERROR__FREQUENCY, "Frequency is out of range"},
           {ERROR__POLARIZATION, "Invalid value for polarization"},
           {ERROR__EPSILON, "Epsilon is out of range"},
           {ERROR__SIGMA, "Sigma is out of range"},
           {ERROR__GROUND_IMPEDANCE,
            "The imaginary portion of the complex impedance is larger than the "
            "real portion"},
           {ERROR__MDVAR, "Invalid value for mode of variability"},
           {ERROR__EFFECTIVE_EARTH,
            "Internally computed effective earth radius is invalid"},
           {ERROR__PATH_DISTANCE, "Path distance is out of range"},
           {ERROR__DELTA_H,
            "Delta H (terrain irregularity parameter) is out of range"},
           {ERROR__TX_SITING_CRITERIA, "Invalid value for TX siting criteria"},
           {ERROR__RX_SITING_CRITERIA, "Invalid value for RX siting criteria"},
           {ERROR__SURFACE_REFRACTIVITY_SMALL,
            "Internally computed surface refractivity value is too small"},
           {ERROR__SURFACE_REFRACTIVITY_LARGE,
            "Internally computed surface refractivity value is too large"}};
    // Construct status message
    std::string msg = LIBRARY_NAME;
    msg += " v";
    msg += LIBRARY_VERSION;
    if (code == SUCCESS) {
        msg += " Status: ";
    } else {
        msg += " Error: ";
    }

    auto it = messages.find(static_cast<ReturnCode>(code));
    if (it != messages.end()) {
        msg += it->second;
    } else {
        msg += "Undefined return code";
    }
    return msg;
}

/*******************************************************************************
 * Get an error message string (as C-style string) from a return code.
 * 
 * @param[in] code  Integer return code.
 * @return          A status message corresponding to the input code.
 ******************************************************************************/
char *GetReturnStatusCharArray(const int code) {
    const std::string msg = GetReturnStatus(code);
    char *c_msg = new char[msg.size() + 1];
#ifdef _WIN32
    strcpy_s(c_msg, msg.size() + 1, msg.c_str());
#else
    strcpy(c_msg, msg.c_str());
#endif
    return c_msg;
}

/*******************************************************************************
 * Free the memory allocated by GetReturnStatusCharArray
 * 
 * @param[in] c_msg  The status message C-style string to delete
 ******************************************************************************/
void FreeReturnStatusCharArray(char *c_msg) {
    delete[] c_msg;
}

}  // namespace ITM
}  // namespace Propagation
}  // namespace ITS