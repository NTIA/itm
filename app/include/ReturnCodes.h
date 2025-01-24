/** @file ReturnCodes.h
 * Defines return codes for the driver
 */
#pragma once

#include <string>  // for std::string

/*******************************************************************************
 * Return Codes defined by this driver software (128-255)
 ******************************************************************************/
// clang-format off
enum DrvrReturnCode {
    // Primary Return Codes
    DRVR__SUCCESS = 128,          /**< Successful execution */
    DRVR__RETURN_SUCCESS,         /**< Indicates driver should exit successfully */
    DRVRERR__MISSING_OPTION,      /**< No value provided for given argument */
    DRVRERR__INVALID_OPTION,      /**< Unknown option specified */
    DRVRERR__OPENING_INPUT_FILE,  /**< Failed to open the input file for reading */
    DRVRERR__OPENING_OUTPUT_FILE, /**< Failed to open the output file for writing */

    // DRVRERR__DLL_LOADING = 1001,
    // DRVRERR__MAJOR_VERSION_MISMATCH = 1002,
    // DRVRERR__INVALID_OPTION = 1003,
    // DRVRERR__GET_P2P_TLS_FUNC_LOADING = 1004,
    // DRVRERR__GET_P2P_CR_FUNC_LOADING = 1005,
    // DRVRERR__GET_AREA_TLS_FUNC_LOADING = 1006,
    // DRVRERR__GET_AREA_CR_FUNC_LOADING = 1007,
    // DRVRERR__PARSE_MODE_VALUE = 1008,

    // Input File Parsing Errors (1100-1199)
    DRVRERR__PARSE = 1100, /**< Failed parsing inputs; unknown parameter */
    DRVRERR__PARSE_HTX = 1101,
    DRVRERR__PARSE_HRX = 1102,
    DRVRERR__PARSE_CLIMATE = 1103,
    DRVRERR__PARSE_N0 = 1104,
    DRVRERR__PARSE_FREQ = 1105,
    DRVRERR__PARSE_POL = 1106,
    DRVRERR__PARSE_EPSILON = 1107,
    DRVRERR__PARSE_SIGMA = 1108,
    DRVRERR__PARSE_MDVAR = 1109,
    DRVRERR__PARSE_TIME = 1110,
    DRVRERR__PARSE_LOCATION = 1111,
    DRVRERR__PARSE_SITUATION = 1112,
    DRVRERR__PARSE_CONFIDENCE = 1113,
    DRVRERR__PARSE_RELIABILITY = 1114,
    DRVRERR__PARSE_TX_SITE = 1115,
    DRVRERR__PARSE_RX_SITE = 1116,
    DRVRERR__PARSE_DKM = 1117,
    DRVRERR__PARSE_DELTAH = 1118,
    DRVRERR__PARSE_INCOMPLETE_DISTANCES = 1119,

    // Validation Errors
    DRVRERR__VALIDATION_MODE = 1201,         /**< Mode not specified */
    DRVRERR__VALIDATION_IN_FILE = 1202,      /**< Input file not specified */
    DRVRERR__VALIDATION_OUT_FILE = 1203,     /**< Output file not specified */
    DRVRERR__VALIDATION_TERRAIN_FILE = 1204, /**< Terrain file not specified */
    // DRVRERR__VALIDATION_HTX = 1205,
    // DRVRERR__VALIDATION_HRX = 1206,
    // DRVRERR__VALIDATION_CLIMATE = 1207,
    // DRVRERR__VALIDATION_N0 = 1208,
    // DRVRERR__VALIDATION_FMHZ = 1209,
    // DRVRERR__VALIDATION_POL = 1210,
    // DRVRERR__VALIDATION_EPSILON = 1211,
    // DRVRERR__VALIDATION_SIGMA = 1212,
    // DRVRERR__VALIDATION_MDVAR = 1213,
    // DRVRERR__VALIDATION_TLS_AND_CR = 1214,
    // DRVRERR__VALIDATION_TLS_OR_CR = 1215,
    // DRVRERR__VALIDATION_HTX_SITE = 1216,
    // DRVRERR__VALIDATION_RTX_SITE = 1217,
    // DRVRERR__VALIDATION_DKM = 1218,
    // DRVRERR__VALIDATION_DELTAH = 1219,
};
// clang-format on

std::string GetDrvrReturnStatusMsg(int code);
