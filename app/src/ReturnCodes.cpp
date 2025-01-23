/** @file ReturnCodes.cpp
 * Maps status message strings to driver return codes.
 */

#include "ReturnCodes.h"

#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map

/*******************************************************************************
 * Get an error message string from a return code.
 * 
 * @param[in] code  Driver return code.
 * @return          A status message corresponding to the input code.
 ******************************************************************************/
std::string GetDrvrReturnStatusMsg(int code) {
    static const std::unordered_map<DrvrReturnCode, std::string> messages = {
        {DRVR__SUCCESS, "Successful execution"},
        {DRVR__RETURN_SUCCESS, "Internal driver success"},
        {DRVRERR__MISSING_OPTION, "No value provided for given argument"},
        {DRVRERR__INVALID_OPTION, "Unknown option specified"},
        {DRVRERR__OPENING_INPUT_FILE,
         "Failed to open the input file for reading"},
        {DRVRERR__OPENING_OUTPUT_FILE,
         "Failed to open the output file for writing"},
        {DRVRERR__PARSE, "Failed parsing inputs; unknown parameter"},
        {DRVRERR__VALIDATION_IN_FILE,
         "Option -i is required but was not provided"},
        {DRVRERR__VALIDATION_OUT_FILE,
         "Option -o is required but was not provided"},
    };

    // Construct status message
    std::string msg = DRIVER_NAME;
    msg += " v";
    msg += DRIVER_VERSION;
    if (code == DRVR__SUCCESS) {
        msg += " Status: ";
    } else {
        msg += " Error: ";
    }

    auto it = messages.find(static_cast<DrvrReturnCode>(code));
    if (it != messages.end()) {
        msg += it->second;
    } else {
        msg += "Undefined return code";
    }
    return msg;
}
