use crate::constants::{PI, SITING_CRITERIA__RANDOM, SITING_CRITERIA__CAREFUL};

/// Initialize area mode parameters.
/// Returns `(h_e__meter, d_hzn__meter, theta_hzn)`.
pub fn initialize_area(
    site_criteria: [i32; 2],
    gamma_e: f64,
    delta_h__meter: f64,
    h__meter: [f64; 2],
) -> ([f64; 2], [f64; 2], [f64; 2]) {
    let mut h_e__meter = [0.0f64; 2];
    let mut d_hzn__meter = [0.0f64; 2];
    let mut theta_hzn = [0.0f64; 2];

    for i in 0..2 {
        h_e__meter[i] = if site_criteria[i] == SITING_CRITERIA__RANDOM {
            h__meter[i]
        } else {
            let b = if site_criteria[i] == SITING_CRITERIA__CAREFUL {
                4.0_f64
            } else {
                9.0_f64
            };
            let b = if h__meter[i] < 5.0 {
                b * (0.1 * PI * h__meter[i]).sin()
            } else {
                b
            };
            h__meter[i]
                + (1.0 + b)
                    * (-f64::min(20.0, 2.0 * h__meter[i] / f64::max(1e-3, delta_h__meter))).exp()
        };

        let d_ls__meter = (2.0 * h_e__meter[i] / gamma_e).sqrt();
        const H_3__METER: f64 = 5.0;
        d_hzn__meter[i] = d_ls__meter
            * (-0.07 * (delta_h__meter / f64::max(h_e__meter[i], H_3__METER)).sqrt()).exp();
        theta_hzn[i] = (0.65 * delta_h__meter * (d_ls__meter / d_hzn__meter[i] - 1.0)
            - 2.0 * h_e__meter[i])
            / d_ls__meter;
    }

    (h_e__meter, d_hzn__meter, theta_hzn)
}
