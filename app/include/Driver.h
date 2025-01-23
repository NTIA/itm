/** @file Driver.h
 * Interface header for this driver executable
 */
#pragma once

#include "CommaSeparatedIterator.h"
#include "ReturnCodes.h"
#include "Structs.h"

// TODO-TEMPLATE: Include your library's main interface header
#include "PropLibTemplate.h"

#include <iomanip>   // for std::left, std::setw
#include <iostream>  // for std::cout
#include <ostream>   // for std::endl, std::ostream
#include <string>    // for std::string

/////////////////////////////
// Macros

/** Shortcut for concise print-to-file statements in driver */
#define PRINT << std::endl << std::left << std::setw(25) <<
/** Shortcut for setting fixed whitespace padding in driver file output */
#define SETW13 << std::setw(13) <<

//////////////////////////////
// Library Namespace
// TODO-TEMPLATE: use the namespace of your library
using namespace ITS;

/////////////////////////////
// Functions
void Help(std::ostream &os = std::cout);
DrvrReturnCode ParseArguments(int argc, char **argv, DrvrParams &params);
DrvrReturnCode ValidateInputs(const DrvrParams &params);

// Driver Utils
std::string GetDatetimeString();
DrvrReturnCode ParseBoolean(const std::string &str, bool &value);
DrvrReturnCode ParseDouble(const std::string &str, double &value);
DrvrReturnCode ParseInteger(const std::string &str, int &value);
void PrintLabel(std::ostream &os, const std::string &lbl);
void StringToLower(std::string &str);
void Version(std::ostream &os = std::cout);