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


//
// DATA STRUCTURES
///////////////////////////////////////////////



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
