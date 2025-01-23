/** @file ReturnCodes.h
 * Defines return codes for the driver
 */
#pragma once

#include <string>  // for std::string

// TODO-TEMPLATE: Add driver return codes here and corresponding entries in app/src/ReturnCodes.cpp

/*******************************************************************************
 * Return Codes defined by this driver software (128-255)
 ******************************************************************************/
// clang-format off
enum DrvrReturnCode {
    // Primary Return Codes
    DRVR__SUCCESS = 128,                 /**< Successful execution */
    DRVR__RETURN_SUCCESS,                /**< Indicates driver should exit successfully */
    DRVRERR__MISSING_OPTION,             /**< No value provided for given argument */
    DRVRERR__INVALID_OPTION,             /**< Unknown option specified */
    DRVRERR__OPENING_INPUT_FILE,         /**< Failed to open the input file for reading */
    DRVRERR__OPENING_OUTPUT_FILE,        /**< Failed to open the output file for writing */

    // Input File Parsing Errors
    DRVRERR__PARSE = 160,                /**< Failed parsing inputs; unknown parameter */

    // Validation Errors
    DRVRERR__VALIDATION_IN_FILE = 192,   /**< Input file not specified */
    DRVRERR__VALIDATION_OUT_FILE,        /**< Output file not specified */
};
// clang-format on

std::string GetDrvrReturnStatusMsg(int code);
