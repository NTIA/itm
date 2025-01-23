/** @file TempTextFile.cpp
 * Implements a class to create and write to temporary text files
 */
#include "TempTextFile.h"

#ifdef _WIN32
    // Ensure tmpnam_s is available on Windows
    #ifndef __STDC_LIB_EXT1__
        #define __STDC_LIB_EXT1__
    #endif
    #ifndef __STDC_WANT_LIB_EXT1__
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
    #include <cstdio>    // for L_tmpnam_s, tmpnam_s
#else                    // macOS and Linux
    #include <cstdlib>   // for mkstemp
    #include <unistd.h>  // for close
#endif

#include <algorithm>  // for std::remove
#include <fstream>    // for std::ofstream
#include <iostream>   // for std::cerr, std::ios::trunc
#include <ostream>    // for std::endl
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

TempTextFile::TempTextFile(const std::string &content) {
#ifdef _WIN32
    // Generate and store a temporary file name
    char tempFileName[L_tmpnam_s];
    if (tmpnam_s(tempFileName, sizeof(tempFileName)) != 0) {
        throw std::runtime_error("Failed to create temporary file name.");
    }
#else
    // Safer implementation for POSIX platforms
    char tempFileName[] = "/tmp/proplib-tempfile.XXXXXX";
    int fd = mkstemp(tempFileName);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temporary file.");
    }
    close(fd);
#endif
    filename = tempFileName;  // Store generated filename
    std::ofstream tempFile(tempFileName, std::ios::trunc);
    if (!tempFile.is_open()) {
        std::cerr << "Temp file name is: " << filename << std::endl;
        throw std::runtime_error("Failed to open temporary file for writing.");
    }
    tempFile << content;
    tempFile.close();
}

TempTextFile::~TempTextFile() {
    // Delete the temporary file upon destruction
    std::remove(filename.c_str());
}

std::string TempTextFile::getFileName() const {
    // Return the name of the temporary file.
    return filename;
}