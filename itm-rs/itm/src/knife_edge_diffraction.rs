use crate::fresnel_integral::fresnel_integral;

/// Knife-edge diffraction loss, in dB.
pub fn knife_edge_diffraction(
    d__meter: f64,
    f__mhz: f64,
    a_e__meter: f64,
    theta_los: f64,
    d_hzn__meter: [f64; 2],
) -> f64 {
    let d_ml = d_hzn__meter[0] + d_hzn__meter[1];
    let theta_nlos = d__meter / a_e__meter - theta_los;
    let d_nlos = d__meter - d_ml;

    let wn = f__mhz / 47.7;
    let v_1 = 0.0795775 * wn * theta_nlos.powi(2) * d_hzn__meter[0] * d_nlos
        / (d_nlos + d_hzn__meter[0]);
    let v_2 = 0.0795775 * wn * theta_nlos.powi(2) * d_hzn__meter[1] * d_nlos
        / (d_nlos + d_hzn__meter[1]);

    fresnel_integral(v_1) + fresnel_integral(v_2)
}
