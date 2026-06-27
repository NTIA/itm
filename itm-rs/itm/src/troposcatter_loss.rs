use crate::constants::SQRT2;
use crate::h0_function::h0_function;

fn f_function(td: f64) -> f64 {
    const A: [f64; 3] = [133.4, 104.6, 71.8];
    const B: [f64; 3] = [0.332e-3, 0.212e-3, 0.157e-3];
    const C: [f64; 3] = [-10.0, -2.5, 5.0];

    let i = if td <= 10_000.0 {
        0
    } else if td <= 70_000.0 {
        1
    } else {
        2
    };

    A[i] + B[i] * td + C[i] * td.log10()
}

/// Troposcatter loss, in dB.
/// `h0` is an in/out parameter: pass `-1.0` on first call, and the stored value
/// on subsequent calls to allow the short-circuit for H_0 > 15 dB.
pub fn troposcatter_loss(
    d__meter: f64,
    theta_hzn: [f64; 2],
    d_hzn__meter: [f64; 2],
    h_e__meter: [f64; 2],
    a_e__meter: f64,
    n_s: f64,
    f__mhz: f64,
    theta_los: f64,
    h0: &mut f64,
) -> f64 {
    let wn = f__mhz / 47.7;

    let h_0 = if *h0 > 15.0 {
        *h0
    } else {
        let mut ad = d_hzn__meter[0] - d_hzn__meter[1];
        let mut rr = h_e__meter[1] / h_e__meter[0];
        if ad < 0.0 {
            ad = -ad;
            rr = 1.0 / rr;
        }

        let theta = theta_hzn[0] + theta_hzn[1] + d__meter / a_e__meter;

        let r_1 = 2.0 * wn * theta * h_e__meter[0];
        let r_2 = 2.0 * wn * theta * h_e__meter[1];

        if r_1 < 0.2 && r_2 < 0.2 {
            return 1001.0;
        }

        let s = ((d__meter - ad) / (d__meter + ad)).max(0.1);
        let q = f64::min(f64::max(0.1, rr / s), 10.0);

        let h_0_meter = (d__meter - ad) * (d__meter + ad) * theta * 0.25 / d__meter;

        const Z_0: f64 = 1755.6;
        const Z_1: f64 = 8000.0;
        let eta_s = (h_0_meter / Z_0)
            * (1.0
                + (0.031 - n_s * 2.32e-3 + n_s.powi(2) * 5.67e-6)
                    * (-f64::min(1.7, h_0_meter / Z_1).powi(6)).exp());

        let h_00 = (h0_function(r_1, eta_s) + h0_function(r_2, eta_s)) / 2.0;
        let delta_h_0 = f64::min(
            h_00,
            6.0 * (0.6 - f64::max(eta_s, 1.0).log10()) * s.log10() * q.log10(),
        );

        let mut h_0_val = (h_00 + delta_h_0).max(0.0);

        if eta_s < 1.0 {
            let special = 10.0
                * ((1.0 + SQRT2 / r_1) * (1.0 + SQRT2 / r_2)).powi(2)
                    .log10()
                    .mul_add(
                        0.0,
                        ((1.0 + SQRT2 / r_1) * (1.0 + SQRT2 / r_2)).powi(2)
                            * (r_1 + r_2)
                            / (r_1 + r_2 + 2.0 * SQRT2),
                    )
                    .log10();
            h_0_val = eta_s * h_0_val + (1.0 - eta_s) * special;
        }

        if h_0_val > 15.0 && *h0 >= 0.0 {
            h_0_val = *h0;
        }

        h_0_val
    };

    *h0 = h_0;
    let th = d__meter / a_e__meter - theta_los;

    const D_0: f64 = 40_000.0;
    const H_M: f64 = 47.7;
    f_function(th * d__meter)
        + 10.0 * (wn * H_M * th.powi(4)).log10()
        - 0.1 * (n_s - 301.0) * (-th * d__meter / D_0).exp()
        + h_0
}
