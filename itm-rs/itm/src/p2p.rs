use crate::constants::MODE__P2P;
use crate::errors::*;
use crate::free_space_loss::free_space_loss;
use crate::initialize_p2p::initialize_p2p;
use crate::longley_rice::longley_rice;
use crate::quick_pfl::quick_pfl;
use crate::types::IntermediateValues;
use crate::validate_inputs::validate_inputs;
use crate::variability::variability;

/// Point-to-point mode with time/location/situation variability.
/// Returns `(error_code, a__db, warnings)`.
pub fn itm_p2p_tls(
    h_tx__meter: f64,
    h_rx__meter: f64,
    pfl: &[f64],
    climate: i32,
    n_0: f64,
    f__mhz: f64,
    pol: i32,
    epsilon: f64,
    sigma: f64,
    mdvar: i32,
    time: f64,
    location: f64,
    situation: f64,
) -> (i32, f64, i32) {
    let (rtn, a__db, warnings, _) =
        itm_p2p_tls_ex(h_tx__meter, h_rx__meter, pfl, climate, n_0, f__mhz, pol, epsilon, sigma, mdvar, time, location, situation);
    (rtn, a__db, warnings)
}

/// Point-to-point mode with time/location/situation variability (extended — returns intermediate values).
pub fn itm_p2p_tls_ex(
    h_tx__meter: f64,
    h_rx__meter: f64,
    pfl: &[f64],
    climate: i32,
    n_0: f64,
    f__mhz: f64,
    pol: i32,
    epsilon: f64,
    sigma: f64,
    mdvar: i32,
    time: f64,
    location: f64,
    situation: f64,
) -> (i32, f64, i32, IntermediateValues) {
    let mut inter = IntermediateValues::default();
    let mut warnings = NO_WARNINGS;

    let rtn = validate_inputs(
        h_tx__meter, h_rx__meter, climate, time, location, situation,
        n_0, f__mhz, pol, epsilon, sigma, mdvar, &mut warnings,
    );
    if rtn != SUCCESS {
        return (rtn, 0.0, warnings, inter);
    }

    inter.d__km = pfl[0] * pfl[1] / 1000.0;
    let np = pfl[0] as usize;

    // Average path height ignoring first and last 10%
    let p10 = (0.1 * np as f64) as usize;
    let h_sys: f64 = pfl[(p10 + 2)..=(np - p10 + 2)]
        .iter()
        .sum::<f64>()
        / (np - 2 * p10 + 1) as f64;

    let (z_g, gamma_e, n_s) = initialize_p2p(f__mhz, h_sys, n_0, pol, epsilon, sigma);
    let h__meter = [h_tx__meter, h_rx__meter];

    let (theta_hzn, d_hzn__meter, h_e__meter, delta_h__meter, d__meter) =
        quick_pfl(pfl, gamma_e, h__meter);

    let (lr_rtn, a_ref__db, propmode) = longley_rice(
        theta_hzn, f__mhz, z_g, d_hzn__meter, h_e__meter, gamma_e, n_s,
        delta_h__meter, h__meter, d__meter, MODE__P2P, &mut warnings,
    );
    if lr_rtn != SUCCESS {
        return (lr_rtn, 0.0, warnings, inter);
    }

    let a_fs__db = free_space_loss(d__meter, f__mhz);
    let a__db = variability(
        time, location, situation, h_e__meter, delta_h__meter,
        f__mhz, d__meter, a_ref__db, climate, mdvar, &mut warnings,
    ) + a_fs__db;

    inter.a_ref__db = a_ref__db;
    inter.a_fs__db = a_fs__db;
    inter.delta_h__meter = delta_h__meter;
    inter.d_hzn__meter = d_hzn__meter;
    inter.h_e__meter = h_e__meter;
    inter.n_s = n_s;
    inter.theta_hzn = theta_hzn;
    inter.mode = propmode;

    let rtn = if warnings != NO_WARNINGS { SUCCESS_WITH_WARNINGS } else { SUCCESS };
    (rtn, a__db, warnings, inter)
}

/// Point-to-point mode with confidence/reliability variability.
pub fn itm_p2p_cr(
    h_tx__meter: f64,
    h_rx__meter: f64,
    pfl: &[f64],
    climate: i32,
    n_0: f64,
    f__mhz: f64,
    pol: i32,
    epsilon: f64,
    sigma: f64,
    mdvar: i32,
    confidence: f64,
    reliability: f64,
) -> (i32, f64, i32) {
    let (rtn, a, w) = cr_to_tls(
        |time, loc, sit| {
            itm_p2p_tls(h_tx__meter, h_rx__meter, pfl, climate, n_0, f__mhz, pol, epsilon, sigma, mdvar, time, loc, sit)
        },
        confidence,
        reliability,
    );
    (rtn, a, w)
}

/// Point-to-point mode with confidence/reliability (extended).
pub fn itm_p2p_cr_ex(
    h_tx__meter: f64,
    h_rx__meter: f64,
    pfl: &[f64],
    climate: i32,
    n_0: f64,
    f__mhz: f64,
    pol: i32,
    epsilon: f64,
    sigma: f64,
    mdvar: i32,
    confidence: f64,
    reliability: f64,
) -> (i32, f64, i32, IntermediateValues) {
    let (mut rtn, a__db, warnings, inter) = itm_p2p_tls_ex(
        h_tx__meter, h_rx__meter, pfl, climate, n_0, f__mhz, pol, epsilon, sigma, mdvar,
        reliability, 50.0, confidence,
    );
    rtn = translate_cr_error(rtn);
    (rtn, a__db, warnings, inter)
}

/// Map TLS time/situation errors to CR reliability/confidence errors.
pub(crate) fn translate_cr_error(rtn: i32) -> i32 {
    if rtn == ERROR__INVALID_TIME {
        ERROR__INVALID_RELIABILITY
    } else if rtn == ERROR__INVALID_SITUATION {
        ERROR__INVALID_CONFIDENCE
    } else {
        rtn
    }
}

fn cr_to_tls<F>(f: F, confidence: f64, reliability: f64) -> (i32, f64, i32)
where
    F: Fn(f64, f64, f64) -> (i32, f64, i32),
{
    let (mut rtn, a, w) = f(reliability, 50.0, confidence);
    rtn = translate_cr_error(rtn);
    (rtn, a, w)
}
