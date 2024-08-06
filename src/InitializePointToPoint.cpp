#include "..\include\itm.h"
#include "..\include\Enums.h"

/*=============================================================================
 |
 |  Description:  Initialize parameters for point-to-point mode
 |
 |        Input:  f__mhz            - Frequency, in MHz
 |                h_sys__meter      - Average height of the path above
 |                                    mean sea level, in meters
 |                N_0               - Refractivity, in N-Units
 |                pol               - Polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                epsilon           - Relative permittivity
 |                sigma             - Conductivity
 |
 |      Outputs:  Z_g               - Complex ground impedance
 |                gamma_e           - Curvature of the effective earth
 |                N_s               - Surface refractivity, in N-Units
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void InitializePointToPoint(const double f__mhz, const double h_sys__meter, const double N_0, const int pol, 
    const double epsilon, const double sigma, complex<double> *Z_g, double *gamma_e, double *N_s)
{
    // gamma_a is the curvature of the actual earth, ~1 / 6370 km
    double gamma_a = 157e-9;

    // scale the refractivity based on the elevation above mean sea level
    if (h_sys__meter == 0.0)
        *N_s = N_0;
    else
        *N_s = N_0 * exp(-h_sys__meter / 9460.0);               // [TN101, Eq 4.3]

    // gamma_e is the curvature of the effective earth
    *gamma_e = gamma_a * (1.0 - 0.04665 * exp(*N_s / 179.3));   // [TN101, Eq 4.4], reworked

    // complex relative permittivity
    complex<double> ep_r = complex<double>(epsilon, 18000 * sigma / f__mhz);

    *Z_g = sqrt(ep_r - 1.0);                        // ground impedance (horizontal polarization)

    if (pol == POLARIZATION__VERTICAL)              // adjust for vertical polarization
        *Z_g = *Z_g / ep_r;
}