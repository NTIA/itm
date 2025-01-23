/** @file PropLibTemplate.h
 * Interface header for this library
 * // TODO-TEMPLATE: Rename this file to your ${LIB_NAME}
 */
#pragma once

#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map

// TODO-TEMPLATE: This header should provide EVERYTHING needed to interface
// with the shared library, without needing to include other headers.

namespace ITS {
// TODO-TEMPLATE: Use your library's namespace

// Define cross-platform PROPLIB_API to export functions
#ifndef DOXYGEN_SHOULD_SKIP
    #ifndef PROPLIB_API
        #ifdef _WIN32
            #define PROPLIB_API extern "C" __declspec(dllexport)
        #else
            #define PROPLIB_API extern "C"
        #endif
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Enums

/*******************************************************************************
 * Return Codes defined by this software (0-127)
 ******************************************************************************/
// clang-format off
enum ReturnCode {
    SUCCESS = 0,                               /**< Successful execution without warnings */
    SUCCESS_WITH_WARNINGS = 1,                 /**< Successful execution with warnings */
    ERROR__TX_TERMINAL_HEIGHT = 1000,          /**< TX terminal height is out of range */
    ERROR__RX_TERMINAL_HEIGHT = 1001,          /**< RX terminal height is out of range */
    ERROR__INVALID_RADIO_CLIMATE = 1002,       /**< Invalid value for radio climate */
    ERROR__INVALID_TIME = 1003,                /**< Time percentage is out of range */
    ERROR__INVALID_LOCATION = 1004,            /**< Location percentage is out of range */
    ERROR__INVALID_SITUATION = 1005,           /**< Situation percentage is out of range */
    ERROR__INVALID_CONFIDENCE = 1006,          /**< Confidence percentage is out of range */
    ERROR__INVALID_RELIABILITY = 1007,         /**< Reliability percentage is out of range */
    ERROR__REFRACTIVITY = 1008,                /**< Refractivity is out of range */
    ERROR__FREQUENCY = 1009,                   /**< Frequency is out of range */
    ERROR__POLARIZATION = 1010,                /**< Invalid value for polarization */
    ERROR__EPSILON = 1011,                     /**< Epsilon is out of range */
    ERROR__SIGMA = 1012,                       /**< Sigma is out of range */
    ERROR__GROUND_IMPEDANCE = 1013,            /**< The imaginary portion of the complex impedance is larger than the real portion */
    ERROR__MDVAR = 1014,                       /**< Invalid value for mode of variability */
    ERROR__EFFECTIVE_EARTH = 1016,             /**< Internally computed effective earth radius is invalid */
    ERROR__PATH_DISTANCE = 1017,               /**< Path distance is out of range */
    ERROR__DELTA_H = 1018,                     /**< Delta H (terrain irregularity parameter) is out of range */
    ERROR__TX_SITING_CRITERIA = 1019,          /**< Invalid value for TX siting criteria */
    ERROR__RX_SITING_CRITERIA = 1020,          /**< Invalid value for RX siting criteria */
    ERROR__SURFACE_REFRACTIVITY_SMALL = 1021,  /**< Internally computed surface refractivity value is too small */
    ERROR__SURFACE_REFRACTIVITY_LARGE = 1022,  /**< Internally computed surface refractivity value is too large */
};
// clang-format on

////////////////////////////////////////////////////////////////////////////////
// Constants
// TODO-TEMPLATE define any global constants here (use constexpr!)

////////////////////////////////////////////////////////////////////////////////
// Public Functions
// TODO-TEMPLATE: Add functions which should be exported in the DLL
PROPLIB_API char *GetReturnStatusCharArray(const int code);
PROPLIB_API void FreeReturnStatusCharArray(char *c_msg);

////////////////////////////////////////////////////////////////////////////////
// Private Functions
// TODO-TEMPLATE: Add other/internal functions here (no need for "PROPLIB_API")
std::string GetReturnStatus(const int code);

}  // namespace ITS
