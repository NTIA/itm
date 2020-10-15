#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Compute sigma_h
 |
 |        Input:  delta_h__meter - Terrain irregularity parameter
 |
 |      Outputs:  [None]
 |
 |      Returns:  sigma_h_meter  - sigma_h
 |
 *===========================================================================*/
double SigmaHFunction(double delta_h__meter)
{
    // "RMS deviation of terrain and terrain clutter within the limits of the first Fresnel zone in the dominant reflecting plane"
    // [ERL 79-ITS 67, Eqn 3.6a]
    return 0.78 * delta_h__meter * exp(-0.5 * pow(delta_h__meter, 0.25));
}