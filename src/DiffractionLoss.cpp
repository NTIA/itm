#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Compute the diffraction loss at a specified distance
 |
 |        Input:  d__meter          - Path distance, in meters
 |                d_hzn__meter[2]   - Horizon distances, in meters
 |                h_e__meter[2]     - Effective terminal heights, in meters
 |                Z_g            - Complex ground impedance
 |                a_e__meter     - Effective earth radius, in meters
 |                delta_h__meter - Terrain irregularity parameter, in meters
 |                h__meter[2]       - Terminal heights, in meters
 |                mode           - Area or Point-to-Point mode flag
 |                theta_los      - Angular distance of line-of-sight region
 |                d_sML__meter   - Maximum line-of-sight distance for 
 |                                 a smooth earth, in meters
 |                f__mhz         - Frequency, in MHz
 |
 |      Outputs:  [None]
 |
 |      Returns:  A_d__db        - Diffraction loss, in dB
 |
 *===========================================================================*/
double DiffractionLoss(const double d__meter, const double d_hzn__meter[2], const double h_e__meter[2], const complex<double> Z_g, const double a_e__meter, 
    const double delta_h__meter, const double h__meter[2], const int mode, const double theta_los, const double d_sML__meter, const double f__mhz)
{
    double A_k__db = KnifeEdgeDiffraction(d__meter, f__mhz, a_e__meter, theta_los, d_hzn__meter);

    double A_se__db = SmoothEarthDiffraction(d__meter, f__mhz, a_e__meter, theta_los, d_hzn__meter, h_e__meter, Z_g);

    //////////////////
    // Terrain clutter

    // Terrain roughness term, using d_sML__meter, per [ERL 79-ITS 67, page 3-13]
    double delta_h_dsML__meter = TerrainRoughness(d_sML__meter, delta_h__meter);

    double sigma_h_d__meter = SigmaHFunction(delta_h_dsML__meter);

    // Clutter factor
    // [ERL 79-ITS 67, Eqn 3.38c]
    double A_fo__db = MIN(15.0, 5 * log10(1.0 + 1e-5 * h__meter[0] * h__meter[1] * f__mhz * sigma_h_d__meter));

    //////////////////////////////
    // Combined diffraction losses

    // compute the weighting factor in the following calculations

    double delta_h_d__meter = TerrainRoughness(d__meter, delta_h__meter);

    double q = h__meter[0] * h__meter[1];
    double qk = h_e__meter[0] * h_e__meter[1] - q;

    // For low antennas with known path parameters, C ~= 10 [ERL 79-ITS 67, page 3-8]
    if (mode == MODE__P2P)
        q += 10.0;

    double term1 = sqrt(1.0 + qk / q);                              // square root term in [ERL 79-ITS 67, Eqn 2.23]

    double d_ML__meter = d_hzn__meter[0] + d_hzn__meter[1];         // Maximum line-of-sight distance for actual path
    q = (term1 + (-theta_los * a_e__meter + d_ML__meter) / d__meter) * MIN(delta_h_d__meter * f__mhz / 47.7, 6283.2);

    // weighting factor [ERL 17-ITS 67, Eqn 3.23]
    double w = 25.1 / (25.1 + sqrt(q));

    double A_d__db = w * A_se__db + (1.0 - w) * A_k__db + A_fo__db;

    return A_d__db;
}