/// Free-space basic transmission loss, in dB.
pub fn free_space_loss(d__meter: f64, f__mhz: f64) -> f64 {
    32.45 + 20.0 * f__mhz.log10() + 20.0 * (d__meter / 1000.0).log10()
}
