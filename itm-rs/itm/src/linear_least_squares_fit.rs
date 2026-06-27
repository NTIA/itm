/// Linear least-squares fit over terrain profile `pfl` from `d_start` to `d_end`.
/// Returns `(fit_y1, fit_y2)` — fitted values at the start and end of the profile.
pub fn linear_least_squares_fit(pfl: &[f64], d_start: f64, d_end: f64) -> (f64, f64) {
    let np = pfl[0] as usize;

    let mut i_start = (((d_start / pfl[1]) - 0.0_f64).max(0.0)) as usize;
    let mut i_end = np - ((np as f64 - d_end / pfl[1]).max(0.0)) as usize;

    if i_end <= i_start {
        i_start = i_start.saturating_sub(1);
        i_end = np - ((np as f64 - (i_end + 1) as f64).max(0.0)) as usize;
    }

    let x_length = (i_end - i_start) as f64;

    let mut mid_shifted_index = -0.5 * x_length;
    let mid_shifted_end = i_end as f64 + mid_shifted_index;

    let mut sum_y = 0.5 * (pfl[i_start + 2] + pfl[i_end + 2]);
    let mut scaled_sum_y = 0.5 * (pfl[i_start + 2] - pfl[i_end + 2]) * mid_shifted_index;

    let mut i_s = i_start;
    for _ in 2..=(x_length as usize) {
        i_s += 1;
        mid_shifted_index += 1.0;
        sum_y += pfl[i_s + 2];
        scaled_sum_y += pfl[i_s + 2] * mid_shifted_index;
    }

    sum_y /= x_length;
    scaled_sum_y *= 12.0 / ((x_length * x_length + 2.0) * x_length);

    let fit_y1 = sum_y - scaled_sum_y * mid_shifted_end;
    let fit_y2 = sum_y + scaled_sum_y * (np as f64 - mid_shifted_end);
    (fit_y1, fit_y2)
}
