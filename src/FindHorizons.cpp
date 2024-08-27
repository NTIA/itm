#include "ITS.Propagation.ITM/ITM.h"

namespace ITS {
namespace Propagation {
namespace ITM {

/*=============================================================================
 |
 |  Description:  Compute the radio horizon's of the terminals
 |
 |        Input:  pfl[]             - Terrain data
 |                a_e__meter        - Effective earth radius, in meters
 |                h__meter[2]       - Terminal structural heights, in meters
 |
 |      Outputs:  theta_hzn[2]      - Terminal radio horizon angle, in radians
 |                d_hzn__meter[2]   - Terminal radio horizon distance, in meters
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void FindHorizons(double pfl[], double a_e__meter, double h__meter[2], double theta_hzn[2], double d_hzn__meter[2])
{
    int np = int(pfl[0]);
    double xi = pfl[1];

    double d__meter = pfl[0] * pfl[1];

    // compute radials (ignore radius of earth since it cancels out in the later math)
    double z_tx__meter = pfl[2] + h__meter[0];
    double z_rx__meter = pfl[np + 2] + h__meter[1];

    // set the terminal horizon angles as if the terminals are line-of-sight
    // [TN101, Eq 6.15]
    theta_hzn[0] = (z_rx__meter - z_tx__meter) / d__meter - d__meter / (2 * a_e__meter);
    theta_hzn[1] = -(z_rx__meter - z_tx__meter) / d__meter - d__meter / (2 * a_e__meter);

    d_hzn__meter[0] = d__meter;
    d_hzn__meter[1] = d__meter;

    double d_tx__meter = 0.0;
    double d_rx__meter = d__meter;

    double theta_tx, theta_rx;

    for (int i = 1; i < np; i++)
    {
        d_tx__meter = d_tx__meter + xi;
        d_rx__meter = d_rx__meter - xi;

        theta_tx = (pfl[i + 2] - z_tx__meter) / d_tx__meter - d_tx__meter / (2 * a_e__meter);
        theta_rx = -(z_rx__meter - pfl[i + 2]) / d_rx__meter - d_rx__meter / (2 * a_e__meter);

        if (theta_tx > theta_hzn[0])
        {
            theta_hzn[0] = theta_tx;
            d_hzn__meter[0] = d_tx__meter;
        }

        if (theta_rx > theta_hzn[1])
        {
            theta_hzn[1] = theta_rx;
            d_hzn__meter[1] = d_rx__meter;
        }
    }
}

}  // namespace ITM
}  // namespace Propagation
}  // namespace ITS