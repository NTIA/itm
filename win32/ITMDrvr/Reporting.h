#include <ctime>
#include <iostream>
#include <fstream>
#include <string.h>
#ifdef _WIN32
#  include <Windows.h>
#else
#  define DLLEXPORT extern "C"
#endif
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

//
// FUNCTIONS
///////////////////////////////////////////////

void PrintClimateLabel(FILE* fp, int climate);
void PrintPolarizationLabel(FILE* fp, int pol);
void PrintLabel(FILE* fp, const char* lbl);
void PrintErrorMsgLabel(FILE* fp, int err);
void PrintSitingCriteriaLabel(FILE* fp, int tx_siting_criteria);
void PrintWarningMessages(FILE* fp, long warnings);
void PrintWarningHelper(FILE* fp, const char* lbl);
void PrintMdvarLabel(FILE* fp, int mdvar);
