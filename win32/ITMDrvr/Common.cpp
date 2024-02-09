#include "ITMDrvr.h"
#include "Common.h"

/*=============================================================================
 |
 |  Description:  Parse an integer value read from the input parameter file
 |
 |        Input:  str           - Input file value as string
 |
 |       Output:  value         - Input file value converted to int
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int ParseInteger(const char* str, int* value) {
    size_t t;

    try {
        *value = ::stoi(str, &t, 10);
    } catch (...) {
        // error parsing the input string value
        return ITM_ERROR;
    }

    // verify the entire string was parsed, and a trailing char wasn't omitted
    if (strlen(str) != t)
        return ITM_ERROR;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Parse a double value read from the input parameter file
 |
 |        Input:  str           - Input file value as string
 |
 |       Output:  value         - Input file value converted to double
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int ParseDouble(const char* str, double* value) {
    try {
        *value = ::stod(str, nullptr);
    }
    catch (...) {
        // error parsing the input string value
        return ITM_ERROR;
    }

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Common error handling function
 |
 |        Input:  err           - Error parsing code
 |                msg           - Error message
 |
 |       Output:  [void]
 |
 |      Returns:  rtn           - Error code from input param
 |
 *===========================================================================*/
int ParsingErrorHelper(int err, const char* msg) {
    printf_s("DrvrErr %i: Unable to parse '%s' value.\n", err, msg);
    return err;
}
