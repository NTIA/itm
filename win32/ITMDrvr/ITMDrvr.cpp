#include "ITMDrvr.h"
#include "PointToPointMode.h"
#include "AreaMode.h"
#include "Labels.h"
#include "Tags.h"
#include "Reporting.h"
#include "Common.h"

#include "Enums.h"
#include "Errors.h"
#include "Warnings.h"

int dllVerMajor = NOT_SET;
int dllVerMinor = NOT_SET;
int drvrVerMajor = NOT_SET;
int drvrVerMinor = NOT_SET;
int drvrVerDrvr = NOT_SET;

wchar_t buf[TIME_SIZE];

/*=============================================================================
 |
 |  Description:  Main function of the ITM driver executable
 |
 *===========================================================================*/
int main(int argc, char** argv) {
    int rtn;
    DrvrParams params;

    // Get the time
    time_t t = time(NULL);
#ifdef _WIN32
    _wctime_s(buf, TIME_SIZE, &t);
#else 
    strncpy((char*) buf, ctime(&t), TIME_SIZE);
#endif

    rtn = ParseArguments(argc, argv, &params);
    if (rtn == DRVR__RETURN_SUCCESS)
        return SUCCESS;
    if (rtn) {
        Help();
        return rtn;
    }

    // validate command line inputs
    rtn = ValidateInputs(&params);
    if (rtn)
    {
        printf_s("\n");
        Help();
        return rtn;
    }

    rtn = LoadDLL();
    if (rtn)
        return rtn;
    

    // validate input file inputs
    vector<double> A__db;
    vector<double> A_fs__db;
    long warnings;
    IntermediateValues inter_vals;
    P2PParams p2p_params;
    AreaParams area_params;

    if (params.mode == MODE_P2P)
        rtn = CallP2PMode(&params, &p2p_params, &inter_vals, &A__db, &warnings);
    else {
        rtn = CallAreaMode(&params, &area_params, &inter_vals, &A__db, &A_fs__db, &warnings);
    }

    if (rtn >= 1100) // these are driver errors
        return rtn;
    
    // print results to file
    FILE* fp;
#ifdef _WIN32
    int err = fopen_s(&fp, params.out_file, "w");
#else
    fp = fopen(params.out_file, "w");
    int err = errno;
#endif
    if (err != 0) {
        printf_s("Error opening output file.  Exiting.\n");
        return err;
    }
    else {
        fprintf_s(fp, "itm.dll Version          v%i.%i\n", dllVerMajor, dllVerMinor);
        fprintf_s(fp, "ITMDrvr.exe Version      v%i.%i.%i\n", drvrVerMajor, drvrVerMinor, drvrVerDrvr);
        fwprintf_s(fp, L"Date Generated           %s", buf);
        fprintf_s(fp, "Input Arguments          ");
        for (int i = 1; i < argc; i++) {
            fprintf_s(fp, "%s ", argv[i]);
        }
        fprintf_s(fp, "\n\n");
        fprintf_s(fp, "Inputs\n");
        if (params.mode == MODE_P2P) {
            WriteP2PInputs(fp, &p2p_params);
            fprintf_s(fp, "Mode                     Point-to-Point\n");
            fprintf_s(fp, "Terrain File             %s\n", params.terrain_file);
        }
        else {
            WriteAreaInputs(fp, &area_params);
            fprintf_s(fp, "Mode                     Area\n");
        }

        fprintf_s(fp, "\n");

        if (rtn != SUCCESS && rtn != SUCCESS_WITH_WARNINGS) {
            fprintf_s(fp, "ITM Error                %-12i ", rtn);
            PrintErrorMsgLabel(fp, rtn);
        }
        else {
            fprintf_s(fp, "Results\n");
            fprintf_s(fp, "ITM Warning Flags        0x%.4X       ", (int) warnings);
            PrintWarningMessages(fp, warnings);
            fprintf_s(fp, "ITM Return Code          %-12i ", rtn);
            PrintErrorMsgLabel(fp, rtn);
            if (A__db.size() == 1) {
                fprintf_s(fp, "Basic Transmission Loss  %-12.1f %s\n", A__db.front(), UNITS__DB);
            }
            else {
                // print table
                if (params.mode == MODE_P2P) {
                    fprintf_s(fp, "\nBasic Transmission Loss Results %s\n", UNITS__DB);
                    fprintf_s(fp, "Reliability    with Confidence\n");
                    fprintf_s(fp, "               ");
                    for (int i = 0; i < p2p_params.confidence.size(); i++) {
                        fprintf_s(fp, "%-10g", p2p_params.confidence[i]);
                    }
                    fprintf_s(fp, "\n");
                    for (int i = 0; i < p2p_params.reliability.size(); i++) {
                        fprintf_s(fp, "%-15g", p2p_params.reliability[i]);

                        for (int j = 0; j < p2p_params.confidence.size(); j++) {
                            fprintf_s(fp, "%-10.1f", A__db[i * p2p_params.confidence.size() + j]);
                        }
                        fprintf_s(fp, "\n");
                    }
                }
                else {
                    fprintf_s(fp, "\nBasic Transmission Loss Results %s\n", UNITS__DB);
                    fprintf_s(fp, "Distance     Free      with Confidence\n");
                    fprintf_s(fp, "  %s       Space     ", UNITS__KM);
                    for (int i = 0; i < area_params.confidence.size(); i++) {
                        fprintf_s(fp, "%-10g", area_params.confidence[i]);
                    }
                    fprintf_s(fp, "\n");
                    for (int i = 0; i < area_params.d__km.size(); i++) {
                        fprintf_s(fp, "%-13g", area_params.d__km[i]);

                        fprintf_s(fp, "%-10.1f", A_fs__db[i]);

                        for (int j = 0; j < area_params.confidence.size(); j++) {
                            fprintf_s(fp, "%-10.1f", A__db[i * area_params.confidence.size() + j]);
                        }
                        fprintf_s(fp, "\n");
                    }
                }
            }

            if (params.DBG &&
                (params.mode == MODE_P2P || (params.mode == MODE_AREA && A__db.size() == 1))) {
                fprintf_s(fp, "\n");
                fprintf_s(fp, "Intermediate Values\n");
                fprintf_s(fp, "Free Space               %-12.1f %s\n", inter_vals.A_fs__db, UNITS__DB);
                fprintf_s(fp, "d__km                    %-12.3f %s\n", inter_vals.d__km, UNITS__KM);
                fprintf_s(fp, "theta_hzn_tx             %-12.3f %s\n", inter_vals.theta_hzn[0] * 1000, UNITS__MRAD);
                fprintf_s(fp, "theta_hzn_rx             %-12.3f %s\n", inter_vals.theta_hzn[1] * 1000, UNITS__MRAD);
                fprintf_s(fp, "d_hzn_tx__meter          %-12.0f %s\n", inter_vals.d_hzn__meter[0], UNITS__METER);
                fprintf_s(fp, "d_hzn_rx__meter          %-12.0f %s\n", inter_vals.d_hzn__meter[1], UNITS__METER);
                fprintf_s(fp, "h_e_tx__meter            %-12.1f %s\n", inter_vals.h_e__meter[0], UNITS__METER);
                fprintf_s(fp, "h_e_rx__meter            %-12.1f %s\n", inter_vals.h_e__meter[1], UNITS__METER);
                fprintf_s(fp, "N_s                      %-12.1f %s\n", inter_vals.N_s, UNITS__NUNIT);
                fprintf_s(fp, "delta_h__meter           %-12.1f %s\n", inter_vals.delta_h__meter, UNITS__METER);
                fprintf_s(fp, "A_ref__db                %-12.1f %s\n", inter_vals.A_ref__db, UNITS__DB);
                fprintf_s(fp, "Mode of Propagation      %-12i ", inter_vals.mode);
                switch (inter_vals.mode) {
                case 1: fprintf_s(fp, "[%s]\n", LBL__LINE_OF_SITE);
                    break;
                case 2: fprintf_s(fp, "[%s]\n", LBL__DIFFRACTION);
                    break;
                case 3: fprintf_s(fp, "[%s]\n", LBL__TROPOSCATTER);
                    break;
                }
            }
        }

        fclose(fp);
    }
}

