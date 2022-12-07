#include "..\include\itm.h"
#include "..\include\Enums.h"
#include "..\include\Errors.h"
#include "..\include\Warnings.h"

/*=============================================================================
 |
 |  Description:  Compute the reference attenuation, using the 
 |                Longley-Rice method
 |
 |        Input:  theta_hzn[2]      - Terminal horizon angles
 |                f__mhz            - Frequency, in MHz
 |                Z_g               - Complex surface transfer impedance
 |                d_hzn__meter[2]   - Terminal horizon distances, in meters
 |                h_e__meter[2]     - Effective terminal heights, in meters
 |                gamma_e           - Curvature of the effective earth
 |                N_s               - Surface refractivity, in N-Units
 |                delta_h__meter    - Terrain irregularity parameter
 |                h__meter[2]       - Terminal structural heights, in meters
 |                d__meter          - Path distance, in meters
 |                mode              - Mode of operation (P2P or Area)
 |
 |      Outputs:  A_ref__db         - Reference attenuation, in dB
 |                warnings          - Warning flags
 |                propmode          - Mode of propagation value
 |
 |      Returns:  error             - Error code
 |
 *===========================================================================*/
int LongleyRice(double theta_hzn[2], double f__mhz, complex<double> Z_g, double d_hzn__meter[2], 
    double h_e__meter[2], double gamma_e, double N_s, double delta_h__meter, double h__meter[2], 
    double d__meter, int mode, double *A_ref__db, long *warnings, int *propmode)
{
    double d_x__meter = 0;

    // effective earth radius
    double a_e__meter = 1 / gamma_e;

    double d_hzn_s__meter[2];
    // Terrestrial smooth earth horizon distance approximation
    for (int i = 0; i < 2; i++)
        d_hzn_s__meter[i] = sqrt(2.0 * h_e__meter[i] * a_e__meter);

    // Maximum line-of-sight distance for smooth earth
    double d_sML__meter = d_hzn_s__meter[0] + d_hzn_s__meter[1];

    // Maximum line-of-sight distance for actual path
    double d_ML__meter = d_hzn__meter[0] + d_hzn__meter[1];

    // Angular distance of line-of-sight region
    double theta_los = -MAX(theta_hzn[0] + theta_hzn[1], -d_ML__meter / a_e__meter);

    // Check validity of small angle approximation
    if (abs(theta_hzn[0]) > 200e-3)
        *warnings |= WARN__TX_HORIZON_ANGLE;
    if (abs(theta_hzn[1]) > 200e-3)
        *warnings |= WARN__RX_HORIZON_ANGLE;

    // Checks that the actual horizon distance can't be less than 1/10 of the smooth earth horizon distance
    if (d_hzn__meter[0] < 0.1 * d_hzn_s__meter[0])
        *warnings |= WARN__TX_HORIZON_DISTANCE_1;
    if (d_hzn__meter[1] < 0.1 * d_hzn_s__meter[1])
        *warnings |= WARN__RX_HORIZON_DISTANCE_1;

    // Checks that the actual horizon distance can't be greater than 3 times the smooth earth horizon distance
    if (d_hzn__meter[0] > 3.0 * d_hzn_s__meter[0])
        *warnings |= WARN__TX_HORIZON_DISTANCE_2;
    if (d_hzn__meter[1] > 3.0 * d_hzn_s__meter[1])
        *warnings |= WARN__RX_HORIZON_DISTANCE_2;

    // Check the surface refractivity
    if (N_s < 150)
        return ERROR__SURFACE_REFRACTIVITY_SMALL;
    if (N_s > 400)
        return ERROR__SURFACE_REFRACTIVITY_LARGE;
    if (N_s < 250) // 150 <= N_s < 250
        *warnings |= WARN__SURFACE_REFRACTIVITY;

    // Check effective earth size
    if (a_e__meter < 4000000 || a_e__meter > 13333333)
        return ERROR__EFFECTIVE_EARTH;

    // Check ground impedance
    if (Z_g.real() <= abs(Z_g.imag()))
        return ERROR__GROUND_IMPEDANCE;

    // Select two distances far in the diffraction region
    double d_3__meter = MAX(d_sML__meter, d_ML__meter + 5.0 * pow(pow(a_e__meter, 2) / f__mhz, 1.0 / 3.0));
    double d_4__meter = d_3__meter + 10.0 * pow(pow(a_e__meter, 2) / f__mhz, 1.0 / 3.0);

    // Compute the diffraction loss at the two distances
    double A_3__db = DiffractionLoss(d_3__meter, d_hzn__meter, h_e__meter, Z_g, a_e__meter, delta_h__meter, h__meter, mode, theta_los, d_sML__meter, f__mhz);
    double A_4__db = DiffractionLoss(d_4__meter, d_hzn__meter, h_e__meter, Z_g, a_e__meter, delta_h__meter, h__meter, mode, theta_los, d_sML__meter, f__mhz);

    // Compute the slope and intercept of the diffraction line
    double M_d = (A_4__db - A_3__db) / (d_4__meter - d_3__meter);
    double A_d0__db = A_3__db - M_d * d_3__meter;

    double d_min__meter = abs(h_e__meter[0] - h_e__meter[1]) / 200e-3;

    if (d__meter < d_min__meter)
        *warnings |= WARN__PATH_DISTANCE_TOO_SMALL_1;
    if (d__meter < 1e3)
        *warnings |= WARN__PATH_DISTANCE_TOO_SMALL_2;
    if (d__meter > 1000e3)
        *warnings |= WARN__PATH_DISTANCE_TOO_BIG_1;
    if (d__meter > 2000e3)
        *warnings |= WARN__PATH_DISTANCE_TOO_BIG_2;

    // if the path distance is less than the maximum smooth earth line of sight distance...
    if (d__meter < d_sML__meter)
    {
        // Compute the diffraction loss at the maximum smooth earth line of sight distance
        double A_sML__db = d_sML__meter * M_d + A_d0__db;

        // [ERL 79-ITS 67, Eqn 3.16a], in meters instead of km and with MIN() part below
        double d_0__meter = 0.04 * f__mhz * h_e__meter[0] * h_e__meter[1];

        double d_1__meter;
        if (A_d0__db >= 0.0)
        {
            d_0__meter = MIN(d_0__meter, 0.5 * d_ML__meter);                // other part of [ERL 79-ITS 67, Eqn 3.16a]
            d_1__meter = d_0__meter + 0.25 * (d_ML__meter - d_0__meter);    // [ERL 79-ITS 67, Eqn 3.16d]
        }
        else
            d_1__meter = MAX(-A_d0__db / M_d, 0.25 * d_ML__meter);

        double A_1__db = LineOfSightLoss(d_1__meter, h_e__meter, Z_g, delta_h__meter, M_d, A_d0__db, d_sML__meter, f__mhz);

        bool flag = false;

        double kHat_1__db_per_meter = 0;
        double kHat_2__db_per_meter = 0;

        if (d_0__meter < d_1__meter)
        {
            double A_0__db = LineOfSightLoss(d_0__meter, h_e__meter, Z_g, delta_h__meter, M_d, A_d0__db, d_sML__meter, f__mhz);

            double q = log(d_sML__meter / d_0__meter);

            // [ERL 79-ITS 67, Eqn 3.20]
            kHat_2__db_per_meter = MAX(0.0, ((d_sML__meter - d_0__meter) * (A_1__db - A_0__db) - (d_1__meter - d_0__meter) * (A_sML__db - A_0__db)) / ((d_sML__meter - d_0__meter) * log(d_1__meter / d_0__meter) - (d_1__meter - d_0__meter) * q));

            flag = A_d0__db > 0.0 || kHat_2__db_per_meter > 0.0;

            if (flag)
            {
                // [ERL 79-ITS 67, Eqn 3.21]
                kHat_1__db_per_meter = (A_sML__db - A_0__db - kHat_2__db_per_meter * q) / (d_sML__meter - d_0__meter);

                if (kHat_1__db_per_meter < 0.0)
                {
                    kHat_1__db_per_meter = 0.0;
                    kHat_2__db_per_meter = DIM(A_sML__db, A_0__db) / q;

                    if (kHat_2__db_per_meter == 0.0)
                        kHat_1__db_per_meter = M_d;
                }
            }
        }

        if (!flag)
        {
            kHat_1__db_per_meter = DIM(A_sML__db, A_1__db) / (d_sML__meter - d_1__meter);
            kHat_2__db_per_meter = 0.0;

            if (kHat_1__db_per_meter == 0.0)
                kHat_1__db_per_meter = M_d;
        }

        double A_o__db = A_sML__db - kHat_1__db_per_meter * d_sML__meter - kHat_2__db_per_meter * log(d_sML__meter);

        // [ERL 79-ITS 67, Eqn 3.19]
        *A_ref__db = A_o__db + kHat_1__db_per_meter * d__meter + kHat_2__db_per_meter * log(d__meter);
    }
    else // this is a trans-horizon path
    {
        // select to points far into the troposcatter region
        double d_5__meter = d_ML__meter + 200e3;
        double d_6__meter = d_ML__meter + 400e3;

        // Compute the troposcatter loss at the two distances
        double h0 = -1;
        double A_6__db = TroposcatterLoss(d_6__meter, theta_hzn, d_hzn__meter, h_e__meter, a_e__meter, N_s, f__mhz, theta_los, &h0);
        double A_5__db = TroposcatterLoss(d_5__meter, theta_hzn, d_hzn__meter, h_e__meter, a_e__meter, N_s, f__mhz, theta_los, &h0);

        double M_s, A_s0__db;

        // if we got a reasonable prediction value back...
        if (A_5__db < 1000.0)
        {
            // Compute the slope of the troposcatter line
            M_s = (A_6__db - A_5__db) / 200e3;

            // Find the diffraction-troposcatter transition distance
            d_x__meter = MAX(MAX(d_sML__meter, d_ML__meter + 1.088 * pow(pow(a_e__meter, 2) / f__mhz, 1.0 / 3.0) * log(f__mhz)), (A_5__db - A_d0__db - M_s * d_5__meter) / (M_d - M_s));

            // Compute the intercept of the troposcatter line
            A_s0__db = (M_d - M_s) * d_x__meter + A_d0__db;
        }
        else
        {
            // troposcatter gives no real results - so use diffraction line parameters for tropo line
            M_s = M_d;
            A_s0__db = A_d0__db;
            d_x__meter = 10e6;
        }

        // Determine if its diffraction or troposcatter and compute the loss
        if (d__meter > d_x__meter)
            *A_ref__db = M_s * d__meter + A_s0__db;
        else
            *A_ref__db = M_d * d__meter + A_d0__db;
    }

    // set mode of propagation
    double delta__meter = d__meter - d_ML__meter;
    if (int(delta__meter) < 0)
        *propmode = MODE__LINE_OF_SIGHT;
    else
    {
        if (d__meter <= d_sML__meter || d__meter <= d_x__meter)
            *propmode = (int(delta__meter) == 0) ? MODE__DIFFRACTION_SINGLE_HORIZON : MODE__DIFFRACTION_DOUBLE_HORIZON;
        else
            *propmode = (int(delta__meter) == 0) ? MODE__TROPOSCATTER_SINGLE_HORIZON : MODE__TROPOSCATTER_DOUBLE_HORIZON;
    }

    // Don't allow a negative loss
    *A_ref__db = MAX(*A_ref__db, 0.0);

    return SUCCESS;
}