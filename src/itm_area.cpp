#include "..\include\itm.h"
#include "..\include\Enums.h"
#include "..\include\Errors.h"

/*=============================================================================
 |
 |  Description:  The ITS Irregular Terrain Model (ITM).  This function
 |                exposes area mode functionality, with variability
 |                specified with time/location/situation (TLS).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                tx_site_criteria  - Siting criteria of the TX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                rx_site_criteria  - Siting criteria of the RX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                d__km             - Path distance, in km
 |                delta_h__meter    - Terrain irregularity parameter
 |                climate           - Radio climate
 |                                      + 1 : CLIMATE__EQUATORIAL
 |                                      + 2 : CLIMATE__CONTINENTAL_SUBTROPICAL
 |                                      + 3 : CLIMATE__MARITIME_SUBTROPICAL
 |                                      + 4 : CLIMATE__DESERT
 |                                      + 5 : CLIMATE__CONTINENTAL_TEMPERATE
 |                                      + 6 : CLIMATE__MARITIME_TEMPERATE_OVER_LAND
 |                                      + 7 : CLIMATE__MARITIME_TEMPERATE_OVER_SEA
 |                N_0               - Refractivity, in N-Units
 |                f__mhz            - Frequency, in MHz
 |                pol               - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon           - Relative permittivity
 |                sigma             - Conductivity
 |                mdvar             - Mode of variability
 |                time              - Time percentage, 0 < time < 100
 |                location          - Location percentage, 0 < location < 100
 |                situation         - Situation percentage, 0 < situation < 100
 |
 |      Outputs:  A__db             - Basic transmission loss, in dB
 |                warnings          - Warning flags
 |
 |      Returns:  error             - Error code
 |
 *===========================================================================*/
int ITM_AREA_TLS(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double time, double location, double situation, double *A__db, long *warnings)
{
    IntermediateValues interValues;

    return ITM_AREA_TLS_Ex(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter, climate,
        N_0, f__mhz, pol, epsilon, sigma, mdvar, time, location, situation, A__db, warnings, &interValues);
}

/*=============================================================================
 |
 |  Description:  The ITS Irregular Terrain Model (ITM).  This function
 |                exposes area mode functionality, with variability
 |                specified with time/location/situation (TLS)
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                tx_site_criteria  - Siting criteria of the TX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                rx_site_criteria  - Siting criteria of the RX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                d__km             - Path distance, in km
 |                delta_h__meter    - Terrain irregularity parameter
 |                climate           - Radio climate
 |                                      + 1 : CLIMATE__EQUATORIAL
 |                                      + 2 : CLIMATE__CONTINENTAL_SUBTROPICAL
 |                                      + 3 : CLIMATE__MARITIME_SUBTROPICAL
 |                                      + 4 : CLIMATE__DESERT
 |                                      + 5 : CLIMATE__CONTINENTAL_TEMPERATE
 |                                      + 6 : CLIMATE__MARITIME_TEMPERATE_OVER_LAND
 |                                      + 7 : CLIMATE__MARITIME_TEMPERATE_OVER_SEA
 |                N_0               - Refractivity, in N-Units
 |                f__mhz            - Frequency, in MHz
 |                pol               - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon           - Relative permittivity
 |                sigma             - Conductivity
 |                mdvar             - Mode of variability
 |                time              - Time percentage, 0 < time < 100
 |                location          - Location percentage, 0 < location < 100
 |                situation         - Situation percentage, 0 < situation < 100
 |
 |      Outputs:  A__db             - Basic transmission loss, in dB
 |                warnings          - Warning flags
 |                interValues       - Struct of intermediate values
 |
 |      Returns:  error             - Error code
 |
 *===========================================================================*/
