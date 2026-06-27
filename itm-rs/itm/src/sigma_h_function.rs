/// RMS deviation of terrain (sigma_h).
pub fn sigma_h_function(delta_h__meter: f64) -> f64 {
    0.78 * delta_h__meter * (-0.5 * delta_h__meter.powf(0.25)).exp()
}
