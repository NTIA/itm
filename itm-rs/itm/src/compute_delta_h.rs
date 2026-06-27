use crate::linear_least_squares_fit::linear_least_squares_fit;

/// Compute the terrain irregularity parameter delta_h between
/// `d_start__meter` and `d_end__meter` in the terrain profile `pfl`.
pub fn compute_delta_h(pfl: &[f64], d_start__meter: f64, d_end__meter: f64) -> f64 {
    let np = pfl[0] as usize;
    let x_start_f = d_start__meter / pfl[1];
    let x_end_f = d_end__meter / pfl[1];

    if x_end_f - x_start_f < 2.0 {
        return 0.0;
    }

    let p10_raw = (0.1 * (x_end_f - x_start_f + 8.0)) as usize;
    let p10 = p10_raw.clamp(4, 25);
    let n = 10 * p10 - 5;
    let p90 = n - p10;

    let np_s = (n - 1) as f64;
    let mut s = vec![0.0f64; n + 2];
    s[0] = np_s;
    s[1] = 1.0;

    let x_step = (x_end_f - x_start_f) / np_s;
    let mut i = x_start_f as usize;
    let mut x_start = x_start_f - (i + 1) as f64;

    for j in 0..n {
        while x_start > 0.0 && i + 1 < np {
            x_start -= 1.0;
            i += 1;
        }
        s[j + 2] = pfl[i + 3] + (pfl[i + 3] - pfl[i + 2]) * x_start;
        x_start += x_step;
    }

    let (fit_y1, fit_y2_raw) = linear_least_squares_fit(&s, 0.0, np_s);
    let fit_step = (fit_y2_raw - fit_y1) / np_s;

    let mut diffs: Vec<f64> = Vec::with_capacity(n);
    let mut running = fit_y1;
    for j in 0..n {
        diffs.push(s[j + 2] - running);
        running += fit_step;
    }

    // 10th percentile (largest) and 90th percentile (smallest of remaining)
    diffs.select_nth_unstable_by(p10 - 1, |a, b| b.partial_cmp(a).unwrap());
    let q10 = diffs[p10 - 1];
    diffs.select_nth_unstable_by(p90, |a, b| b.partial_cmp(a).unwrap());
    let q90 = diffs[p90];

    let delta_h_d__meter = q10 - q90;
    delta_h_d__meter / (1.0 - 0.8 * (-(d_end__meter - d_start__meter) / 50_000.0).exp())
}
