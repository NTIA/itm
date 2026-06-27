use crate::complex::Complex;
use crate::constants::{POLARIZATION__VERTICAL};

/// Initialize parameters for point-to-point mode.
/// Returns `(z_g, gamma_e, n_s)`.
pub fn initialize_p2p(
    f__mhz: f64,
    h_sys__meter: f64,
    n_0: f64,
    pol: i32,
    epsilon: f64,
    sigma: f64,
) -> (Complex, f64, f64) {
    const GAMMA_A: f64 = 157e-9; // curvature of actual earth ~1/6370km

    let n_s = if h_sys__meter == 0.0 {
        n_0
    } else {
        n_0 * (-h_sys__meter / 9460.0).exp()
    };

    let gamma_e = GAMMA_A * (1.0 - 0.04665 * (n_s / 179.3).exp());

    let ep_r = Complex::new(epsilon, 18_000.0 * sigma / f__mhz);
    let mut z_g = (ep_r - 1.0).sqrt();
    if pol == POLARIZATION__VERTICAL {
        z_g = z_g / ep_r;
    }

    (z_g, gamma_e, n_s)
}
