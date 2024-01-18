#include "..\include\itm.h"

/*=============================================================================
 |
 |  Description:  Perform a linear least squares fit to the terrain data
 |
 |        Input:  pfl[2]         - Input data array, in pfl format
 |                d_start        - Start distance
 |                d_end          - End distance
 |
 |      Outputs:  fit_y1         - Fitted y1 value
 |                fit_y2         - Fitted y2 value
 |
 |      Returns:  [None]
 |
 *===========================================================================*/
void LinearLeastSquaresFit(const double pfl[], double d_start, double d_end, double *fit_y1, double *fit_y2)
{
    int np = (int)pfl[0];

    int i_start = int(fdim(d_start / pfl[1], 0.0));
    int i_end = np - int(fdim(np, d_end / pfl[1]));

    if (i_end <= i_start)
    {
        i_start = (int)fdim(i_start, 1.0);
        i_end = np - (int)fdim(np, i_end + 1.0);
    }

    double x_length = i_end - i_start;

    double mid_shifted_index = -0.5 * x_length;
    double mid_shifted_end = i_end + mid_shifted_index;

    double sum_y = 0.5 * (pfl[i_start + 2] + pfl[i_end + 2]);
    double scaled_sum_y = 0.5 * (pfl[i_start + 2] - pfl[i_end + 2]) * mid_shifted_index;

    for (int i = 2; i <= x_length; i++)
    {
        i_start++;
        mid_shifted_index++;

        sum_y += pfl[i_start + 2];
        scaled_sum_y += pfl[i_start + 2] * mid_shifted_index;
    }

    sum_y = sum_y / x_length;
    scaled_sum_y = scaled_sum_y * 12.0 / ((x_length * x_length + 2.0) * x_length);

    *fit_y1 = sum_y - scaled_sum_y * mid_shifted_end;
    *fit_y2 = sum_y + scaled_sum_y * (np - mid_shifted_end);
}