/// Compute terminal radio horizons from the terrain profile.
/// Returns `(theta_hzn, d_hzn__meter)` — horizon angles (radians) and
/// horizon distances (meters) for [TX, RX].
pub fn find_horizons(pfl: &[f64], a_e__meter: f64, h__meter: [f64; 2]) -> ([f64; 2], [f64; 2]) {
    let np = pfl[0] as usize;
    let xi = pfl[1];
    let d__meter = pfl[0] * pfl[1];

    let z_tx = pfl[2] + h__meter[0];
    let z_rx = pfl[np + 2] + h__meter[1];

    let mut theta_hzn = [
        (z_rx - z_tx) / d__meter - d__meter / (2.0 * a_e__meter),
        -(z_rx - z_tx) / d__meter - d__meter / (2.0 * a_e__meter),
    ];
    let mut d_hzn__meter = [d__meter, d__meter];

    let mut d_tx = 0.0_f64;
    let mut d_rx = d__meter;

    for i in 1..np {
        d_tx += xi;
        d_rx -= xi;

        let theta_tx = (pfl[i + 2] - z_tx) / d_tx - d_tx / (2.0 * a_e__meter);
        let theta_rx = -(z_rx - pfl[i + 2]) / d_rx - d_rx / (2.0 * a_e__meter);

        if theta_tx > theta_hzn[0] {
            theta_hzn[0] = theta_tx;
            d_hzn__meter[0] = d_tx;
        }
        if theta_rx > theta_hzn[1] {
            theta_hzn[1] = theta_rx;
            d_hzn__meter[1] = d_rx;
        }
    }

    (theta_hzn, d_hzn__meter)
}
