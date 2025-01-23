/** @file TempTextFile.h
 * Header for a class which manages temporary text files.
 */
#pragma once

#include <string>  // for std::string

/*******************************************************************************
 * A class to manage a temporary text file.
 *
 * The TempTextFile class creates a temporary text file from a string that is
 * automatically deleted when the object is destroyed.
 ******************************************************************************/
class TempTextFile {
    public:
        /***********************************************************************
         * Constructor that creates a temporary file and writes content to it.
         * 
         * @param[in] content             String content to write to the file.
         * @throws    std::runtime_error  On failure to create or write to file.
         **********************************************************************/
        TempTextFile(const std::string &content);

        /***********************************************************************
         * Destructor that closes (and deletes) the temporary file.
         **********************************************************************/
        ~TempTextFile();

        /***********************************************************************
         * Retrieve the name of the temporary file
         * 
         * @return A string containing the name of the temporary file.
         **********************************************************************/
        std::string getFileName() const;
    private:
        std::string filename; /**< Name of the temporary file */
};
