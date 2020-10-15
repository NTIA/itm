#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Compute delta_h_d
 |
 |        Input:  d__meter       - Path distance, in meters
 |                delta_h__meter - Terrain irregularity parameter
 |
 |      Outputs:  [None]
 |
 |      Returns:  delta_h_d      - Terrain irregularity of path
 |
 *===========================================================================*/
double TerrainRoughness(double d__meter, double delta_h__meter)
{
    // [ERL 79-ITS 67, Eqn 3], with distance in meters instead of kilometers
    return delta_h__meter * (1.0 - 0.8 * exp(-d__meter / 50e3));
}