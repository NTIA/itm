use crate::complex::Complex;
use crate::constants::MODE__P2P;
use crate::knife_edge_diffraction::knife_edge_diffraction;
use crate::sigma_h_function::sigma_h_function;
use crate::smooth_earth_diffraction::smooth_earth_diffraction;
use crate::terrain_roughness::terrain_roughness;

/// Combined diffraction loss, in dB.
pub fn diffraction_loss(
    d__meter: f64,
    d_hzn__meter: [f64; 2],
    h_e__meter: [f64; 2],
    z_g: Complex,
    a_e__meter: f64,
    delta_h__meter: f64,
    h__meter: [f64; 2],
    mode: i32,
    theta_los: f64,
    d_sml__meter: f64,
    f__mhz: f64,
) -> f64 {
    let a_k = knife_edge_diffraction(d__meter, f__mhz, a_e__meter, theta_los, d_hzn__meter);
    let a_se = smooth_earth_diffraction(
        d__meter,
        f__mhz,
        a_e__meter,
        theta_los,
        d_hzn__meter,
        h_e__meter,
        z_g,
    );

    let delta_h_dsml = terrain_roughness(d_sml__meter, delta_h__meter);
    let sigma_h_d = sigma_h_function(delta_h_dsml);
    let a_fo = f64::min(
        15.0,
        5.0 * (1.0 + 1e-5 * h__meter[0] * h__meter[1] * f__mhz * sigma_h_d).log10(),
    );

    let delta_h_d = terrain_roughness(d__meter, delta_h__meter);
    let d_ml = d_hzn__meter[0] + d_hzn__meter[1];

    let mut q = h__meter[0] * h__meter[1];
    let qk = h_e__meter[0] * h_e__meter[1] - q;
    if mode == MODE__P2P {
        q += 10.0;
    }

    let term1 = (1.0 + qk / q).sqrt();
    let q = (term1 + (-theta_los * a_e__meter + d_ml) / d__meter)
        * f64::min(delta_h_d * f__mhz / 47.7, 6283.2);

    let w = 25.1 / (25.1 + q.sqrt());
    w * a_se + (1.0 - w) * a_k + a_fo
}
