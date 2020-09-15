#include "..\include\itm.h"
#include "..\include\Enums.h"

/*=============================================================================
 |
 |  Description:  Initialize area mode calculations
 |
 |        Input:  site_criteria[2]  - Siting criteria of the terminals
 |                                      + 0 : SITING_CRITERIA__RANDOM
 |                                      + 1 : SITING_CRITERIA__CAREFUL
 |                                      + 2 : SITING_CRITERIA__VERY_CAREFUL
 |                gamma_e           - Curvature of the effective earth
 |                delta_h__meter    - Terrain irregularity parameter, in meters
 |                h__meter[2]       - Terminal structural heights, in meters
 |
 |      Outputs:  h_e__meter[2]     - Effective terminal heights, in meters
 |                d_hzn__meter[2]   - Terminal horizon distances, in meters
 |                theta_hzn[2]      - Terminal horizon angle
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void InitializeArea(int site_criteria[2], double gamma_e, double delta_h__meter,
    double h__meter[2], double h_e__meter[2], double d_hzn__meter[2],  double theta_hzn[2])
{
    for (int i = 0; i < 2; i++)
    {
        if (site_criteria[i] == SITING_CRITERIA__RANDOM)
            h_e__meter[i] = h__meter[i];
        else
        {
            double B;

            if (site_criteria[i] == SITING_CRITERIA__CAREFUL)
                B = 4.0;
            else // SITING_CRITERIA__VERY_CAREFUL
                B = 9.0;

            if (h__meter[i] < 5.0)
                B = B * sin(0.1 * PI * h__meter[i]);

            // [Algorithm, Eqn 3.2]
            h_e__meter[i] = h__meter[i] + (1.0 + B) * exp(-MIN(20.0, 2.0 * h__meter[i] / MAX(1e-3, delta_h__meter)));
        }

        double d_Ls__meter = sqrt(2.0 * h_e__meter[i] / gamma_e);

        // [Algorithm, Eqn 3.3]
        double H_3__meter = 5;
        d_hzn__meter[i] = d_Ls__meter * exp(-0.07 * sqrt(delta_h__meter / MAX(h_e__meter[i], H_3__meter)));

        // [Algorithm, Eqn 3.4]
        theta_hzn[i] = (0.65 * delta_h__meter * (d_Ls__meter / d_hzn__meter[i] - 1.0) - 2.0 * h_e__meter[i]) / d_Ls__meter;
    }
}