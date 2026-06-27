/// Approximate knife-edge diffraction loss.
/// `v2` is v^2 (so 5.76 corresponds to v = 2.4).
pub fn fresnel_integral(v2: f64) -> f64 {
    if v2 < 5.76 {
        6.02 + 9.11 * v2.sqrt() - 1.27 * v2
    } else {
        12.953 + 10.0 * v2.log10()
    }
}
