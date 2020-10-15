#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Approximate to ideal knife edge diffraction loss
 |
 |        Input:  v2             - v^2 parameter
 |
 |      Outputs:  [None]
 |
 |      Returns:  A(v, 0)        - Loss, in dB
 |
 *===========================================================================*/
double FresnelIntegral(double v2)
{
    // Note: v2  is v^2, so 5.76 is actually comparing v to 2.4

    if (v2 < 5.76)
        return 6.02 + 9.11 * sqrt(v2) - 1.27 * v2;      // [TN101v2, Eqn III.24b] and [ERL 79-ITS 67, Eqn 3.27a & 3.27b]
    else
        return 12.953 + 10 * log10(v2);                 // [TN101v2, Eqn III.24c] and [ERL 79-ITS 67, Eqn 3.27a & 3.27b]
}