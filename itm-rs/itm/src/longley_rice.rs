use crate::complex::Complex;
use crate::constants::*;
use crate::diffraction_loss::diffraction_loss;
use crate::errors::*;
use crate::line_of_sight_loss::line_of_sight_loss;
use crate::troposcatter_loss::troposcatter_loss;
use crate::warnings::*;

/// Compute the reference attenuation using the Longley-Rice method.
/// Returns `(error_code, a_ref__db, propmode)`.
pub fn longley_rice(
    theta_hzn: [f64; 2],
    f__mhz: f64,
    z_g: Complex,
    d_hzn__meter: [f64; 2],
    h_e__meter: [f64; 2],
    gamma_e: f64,
    n_s: f64,
    delta_h__meter: f64,
    h__meter: [f64; 2],
    d__meter: f64,
    mode: i32,
    warnings: &mut i32,
) -> (i32, f64, i32) {
    let a_e__meter = 1.0 / gamma_e;

    let d_hzn_s = [
        (2.0 * h_e__meter[0] * a_e__meter).sqrt(),
        (2.0 * h_e__meter[1] * a_e__meter).sqrt(),
    ];
    let d_sml = d_hzn_s[0] + d_hzn_s[1];
    let d_ml = d_hzn__meter[0] + d_hzn__meter[1];
    let theta_los = -(theta_hzn[0] + theta_hzn[1]).max(-d_ml / a_e__meter);

    if theta_hzn[0].abs() > 200e-3 {
        *warnings |= WARN__TX_HORIZON_ANGLE;
    }
    if theta_hzn[1].abs() > 200e-3 {
        *warnings |= WARN__RX_HORIZON_ANGLE;
    }
    if d_hzn__meter[0] < 0.1 * d_hzn_s[0] {
        *warnings |= WARN__TX_HORIZON_DISTANCE_1;
    }
    if d_hzn__meter[1] < 0.1 * d_hzn_s[1] {
        *warnings |= WARN__RX_HORIZON_DISTANCE_1;
    }
    if d_hzn__meter[0] > 3.0 * d_hzn_s[0] {
        *warnings |= WARN__TX_HORIZON_DISTANCE_2;
    }
    if d_hzn__meter[1] > 3.0 * d_hzn_s[1] {
        *warnings |= WARN__RX_HORIZON_DISTANCE_2;
    }

    if n_s < 150.0 {
        return (ERROR__SURFACE_REFRACTIVITY_SMALL, 0.0, MODE__NOT_SET);
    }
    if n_s > 400.0 {
        return (ERROR__SURFACE_REFRACTIVITY_LARGE, 0.0, MODE__NOT_SET);
    }
    if n_s < 250.0 {
        *warnings |= WARN__SURFACE_REFRACTIVITY;
    }

    if a_e__meter < 4_000_000.0 || a_e__meter > 13_333_333.0 {
        return (ERROR__EFFECTIVE_EARTH, 0.0, MODE__NOT_SET);
    }

    if z_g.re <= z_g.im.abs() {
        return (ERROR__GROUND_IMPEDANCE, 0.0, MODE__NOT_SET);
    }

    let cbrt_term = (a_e__meter.powi(2) / f__mhz).powf(1.0 / 3.0);
    let d_3 = f64::max(d_sml, d_ml + 5.0 * cbrt_term);
    let d_4 = d_3 + 10.0 * cbrt_term;

    let a_3 = diffraction_loss(
        d_3,
        d_hzn__meter,
        h_e__meter,
        z_g,
        a_e__meter,
        delta_h__meter,
        h__meter,
        mode,
        theta_los,
        d_sml,
        f__mhz,
    );
    let a_4 = diffraction_loss(
        d_4,
        d_hzn__meter,
        h_e__meter,
        z_g,
        a_e__meter,
        delta_h__meter,
        h__meter,
        mode,
        theta_los,
        d_sml,
        f__mhz,
    );

    let m_d = (a_4 - a_3) / (d_4 - d_3);
    let a_d0 = a_3 - m_d * d_3;

    let d_min = (h_e__meter[0] - h_e__meter[1]).abs() / 200e-3;
    if d__meter < d_min {
        *warnings |= WARN__PATH_DISTANCE_TOO_SMALL_1;
    }
    if d__meter < 1_000.0 {
        *warnings |= WARN__PATH_DISTANCE_TOO_SMALL_2;
    }
    if d__meter > 1_000_000.0 {
        *warnings |= WARN__PATH_DISTANCE_TOO_BIG_1;
    }
    if d__meter > 2_000_000.0 {
        *warnings |= WARN__PATH_DISTANCE_TOO_BIG_2;
    }

    let (a_ref, propmode) = if d__meter < d_sml {
        let a_sml = d_sml * m_d + a_d0;
        let mut d_0 = 0.04 * f__mhz * h_e__meter[0] * h_e__meter[1];
        let d_1;

        if a_d0 >= 0.0 {
            d_0 = f64::min(d_0, 0.5 * d_ml);
            d_1 = d_0 + 0.25 * (d_ml - d_0);
        } else {
            d_1 = f64::max(-a_d0 / m_d, 0.25 * d_ml);
        }

        let a_1 = line_of_sight_loss(d_1, h_e__meter, z_g, delta_h__meter, m_d, a_d0, d_sml, f__mhz);

        let mut flag = false;
        let mut k1 = 0.0_f64;
        let mut k2 = 0.0_f64;

        if d_0 < d_1 {
            let a_0 = line_of_sight_loss(d_0, h_e__meter, z_g, delta_h__meter, m_d, a_d0, d_sml, f__mhz);
            let q = (d_sml / d_0).ln();

            k2 = f64::max(
                0.0,
                ((d_sml - d_0) * (a_1 - a_0) - (d_1 - d_0) * (a_sml - a_0))
                    / ((d_sml - d_0) * (d_1 / d_0).ln() - (d_1 - d_0) * q),
            );

            flag = a_d0 > 0.0 || k2 > 0.0;

            if flag {
                k1 = (a_sml - a_0 - k2 * q) / (d_sml - d_0);
                if k1 < 0.0 {
                    k1 = 0.0;
                    k2 = ((a_sml - a_0).max(0.0)) / q;
                    if k2 == 0.0 {
                        k1 = m_d;
                    }
                }
            }
        }

        if !flag {
            k1 = (a_sml - a_1).max(0.0) / (d_sml - d_1);
            k2 = 0.0;
            if k1 == 0.0 {
                k1 = m_d;
            }
        }

        let a_o = a_sml - k1 * d_sml - k2 * d_sml.ln();
        let a_ref = a_o + k1 * d__meter + k2 * d__meter.ln();
        (a_ref, MODE__LINE_OF_SIGHT)
    } else {
        let d_5 = d_ml + 200_000.0;
        let d_6 = d_ml + 400_000.0;

        let mut h0 = -1.0;
        let a_6 = troposcatter_loss(
            d_6, theta_hzn, d_hzn__meter, h_e__meter, a_e__meter, n_s, f__mhz, theta_los, &mut h0,
        );
        let a_5 = troposcatter_loss(
            d_5, theta_hzn, d_hzn__meter, h_e__meter, a_e__meter, n_s, f__mhz, theta_los, &mut h0,
        );

        let (m_s, a_s0, d_x) = if a_5 < 1000.0 {
            let m_s = (a_6 - a_5) / 200_000.0;
            let d_x = f64::max(
                f64::max(
                    d_sml,
                    d_ml + 1.088 * (a_e__meter.powi(2) / f__mhz).powf(1.0 / 3.0) * f__mhz.ln(),
                ),
                (a_5 - a_d0 - m_s * d_5) / (m_d - m_s),
            );
            let a_s0 = (m_d - m_s) * d_x + a_d0;
            (m_s, a_s0, d_x)
        } else {
            (m_d, a_d0, 10_000_000.0)
        };

        if d__meter > d_x {
            (m_s * d__meter + a_s0, MODE__TROPOSCATTER)
        } else {
            (m_d * d__meter + a_d0, MODE__DIFFRACTION)
        }
    };

    (SUCCESS, a_ref.max(0.0), propmode)
}
