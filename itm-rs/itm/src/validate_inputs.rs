use crate::errors::*;
use crate::warnings::*;
use crate::constants::*;

/// Validate inputs common to both P2P and Area modes.
/// Returns an error code or SUCCESS; also sets warning bits.
pub fn validate_inputs(
    h_tx__meter: f64,
    h_rx__meter: f64,
    climate: i32,
    time: f64,
    location: f64,
    situation: f64,
    n_0: f64,
    f__mhz: f64,
    pol: i32,
    epsilon: f64,
    sigma: f64,
    mdvar: i32,
    warnings: &mut i32,
) -> i32 {
    if h_tx__meter < 1.0 || h_tx__meter > 1000.0 {
        *warnings |= WARN__TX_TERMINAL_HEIGHT;
    }
    if h_tx__meter < 0.5 || h_tx__meter > 3000.0 {
        return ERROR__TX_TERMINAL_HEIGHT;
    }

    if h_rx__meter < 1.0 || h_rx__meter > 1000.0 {
        *warnings |= WARN__RX_TERMINAL_HEIGHT;
    }
    if h_rx__meter < 0.5 || h_rx__meter > 3000.0 {
        return ERROR__RX_TERMINAL_HEIGHT;
    }

    if climate != CLIMATE__EQUATORIAL
        && climate != CLIMATE__CONTINENTAL_SUBTROPICAL
        && climate != CLIMATE__MARITIME_SUBTROPICAL
        && climate != CLIMATE__DESERT
        && climate != CLIMATE__CONTINENTAL_TEMPERATE
        && climate != CLIMATE__MARITIME_TEMPERATE_OVER_LAND
        && climate != CLIMATE__MARITIME_TEMPERATE_OVER_SEA
    {
        return ERROR__INVALID_RADIO_CLIMATE;
    }

    if n_0 < 250.0 || n_0 > 400.0 {
        return ERROR__REFRACTIVITY;
    }

    if f__mhz < 40.0 || f__mhz > 10_000.0 {
        *warnings |= WARN__FREQUENCY;
    }
    if f__mhz < 20.0 || f__mhz > 20_000.0 {
        return ERROR__FREQUENCY;
    }

    if pol != POLARIZATION__HORIZONTAL && pol != POLARIZATION__VERTICAL {
        return ERROR__POLARIZATION;
    }

    if epsilon < 1.0 {
        return ERROR__EPSILON;
    }
    if sigma <= 0.0 {
        return ERROR__SIGMA;
    }

    if mdvar < 0
        || (mdvar > 3 && mdvar < 10)
        || (mdvar > 13 && mdvar < 20)
        || (mdvar > 23 && mdvar < 30)
        || mdvar > 33
    {
        return ERROR__MDVAR;
    }

    if situation <= 0.0 || situation >= 100.0 {
        return ERROR__INVALID_SITUATION;
    }
    if time <= 0.0 || time >= 100.0 {
        return ERROR__INVALID_TIME;
    }
    if location <= 0.0 || location >= 100.0 {
        return ERROR__INVALID_LOCATION;
    }

    SUCCESS
}
