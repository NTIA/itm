use crate::complex::Complex;
use crate::constants::{A_0__METER, THIRD};

fn height_function(x__km: f64, k: f64) -> f64 {
    if x__km < 200.0 {
        let w = -k.ln();
        if k < 1e-5 || x__km * w.powi(3) > 5495.0 {
            let mut result = -117.0;
            if x__km > 1.0 {
                result += 17.372 * x__km.ln();
            }
            result
        } else {
            2.5e-5 * x__km.powi(2) / k - 8.686 * w - 15.0
        }
    } else {
        let result = 0.05751 * x__km - 4.343 * x__km.ln();
        if x__km < 2000.0 {
            let w = 0.0134 * x__km * (-0.005 * x__km).exp();
            (1.0 - w) * result + w * (17.372 * x__km.ln() - 117.0)
        } else {
            result
        }
    }
}

/// Smooth-earth diffraction loss using the Vogler 3-radii method, in dB.
pub fn smooth_earth_diffraction(
    d__meter: f64,
    f__mhz: f64,
    a_e__meter: f64,
    theta_los: f64,
    d_hzn__meter: [f64; 2],
    h_e__meter: [f64; 2],
    z_g: Complex,
) -> f64 {
    let theta_nlos = d__meter / a_e__meter - theta_los;
    let d_ml = d_hzn__meter[0] + d_hzn__meter[1];

    let a = [
        (d__meter - d_ml) / (d__meter / a_e__meter - theta_los),
        0.5 * d_hzn__meter[0].powi(2) / h_e__meter[0],
        0.5 * d_hzn__meter[1].powi(2) / h_e__meter[1],
    ];

    let d__km = [
        a[0] * theta_nlos / 1000.0,
        d_hzn__meter[0] / 1000.0,
        d_hzn__meter[1] / 1000.0,
    ];

    let mut c_0 = [0.0f64; 3];
    let mut k = [0.0f64; 3];
    let mut b_0 = [0.0f64; 3];

    for i in 0..3 {
        c_0[i] = ((4.0 / 3.0) * A_0__METER / a[i]).powf(THIRD);
        k[i] = 0.017778 * c_0[i] * f__mhz.powf(-THIRD) / z_g.abs();
        b_0[i] = 1.607 - k[i];
    }

    let mut x__km = [0.0f64; 3];
    x__km[1] = b_0[1] * c_0[1].powi(2) * f__mhz.powf(THIRD) * d__km[1];
    x__km[2] = b_0[2] * c_0[2].powi(2) * f__mhz.powf(THIRD) * d__km[2];
    x__km[0] = b_0[0] * c_0[0].powi(2) * f__mhz.powf(THIRD) * d__km[0] + x__km[1] + x__km[2];

    let f_x = [height_function(x__km[1], k[1]), height_function(x__km[2], k[2])];
    let g_x = 0.05751 * x__km[0] - 10.0 * x__km[0].log10();

    g_x - f_x[0] - f_x[1] - 20.0
}
