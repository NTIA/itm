#include "ITS.Propagation.ITM/ITM.h"

namespace ITS {
namespace Propagation {
namespace ITM {
    
/*=============================================================================
 |
 |  Description:  Compute the loss in the line-of-sight region
 |
 |        Input:  d__meter          - Path distance, in meters
 |                h_e__meter[2]     - Terminal effective heights, in meters
 |                Z_g               - Complex surface transfer impedance
 |                delta_h__meter    - Terrain irregularity parameter
 |                M_d               - Diffraction slope
 |                A_d0              - Diffraction intercept
 |                d_sML__meter      - Maximum line-of-sight distance for
 |                                    a smooth earth, in meters
 |                f__mhz            - Frequency, in MHz
 |
 |      Outputs:  [None]
 |
 |      Returns:  A_los__db         - Loss, in dB
 |
 *===========================================================================*/
double LineOfSightLoss(double d__meter, double h_e__meter[2], complex<double> Z_g, double delta_h__meter, 
    double M_d, double A_d0, double d_sML__meter, double f__mhz)
{
    double delta_h_d__meter = TerrainRoughness(d__meter, delta_h__meter);

    double sigma_h_d__meter = SigmaHFunction(delta_h_d__meter);

    // wavenumber, k
    double wn = f__mhz / 47.7;

    // [Algorithm, Eqn 4.46]
    double sin_psi = (h_e__meter[0] + h_e__meter[1]) / sqrt(pow(d__meter, 2) + pow(h_e__meter[0] + h_e__meter[1], 2));

    // [Algorithm, Eqn 4.47]
    complex<double> R_e = (sin_psi - Z_g) / (sin_psi + Z_g) * exp(-MIN(10.0, wn * sigma_h_d__meter * sin_psi));

    // q = Magnitude of R_e', [Algorithm, Eqn 4.48]
    double q = pow(R_e.real(), 2) + pow(R_e.imag(), 2);
    if (q < 0.25 || q < sin_psi)
        R_e = R_e * sqrt(sin_psi / q);

    // phase difference between rays, [Algorithm, Eqn 4.49]
    double delta_phi = wn * 2.0 * h_e__meter[0] * h_e__meter[1] / d__meter;

    // [Algorithm, Eqn 4.50]
    if (delta_phi > PI / 2.0)
        delta_phi = PI - pow(PI / 2.0, 2) / delta_phi;

    // Two-ray attenuation
    complex<double> rr = complex<double>(cos(delta_phi), -sin(delta_phi)) + R_e;
    double A_t__db = -10 * log10(pow(rr.real(), 2) + pow(rr.imag(), 2));

    // Extended diffraction attenuation
    double A_d__db = M_d * d__meter + A_d0;

    // weighting factor
    double w = 1 / (1 + f__mhz * delta_h__meter / MAX(10e3, d_sML__meter));

    double A_los__db = w * A_t__db + (1 - w) * A_d__db;

    return A_los__db;
}

}  // namespace ITM
}  // namespace Propagation
}  // namespace ITS