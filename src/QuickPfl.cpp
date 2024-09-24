#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Extract parameters from the terrain pfl
 |
 |        Input:  pfl[2]            - Terrain data in pfl format
 |                gamma_e           - Effective earth curvature
 |                h__meter[2]       - Terminal structural heights, in meters
 |
 |      Outputs:  theta_hzn[2]      - Terminal horizon angles
 |                d_hzn__meter[2]   - Terminal horizon distances, in meters
 |                h_e__meter[2]     - Effective terminal heights, in meters
 |                delta_h__meter    - Terrain irregularity parameter
 |                d__meter          - Path distance, in meters
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void QuickPfl(const double pfl[], const double gamma_e, const double h__meter[2], double theta_hzn[2], 
    double d_hzn__meter[2], double h_e__meter[2], double *delta_h__meter, double *d__meter)
{
    double fit_tx, fit_rx, q;
    double d_start__meter;
    double d_end__meter;

    *d__meter = pfl[0] * pfl[1];

    const int np = int(pfl[0]);

    const double a_e__meter = 1 / gamma_e;        // effective earth radius

    FindHorizons(pfl, a_e__meter, h__meter, theta_hzn, d_hzn__meter);

    // "In our own work we have sometimes said that consideration of terrain elevations should begin at a point about 15 times the tower height"
    //      - [Hufford, 1982] Page 25
    d_start__meter = MIN(15.0 * h__meter[0], 0.1 * d_hzn__meter[0]);             // take lesser: 10% of horizon distance or 15x terminal height
    d_end__meter = *d__meter - MIN(15.0 * h__meter[1], 0.1 * d_hzn__meter[1]);   // << ditto, but measured from the far end of the link >>

    *delta_h__meter = ComputeDeltaH(pfl, d_start__meter, d_end__meter);

    if (d_hzn__meter[0] + d_hzn__meter[1] > 1.5 * *d__meter)
    {
        // The combined horizon distance is at least 50% larger than the total path distance
        //  -> so we are well within the line-of-sight range

        LinearLeastSquaresFit(pfl, d_start__meter, d_end__meter, &fit_tx, &fit_rx);

        h_e__meter[0] = h__meter[0] + fdim(pfl[2], fit_tx);
        h_e__meter[1] = h__meter[1] + fdim(pfl[np + 2], fit_rx);

        for (int i = 0; i < 2; i++)
            d_hzn__meter[i] = sqrt(2.0 * h_e__meter[i] * a_e__meter) * exp(-0.07 * sqrt(*delta_h__meter / MAX(h_e__meter[i], 5.0)));

        const double combined_horizons__meter = d_hzn__meter[0] + d_hzn__meter[1];
        if (combined_horizons__meter <= *d__meter)
        {
            q = pow(*d__meter / combined_horizons__meter, 2);

            for (int i = 0; i < 2; i++)
            {
                h_e__meter[i] = h_e__meter[i] * q;
                d_hzn__meter[i] = sqrt(2.0 * h_e__meter[i] * a_e__meter) * exp(-0.07 * sqrt(*delta_h__meter / MAX(h_e__meter[i], 5.0)));
            }
        }

        for (int i = 0; i < 2; i++)
        {
            q = sqrt(2.0 * h_e__meter[i] * a_e__meter);
            theta_hzn[i] = (0.65 * *delta_h__meter * (q / d_hzn__meter[i] - 1.0) - 2.0 * h_e__meter[i]) / q;
        }
    }
    else
    {
        double dummy = 0;

        LinearLeastSquaresFit(pfl, d_start__meter, 0.9 * d_hzn__meter[0], &fit_tx, &dummy);
        h_e__meter[0] = h__meter[0] + fdim(pfl[2], fit_tx);

        LinearLeastSquaresFit(pfl, *d__meter - 0.9 * d_hzn__meter[1], d_end__meter, &dummy, &fit_rx);
        h_e__meter[1] = h__meter[1] + fdim(pfl[np + 2], fit_rx);
    }
}