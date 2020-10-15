#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  The attenuation function, F(th * d)
 |
 |        Input:  td             - theta * distance
 |
 |      Outputs:  [None]
 |
 |      Returns:  F()            - in dB
 |
 *===========================================================================*/
double FFunction(double td)
{
    // constants from [Algorithm, 6.9]
    double a[] = { 133.4, 104.6, 71.8 };
    double b[] = { 0.332e-3, 0.212e-3, 0.157e-3 };
    double c[] = { -10, -2.5, 5 };

    int i;

    // select the set of values to use
    if (td <= 10e3)         // <= 10 km
        i = 0;
    else if (td <= 70e3)    // 10 km to 70 km 
        i = 1;
    else                    // > 70 km
        i = 2;

    double F_0 = a[i] + b[i] * td + c[i] * log10(td);     // [Algorithm, 6.9]

    return F_0;
}

/*=============================================================================
 |
 |  Description:  Troposcatter loss
 |
 |        Input:  d__meter          - Path distance, in meters
 |                theta_hzn[2]      - Terminal horizon angles
 |                d_hzn__meter[2]   - Terminal horizon distances, in meters
 |                h_e__meter[2]     - Effective terminal heights, in meters
 |                a_e__meter        - Effective earth radius, in meters
 |                N_s               - Surface refractivity, in N-Units
 |                f__mhz            - Frequency, in MHz
 |                theta_los         - Angular distance of LOS region
 |
 |      Outputs:  h0                - H_0() value
 |
 |      Returns:  F()               - in dB
 |
 *===========================================================================*/
double TroposcatterLoss(double d__meter, double theta_hzn[2], double d_hzn__meter[2], double h_e__meter[2], 
    double a_e__meter, double N_s, double f__mhz, double theta_los, double *h0)
{
    double H_0;

    // wavenumber, k
    double wn = f__mhz / 47.7;

    if (*h0 > 15.0)     // short-circuit calculations if already greater than 15 dB
        H_0 = *h0;
    else
    {
        double ad = d_hzn__meter[0] - d_hzn__meter[1];
        double rr = h_e__meter[1] / h_e__meter[0];

        if (ad < 0.0)       // ensure correct frame of reference
        {
            ad = -ad;
            rr = 1.0 / rr;
        }

        double theta = theta_hzn[0] + theta_hzn[1] + d__meter / a_e__meter;    // angular distance, in radians

        // [TN101, Eqn 9.4a]
        double r_1 = 2.0 * wn * theta * h_e__meter[0];
        double r_2 = 2.0 * wn * theta * h_e__meter[1];

        if (r_1 < 0.2 && r_2 < 0.2)
            return 1001;                // "If both r_1 and r_2 are less than 0.2 the function A_scat is not defined (or is infinite)" [Algorithm, page 11]

        double s = (d__meter - ad) / (d__meter + ad);       // asymmetry parameter

        // "In all of this, we truncate the values of s and q at 0.1 and 10" [Algorithm, page 16]
        double q = MIN(MAX(0.1, rr / s), 10.0);             // TN101, Eqn 9.5
        s = MAX(0.1, s);                                    // TN101, Eqn 9.5

        double h_0__meter = (d__meter - ad) * (d__meter + ad) * theta * 0.25 / d__meter;   // height of cross-over, [Algorithm, 4.66] [TN101v1, 9.3b]

        double Z_0__meter = 1.7556e3;                       // Scale height, [Algorithm, 4.67]
        double Z_1__meter = 8.0e3;                          // [Algorithm, 4.67]
        double eta_s = (h_0__meter / Z_0__meter) * (1.0 + (0.031 - N_s * 2.32e-3 + pow(N_s, 2) * 5.67e-6) * exp(-pow(MIN(1.7, h_0__meter / Z_1__meter), 6)));     // Scattering efficiency factor, eta_s [TN101 Eqn 9.3a]

        double H_00 = (H0Function(r_1, eta_s) + H0Function(r_2, eta_s)) / 2;                        // First term in TN101v1, Eqn 9.5
        double Delta_H_0 = MIN(H_00, 6.0 * (0.6 - log10(MAX(eta_s, 1.0))) * log10(s) * log10(q));

        H_0 = H_00 + Delta_H_0;                             // TN101, Eqn 9.5
        H_0 = MAX(H_0, 0.0);                                // "If Delta_H_0 would make H_0 negative, use H_0 = 0" [TN101v1, p9.4] 

        if (eta_s < 1.0)    // if <=1, interpolate with the special case of eta_s = 0
            H_0 = eta_s * H_0 + (1.0 - eta_s) * 10 * log10(pow((1.0 + SQRT2 / r_1) * (1.0 + SQRT2 / r_2), 2) * (r_1 + r_2) / (r_1 + r_2 + 2 * SQRT2));

        // "If, at d_5, calculations show that H_0 will exceed 15 dB, they are replaced by the value it has at d_6" [Algorithm, page 12]
        if (H_0 > 15.0 && *h0 >= 0.0)
            H_0 = *h0;
    }

    *h0 = H_0;
    double th = d__meter / a_e__meter - theta_los;

    double D_0__meter = 40e3;   // [Algorithm, 6.8]
    double H__meter = 47.7;     // [Algorithm, 4.63]
    return FFunction(th * d__meter) + 10 * log10(wn * H__meter * pow(th, 4)) - 0.1 * (N_s - 301.0) * exp(-th * d__meter / D_0__meter) + H_0;    // [Algorithm, 4.63]
}