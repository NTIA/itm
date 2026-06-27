pub const PI: f64 = std::f64::consts::PI;
pub const SQRT2: f64 = std::f64::consts::SQRT_2;
pub const A_0__METER: f64 = 6_370_000.0;   // mean earth radius
pub const A_9000__METER: f64 = 9_000_000.0;
pub const THIRD: f64 = 1.0 / 3.0;

// Mode of operation (P2P vs Area)
pub const MODE__P2P: i32 = 0;
pub const MODE__AREA: i32 = 1;

// Mode of propagation
pub const MODE__NOT_SET: i32 = 0;
pub const MODE__LINE_OF_SIGHT: i32 = 1;
pub const MODE__DIFFRACTION: i32 = 2;
pub const MODE__TROPOSCATTER: i32 = 3;

// Polarization
pub const POLARIZATION__HORIZONTAL: i32 = 0;
pub const POLARIZATION__VERTICAL: i32 = 1;

// Siting criteria
pub const SITING_CRITERIA__RANDOM: i32 = 0;
pub const SITING_CRITERIA__CAREFUL: i32 = 1;
pub const SITING_CRITERIA__VERY_CAREFUL: i32 = 2;

// Radio climate
pub const CLIMATE__EQUATORIAL: i32 = 1;
pub const CLIMATE__CONTINENTAL_SUBTROPICAL: i32 = 2;
pub const CLIMATE__MARITIME_SUBTROPICAL: i32 = 3;
pub const CLIMATE__DESERT: i32 = 4;
pub const CLIMATE__CONTINENTAL_TEMPERATE: i32 = 5;
pub const CLIMATE__MARITIME_TEMPERATE_OVER_LAND: i32 = 6;
pub const CLIMATE__MARITIME_TEMPERATE_OVER_SEA: i32 = 7;

// Variability modes
pub const SINGLE_MESSAGE_MODE: i32 = 0;
pub const ACCIDENTAL_MODE: i32 = 1;
pub const MOBILE_MODE: i32 = 2;
pub const BROADCAST_MODE: i32 = 3;
