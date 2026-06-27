/// Inverse complementary cumulative distribution function approximation
/// (Abramowitz & Stegun formula 26.2.23, |error| < 4.5e-4).
pub fn iccdf(q: f64) -> f64 {
    const C_0: f64 = 2.515516;
    const C_1: f64 = 0.802853;
    const C_2: f64 = 0.010328;
    const D_1: f64 = 1.432788;
    const D_2: f64 = 0.189269;
    const D_3: f64 = 0.001308;

    let x = if q > 0.5 { 1.0 - q } else { q };
    let t_x = (-2.0 * x.ln()).sqrt();
    let zeta_x = ((C_2 * t_x + C_1) * t_x + C_0) / (((D_3 * t_x + D_2) * t_x + D_1) * t_x + 1.0);
    let q_q = t_x - zeta_x;
    if q > 0.5 { -q_q } else { q_q }
}
