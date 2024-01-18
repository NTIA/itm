#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Compute the smooth earth diffraction loss using the 
 |                Vogler 3-radii method
 |
 |        Input:  d__meter          - Path distance, in meters
 |                f__mhz            - Frequency, in MHz
 |                a_e__meter        - Effective earth radius, in meters
 |                theta_los         - Angular distance of line-of-sight region
 |                d_hzn__meter[2]   - Horizon distances, in meters
 |                h_e__meter[2]     - Effective terminal heights, in meters
 |                Z_g               - Complex ground impedance
 |
 |      Outputs:  [None]
 |
 |      Returns:  A_r__db           - Smooth-earth diffraction loss, in dB
 |
 *===========================================================================*/
double SmoothEarthDiffraction(double d__meter, double f__mhz, double a_e__meter, double theta_los, 
    const double d_hzn__meter[2], const double h_e__meter[2], complex<double> Z_g)
{
    double a__meter[3];
    double d__km[3];
    double F_x__db[2];
    double K[3];
    double B_0[3];
    double x__km[3];
    double C_0[3];

    double theta_nlos = d__meter / a_e__meter - theta_los;                          // [Algorithm, Eqn 4.12]
    double d_ML__meter = d_hzn__meter[0] + d_hzn__meter[1];                         // Maximum line-of-sight distance for actual path

    // compute 3 radii
    a__meter[0] = (d__meter - d_ML__meter) / (d__meter / a_e__meter - theta_los);   // which is a_e__meter when theta_los = d_ML__meter / a_e__meter
    a__meter[1] = 0.5 * pow(d_hzn__meter[0], 2) / h_e__meter[0];                    // Compute the radius of the effective earth for terminal j using[Volger 1964, Eqn 3] re - arranged
    a__meter[2] = 0.5 * pow(d_hzn__meter[1], 2) / h_e__meter[1];                    // Compute the radius of the effective earth for terminal j using[Volger 1964, Eqn 3] re - arranged

    d__km[0] = (a__meter[0] * theta_nlos) / 1000.0;                                 // angular distance of the "diffraction path"
    d__km[1] = d_hzn__meter[0] / 1000.0;
    d__km[2] = d_hzn__meter[1] / 1000.0;

    for (int i = 0; i < 3; i++)
    {
        // C_0 is the ratio of the 4/3 earth to effective earth (technically Vogler 1964 ratio is 4/3 to effective earth k value), all raised to the (1/3) power.
        // C_0 = (4 / 3k) ^ (1 / 3) [Vogler 1964, Eqn 2]
        C_0[i] = pow((4.0 / 3.0) * a_0__meter / a__meter[i], THIRD);

        // [Vogler 1964, Eqn 6a / 7a]
        K[i] = 0.017778 * C_0[i] * pow(f__mhz, -THIRD) / abs(Z_g);

        // compute B_0 for each radius
        // [Vogler 1964, Fig 4]
        B_0[i] = 1.607 - K[i];
    }

    // compute x__km for each radius [Vogler 1964, Eqn 2]
    x__km[1] = B_0[1] * pow(C_0[1], 2) * pow(f__mhz, THIRD) * d__km[1];
    x__km[2] = B_0[2] * pow(C_0[2], 2) * pow(f__mhz, THIRD) * d__km[2];
    x__km[0] = B_0[0] * pow(C_0[0], 2) * pow(f__mhz, THIRD) * d__km[0] + x__km[1] + x__km[2];

    // compute height gain functions
    F_x__db[0] = HeightFunction(x__km[1], K[1]);
    F_x__db[1] = HeightFunction(x__km[2], K[2]);

    // compute distance function
    double G_x__db = 0.05751 * x__km[0] - 10.0 * log10(x__km[0]);                   // [TN101, Eqn 8.4] & [Volger 1964, Eqn 13]

    return G_x__db - F_x__db[0] - F_x__db[1] - 20;                                  // [Algorithm, Eqn 4.20] & [Volger 1964]
}

/*=============================================================================
 |
 |  Description:  Height Function, F(x, K) for smooth earth diffraction
 |
 |        Input:  x__km          - Normalized distance, in meters
 |                K              - K value
 |
 |      Outputs:  [None]
 |
 |      Returns:  F(x, K)        - in dB
 |
 *===========================================================================*/
double HeightFunction(double x__km, double K)
{
    double w;
    double result;

    if (x__km < 200.0)
    {
        w = -log(K);

        if (K < 1e-5 || x__km * pow(w, 3) > 5495.0)
        {
            result = -117.0;

            if (x__km > 1.0)
                result = 17.372 * log(x__km) + result;
        }
        else
            result = 2.5e-5 * pow(x__km, 2) / K - 8.686 * w - 15.0;
    }
    else
    {
        result = 0.05751 * x__km - 4.343 * log(x__km);

        if (x__km < 2000)
        {
            w = 0.0134 * x__km * exp(-0.005 * x__km);
            result = (1.0 - w) * result + w * (17.372 * log(x__km) - 117.0);
        }
    }

    return result;
}