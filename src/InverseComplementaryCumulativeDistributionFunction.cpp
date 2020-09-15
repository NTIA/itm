#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  This function computes the inverse complementary
 |                cumulative distribution function approximation as
 |                described in Formula 26.2.23 in Abramowitz & Stegun.
 |                This approximation has an error of 
 |                abs(epsilon(p)) < 4.5e-4
 |
 |        Input:  q              - Quantile, 0.0 < q < 1.0
 |
 |      Outputs:  [None]
 |
 |      Returns:  Q_q            - Q(q)^-1
 |
 *===========================================================================*/
double InverseComplementaryCumulativeDistributionFunction(double q)
{
    double C_0 = 2.515516;
    double C_1 = 0.802853;
    double C_2 = 0.010328;
    double D_1 = 1.432788;
    double D_2 = 0.189269;
    double D_3 = 0.001308;

    double x = q;
    if (q > 0.5)
        x = 1.0 - x;

    double T_x = sqrt(-2.0 * log(x));

    double zeta_x = ((C_2 * T_x + C_1) * T_x + C_0) / (((D_3 * T_x + D_2) * T_x + D_1) * T_x + 1.0);

    double Q_q = T_x - zeta_x;

    if (q > 0.5)
        Q_q = -Q_q;

    return Q_q;
}