int ITM_AREA_TLS_Ex(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km, 
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, 
    int mdvar, double time, double location, double situation, double *A__db, long *warnings, IntermediateValues *interValues)
{
    *warnings = NO_WARNINGS;

    // inital input validation check - some validation occurs later in calculations
    int rtn = ValidateInputs(h_tx__meter, h_rx__meter, climate, time, location, situation, N_0, f__mhz, pol, epsilon, sigma, mdvar, warnings);
    if (rtn != SUCCESS)
        return rtn;

    // switch from percentages to ratios
    time /= 100;
    location /= 100;
    situation /= 100;

    // additional area mode parameter validation checks
    if (d__km <= 0)
        return ERROR__PATH_DISTANCE;
    if (delta_h__meter < 0)
        return ERROR__DELTA_H;
    if (tx_site_criteria != SITING_CRITERIA__RANDOM &&
        tx_site_criteria != SITING_CRITERIA__CAREFUL &&
        tx_site_criteria != SITING_CRITERIA__VERY_CAREFUL)
        return ERROR__TX_SITING_CRITERIA;
    if (rx_site_criteria != SITING_CRITERIA__RANDOM &&
        rx_site_criteria != SITING_CRITERIA__CAREFUL &&
        rx_site_criteria != SITING_CRITERIA__VERY_CAREFUL)
        return ERROR__RX_SITING_CRITERIA;

    int site_criteria[2] = { tx_site_criteria, rx_site_criteria };
    double h__meter[2] = { h_tx__meter, h_rx__meter };
    interValues->d__km = d__km;

    double theta_hzn[2];
    double d_hzn__meter[2];
    double h_e__meter[2];
    complex<double> Z_g;
    double N_s;
    double gamma_e;
    double A_ref__db = 0;

    InitializePointToPoint(f__mhz, 0.0, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s);

    InitializeArea(site_criteria, gamma_e, delta_h__meter, h__meter, h_e__meter, d_hzn__meter, theta_hzn);

    double d__meter = d__km * 1000;
    int propmode = MODE__NOT_SET;
    rtn = LongleyRice(theta_hzn, f__mhz, Z_g, d_hzn__meter, h_e__meter, gamma_e, N_s, delta_h__meter, h__meter, d__meter, MODE__AREA, 
        &A_ref__db, warnings, &propmode);
    if (rtn != SUCCESS)
        return rtn;

    double A_fs__db = FreeSpaceLoss(d__meter, f__mhz);

    *A__db = A_fs__db + Variability(time, location, situation, h_e__meter, delta_h__meter, f__mhz, d__meter, A_ref__db, climate, mdvar, warnings);

    // Save off intermediate values
    interValues->A_ref__db = A_ref__db;
    interValues->A_fs__db = A_fs__db;
    interValues->delta_h__meter = delta_h__meter;
    interValues->d_hzn__meter[0] = d_hzn__meter[0];
    interValues->d_hzn__meter[1] = d_hzn__meter[1];
    interValues->h_e__meter[0] = h_e__meter[0];
    interValues->h_e__meter[1] = h_e__meter[1];
    interValues->N_s = N_s;
    interValues->theta_hzn[0] = theta_hzn[0];
    interValues->theta_hzn[1] = theta_hzn[1];
    interValues->mode = propmode;

    if (*warnings != NO_WARNINGS)
        return SUCCESS_WITH_WARNINGS;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  The ITS Irregular Terrain Model (ITM).  This function
 |                exposes area mode functionality, with variability
 |               specified with confidence/reliability (CR).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                tx_site_criteria  - Siting criteria of the TX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                rx_site_criteria  - Siting criteria of the RX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                d__km             - Path distance, in km
 |                delta_h__meter    - Terrain irregularity parameter
 |                climate           - Radio climate
 |                                      + 1 : CLIMATE__EQUATORIAL
 |                                      + 2 : CLIMATE__CONTINENTAL_SUBTROPICAL
 |                                      + 3 : CLIMATE__MARITIME_SUBTROPICAL
 |                                      + 4 : CLIMATE__DESERT
 |                                      + 5 : CLIMATE__CONTINENTAL_TEMPERATE
 |                                      + 6 : CLIMATE__MARITIME_TEMPERATE_OVER_LAND
 |                                      + 7 : CLIMATE__MARITIME_TEMPERATE_OVER_SEA
 |                N_0               - Refractivity, in N-Units
 |                f__mhz            - Frequency, in MHz
 |                pol               - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon           - Relative permittivity
 |                sigma             - Conductivity
 |                mdvar             - Mode of variability
 |                confidence        - Confidence percentage, 0 < confidence < 100
 |                reliability       - Reliability percentage, 0 < reliability < 100
 |
 |      Outputs:  A__db             - Basic transmission loss, in dB
 |                warnings          - Warning flags
 |
 |      Returns:  error             - Error code
 |
 *===========================================================================*/
int ITM_AREA_CR(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double confidence, double reliability, double *A__db, long *warnings)
{
    IntermediateValues interValues;

    int rtn = ITM_AREA_TLS_Ex(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km,
        delta_h__meter, climate, N_0, f__mhz, pol, epsilon, sigma,
        mdvar, reliability, 50, confidence, A__db, warnings, &interValues);

    // convert TLS error codes for time and situation into CR error codes for confidence and reliability, if needed
    if (rtn == ERROR__INVALID_TIME)
        return ERROR__INVALID_RELIABILITY;
    if (rtn == ERROR__INVALID_SITUATION)
        return ERROR__INVALID_CONFIDENCE;
    else
        return rtn;
}

/*=============================================================================
 |
 |  Description:  The ITS Irregular Terrain Model (ITM).  This function
 |                exposes area mode functionality, with variability
 |               specified with confidence/reliability (CR).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                tx_site_criteria  - Siting criteria of the TX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                rx_site_criteria  - Siting criteria of the RX
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                d__km             - Path distance, in km
 |                delta_h__meter    - Terrain irregularity parameter
 |                climate           - Radio climate
 |                                      + 1 : CLIMATE__EQUATORIAL
 |                                      + 2 : CLIMATE__CONTINENTAL_SUBTROPICAL
 |                                      + 3 : CLIMATE__MARITIME_SUBTROPICAL
 |                                      + 4 : CLIMATE__DESERT
 |                                      + 5 : CLIMATE__CONTINENTAL_TEMPERATE
 |                                      + 6 : CLIMATE__MARITIME_TEMPERATE_OVER_LAND
 |                                      + 7 : CLIMATE__MARITIME_TEMPERATE_OVER_SEA
 |                N_0               - Refractivity, in N-Units
 |                f__mhz            - Frequency, in MHz
 |                pol               - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon           - Relative permittivity
 |                sigma             - Conductivity
 |                mdvar             - Mode of variability
 |                confidence        - Confidence percentage, 0 < confidence < 100
 |                reliability       - Reliability percentage, 0 < reliability < 100
 |
 |      Outputs:  A__db             - Basic transmission loss, in dB
 |                warnings          - Warning flags
 |                interValues       - Struct of intermediate values
 |
 |      Returns:  error             - Error code
 |
 *===========================================================================*/
int ITM_AREA_CR_Ex(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double confidence, double reliability, double *A__db, long *warnings, IntermediateValues *interValues)
{
    int rtn = ITM_AREA_TLS_Ex(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km,
        delta_h__meter, climate, N_0, f__mhz, pol, epsilon, sigma,
        mdvar, reliability, 50, confidence, A__db, warnings, interValues);

    // convert TLS error codes for time and situation into CR error codes for confidence and reliability, if needed
    if (rtn == ERROR__INVALID_TIME)
        return ERROR__INVALID_RELIABILITY;
    if (rtn == ERROR__INVALID_SITUATION)
        return ERROR__INVALID_CONFIDENCE;
    else
        return rtn;
}