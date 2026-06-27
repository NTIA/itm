use crate::compute_delta_h::compute_delta_h;
use crate::find_horizons::find_horizons;
use crate::linear_least_squares_fit::linear_least_squares_fit;

/// Extract propagation parameters from terrain profile.
/// Returns `(theta_hzn, d_hzn__meter, h_e__meter, delta_h__meter, d__meter)`.
pub fn quick_pfl(
    pfl: &[f64],
    gamma_e: f64,
    h__meter: [f64; 2],
) -> ([f64; 2], [f64; 2], [f64; 2], f64, f64) {
    let d__meter = pfl[0] * pfl[1];
    let np = pfl[0] as usize;
    let a_e__meter = 1.0 / gamma_e;

    let (mut theta_hzn, mut d_hzn__meter) = find_horizons(pfl, a_e__meter, h__meter);

    let d_start = f64::min(15.0 * h__meter[0], 0.1 * d_hzn__meter[0]);
    let d_end = d__meter - f64::min(15.0 * h__meter[1], 0.1 * d_hzn__meter[1]);

    let delta_h__meter = compute_delta_h(pfl, d_start, d_end);

    let mut h_e__meter = [0.0f64; 2];

    if d_hzn__meter[0] + d_hzn__meter[1] > 1.5 * d__meter {
        // Well within LOS
        let (fit_tx, fit_rx) = linear_least_squares_fit(pfl, d_start, d_end);

        h_e__meter[0] = h__meter[0] + (pfl[2] - fit_tx).max(0.0);
        h_e__meter[1] = h__meter[1] + (pfl[np + 2] - fit_rx).max(0.0);

        for i in 0..2 {
            d_hzn__meter[i] = (2.0 * h_e__meter[i] * a_e__meter).sqrt()
                * (-0.07 * (delta_h__meter / f64::max(h_e__meter[i], 5.0)).sqrt()).exp();
        }

        let combined = d_hzn__meter[0] + d_hzn__meter[1];
        if combined <= d__meter {
            let q = (d__meter / combined).powi(2);
            for i in 0..2 {
                h_e__meter[i] *= q;
                d_hzn__meter[i] = (2.0 * h_e__meter[i] * a_e__meter).sqrt()
                    * (-0.07 * (delta_h__meter / f64::max(h_e__meter[i], 5.0)).sqrt()).exp();
            }
        }

        for i in 0..2 {
            let q = (2.0 * h_e__meter[i] * a_e__meter).sqrt();
            theta_hzn[i] =
                (0.65 * delta_h__meter * (q / d_hzn__meter[i] - 1.0) - 2.0 * h_e__meter[i]) / q;
        }
    } else {
        let (fit_tx, _) = linear_least_squares_fit(pfl, d_start, 0.9 * d_hzn__meter[0]);
        h_e__meter[0] = h__meter[0] + (pfl[2] - fit_tx).max(0.0);

        let (_, fit_rx) = linear_least_squares_fit(pfl, d__meter - 0.9 * d_hzn__meter[1], d_end);
        h_e__meter[1] = h__meter[1] + (pfl[np + 2] - fit_rx).max(0.0);
    }

    (
        theta_hzn,
        d_hzn__meter,
        h_e__meter,
        delta_h__meter,
        d__meter,
    )
}
