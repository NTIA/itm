/** @file TestDriver.h
 * Primary header and test fixture for command line driver tests.
 */
#pragma once

// clang-format off
// GoogleTest must be included first
#include <gtest/gtest.h>  // GoogleTest
// clang-format on

#include "Driver.h"
#include "TempTextFile.h"

#include <cstdio>    // for std::remove, std::perror
#include <cstdlib>   // for std::system
#include <iostream>  // for std::cout
#include <ostream>   // for std::endl, std::flush
#include <string>    // for std::string

#ifndef _WIN32
    #include <unistd.h>  // for WEXITSTATUS
#endif

/*******************************************************************************
 * @class DriverTest
 * Test fixture for running the driver executable tests.
 * 
 * This class extends the Google Test framework's Test class and provides
 * utilities to set up, execute, and manage the output of the driver executable.
 ******************************************************************************/
class DriverTest: public ::testing::Test {
    protected:
        /***********************************************************************
         * Sets up the test environment.
         **********************************************************************/
        void SetUp() override {
            // TODO-TEMPLATE review and optionally adjust default params here
            // Set the default driver params
            params.DBG = false;
            params.out_file = "tmp_out.txt";

            // Get the name of the executable to test
            executable = std::string(DRIVER_LOCATION);
        }

        /***********************************************************************
         * Suppresses the output of the command.
         * 
         * Appends redirection to suppress standard output and error based on
         * the platform.
         * 
         * @param[in, out] cmd  The command string to modify
         **********************************************************************/
        void SuppressOutputs(std::string &cmd) {
#ifdef _WIN32
            cmd += " > nul";
#else
            cmd += " > /dev/null";
#endif
            cmd += " 2>&1";
        }

        /***********************************************************************
         * Builds the command string to run the driver.
         * 
         * Constructs a command string using the provided driver parameters
         * struct. Optionally, the command can be written such that stdout and
         * are suppressed.
         * 
         * @param[in] dParams          The driver parameters
         * @param[in] suppressOutputs  Whether to suppress outputs (default: true)
         * @return                     The constructed command string
         **********************************************************************/
        std::string BuildCommand(
            const DrvrParams &dParams, const bool suppressOutputs = true
        ) {
            // TODO-TEMPLATE: Modify this function to correctly
            // unpack the DrvrParams struct and build the command

            // Construct command from parameters
            std::string command = executable;
            command += " -i " + dParams.in_file;
            if (dParams.DBG) {
                command += " -DBG";
            }
            command += " -o " + dParams.out_file;

            // Suppress text output of the driver, to avoid cluttering
            // test outputs.
            if (suppressOutputs) {
                SuppressOutputs(command);
            }
            // Return the full command string
            return command;
        }

        /***********************************************************************
         * Runs the provided command (cross-platform)
         * 
         * Note that on POSIX platforms the exit code of the command should be
         * between 0 and 255. Exit codes outside this range will be shifted into
         * this range and cannot be unambiguously compared to expectations.
         * 
         * @param[in] cmd The command to run
         * @return        The exit code of the command.
         **********************************************************************/
        int RunCommand(const std::string &cmd) {
            std::cout << std::flush;
            int rtn = std::system(cmd.c_str());
#ifndef _WIN32
            rtn = WEXITSTATUS(rtn);  // Get child process exit code on POSIX
#endif
            return rtn;
        }

        /***********************************************************************
         * Runs the driver executable.
         * 
         * @param[in] dParams  Parameters to parse as command line arguments
         * @return             Return code from the driver execution
         **********************************************************************/
        int RunDriver(const DrvrParams &dParams) {
            std::string cmd = BuildCommand(dParams);
            return RunCommand(cmd);
        }

        /***********************************************************************
         * Runs the driver using the specified input file contents.
         * 
         * This method creates a temporary text file containing the contents
         * of `inFileContents` and then runs the driver using the temporary
         * file as the input file. The rest of the required driver parameters
         * are provided in the `params` input; the `params.in_file` value is
         * ignored and can be unset. If an output file was produced by the
         * driver, it is deleted before this method returns.
         * 
         * @param[in] inFileContents  The contents to write to the input file
         * @param[in] dParams         A populated driver parameters struct (see above)
         * @return                    Return code from the driver execution
         **********************************************************************/
        int RunDriverWithInputFile(
            const std::string &inFileContents, const DrvrParams &dParams
        ) {
            DrvrParams updated_params = dParams;
            TempTextFile tempFile(inFileContents);
            updated_params.in_file = tempFile.getFileName();
            int rtn = RunDriver(updated_params);
            // Cleanup: delete output file if it was created
            DeleteOutputFile(updated_params.out_file);
            return rtn;
        }

        /***********************************************************************
         * Deletes the specified output file if it exists.
         * 
         * Checks if the file exists and attempts to delete it. Reports any
         * errors encountered during deletion.
         * 
         * @param[in] fileName  The name of the file to delete.
         **********************************************************************/
        void DeleteOutputFile(const std::string &fileName) {
            bool fileExists = false;
#ifdef _WIN32
            fileExists = _access(fileName.c_str(), 0) == 0;
#else
            fileExists = access(fileName.c_str(), F_OK) == 0;
#endif
            if (fileExists) {
                if (std::remove(fileName.c_str()) != 0) {
                    std::perror("Error deleting output file");
                }
            }
        }

        /** Platform-dependent string to call the executable */
        std::string executable;

        /** Driver parameters struct which may be used by tests */
        DrvrParams params;
};
