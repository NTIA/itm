#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Curve fit helper function to approximate H_0()
 |
 |        Input:  j              - eta_s curve
 |                r              - Input parameter r_1,2
 |
 |      Outputs:  [None]
 |
 |      Returns: H_01(r, j)      - in dB
 |
 *===========================================================================*/
double H0Curve(int j, double r)
{
    // values from [Algorithm, 6.13]
    double a[] = { 25.0, 80.0, 177.0, 395.0, 705.0 };
    double b[] = { 24.0, 45.0, 68.0, 80.0, 105.0 };

    return 10 * log10(1 + a[j] * pow(1 / r, 4) + b[j] * pow(1.0 / r, 2));    // related to TN101v2, Eqn III.49, but from [Algorithm, 6.13]
}


/*=============================================================================
 |
 |  Description:  Troposcatter frequency gain function, H_0(), from 
 |                [TN101v1, Ch 9.2]
 |
 |        Input:  r              - Input parameter r_1,2
 |                eta_s          - Parameter eta_s
 |
 |      Outputs:  [None]
 |
 |      Returns:  H_0()          - in dB
 |
 *===========================================================================*/
double H0Function(double r, double eta_s)
{
    eta_s = MIN(MAX(eta_s, 1), 5);  // range 1 <= eta_s <= 5

    int i = int(eta_s);             // integer part of eta_s
    double q = eta_s - i;           // decimal part of eta_s

    double result = H0Curve(i - 1, r);

    if (q != 0.0)                   // interpolate with next curve, if needed
        result = (1.0 - q) * result + q * H0Curve(i, r);

    return result;
}
