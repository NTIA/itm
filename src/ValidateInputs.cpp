#include "..\include\itm.h"
#include "..\include\Enums.h"
#include "..\include\Errors.h"
#include "..\include\Warnings.h"

/*=============================================================================
 |
 |  Description:  Perform input parameter validation.  This function only
 |                applies to the set of variables common to both ITM
 |                point-to-point mode and area mode.
 |
 |        Input:  h_tx__meter    - Structural height of the TX, in meters
 |				  h_rx__meter    - Structural height of the RX, in meters
 |                climate        - Radio climate enum
 |                                      + 1 : CLIMATE__EQUATORIAL
 |                                      + 2 : CLIMATE__CONTINENTAL_SUBTROPICAL
 |                                      + 3 : CLIMATE__MARITIME_SUBTROPICAL
 |                                      + 4 : CLIMATE__DESERT
 |                                      + 5 : CLIMATE__CONTINENTAL_TEMPERATE
 |                                      + 6 : CLIMATE__MARITIME_TEMPERATE_OVER_LAND
 |                                      + 7 : CLIMATE__MARITIME_TEMPERATE_OVER_SEA
 |                time           - Time percentage, 0 < time < 100
 |                location       - Location percentage, 0 < location < 100
 |                situation      - Situation percentage, 0 < situation < 100
 |                N_0            - Refractivity, in N-Units
 |                f__mhz         - Frequency, in MHz
 |                pol            - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon        - Relative permittivity
 |                sigma          - Conductivity
 |                mdvar          - Mode of variability
 |
 |      Outputs:  warnings       - Warning messages
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
int ValidateInputs(double h_tx__meter, double h_rx__meter, int climate, double time,
    double location, double situation, double N_0, double f__mhz, int pol,
    double epsilon, double sigma, int mdvar, long *warnings)
{
    if (h_tx__meter < 1.0 || h_tx__meter > 1000.0)
        *warnings |= WARN__TX_TERMINAL_HEIGHT;

    if (h_tx__meter < 0.5 || h_tx__meter > 3000.0)
        return ERROR__TX_TERMINAL_HEIGHT;

    if (h_rx__meter < 1.0 || h_rx__meter > 1000.0)
        *warnings |= WARN__RX_TERMINAL_HEIGHT;

    if (h_rx__meter < 0.5 || h_rx__meter > 3000.0)
        return ERROR__RX_TERMINAL_HEIGHT;

    if (climate != CLIMATE__EQUATORIAL &&
        climate != CLIMATE__CONTINENTAL_SUBTROPICAL &&
        climate != CLIMATE__MARITIME_SUBTROPICAL &&
        climate != CLIMATE__DESERT &&
        climate != CLIMATE__CONTINENTAL_TEMPERATE &&
        climate != CLIMATE__MARITIME_TEMPERATE_OVER_LAND &&
        climate != CLIMATE__MARITIME_TEMPERATE_OVER_SEA)
        return ERROR__INVALID_RADIO_CLIMATE;

    if (N_0 < 250 || N_0 > 400)
        return ERROR__REFRACTIVITY;

    if (f__mhz < 40.0 || f__mhz > 10000.0)
        *warnings |= WARN__FREQUENCY;

    if (f__mhz < 20 || f__mhz > 20000)
        return ERROR__FREQUENCY;

    if (pol != POLARIZATION__HORIZONTAL &&
        pol != POLARIZATION__VERTICAL)
        return ERROR__POLARIZATION;

    if (epsilon < 1)
        return ERROR__EPSILON;

    if (sigma <= 0)
        return ERROR__SIGMA;

    if ((mdvar < 0) ||
        (mdvar > 3 && mdvar < 10) ||
        (mdvar > 13 && mdvar < 20) ||
        (mdvar > 23 && mdvar < 30) ||
        (mdvar > 33))
        return ERROR__MDVAR;

    if (situation <= 0 || situation >= 100)
        return ERROR__INVALID_SITUATION;

    if (time <= 0 || time >= 100)
        return ERROR__INVALID_TIME;

    if (location <= 0 || location >= 100)
        return ERROR__INVALID_LOCATION;

    return SUCCESS;
}
