#include "ITMDrvr.h"
#include "PointToPointMode.h"
#include "Tags.h"
#include "Reporting.h"
#include "Common.h"

// ITM DLL Functions
typedef int(__stdcall *itm_p2p_tls_ex_func)(double h_tx__meter, double h_rx__meter,
    const double pfl[], int climate, double N_0, double f__mhz, int pol, double epsilon,
    double sigma, int mdvar, double time, double location, double situation,
    double *A__db, long *warnings, struct IntermediateValues *interValues);

typedef int(__stdcall *itm_p2p_cr_ex_func)(double h_tx__meter, double h_rx__meter,
    const double pfl[], int climate, double N_0, double f__mhz, int pol, double epsilon,
    double sigma, int mdvar, double confidence, double reliability,
    double *A__db, long *warnings, struct IntermediateValues *interValues);

// Local Globals
itm_p2p_tls_ex_func itm_p2p_tls_ex;
itm_p2p_cr_ex_func itm_p2p_cr_ex;

/*=============================================================================
 |
 |  Description:  Top-level control function for P2P mode operation
 |
 |        Input:  params        - Driver input parameter struct
 |
 |       Output:  p2p_params    - P2P input parameter struct
 |                inter_vals    - ITM intermediate values struct
 |                A__db         - Basic transmission loss, in dB
 |                warnings      - ITM return warning flags
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int CallP2PMode(DrvrParams* params, P2PParams* p2p_params, IntermediateValues* inter_vals, vector<double>* A__db, long* warnings) {
    int rtn = ParseP2PInputFile(params->in_file, p2p_params);
    if (rtn != SUCCESS)
        return rtn;

    rtn = ValidateP2PInputs(p2p_params);
    if (rtn != SUCCESS)
        return rtn;

    vector<double> pfl;
    ParseTerrainFile(params->terrain_file, &pfl);

    double ADB;
    if (p2p_params->mode == VARY_TLS) {
        rtn = itm_p2p_tls_ex(
            p2p_params->h_tx__meter,
            p2p_params->h_rx__meter,
            &pfl[0],
            p2p_params->climate,
            p2p_params->N_0,
            p2p_params->f__mhz,
            p2p_params->pol,
            p2p_params->epsilon,
            p2p_params->sigma,
            p2p_params->mdvar,
            p2p_params->time,
            p2p_params->location,
            p2p_params->situation,
            &ADB,
            warnings,
            inter_vals);

        A__db->push_back(ADB);
    }
    else {
        long WARN;
        *warnings = 0;

        for (int i = 0; i < p2p_params->reliability.size(); i++) {
            for (int j = 0; j < p2p_params->confidence.size(); j++) {
                rtn = itm_p2p_cr_ex(
                    p2p_params->h_tx__meter,
                    p2p_params->h_rx__meter,
                    &pfl[0],
                    p2p_params->climate,
                    p2p_params->N_0,
                    p2p_params->f__mhz,
                    p2p_params->pol,
                    p2p_params->epsilon,
                    p2p_params->sigma,
                    p2p_params->mdvar,
                    p2p_params->confidence[j],
                    p2p_params->reliability[i],
                    &ADB,
                    &WARN,
                    inter_vals);

                A__db->push_back(ADB);
                *warnings = *warnings | WARN;
            }
        }
        
    }

    return rtn;
}

/*=============================================================================
 |
 |  Description:  Load P2P functions from the ITM DLL
 |
 |        Input:  hLib          - DLL handle
 |
 |       Output:  [void]
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int LoadP2PFunctions(HINSTANCE hLib) {
    itm_p2p_tls_ex = (itm_p2p_tls_ex_func)GetProcAddress((HMODULE)hLib, "ITM_P2P_TLS_Ex");
    if (itm_p2p_tls_ex == nullptr)
        return DRVRERR__GET_P2P_TLS_FUNC_LOADING;

    itm_p2p_cr_ex = (itm_p2p_cr_ex_func)GetProcAddress((HMODULE)hLib, "ITM_P2P_CR_Ex");
    if (itm_p2p_cr_ex == nullptr)
        return DRVRERR__GET_P2P_CR_FUNC_LOADING;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Parse P2P input parameter file
 |
 |        Input:  in_file       - Path to P2P input parameter file
 |
 |       Output:  p2p_params    - P2P input parameter struct
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int ParseP2PInputFile(const char* in_file, P2PParams *p2p_params) {
    ifstream file;
    file.open(in_file);
    string line;

    while (getline(file, line)) {
        int i = line.find(",");

        string key = line.substr(0, i);
        string value = line.substr(i + 1);

        transform(key.begin(), key.end(), key.begin(), ::tolower);

        if (key.compare(TAG__HTX) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->h_tx__meter) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_HTX, TAG__HTX);
            }
        }
        else if (key.compare(TAG__HRX) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->h_rx__meter) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_HRX, TAG__HRX);
            }
        }
        else if (key.compare(TAG__CLIMATE) == 0) {
            if (ParseInteger(value.c_str(), &p2p_params->climate) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_CLIMATE, TAG__CLIMATE);
            }
        }
        else if (key.compare("n_0") == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->N_0) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_N0, TAG__N0);
            }
        }
        else if (key.compare(TAG__FREQ) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->f__mhz) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_FREQ, TAG__FREQ);
            }
        }
        else if (key.compare(TAG__POL) == 0) {
            if (ParseInteger(value.c_str(), &p2p_params->pol) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_POL, TAG__POL);
            }
        }
        else if (key.compare(TAG__EPSILON) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->epsilon) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_EPSILON, TAG__EPSILON);
            }
        }
        else if (key.compare(TAG__SIGMA) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->sigma) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_SIGMA, TAG__SIGMA);
            }
        }
        else if (key.compare(TAG__MDVAR) == 0) {
            if (ParseInteger(value.c_str(), &p2p_params->mdvar) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_MDVAR, TAG__MDVAR);
            }
        }
        else if (key.compare(TAG__TIME) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->time) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_TIME, TAG__TIME);
            }
        }
        else if (key.compare(TAG__LOCATION) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->location) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_LOCATION, TAG__LOCATION);
            }
        }
        else if (key.compare(TAG__SITUATION) == 0) {
            if (ParseDouble(value.c_str(), &p2p_params->situation) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_SITUATION, TAG__SITUATION);
            }
        }
        else if (key.compare(TAG__CONFIDENCE) == 0) {
            int s = 0, e = 0;
            do {
                e = value.find(',', s);
                string str = value.substr(s, e);

                double confidence;
                if (ParseDouble(str.c_str(), &confidence) == ERROR) {
                    return ParsingErrorHelper(DRVRERR__PARSE_CONFIDENCE, TAG__CONFIDENCE);
                }

                p2p_params->confidence.push_back(confidence);
                s = e + 1;
            } while (e != value.npos);
        }
        else if (key.compare(TAG__RELIABILITY) == 0) {
            int s = 0, e = 0;
            do {
                e = value.find(',', s);
                string str = value.substr(s, e);

                double reliability;
                if (ParseDouble(str.c_str(), &reliability) == ERROR) {
                    return ParsingErrorHelper(DRVRERR__PARSE_RELIABILITY, TAG__RELIABILITY);
                }

                p2p_params->reliability.push_back(reliability);
                s = e + 1;
            } while (e != value.npos);
        }
        else {
            printf("Unknown input parameter\n");
        }
    }

    file.close();

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Parse terrain file
 |
 |        Input:  terrain_file  - Path to file containing PFL
 |
 |       Output:  pfl           - PFL array
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void ParseTerrainFile(const char* terrain_file, vector<double>* pfl) {
    ifstream file;
    file.open(terrain_file);
    string line;
    getline(file, line);

    int i_start, i_end;

    i_end = line.find(",");
    int np = ::atoi((line.substr(0, i_end)).c_str());

    pfl->push_back(np);

    for (int i = 0; i < np + 1; i++) {
        i_start = i_end + 1;

        i_end = line.find(",", i_start);

        pfl->push_back(::atof((line.substr(i_start, i_end)).c_str()));
    }

    i_start = i_end + 1;
    pfl->push_back(::atof((line.substr(i_start)).c_str()));

    file.close();
}

/*=============================================================================
 |
 |  Description:  Validate P2P input parameters.  This function only validates
 |                that required parameters are present - not that they are
 |                valid values.  The ITM DLL performs validation to ensure
 |                the parameters are within valid range.
 |
 |        Input:  p2p_params    - P2P input parameter struct
 |
 |       Output:  [void]
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int ValidateP2PInputs(P2PParams* p2p_params) {
    if (p2p_params->h_tx__meter == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__HTX, DRVRERR__VALIDATION_HTX);

    if (p2p_params->h_rx__meter == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__HRX, DRVRERR__VALIDATION_HTX);

    if (p2p_params->climate == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__CLIMATE, DRVRERR__VALIDATION_CLIMATE);

    if (p2p_params->N_0 == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__N0, DRVRERR__VALIDATION_N0);

    if (p2p_params->f__mhz == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__FREQ, DRVRERR__VALIDATION_FMHZ);

    if (p2p_params->pol == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__POL, DRVRERR__VALIDATION_POL);

    if (p2p_params->epsilon == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__EPSILON, DRVRERR__VALIDATION_EPSILON);

    if (p2p_params->sigma == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__SIGMA, DRVRERR__VALIDATION_SIGMA);

    if (p2p_params->mdvar == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__MDVAR, DRVRERR__VALIDATION_MDVAR);

    // infer TLS vs CR calling
    bool isTLS = (p2p_params->time != NOT_SET && p2p_params->location != NOT_SET && p2p_params->situation != NOT_SET);
    bool isCR = (p2p_params->confidence.size() > 0 && p2p_params->reliability.size() > 0);

    if (isTLS && isCR)
        return GeneralErrorMsgHelper("Provided both time/location/situation parameters and confidence/reliability", DRVRERR__VALIDATION_TLS_AND_CR);

    if (!isTLS && !isCR)
        return GeneralErrorMsgHelper("Must provide time/location/situation parameters or confidence/reliability", DRVRERR__VALIDATION_TLS_OR_CR);

    if (isTLS)
        p2p_params->mode = VARY_TLS;
    else
        p2p_params->mode = VARY_CR;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Write P2P inputs to the report file
 |
 |        Input:  fp            - File pointer
 |                p2p_params    - P2P input parameter struct
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void WriteP2PInputs(FILE* fp, P2PParams* p2p_params) {
    fprintf_s(fp, "%-24s %-12.2g %s\n", TAG__HTX, p2p_params->h_tx__meter, UNITS__METER);
    fprintf_s(fp, "%-24s %-12.2g %s\n", TAG__HRX, p2p_params->h_rx__meter, UNITS__METER);
    fprintf_s(fp, "%-24s %-12i ", TAG__CLIMATE, p2p_params->climate);
    PrintClimateLabel(fp, p2p_params->climate);
    fprintf_s(fp, "%-24s %-12.2f %s\n", TAG__N0, p2p_params->N_0, UNITS__NUNIT);
    fprintf_s(fp, "%-24s %-12.2f %s\n", TAG__FREQ, p2p_params->f__mhz, UNITS__MHZ);
    fprintf_s(fp, "%-24s %-12i ", TAG__POL, p2p_params->pol);
    PrintPolarizationLabel(fp, p2p_params->pol);
    fprintf_s(fp, "%-24s %.2g\n", TAG__EPSILON, p2p_params->epsilon);
    fprintf_s(fp, "%-24s %.2g\n", TAG__SIGMA, p2p_params->sigma);
    fprintf_s(fp, "%-24s %-12i ", TAG__MDVAR, p2p_params->mdvar);
    PrintMdvarLabel(fp, p2p_params->mdvar);
    if (p2p_params->mode == VARY_TLS) {
        fprintf_s(fp, "%-24s %.2g\n", TAG__TIME, p2p_params->time);
        fprintf_s(fp, "%-24s %.2g\n", TAG__LOCATION, p2p_params->location);
        fprintf_s(fp, "%-24s %.2g\n", TAG__SITUATION, p2p_params->situation);
    }
    else {
        fprintf_s(fp, "%-24s ", TAG__CONFIDENCE);
        for (int i = 0; i < p2p_params->confidence.size() - 1; i++) {
            fprintf_s(fp, "%.2g,", p2p_params->confidence[i]);
        }
        fprintf_s(fp, "%.2g\n", p2p_params->confidence.back());

        fprintf_s(fp, "%-24s ", TAG__RELIABILITY);
        for (int i = 0; i < p2p_params->reliability.size() - 1; i++) {
            fprintf_s(fp, "%.2g,", p2p_params->reliability[i]);
        }
        fprintf_s(fp, "%.2g\n", p2p_params->reliability.back());
    }
}
