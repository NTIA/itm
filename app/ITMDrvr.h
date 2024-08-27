#include <ctime>
#include <iostream>
#include <fstream>
#include <string.h>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

//
// CONSTANTS
///////////////////////////////////////////////

#define     MODE_P2P                                0
#define     MODE_AREA                               1

#define     VARY_TLS                                0
#define     VARY_CR                                 1
#define     TIME_SIZE                               26

//
// GENERAL ERRORS AND RETURN VALUES
///////////////////////////////////////////////

#define     NOT_SET                                 -1
#define     ERROR                                   -1
#define     SUCCESS                                 0
#define     SUCCESS_WITH_WARNINGS                   1
#define     DRVR__RETURN_SUCCESS                    1000

#define     DRVRERR__DLL_LOADING                    1001
#define     DRVRERR__MAJOR_VERSION_MISMATCH         1002
#define     DRVRERR__INVALID_OPTION                 1003
#define     DRVRERR__GET_P2P_TLS_FUNC_LOADING       1004
#define     DRVRERR__GET_P2P_CR_FUNC_LOADING        1005
#define     DRVRERR__GET_AREA_TLS_FUNC_LOADING      1006
#define     DRVRERR__GET_AREA_CR_FUNC_LOADING       1007
#define     DRVRERR__PARSE_MODE_VALUE               1008
// Input File Parsing Errors (1100-1199)
#define     DRVRERR__PARSE_HTX                      1101
#define     DRVRERR__PARSE_HRX                      1102
#define     DRVRERR__PARSE_CLIMATE                  1103
#define     DRVRERR__PARSE_N0                       1104
#define     DRVRERR__PARSE_FREQ                     1105
#define     DRVRERR__PARSE_POL                      1106
#define     DRVRERR__PARSE_EPSILON                  1107
#define     DRVRERR__PARSE_SIGMA                    1108
#define     DRVRERR__PARSE_MDVAR                    1109
#define     DRVRERR__PARSE_TIME                     1110
#define     DRVRERR__PARSE_LOCATION                 1111
#define     DRVRERR__PARSE_SITUATION                1112
#define     DRVRERR__PARSE_CONFIDENCE               1113
#define     DRVRERR__PARSE_RELIABILITY              1114
#define     DRVRERR__PARSE_TX_SITE                  1115
#define     DRVRERR__PARSE_RX_SITE                  1116
#define     DRVRERR__PARSE_DKM                      1117
#define     DRVRERR__PARSE_DELTAH                   1118
#define     DRVRERR__PARSE_INCOMPLETE_DISTANCES     1119
// Validation Errors (1200-1299)
#define     DRVRERR__VALIDATION_MODE                1201
#define     DRVRERR__VALIDATION_IN_FILE             1202
#define     DRVRERR__VALIDATION_OUT_FILE            1203
#define     DRVRERR__VALIDATION_TERRAIN_FILE        1204
#define     DRVRERR__VALIDATION_HTX                 1205
#define     DRVRERR__VALIDATION_HRX                 1206
#define     DRVRERR__VALIDATION_CLIMATE             1207
#define     DRVRERR__VALIDATION_N0                  1208
#define     DRVRERR__VALIDATION_FMHZ                1209
#define     DRVRERR__VALIDATION_POL                 1210
#define     DRVRERR__VALIDATION_EPSILON             1211
#define     DRVRERR__VALIDATION_SIGMA               1212
#define     DRVRERR__VALIDATION_MDVAR               1213
#define     DRVRERR__VALIDATION_TLS_AND_CR          1214
#define     DRVRERR__VALIDATION_TLS_OR_CR           1215
#define     DRVRERR__VALIDATION_HTX_SITE            1216
#define     DRVRERR__VALIDATION_RTX_SITE            1217
#define     DRVRERR__VALIDATION_DKM                 1218
#define     DRVRERR__VALIDATION_DELTAH              1219

//
// DATA STRUCTURES
///////////////////////////////////////////////

struct DrvrParams {
    char terrain_file[256]  = { 0 };    // Terrain file
    char in_file[256]       = { 0 };    // Input file
    char out_file[256]      = { 0 };    // Output file

    int mode                = NOT_SET;  // Mode (P2P, AREA)
    bool DBG                = false;    // Dump intermediate values to file?
};

struct P2PParams {
    double h_tx__meter      = NOT_SET;
    double h_rx__meter      = NOT_SET;
    int climate             = NOT_SET;
    double N_0              = NOT_SET;
    double f__mhz           = NOT_SET;
    int pol                 = NOT_SET;
    double epsilon          = NOT_SET;
    double sigma            = NOT_SET;
    int mdvar               = NOT_SET;
    double time             = NOT_SET;
    double location         = NOT_SET;
    double situation        = NOT_SET;

    vector<double> confidence;
    vector<double> reliability;

    int mode                = NOT_SET;
};

struct AreaParams {
    double h_tx__meter      = NOT_SET;
    double h_rx__meter      = NOT_SET;
    int tx_site_criteria    = NOT_SET;
    int rx_site_criteria    = NOT_SET;
    double delta_h__meter   = NOT_SET;
    int climate             = NOT_SET;
    double N_0              = NOT_SET;
    double f__mhz           = NOT_SET;
    int pol                 = NOT_SET;
    double epsilon          = NOT_SET;
    double sigma            = NOT_SET;
    int mdvar               = NOT_SET;
    double time             = NOT_SET;
    double location         = NOT_SET;
    double reliability      = NOT_SET;

    double d_format[5]      = { 0 };

    vector<double> d__km;
    vector<double> confidence;
    vector<double> situation;

    int mode                = NOT_SET;
};

struct IntermediateValues
{
    double theta_hzn[2];        // Terminal horizon angles
    double d_hzn__meter[2];     // Terminal horizon distances, in meters
    double h_e__meter[2];       // Terminal effective heights, in meters
    double N_s;                 // Surface refractivity, in N-Units
    double delta_h__meter;      // Terrain irregularity parameter, in meters
    double A_ref__db;           // Reference attenuation, in dB
    double A_fs__db;            // Free space basic transmission loss, in dB
    double d__km;               // Path distance, in km
    int mode;                   // Mode of propagation value
};

//
// FUNCTIONS
///////////////////////////////////////////////

int ParseArguments(int argc, char** argv, DrvrParams* params);
void Lowercase(char* argv);
bool Match(const char* opt, char* arg);
void Help();
void Version();
int ParseErrorMsgHelper(const char* opt, int err);
int ValidateInputs(DrvrParams* params);
int Validate_RequiredErrMsgHelper(const char* opt, int err);


int LoadDLL();
void GetDLLVersionInfo();
void GetDrvrVersionInfo();
int GeneralErrorMsgHelper(const char* msg, int err);