/*=============================================================================
 |
 |  Description:  Parse the command line arguments
 |
 |        Input:  argc      - Number of arguments
 |                argv      - Command line arguments
 |
 |       Output:  params    - Structure with user input params
 |
 |      Returns:  SUCCESS, or error code encountered
 |
 *===========================================================================*/
int ParseArguments(int argc, char** argv, DrvrParams* params) {
    for (int i = 1; i < argc; i++) {
        Lowercase(argv[i]);

        if (Match("-i", argv[i])) {
            sprintf_s(params->in_file, "%s", argv[i + 1]);
            i++;
        }
        else if (Match("-o", argv[i])) {
            sprintf_s(params->out_file, "%s", argv[i + 1]);
            i++;
        }
        else if (Match("-t", argv[i])) {
            sprintf_s(params->terrain_file, "%s", argv[i + 1]);
            i++;
        }
        else if (Match("-mode", argv[i]))
        {
            Lowercase(argv[i + 1]);

            if (Match("p2p", argv[i + 1]))
                params->mode = MODE_P2P;
            else if (Match("area", argv[i + 1]))
                params->mode = MODE_AREA;
            else
                return ParseErrorMsgHelper("-mode [mode]", DRVRERR__PARSE_MODE_VALUE);

            i++;
        }
        else if (Match("-dbg", argv[i])) {
            params->DBG = true;
        }
        else if (Match("-v", argv[i])) {
            Version();
            return DRVR__RETURN_SUCCESS;
        }
        else if (Match("-h", argv[i])) {
            Help();
            return DRVR__RETURN_SUCCESS;
        }
        else {
            printf_s("Unknown option: %s\n", argv[i]);
            return DRVRERR__INVALID_OPTION;
        }
    }

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Convert the char array to lower case
 |
 | Input/Output:  argv      - value
 |
 |      Returns:  [Void]
 |
 *===========================================================================*/
void Lowercase(char* argv) {
    for (int i = 0; i < strlen(argv); i++)
        argv[i] = tolower(argv[i]);
}

/*=============================================================================
 |
 |  Description:  Compare to strings to see if they match
 |
 |        Input:  opt       - Given char array
 |                arg       - Expected char array
 |
 |      Returns:  True/False
 |
 *===========================================================================*/
bool Match(const char* opt, char* arg) {
    if (strcmp(opt, arg) == 0)
        return true;
    return false;
}

/*=============================================================================
 |
 |  Description:  Print help instructions to the terminal
 |
 |        Input:  [Void]
 |
 |      Returns:  [Void]
 |
 *===========================================================================*/
void Help() {
    printf_s("\n");
    printf_s("Usage: ITMDrvr.exe [Options]\n");
    printf_s("Options (not case sensitive)\n");
    printf_s("\t-i    :: Input file name\n");
    printf_s("\t-t    :: Terrain file name\n");
    printf_s("\t-o    :: Output file name\n");
    printf_s("\t-mode :: Mode of operation [P2P, AREA]\n");
    printf_s("\t-dbg  :: Dump intermediate values to output file [optional]\n");
    printf_s("\n");
    printf_s("Examples:\n");
    printf_s("\tITMDrvr.exe -i inputs.txt -t terrain.txt -o results.txt -mode P2P\n");
    printf_s("\n");
};

/*=============================================================================
 |
 |  Description:  Print version information to the terminal
 |
 |        Input:  [Void]
 |
 |      Returns:  [Void]
 |
 *===========================================================================*/
void Version() {
    printf_s("*******************************************************\n");
    printf_s("Institute for Telecommunications Sciences - Boulder, CO\n");
    printf_s("\tITM Driver Version: %i.%i\n", drvrVerMajor, drvrVerMinor);
    printf_s("\tITM DLL Version: %i.%i\n", dllVerMajor, dllVerMinor);
    wprintf_s(L"Time: %s", buf);
    printf_s("*******************************************************\n");
}

/*=============================================================================
 |
 |  Description:  Helper function to format and print error messages
 |                encountered during command argument parsing
 |
 |        Input:  opt       - Command flag in error
 |                err       - Error code
 |
 |      Returns:  Error code
 |
 *===========================================================================*/
int ParseErrorMsgHelper(const char* opt, int err) {
    printf_s("DrvrErr %i: Unable to parse %s value.\n", err, opt);
    return err;
}

/*=============================================================================
 |
 |  Description:  Helper function to format and print error messages
 |                encountered during driver execution
 |
 |        Input:  msg       - Command flag in error
 |                err       - Error code
 |
 |      Returns:  Error code
 |
 *===========================================================================*/
int GeneralErrorMsgHelper(const char* msg, int err) {
    printf_s("DrvrErr %i: %s.\n", err, msg);
    return err;
}

/*=============================================================================
 |
 |  Description:  Validate that the required inputs are present for the
 |                mode specified by the user.  This function DOES NOT
 |                check the validity of the parameter values - only that
 |                required parameters have been specified by the user
 |
 |        Input:  params        - Structure with user input parameters
 |
 |      Returns:  SUCCESS, or error code encountered
 |
 *===========================================================================*/
int ValidateInputs(DrvrParams* params) {
    if (params->mode == NOT_SET)
        return Validate_RequiredErrMsgHelper("-mode", DRVRERR__VALIDATION_MODE);

    if (strlen(params->in_file) == 0)
        return Validate_RequiredErrMsgHelper("-i", DRVRERR__VALIDATION_IN_FILE);

    if (strlen(params->out_file) == 0)
        return Validate_RequiredErrMsgHelper("-o", DRVRERR__VALIDATION_OUT_FILE);

    if (params->mode == MODE_P2P) {
        if (strlen(params->terrain_file) == 0)
            return Validate_RequiredErrMsgHelper("-t", DRVRERR__VALIDATION_TERRAIN_FILE);
    }

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Helper function to format and print error messages
 |                encountered during validation of input parameters
 |
 |        Input:  opt       - Command flag in error
 |                err       - Error code
 |
 |      Returns:  Error code
 |
 *===========================================================================*/
int Validate_RequiredErrMsgHelper(const char* opt, int err) {
    printf_s("DrvrError %i: Option %s is required but was not provided\n", err, opt);
    return err;
}

/*=============================================================================
 |
 |  Description:  Loads the ITM DLL
 |
 |        Input:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
int LoadDLL() {
#ifndef _WIN32
    // empty
#else
    HINSTANCE hLib = LoadLibrary(TEXT("itm.dll"));

    if (hLib == NULL)
        return DRVRERR__DLL_LOADING;

    GetDLLVersionInfo();
    GetDrvrVersionInfo();
    if (dllVerMajor != drvrVerMajor)
        return DRVRERR__MAJOR_VERSION_MISMATCH;

    // Grab the functions in the DLL
    int rtn = LoadP2PFunctions(hLib);
    if (rtn != SUCCESS)
        return rtn;

    rtn = LoadAreaFunctions(hLib);
    if (rtn != SUCCESS)
        return rtn;
#endif
    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Get the version information of the ITM DLL
 |
 |        Input:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void GetDLLVersionInfo() {
#ifndef _WIN32
    float version = ITMLIB_VERSION;
    dllVerMajor = int(version);
    dllVerMinor = ( version - int(version)) * 100;
#else
    DWORD  verHandle = NULL;
    UINT   size = 0;
    LPBYTE lpBuffer = NULL;
    DWORD  verSize = GetFileVersionInfoSize(TEXT("itm.dll"), &verHandle);

    if (verSize != NULL)
    {
        LPSTR verData = new char[verSize];

        if (GetFileVersionInfo(TEXT("itm.dll"), verHandle, verSize, verData))
        {
            if (VerQueryValue(verData, TEXT("\\"), (VOID FAR* FAR*)&lpBuffer, &size))
            {
                if (size)
                {
                    VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        dllVerMajor = (verInfo->dwFileVersionMS >> 16) & 0xffff;
                        dllVerMinor = (verInfo->dwFileVersionMS >> 0) & 0xffff;
                    }
                }
            }
        }

        delete[] verData;
    }
#endif
    return;
}

/*=============================================================================
 |
 |  Description:  Get the version information of this driver
 |
 |        Input:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void GetDrvrVersionInfo()
{
#ifndef _WIN32 
    // empty
#else
    DWORD  verHandle = NULL;
    UINT   size = 0;
    LPBYTE lpBuffer = NULL;
    WCHAR fileName[MAX_PATH];

    GetModuleFileName(NULL, fileName, MAX_PATH);
    DWORD  verSize = GetFileVersionInfoSize(fileName, &verHandle);

    if (verSize != NULL)
    {
        LPSTR verData = new char[verSize];

        if (GetFileVersionInfo(fileName, verHandle, verSize, verData))
        {
            if (VerQueryValue(verData, L"\\", (VOID FAR * FAR*) &lpBuffer, &size))
            {
                if (size)
                {
                    VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        drvrVerMajor = (verInfo->dwFileVersionMS >> 16) & 0xffff;
                        drvrVerMinor = (verInfo->dwFileVersionMS >> 0) & 0xffff;
                        drvrVerDrvr = (verInfo->dwFileVersionLS >> 16) & 0xffff;
                    }
                }
            }
        }

        delete[] verData;
    }
#endif
    return;
}