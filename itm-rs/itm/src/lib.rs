#![allow(non_snake_case, dead_code, clippy::all)]

mod complex;
mod constants;
mod types;
mod errors;
mod warnings;
mod fresnel_integral;
mod free_space_loss;
mod terrain_roughness;
mod sigma_h_function;
mod h0_function;
mod linear_least_squares_fit;
mod compute_delta_h;
mod find_horizons;
mod initialize_p2p;
mod initialize_area;
mod validate_inputs;
mod quick_pfl;
mod knife_edge_diffraction;
mod smooth_earth_diffraction;
mod diffraction_loss;
mod line_of_sight_loss;
mod troposcatter_loss;
mod iccdf;
mod variability;
mod longley_rice;
pub mod p2p;
pub mod area;

pub use types::IntermediateValues;
pub use errors::*;
pub use warnings::*;
pub use p2p::{itm_p2p_tls, itm_p2p_tls_ex, itm_p2p_cr, itm_p2p_cr_ex};
pub use area::{itm_area_tls, itm_area_tls_ex, itm_area_cr, itm_area_cr_ex};

#[cfg(test)]
mod tests;

/// Radio climate
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(i32)]
pub enum Climate {
    Equatorial = 1,
    ContinentalSubtropical = 2,
    MaritimeSubtropical = 3,
    Desert = 4,
    ContinentalTemperate = 5,
    MaritimeTemperateOverLand = 6,
    MaritimeTemperateOverSea = 7,
}

/// Polarization
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(i32)]
pub enum Polarization {
    Horizontal = 0,
    Vertical = 1,
}

/// Siting criteria
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(i32)]
pub enum SitingCriteria {
    Random = 0,
    Careful = 1,
    VeryCareful = 2,
}

/// Mode of propagation (returned in IntermediateValues)
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(i32)]
pub enum PropMode {
    NotSet = 0,
    LineOfSight = 1,
    Diffraction = 2,
    Troposcatter = 3,
}
