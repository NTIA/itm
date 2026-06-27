use crate::constants::*;
use crate::errors::*;
use crate::free_space_loss::free_space_loss;
use crate::initialize_area::initialize_area;
use crate::initialize_p2p::initialize_p2p;
use crate::longley_rice::longley_rice;
use crate::p2p::translate_cr_error;
use crate::types::IntermediateValues;
use crate::validate_inputs::validate_inputs;
use crate::variability::variability;

/// Area mode with time/location/situation variability.
pub fn itm_area_tls(
    h_tx__meter: f64,
    h_rx__meter: f64,
    tx_site_criteria: i32,
    rx_site_criteria: i32,
    d__km: f64,
    delta_h__meter: f64,
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
    let (rtn, a, w, _) = itm_area_tls_ex(
        h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
        climate, n_0, f__mhz, pol, epsilon, sigma, mdvar, time, location, situation,
    );
    (rtn, a, w)
}

/// Area mode with time/location/situation variability (extended).
pub fn itm_area_tls_ex(
    h_tx__meter: f64,
    h_rx__meter: f64,
    tx_site_criteria: i32,
    rx_site_criteria: i32,
    d__km: f64,
    delta_h__meter: f64,
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

    if d__km <= 0.0 {
        return (ERROR__PATH_DISTANCE, 0.0, warnings, inter);
    }
    if delta_h__meter < 0.0 {
        return (ERROR__DELTA_H, 0.0, warnings, inter);
    }
    if tx_site_criteria != SITING_CRITERIA__RANDOM
        && tx_site_criteria != SITING_CRITERIA__CAREFUL
        && tx_site_criteria != SITING_CRITERIA__VERY_CAREFUL
    {
        return (ERROR__TX_SITING_CRITERIA, 0.0, warnings, inter);
    }
    if rx_site_criteria != SITING_CRITERIA__RANDOM
        && rx_site_criteria != SITING_CRITERIA__CAREFUL
        && rx_site_criteria != SITING_CRITERIA__VERY_CAREFUL
    {
        return (ERROR__RX_SITING_CRITERIA, 0.0, warnings, inter);
    }

    inter.d__km = d__km;
    let h__meter = [h_tx__meter, h_rx__meter];
    let site_criteria = [tx_site_criteria, rx_site_criteria];

    let (z_g, gamma_e, n_s) = initialize_p2p(f__mhz, 0.0, n_0, pol, epsilon, sigma);
    let (h_e__meter, d_hzn__meter, theta_hzn) =
        initialize_area(site_criteria, gamma_e, delta_h__meter, h__meter);

    let d__meter = d__km * 1000.0;
    let (lr_rtn, a_ref__db, propmode) = longley_rice(
        theta_hzn, f__mhz, z_g, d_hzn__meter, h_e__meter, gamma_e, n_s,
        delta_h__meter, h__meter, d__meter, MODE__AREA, &mut warnings,
    );
    if lr_rtn != SUCCESS {
        return (lr_rtn, 0.0, warnings, inter);
    }

    let a_fs__db = free_space_loss(d__meter, f__mhz);
    let a__db = a_fs__db
        + variability(
            time, location, situation, h_e__meter, delta_h__meter,
            f__mhz, d__meter, a_ref__db, climate, mdvar, &mut warnings,
        );

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

/// Area mode with confidence/reliability variability.
pub fn itm_area_cr(
    h_tx__meter: f64,
    h_rx__meter: f64,
    tx_site_criteria: i32,
    rx_site_criteria: i32,
    d__km: f64,
    delta_h__meter: f64,
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
    let (mut rtn, a, w, _) = itm_area_tls_ex(
        h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
        climate, n_0, f__mhz, pol, epsilon, sigma, mdvar, reliability, 50.0, confidence,
    );
    rtn = translate_cr_error(rtn);
    (rtn, a, w)
}

/// Area mode with confidence/reliability variability (extended).
pub fn itm_area_cr_ex(
    h_tx__meter: f64,
    h_rx__meter: f64,
    tx_site_criteria: i32,
    rx_site_criteria: i32,
    d__km: f64,
    delta_h__meter: f64,
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
    let (mut rtn, a, w, inter) = itm_area_tls_ex(
        h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
        climate, n_0, f__mhz, pol, epsilon, sigma, mdvar, reliability, 50.0, confidence,
    );
    rtn = translate_cr_error(rtn);
    (rtn, a, w, inter)
}
