#include "ITMDrvr.h"
#include "AreaMode.h"
#include "Tags.h"
#include "Reporting.h"
#include "Common.h"

// ITM DLL Functions
typedef int(__stdcall *itm_area_tls_ex_func)(double h_tx__meter, double h_rx__meter,
    int tx_site_criteria, int rx_site_criteria, double d__km, double delta_h__meter,
    int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double time, double location, double situation, double *A__db,
    long *warnings, struct IntermediateValues *interValues);

typedef int(__stdcall *itm_area_cr_ex_func)(double h_tx__meter, double h_rx__meter,
    int tx_site_criteria, int rx_site_criteria, double d__km, double delta_h__meter,
    int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double confidence, double reliability, double *A__db,
    long *warnings, struct IntermediateValues *interValues);

// Local Globals
itm_area_tls_ex_func itm_area_tls_ex;
itm_area_cr_ex_func itm_area_cr_ex;

/*=============================================================================
 |
 |  Description:  Top-level control function for Area mode operation
 |
 |        Input:  params        - Driver input parameter struct
 |                area_params   - Area input parameter struct
 |
 |       Output:  inter_vals    - ITM intermediate values struct
 |                A__db         - Basic transmission loss, in dB
 |                warnings      - ITM return warning flags
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int CallAreaMode(DrvrParams* params, AreaParams* area_params, IntermediateValues* inter_vals, 
    vector<double>* A__db, vector<double>* A_fs__db, long* warnings) {
    int rtn = ParseAreaInputFile(params->in_file, area_params);
    if (rtn != SUCCESS)
        return rtn;

    rtn = ValidateAreaInputs(area_params);
    if (rtn != SUCCESS)
        return rtn;

    *warnings = 0;

    double ADB;
    long WARN;
    if (area_params->mode == VARY_TLS) {
        for (int i = 0; i < area_params->d__km.size(); i++) {
            for (int j = 0; j < area_params->situation.size(); j++) {
                rtn = itm_area_tls_ex(
                    area_params->h_tx__meter,
                    area_params->h_rx__meter,
                    area_params->tx_site_criteria,
                    area_params->rx_site_criteria,
                    area_params->d__km[i],
                    area_params->delta_h__meter,
                    area_params->climate,
                    area_params->N_0,
                    area_params->f__mhz,
                    area_params->pol,
                    area_params->epsilon,
                    area_params->sigma,
                    area_params->mdvar,
                    area_params->time,
                    area_params->location,
                    area_params->situation[j],
                    &ADB,
                    &WARN,
                    inter_vals);

                A__db->push_back(ADB);
                *warnings = *warnings | WARN;
            }

            A_fs__db->push_back(inter_vals->A_fs__db);
        }

        // confidence is situation variability
        area_params->confidence = area_params->situation;
    }
    else {
        for (int i = 0; i < area_params->d__km.size(); i++) {
            for (int j = 0; j < area_params->confidence.size(); j++) {
                rtn = itm_area_cr_ex(
                    area_params->h_tx__meter,
                    area_params->h_rx__meter,
                    area_params->tx_site_criteria,
                    area_params->rx_site_criteria,
                    area_params->d__km[i],
                    area_params->delta_h__meter,
                    area_params->climate,
                    area_params->N_0,
                    area_params->f__mhz,
                    area_params->pol,
                    area_params->epsilon,
                    area_params->sigma,
                    area_params->mdvar,
                    area_params->confidence[j],
                    area_params->reliability,
                    &ADB,
                    &WARN,
                    inter_vals);

                A__db->push_back(ADB);
                *warnings = *warnings | WARN;
            }


            A_fs__db->push_back(inter_vals->A_fs__db);
        }

    }

    return rtn;
}

/*=============================================================================
 |
 |  Description:  Load Area functions from the ITM DLL
 |
 |        Input:  hLib          - DLL handle
 |
 |       Output:  [void]
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int LoadAreaFunctions(HINSTANCE hLib) {
    itm_area_tls_ex = (itm_area_tls_ex_func)GetProcAddress((HMODULE)hLib, "ITM_AREA_TLS_Ex");
    if (itm_area_tls_ex == nullptr)
        return DRVRERR__GET_AREA_TLS_FUNC_LOADING;

    itm_area_cr_ex = (itm_area_cr_ex_func)GetProcAddress((HMODULE)hLib, "ITM_AREA_CR_Ex");
    if (itm_area_tls_ex == nullptr)
        return DRVRERR__GET_AREA_CR_FUNC_LOADING;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Parse Area input parameter file
 |
 |        Input:  in_file       - Path to Area input parameter file
 |
 |       Output:  area_params   - Area input parameter struct
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int ParseAreaInputFile(const char* in_file, AreaParams* area_params) {
    ifstream file;
    file.open(in_file);
    string line;

    while (getline(file, line)) {
        int i = line.find(",");

        string key = line.substr(0, i);
        string value = line.substr(i + 1);

        transform(key.begin(), key.end(), key.begin(), ::tolower);

        if (key.compare(TAG__HTX) == 0) {
            if (ParseDouble(value.c_str(), &area_params->h_tx__meter) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_HTX, TAG__HTX);
            }
        }
        else if (key.compare(TAG__HRX) == 0) {
            if (ParseDouble(value.c_str(), &area_params->h_rx__meter) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_HRX, TAG__HRX);
            }
        }
        else if (key.compare(TAG__TX_SITE) == 0) {
            if (ParseInteger(value.c_str(), &area_params->tx_site_criteria) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_TX_SITE, TAG__TX_SITE);
            }
        }
        else if (key.compare(TAG__RX_SITE) == 0) {
            if (ParseInteger(value.c_str(), &area_params->rx_site_criteria) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_RX_SITE, TAG__RX_SITE);
            }
        }
        else if (key.compare(TAG__DKM) == 0) {
            int s = 0, e = 0, i_p = 0;
            do {
                e = value.find(',', s);
                string str = value.substr(s, e);

                double p;
                if (ParseDouble(str.c_str(), &p) == ERROR) {
                    return ParsingErrorHelper(DRVRERR__PARSE_DKM, TAG__DKM);
                }

                area_params->d_format[i_p] = p;
                i_p++;

                s = e + 1;
            } while (e != value.npos && i_p < 5);

            // check if all 5 distance params were provided
            //  -> or if the user just provided a single distance
            if (i_p == 1) {
                area_params->d__km.push_back(area_params->d_format[0]);
            }
            else if (i_p != 5) {
                return ParsingErrorHelper(DRVRERR__PARSE_INCOMPLETE_DISTANCES, TAG__DKM);
            }
            else {
                // generate the list of distances
                // data is formatted as:
                //    D0, D1, DS1, D2, DS2, which can be described as distances D0 to D1 
                //    in increments of DS1, and then from D1 to D2 in increments of DS2
                
                // first do loop
                int i1 = 0;
                double d__km;
                do {
                    d__km = area_params->d_format[0] + i1 * area_params->d_format[2];
                    area_params->d__km.push_back(d__km);
                    i1++;
                } while (d__km < area_params->d_format[1]);

                // second do loop
                int i2 = 0;
                do {
                    d__km += area_params->d_format[4];
                    area_params->d__km.push_back(d__km);
                    i2++;
                } while (d__km < area_params->d_format[3]);
            }
        }
        else if (key.compare(TAG__DELTAH) == 0) {
            if (ParseDouble(value.c_str(), &area_params->delta_h__meter) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_DELTAH, TAG__DELTAH);
            }
        }
        else if (key.compare(TAG__CLIMATE) == 0) {
            if (ParseInteger(value.c_str(), &area_params->climate) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_CLIMATE, TAG__CLIMATE);
            }
        }
        else if (key.compare("n_0") == 0) {
            if (ParseDouble(value.c_str(), &area_params->N_0) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_N0, TAG__N0);
            }
        }
        else if (key.compare(TAG__FREQ) == 0) {
            if (ParseDouble(value.c_str(), &area_params->f__mhz) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_FREQ, TAG__FREQ);
            }
        }
        else if (key.compare(TAG__POL) == 0) {
            if (ParseInteger(value.c_str(), &area_params->pol) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_POL, TAG__POL);
            }
        }
        else if (key.compare(TAG__EPSILON) == 0) {
            if (ParseDouble(value.c_str(), &area_params->epsilon) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_EPSILON, TAG__EPSILON);
            }
        }
        else if (key.compare(TAG__SIGMA) == 0) {
            if (ParseDouble(value.c_str(), &area_params->sigma) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_SIGMA, TAG__SIGMA);
            }
        }
        else if (key.compare(TAG__MDVAR) == 0) {
            if (ParseInteger(value.c_str(), &area_params->mdvar) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_MDVAR, TAG__MDVAR);
            }
        }
        else if (key.compare(TAG__TIME) == 0) {
            if (ParseDouble(value.c_str(), &area_params->time) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_TIME, TAG__TIME);
            }
        }
        else if (key.compare(TAG__LOCATION) == 0) {
            if (ParseDouble(value.c_str(), &area_params->location) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_LOCATION, TAG__LOCATION);
            }
        }
        else if (key.compare(TAG__SITUATION) == 0) {
            int s = 0, e = 0;
            do {
                e = value.find(',', s);
                string str = value.substr(s, e);

                double situation;
                if (ParseDouble(str.c_str(), &situation) == ERROR) {
                    return ParsingErrorHelper(DRVRERR__PARSE_SITUATION, TAG__SITUATION);
                }

                area_params->situation.push_back(situation);
                s = e + 1;
            } while (e != value.npos);
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

                area_params->confidence.push_back(confidence);
                s = e + 1;
            } while (e != value.npos);
        }
        else if (key.compare(TAG__RELIABILITY) == 0) {
            if (ParseDouble(value.c_str(), &area_params->reliability) == ERROR) {
                return ParsingErrorHelper(DRVRERR__PARSE_RELIABILITY, TAG__RELIABILITY);
            }
        }
        else {
            printf("Unknown area mode input parameter '%s'.  Skipping line...\n", key.c_str());
        }
    }

    file.close();

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Validate Area input parameters.  This function only validates
 |                that required parameters are present - not that they are
 |                valid values.  The ITM DLL performs validation to ensure
 |                the parameters are within valid range.
 |
 |        Input:  area_params   - Area input parameter struct
 |
 |       Output:  [void]
 |
 |      Returns:  rtn           - Return error code
 |
 *===========================================================================*/
