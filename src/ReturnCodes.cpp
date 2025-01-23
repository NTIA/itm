/** @file ReturnCodes.cpp
 * Maps status messages to library return codes
 */

// TODO-TEMPLATE include your primary library header
#include "PropLibTemplate.h"

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
// TODO-TEMPLATE: put these functions in this software's namespace

/*******************************************************************************
 * Get an error message string from a return code.
 * 
 * @param[in] code  Integer return code.
 * @return          A status message corresponding to the input code.
 ******************************************************************************/
std::string GetReturnStatus(int code) {
    static const std::unordered_map<ReturnCode, std::string> messages = {
        {SUCCESS, "Successful execution"}
        // TODO-TEMPLATE: Add messages corresponding to all return codes here
    };
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

}  // namespace ITS