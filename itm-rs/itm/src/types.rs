/// Intermediate values computed during ITM propagation calculations.
#[derive(Clone, Debug, Default)]
pub struct IntermediateValues {
    /// Terminal horizon angles, in radians [TX, RX]
    pub theta_hzn: [f64; 2],
    /// Terminal horizon distances, in meters [TX, RX]
    pub d_hzn__meter: [f64; 2],
    /// Terminal effective heights, in meters [TX, RX]
    pub h_e__meter: [f64; 2],
    /// Surface refractivity, in N-Units
    pub n_s: f64,
    /// Terrain irregularity parameter, in meters
    pub delta_h__meter: f64,
    /// Reference attenuation, in dB
    pub a_ref__db: f64,
    /// Free-space basic transmission loss, in dB
    pub a_fs__db: f64,
    /// Path distance, in km
    pub d__km: f64,
    /// Mode of propagation (0=not set, 1=LOS, 2=diffraction, 3=troposcatter)
    pub mode: i32,
}