int ValidateAreaInputs(AreaParams* area_params) {
    if (area_params->h_tx__meter == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__HTX, DRVRERR__VALIDATION_HTX);

    if (area_params->h_rx__meter == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__HRX, DRVRERR__VALIDATION_HTX);

    if (area_params->tx_site_criteria == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__TX_SITE, DRVRERR__VALIDATION_HTX_SITE);

    if (area_params->rx_site_criteria == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__RX_SITE, DRVRERR__VALIDATION_RTX_SITE);

    if (area_params->d__km.size() == 0)
        return Validate_RequiredErrMsgHelper(TAG__DKM, DRVRERR__VALIDATION_DKM);

    if (area_params->delta_h__meter == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__DELTAH, DRVRERR__VALIDATION_DELTAH);

    if (area_params->climate == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__CLIMATE, DRVRERR__VALIDATION_CLIMATE);

    if (area_params->N_0 == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__N0, DRVRERR__VALIDATION_N0);

    if (area_params->f__mhz == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__FREQ, DRVRERR__VALIDATION_FMHZ);

    if (area_params->pol == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__POL, DRVRERR__VALIDATION_POL);

    if (area_params->epsilon == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__EPSILON, DRVRERR__VALIDATION_EPSILON);

    if (area_params->sigma == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__SIGMA, DRVRERR__VALIDATION_SIGMA);

    if (area_params->mdvar == NOT_SET)
        return Validate_RequiredErrMsgHelper(TAG__MDVAR, DRVRERR__VALIDATION_MDVAR);

    // infer TLS vs CR calling
    bool isTLS = (area_params->time != NOT_SET && area_params->location != NOT_SET && area_params->situation.size() > 0);
    bool isCR = (area_params->confidence.size() > 0 && area_params->reliability != NOT_SET);

    if (isTLS && isCR)
        return GeneralErrorMsgHelper("Provided both time/location/situation parameters and confidence/reliability", DRVRERR__VALIDATION_TLS_AND_CR);

    if (!isTLS && !isCR)
        return GeneralErrorMsgHelper("Must provide time/location/situation parameters or confidence/reliability", DRVRERR__VALIDATION_TLS_OR_CR);

    if (isTLS)
        area_params->mode = VARY_TLS;
    else
        area_params->mode = VARY_CR;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Write Area inputs to the report file
 |
 |        Input:  fp            - File pointer
 |                area_params   - Area input parameter struct
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void WriteAreaInputs(FILE* fp, AreaParams* area_params) {
    fprintf_s(fp, "%-24s %-12g %s\n", TAG__HTX, area_params->h_tx__meter, UNITS__METER);
    fprintf_s(fp, "%-24s %-12g %s\n", TAG__HRX, area_params->h_rx__meter, UNITS__METER);
    fprintf_s(fp, "%-24s %-12i ", TAG__TX_SITE, area_params->tx_site_criteria);
    PrintSitingCriteriaLabel(fp, area_params->tx_site_criteria);
    fprintf_s(fp, "%-24s %-12i ", TAG__RX_SITE, area_params->rx_site_criteria);
    PrintSitingCriteriaLabel(fp, area_params->rx_site_criteria);
    if (area_params->d__km.size() == 1) {
        fprintf_s(fp, "%-24s %-12g %s\n", TAG__DKM, area_params->d__km.front(), UNITS__KM);
    }
    else {
        fprintf_s(fp, "%-24s %g,%g,%g,%g,%g\n", 
            TAG__DKM, 
            area_params->d_format[0],
            area_params->d_format[1],
            area_params->d_format[2],
            area_params->d_format[3],
            area_params->d_format[4]);
    }
    fprintf_s(fp, "%-24s %-12g %s\n", TAG__DELTAH, area_params->delta_h__meter, UNITS__METER);
    fprintf_s(fp, "%-24s %-12i ", TAG__CLIMATE, area_params->climate);
    PrintClimateLabel(fp, area_params->climate);
    fprintf_s(fp, "%-24s %-12.2f %s\n", TAG__N0, area_params->N_0, UNITS__NUNIT);
    fprintf_s(fp, "%-24s %-12.2f %s\n", TAG__FREQ, area_params->f__mhz, UNITS__MHZ);
    fprintf_s(fp, "%-24s %-12i ", TAG__POL, area_params->pol);
    PrintPolarizationLabel(fp, area_params->pol);
    fprintf_s(fp, "%-24s %g\n", TAG__EPSILON, area_params->epsilon);
    fprintf_s(fp, "%-24s %g\n", TAG__SIGMA, area_params->sigma);
    fprintf_s(fp, "%-24s %-12i ", TAG__MDVAR, area_params->mdvar);
    PrintMdvarLabel(fp, area_params->mdvar);
    if (area_params->mode == VARY_TLS) {
        fprintf_s(fp, "%-24s %g\n", TAG__TIME, area_params->time);
        fprintf_s(fp, "%-24s %g\n", TAG__LOCATION, area_params->location);
        fprintf_s(fp, "%-24s ", TAG__SITUATION);
        for (int i = 0; i < area_params->situation.size() - 1; i++) {
            fprintf_s(fp, "%g,", area_params->situation[i]);
        }
        fprintf_s(fp, "%g\n", area_params->situation.back());
    }
    else {
        fprintf_s(fp, "%-24s ", TAG__CONFIDENCE);
        for (int i = 0; i < area_params->confidence.size() - 1; i++) {
            fprintf_s(fp, "%g,", area_params->confidence[i]);
        }
        fprintf_s(fp, "%g\n", area_params->confidence.back());

        fprintf_s(fp, "%-24s %g\n", TAG__RELIABILITY, area_params->reliability);
    }
}
