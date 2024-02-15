#include "..\include\itm.h"
#include "..\include\Enums.h"
#include "..\include\Errors.h"

/*=============================================================================
 |
 |  Description: The ITS Irregular Terrain Model (ITM).  This function
 |               exposes point-to-point mode functionality, with variability
 |               specified with time/location/situation (TLS).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                pfl[2]            - Terrain data, in PFL format
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
int ITM_P2P_TLS(double h_tx__meter, double h_rx__meter, const double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
    double *A__db, long *warnings)
{
    IntermediateValues interValues;

    return ITM_P2P_TLS_Ex(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
        time, location, situation, A__db, warnings, &interValues);
}

/*=============================================================================
 |
 |  Description: The ITS Irregular Terrain Model (ITM).  This function
 |               exposes point-to-point mode functionality, with variability
 |               specified with confidence/reliability (CR).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                pfl[2]            - Terrain data, in PFL format
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
int ITM_P2P_CR(double h_tx__meter, double h_rx__meter, const double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
    double *A__db, long *warnings)
{
    IntermediateValues interValues;

    int rtn = ITM_P2P_TLS_Ex(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
        reliability, 50, confidence, A__db, warnings, &interValues);

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
 |  Description: The ITS Irregular Terrain Model (ITM).  This function
 |               exposes point-to-point mode functionality, with variability
 |               specified with confidence/reliability (CR).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                pfl[2]            - Terrain data, in PFL format
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
int ITM_P2P_CR_Ex(double h_tx__meter, double h_rx__meter, const double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
    double *A__db, long *warnings, IntermediateValues *interValues)
{
    int rtn = ITM_P2P_TLS_Ex(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
        reliability, 50, confidence, A__db, warnings, interValues);

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
 |  Description: The ITS Irregular Terrain Model (ITM).  This function
 |               exposes point-to-point mode functionality, with variability
 |               specified with time/location/situation (TLS).
 |
 |        Input:  h_tx__meter       - Structural height of the TX, in meters
 |                h_rx__meter       - Structural height of the RX, in meters
 |                pfl[2]            - Terrain data, in PFL format
 |                climate           - Radio climate enum
 |                N_0               - Refractivity, in N-Units
 |                f__mhz            - Frequency, in MHz
 |                pol               - Polarization enum
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
int ITM_P2P_TLS_Ex(double h_tx__meter, double h_rx__meter, const double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
    double *A__db, long *warnings, IntermediateValues *interValues)
{
    double N_s;                 // Surface refractivity, in N-Units
    double gamma_e;             // Curvature of the effective earth
    double delta_h__meter;      // Terrain irregularity parameter
    double d__meter;            // Path distance, in meters
    complex<double> Z_g;	    // Ground impedance
    double theta_hzn[2];        // Terminal horizon angles
    double d_hzn__meter[2];     // Terminal horizon distances
    double h_e__meter[2];       // Terminal effective heights

    *warnings = NO_WARNINGS;    // Initialize to no warnings

    // initial input validation check - some validation occurs later in calculations
    int rtn = ValidateInputs(h_tx__meter, h_rx__meter, climate, time, location, situation, N_0, f__mhz, pol, epsilon, sigma, mdvar, warnings);
    if (rtn != SUCCESS)
        return rtn;

    interValues->d__km = (pfl[0] * pfl[1]) / 1000;

    int np = int(pfl[0]);       // number of points in the pfl

    // switch from percentages to ratios
    time /= 100;
    location /= 100;
    situation /= 100;

    // compute the average path height, ignoring first and last 10%
    int p10 = int(0.1 * np);    // 10% of np
    double h_sys__meter = 0;    // Height of the system above mean sea level

    for (int i = p10; i <= np - p10; i++)
        h_sys__meter += pfl[i + 2];

    h_sys__meter = h_sys__meter / (np - 2 * p10 + 1);

    InitializePointToPoint(f__mhz, h_sys__meter, N_0, pol, epsilon, sigma, &Z_g, &gamma_e, &N_s);

    const double h__meter[2] = { h_tx__meter, h_rx__meter };
    QuickPfl(pfl, gamma_e, h__meter, theta_hzn, d_hzn__meter, h_e__meter, &delta_h__meter, &d__meter);

    // Reference attenuation, in dB
    double A_ref__db = 0;
    int propmode = MODE__NOT_SET;
    rtn = LongleyRice(theta_hzn, f__mhz, Z_g, d_hzn__meter, h_e__meter, gamma_e, N_s, delta_h__meter, h__meter, d__meter, MODE__P2P, 
        &A_ref__db, warnings, &propmode);
    if (rtn != SUCCESS)
        return rtn;

    double A_fs__db = FreeSpaceLoss(d__meter, f__mhz);

    *A__db = Variability(time, location, situation, h_e__meter, delta_h__meter, f__mhz, d__meter, A_ref__db, climate, mdvar, warnings) + A_fs__db;

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
};
