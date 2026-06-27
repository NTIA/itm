use crate::complex::Complex;
use crate::constants::PI;
use crate::sigma_h_function::sigma_h_function;
use crate::terrain_roughness::terrain_roughness;

/// Line-of-sight path loss, in dB.
pub fn line_of_sight_loss(
    d__meter: f64,
    h_e__meter: [f64; 2],
    z_g: Complex,
    delta_h__meter: f64,
    m_d: f64,
    a_d0: f64,
    d_sml__meter: f64,
    f__mhz: f64,
) -> f64 {
    let delta_h_d = terrain_roughness(d__meter, delta_h__meter);
    let sigma_h_d = sigma_h_function(delta_h_d);
    let wn = f__mhz / 47.7;

    let sin_psi =
        (h_e__meter[0] + h_e__meter[1]) / (d__meter.powi(2) + (h_e__meter[0] + h_e__meter[1]).powi(2)).sqrt();

    let exp_val = (-f64::min(10.0, wn * sigma_h_d * sin_psi)).exp();
    let r_e_raw = (sin_psi - z_g) / (sin_psi + z_g) * exp_val;

    let q = r_e_raw.re.powi(2) + r_e_raw.im.powi(2);
    let r_e = if q < 0.25 || q < sin_psi {
        r_e_raw * (sin_psi / q).sqrt()
    } else {
        r_e_raw
    };

    let mut delta_phi = wn * 2.0 * h_e__meter[0] * h_e__meter[1] / d__meter;
    if delta_phi > PI / 2.0 {
        delta_phi = PI - (PI / 2.0).powi(2) / delta_phi;
    }

    let rr = Complex::new(delta_phi.cos(), -delta_phi.sin()) + r_e;
    let a_t = -10.0 * (rr.re.powi(2) + rr.im.powi(2)).log10();

    let a_d = m_d * d__meter + a_d0;
    let w = 1.0 / (1.0 + f__mhz * delta_h__meter / f64::max(10_000.0, d_sml__meter));

    w * a_t + (1.0 - w) * a_d
}
