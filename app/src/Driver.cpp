/** @file Driver.cpp
 * Implements the main function of the executable, and other high-level functions
 */
#include "Driver.h"

#include <algorithm> // for std::find
#include <fstream>   // for std::ifstream, std::ofstream
#include <iomanip>   // for std::setw
#include <ios>       // for std::left
#include <iostream>  // for std::cerr
#include <ostream>   // for std::endl
#include <string>    // for std::string
#include <vector>    // for std::vector

/*******************************************************************************
 * Main function of the driver executable
 * 
 * @param[in] argc  Number of arguments entered on the command line
 * @param[in] argv  Array containing the provided command-line arguments
 * @return          Return code
 ******************************************************************************/
int main(int argc, char **argv) {
    int rtn;
    DrvrParams params;

    // Parse command line arguments
    rtn = ParseArguments(argc, argv, params);
    if (rtn == DRVR__RETURN_SUCCESS) {
        return SUCCESS;
    } else if (rtn != DRVR__SUCCESS) {
        Help();
        return rtn;
    }

    // Ensure required options were provided
    rtn = ValidateInputs(params);
    if (rtn != DRVR__SUCCESS) {
        Help();
        return rtn;
    }

    // TODO-TEMPLATE: Add driver logic, e.g. validating inputs and calling the model

    // TODO-TEMPLATE this code block exists for a unit test to pass. Similar logic
    // should be added to functions which parse input files.
    std::ifstream file(params.in_file);
    if (!file) {
        std::cerr << "Failed to open file " << params.in_file << std::endl;
        return DRVRERR__OPENING_INPUT_FILE;
    }

    // Return driver error code if one was returned
    if (rtn > DRVR__RETURN_SUCCESS) {
        std::cerr << GetDrvrReturnStatusMsg(rtn) << std::endl;
        return rtn;
    }

    // Open output file for writing
    std::ofstream fp(params.out_file);
    if (!fp) {
        std::cerr << "Error opening output file. Exiting." << std::endl;
        return DRVRERR__OPENING_OUTPUT_FILE;
    }

    // Print generator information to file
    fp << std::left << std::setw(25) << "Model" << LIBRARY_NAME;
    fp PRINT "Library Version" << "v" << LIBRARY_VERSION;
    fp PRINT "Driver Version" << "v" << DRIVER_VERSION;
    fp PRINT "Date Generated" << GetDatetimeString();
    fp PRINT "Input Arguments";
    for (int i = 1; i < argc; i++) {
        fp << argv[i] << " ";
    }
    fp << std::endl << std::endl;
    // TODO-TEMPLATE populate the rest of the output file
    fp.close();
    return SUCCESS;
}

/*******************************************************************************
 * Parse the command line arguments
 * 
 * @param[in]  argc    Number of arguments
 * @param[in]  argv    Command line arguments
 * @param[out] params  Structure with user input params
 * @return             Return code
 ******************************************************************************/
DrvrReturnCode ParseArguments(int argc, char **argv, DrvrParams &params) {
    // TODO-TEMPLATE: Populate vector with all valid arguments
    const std::vector<std::string> validArgs
        = {"-i", "-o", "-dbg", "-h", "--help", "-v", "--version"};

    for (int i = 1; i < argc; i++) {
        // Parse arg to lowercase string
        std::string arg(argv[i]);
        StringToLower(arg);

        // Check if provided flag is valid
        if (std::find(validArgs.begin(), validArgs.end(), arg)
            == validArgs.end()) {
            // Invalid argument provided
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return DRVRERR__INVALID_OPTION;
        }

        // Handle simple flags which don't have associated values (e.g., "-v", "-DBG")
        if (arg == "-v" || arg == "--version") {
            Version();
            return DRVR__RETURN_SUCCESS;
        } else if (arg == "-h" || arg == "--help") {
            Help();
            return DRVR__RETURN_SUCCESS;
            // TODO-TEMPLATE handle any model input flags here
        } else if (arg == "-dbg") {
            params.DBG = true;
            continue;
        }

        // Check if end of arguments reached or next argument is another flag
        if (i + 1 >= argc || argv[i + 1][0] == '-') {
            std::cerr << "Error: no value given for " << arg << std::endl;
            return DRVRERR__MISSING_OPTION;
        }

        // TODO-TEMPLATE: Handle inputs which provide values (e.g. "-i in.txt").
        // Template code will set in_file and out_file in DrvrParams based on -i
        // and -o options. It will also set DrvrParams.DBG based on a -dbg flag
        if (arg == "-i") {
            params.in_file = argv[i + 1];
            i++;
        } else if (arg == "-o") {
            params.out_file = argv[i + 1];
            i++;
        }
    }

    return DRVR__SUCCESS;
}

/*******************************************************************************
 * Print help instructions to the terminal
 * 
 * @param[in] os  Output stream for writing; defaults to `std::cout`
 ******************************************************************************/
void Help(std::ostream &os) {
    // TODO-TEMPLATE: Update driver help message
    os << std::endl << "Usage: .\\<Driver Executable> [Options]" << std::endl;
    os << "Options (not case sensitive)" << std::endl;
    os << "\t-i    :: Input file name" << std::endl;
    os << "\t-t    :: Terrain file name" << std::endl;
    os << "\t-o    :: Output file name" << std::endl;
    os << "\t-dbg  :: Dump intermediate values to output file [optional]"
       << std::endl;
    os << std::endl << "Examples:" << std::endl;
    os << "\t[WINDOWS] " << DRIVER_NAME << ".exe -i in.txt -o out.txt"
       << std::endl;
    os << "\t[LINUX]   .\\" << DRIVER_NAME << " -i in.txt -o out.txt"
       << std::endl;
    os << "Other Options (which don't run the model)" << std::endl;
    os << "\t-h    :: Display this help message" << std::endl;
    os << "\t-v    :: Display program version information" << std::endl;
    os << std::endl;
};

/*******************************************************************************
 * Validate that required inputs are present for the mode specified by the user.
 * 
 * This function DOES NOT check the validity of the parameter values, only that
 * required parameters have been specified by the user
 * 
 * @param[in] params  Structure with user input parameters
 * @return            Return code
 ******************************************************************************/
DrvrReturnCode ValidateInputs(const DrvrParams &params) {
    DrvrParams not_set;
    DrvrReturnCode rtn = DRVR__SUCCESS;
    // TODO-TEMPLATE: Check that required inputs were provided.
    // This template code checks that input/output files were given with -i and -o
    if (params.in_file == not_set.in_file)
        rtn = DRVRERR__VALIDATION_IN_FILE;
    if (params.out_file == not_set.out_file)
        rtn = DRVRERR__VALIDATION_OUT_FILE;

    if (rtn != DRVR__SUCCESS)
        std::cerr << GetDrvrReturnStatusMsg(rtn) << std::endl;

    return rtn;
}
