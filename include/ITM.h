/** @file ITM.h
 * Interface header for this library
 */
#pragma once

#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map
#include <complex>

// TODO-TEMPLATE: This header should provide EVERYTHING needed to interface
// with the shared library, without needing to include other headers.

namespace ITS {
// TODO-TEMPLATE: Use your library's namespace

// Define cross-platform PROPLIB_API to export functions
#ifndef DOXYGEN_SHOULD_SKIP
    #ifndef PROPLIB_API
        #ifdef _WIN32
            #define PROPLIB_API extern "C" __declspec(PROPLIB_API)
        #else
            #define PROPLIB_API extern "C"
        #endif
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Enums

/*******************************************************************************
 * Return Codes defined by this software (0-127)
 ******************************************************************************/
// clang-format off
enum ReturnCode {
    SUCCESS = 0,                               /**< Successful execution without warnings */
    SUCCESS_WITH_WARNINGS = 1,                 /**< Successful execution with warnings */
    ERROR__TX_TERMINAL_HEIGHT = 1000,          /**< TX terminal height is out of range */
    ERROR__RX_TERMINAL_HEIGHT = 1001,          /**< RX terminal height is out of range */
    ERROR__INVALID_RADIO_CLIMATE = 1002,       /**< Invalid value for radio climate */
    ERROR__INVALID_TIME = 1003,                /**< Time percentage is out of range */
    ERROR__INVALID_LOCATION = 1004,            /**< Location percentage is out of range */
    ERROR__INVALID_SITUATION = 1005,           /**< Situation percentage is out of range */
    ERROR__INVALID_CONFIDENCE = 1006,          /**< Confidence percentage is out of range */
    ERROR__INVALID_RELIABILITY = 1007,         /**< Reliability percentage is out of range */
    ERROR__REFRACTIVITY = 1008,                /**< Refractivity is out of range */
    ERROR__FREQUENCY = 1009,                   /**< Frequency is out of range */
    ERROR__POLARIZATION = 1010,                /**< Invalid value for polarization */
    ERROR__EPSILON = 1011,                     /**< Epsilon is out of range */
    ERROR__SIGMA = 1012,                       /**< Sigma is out of range */
    ERROR__GROUND_IMPEDANCE = 1013,            /**< The imaginary portion of the complex impedance is larger than the real portion */
    ERROR__MDVAR = 1014,                       /**< Invalid value for mode of variability */
    ERROR__EFFECTIVE_EARTH = 1016,             /**< Internally computed effective earth radius is invalid */
    ERROR__PATH_DISTANCE = 1017,               /**< Path distance is out of range */
    ERROR__DELTA_H = 1018,                     /**< Delta H (terrain irregularity parameter) is out of range */
    ERROR__TX_SITING_CRITERIA = 1019,          /**< Invalid value for TX siting criteria */
    ERROR__RX_SITING_CRITERIA = 1020,          /**< Invalid value for RX siting criteria */
    ERROR__SURFACE_REFRACTIVITY_SMALL = 1021,  /**< Internally computed surface refractivity value is too small */
    ERROR__SURFACE_REFRACTIVITY_LARGE = 1022,  /**< Internally computed surface refractivity value is too large */
};
// clang-format on

enum WarningCode {
    NO_WARNINGS = 0,
    WARN__TX_TERMINAL_HEIGHT = 0x0001,        /**< TX terminal height is near its limits */
    WARN__RX_TERMINAL_HEIGHT = 0x0002,        /**< RX terminal height is near its limits */
    WARN__FREQUENCY = 0x0004,                 /**< Frequency is near its limits */
    WARN__PATH_DISTANCE_TOO_BIG_1 = 0x0008,   /**< Path distance is near its upper limit */
    WARN__PATH_DISTANCE_TOO_BIG_2 = 0x0010,   /**< Path distance is large - care must be taken with result */
    WARN__PATH_DISTANCE_TOO_SMALL_1 = 0x0020, /**< Path distance is near its lower limit */
    WARN__PATH_DISTANCE_TOO_SMALL_2 = 0x0040, /**< Path distance is small - care must be taken with result */
    WARN__TX_HORIZON_ANGLE = 0x0080,          /**< TX horizon angle is large - small angle approximations could break down */
    WARN__RX_HORIZON_ANGLE = 0x0100,          /**< RX horizon angle is large - small angle approximations could break down */
    WARN__TX_HORIZON_DISTANCE_1 = 0x0200,     /**< TX horizon distance is less than 1/10 of the smooth earth horizon distance */
    WARN__RX_HORIZON_DISTANCE_1 = 0x0400,     /**< RX horizon distance is less than 1/10 of the smooth earth horizon distance */
    WARN__TX_HORIZON_DISTANCE_2 = 0x0800,     /**< TX horizon distance is greater than 3 times the smooth earth horizon distance */
    WARN__RX_HORIZON_DISTANCE_2 = 0x1000,     /**< RX horizon distance is greater than 3 times the smooth earth horizon distance */
    WARN__EXTREME_VARIABILITIES = 0x2000,     /**< One of the provided variabilities is located far in the tail of its distribution */
    WARN__SURFACE_REFRACTIVITY = 0x4000       /**< Internally computed surface refractivity value is small - care must be taken with result */
};

enum RunningMode {
    MODE__P2P = 0,
    MODE__AREA = 1
};

enum ITMMode {
    SINGLE_MESSAGE_MODE = 0,
    ACCIDENTAL_MODE = 1,
    MOBILE_MODE = 2,
    BROADCAST_MODE = 3
};

// List of valid polarizations
enum Polarizationmode {
    POLARIZATION__HORIZONTAL = 0,
    POLARIZATION__VERTICAL = 1
};

// List of valid siting criteria
enum SitingCriteria {
    SITING_CRITERIA__RANDOM = 0,
    SITING_CRITERIA__CAREFUL = 1,
    SITING_CRITERIA__VERY_CAREFUL = 2
};

// List of valid radio climates
enum RadioClimate {
    CLIMATE__EQUATORIAL = 1,
    CLIMATE__CONTINENTAL_SUBTROPICAL = 2,
    CLIMATE__MARITIME_SUBTROPICAL = 3,
    CLIMATE__DESERT = 4,
    CLIMATE__CONTINENTAL_TEMPERATE = 5,
    CLIMATE__MARITIME_TEMPERATE_OVER_LAND = 6,
    CLIMATE__MARITIME_TEMPERATE_OVER_SEA = 7
};

// List of valid modes of propagation
enum PropagationMode {
    MODE__NOT_SET = 0,
    MODE__LINE_OF_SIGHT = 1,
    MODE__DIFFRACTION = 2,
    MODE__TROPOSCATTER = 3
};

// List of modes of variability
enum VariabilityMode {
    MDVAR__SINGLE_MESSAGE_MODE = 0,
    MDVAR__ACCIDENTAL_MODE = 1,
    MDVAR__MOBILE_MODE = 2,
    MDVAR__BROADCAST_MODE = 3
};
////////////////////////////////////////////////////////////////////////////////
// Constants
// TODO-TEMPLATE define any global constants here (use constexpr!)

constexpr double PI = 3.1415926535897932384;
constexpr double a_0__meter = 6370e3;
constexpr double a_9000__meter = 9000e3;
constexpr double THIRD = 1.0 / 3.0;



////////////////////////////////////////////////////////////////////////////////
// Data Structures
struct IntermediateValues {
    double theta_hzn[2];     // Terminal horizon angles
    double d_hzn__meter[2];  // Terminal horizon distances, in meters
    double h_e__meter[2];    // Terminal effective heights, in meters
    double N_s;              // Surface refractivity, in N-Units
    double delta_h__meter;   // Terrain irregularity parameter, in meters
    double A_ref__db;        // Reference attenuation, in dB
    double A_fs__db;         // Free space basic transmission loss, in dB
    double d__km;            // Path distance, in km
    int mode;                // Mode of propagation value
};

////////////////////////////////////////////////////////////////////////////////
// Public Functions
// TODO-TEMPLATE: Add functions which should be PROPLIB_API in the DLL
PROPLIB_API ReturnCode TM_P2P_TLS(
    const double h_tx__meter,
    const double h_rx__meter,
    const double pfl[],
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double time,
    const double location,
    const double situation,
    double *A__db,
    long *warnings
);
PROPLIB_API ReturnCode ITM_P2P_TLS_Ex(
    const double h_tx__meter,
    const double h_rx__meter,
    const double pfl[],
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double time,
    const double location,
    const double situation,
    double *A__db,
    long *warnings,
    IntermediateValues *interValues
);
PROPLIB_API ReturnCode ITM_P2P_CR(
    const double h_tx__meter,
    const double h_rx__meter,
    const double pfl[],
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double confidence,
    const double reliability,
    double *A__db,
    long *warnings
);
PROPLIB_API ReturnCode ITM_P2P_CR_Ex(
    const double h_tx__meter,
    const double h_rx__meter,
    const double pfl[],
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double confidence,
    const double reliability,
    double *A__db,
    long *warnings,
    IntermediateValues *interValues
);
PROPLIB_API ReturnCode ITM_AREA_TLS(
    const double h_tx__meter,
    const double h_rx__meter,
    const int tx_site_criteria,
    const int rx_site_criteria,
    const double d__km,
    const double delta_h__meter,
    const int climate,
    const double N_0,
    double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double time,
    const double location,
    const double situation,
    double *A__db,
    long *warnings
);
PROPLIB_API ReturnCode ITM_AREA_TLS_Ex(
    const double h_tx__meter,
    const double h_rx__meter,
    const int tx_site_criteria,
    const int rx_site_criteria,
    const double d__km,
    const double delta_h__meter,
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double time,
    const double location,
    const double situation,
    double *A__db,
    long *warnings,
    IntermediateValues *interValues
);
PROPLIB_API ReturnCode ITM_AREA_CR(
    const double h_tx__meter,
    const double h_rx__meter,
    const int tx_site_criteria,
    const int rx_site_criteria,
    const double d__km,
    const double delta_h__meter,
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double confidence,
    const double reliability,
    double *A__db,
    long *warnings
);
PROPLIB_API ReturnCode ITM_AREA_CR_Ex(
    const double h_tx__meter,
    const double h_rx__meter,
    const int tx_site_criteria,
    const int rx_site_criteria,
    const double d__km,
    const double delta_h__meter,
    const int climate,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    const double confidence,
    const double reliability,
    double *A__db,
    long *warnings,
    IntermediateValues *interValues
);

/////////////////////////////
// ITM Helper Functions

PROPLIB_API double ComputeDeltaH(
    const double pfl[], const double d_start__meter, const double d_end__meter
);
PROPLIB_API double DiffractionLoss(
    const double d__meter,
    const double d_hzn__meter[2],
    const double h_e__meter[2],
    const std::complex<double> Z_g,
    const double a_e__meter,
    const double delta_h__meter,
    const double h__meter[2],
    const int mode,
    const double theta_los,
    const double d_sML__meter,
    const double f__mhz
);
PROPLIB_API double FFunction(const double td);
PROPLIB_API void FindHorizons(
    const double pfl[],
    const double a_e__meter,
    const double h__meter[2],
    double theta_hzn[2],
    double d_hzn__meter[2]
);
PROPLIB_API double FreeSpaceLoss(const double d__meter, const double f__mhz);
PROPLIB_API double FresnelIntegral(const double v2);
PROPLIB_API double H0Function(const double r, double eta_s);
PROPLIB_API double HeightFunction(const double x__km, const double K);
PROPLIB_API void InitializeArea(
    const int site_criteria[2],
    const double gamma_e,
    const double delta_h__meter,
    const double h__meter[2],
    double h_e__meter[2],
    double d_hzn__meter[2],
    double theta_hzn[2]
);
PROPLIB_API void InitializePointToPoint(
    const double f__mhz,
    const double h_sys__meter,
    const double N_0,
    const int pol,
    const double epsilon,
    const double sigma,
    std::complex<double> *Z_g,
    double *gamma_e,
    double *N_s
);
PROPLIB_API double
    InverseComplementaryCumulativeDistributionFunction(const double q);
PROPLIB_API double KnifeEdgeDiffraction(
    const double d__meter,
    const double f__mhz,
    const double a_e__meter,
    const double theta_los,
    const double d_hzn__meter[2]
);
PROPLIB_API void LinearLeastSquaresFit(
    const double pfl[],
    const double d_start,
    const double d_end,
    double *fit_y1,
    double *fit_y2
);
PROPLIB_API double LineOfSightLoss(
    const double d__meter,
    const double h_e__meter[2],
    const std::complex<double> Z_g,
    const double delta_h__meter,
    const double M_d,
    const double A_d0,
    const double d_sML__meter,
    const double f__mhz
);
PROPLIB_API ReturnCode LongleyRice(
    const double theta_hzn[2],
    const double f__mhz,
    const std::complex<double> Z_g,
    const double d_hzn__meter[2],
    const double h_e__meter[2],
    const double gamma_e,
    const double N_s,
    const double delta_h__meter,
    const double h__meter[2],
    const double d__meter,
    const int mode,
    double *A_ref__db,
    long *warnings,
    int *propmode
);
PROPLIB_API void QuickPfl(
    const double pfl[],
    const double gamma_e,
    const double h__meter[2],
    double theta_hzn[2],
    double d_hzn__meter[2],
    double h_e__meter[2],
    double *delta_h__meter,
    double *d__meter
);
PROPLIB_API double SigmaHFunction(const double delta_h__meter);
PROPLIB_API double SmoothEarthDiffraction(
    const double d__meter,
    const double f__mhz,
    const double a_e__meter,
    const double theta_los,
    const double d_hzn__meter[2],
    const double h_e__meter[2],
    const std::complex<double> Z_g
);
PROPLIB_API double
    TerrainRoughness(const double d__meter, const double delta_h__meter);
PROPLIB_API double TroposcatterLoss(
    const double d__meter,
    const double theta_hzn[2],
    const double d_hzn__meter[2],
    const double h_e__meter[2],
    const double a_e__meter,
    const double N_s,
    const double f__mhz,
    const double theta_los,
    double *h0
);
PROPLIB_API ReturnCode ValidateInputs(
    const double h_tx__meter,
    const double h_rx__meter,
    const int climate,
    const double time,
    const double location,
    const double situation,
    const double N_0,
    const double f__mhz,
    const int pol,
    const double epsilon,
    const double sigma,
    const int mdvar,
    long *warnings
);
PROPLIB_API double Variability(
    const double time,
    const double location,
    const double situation,
    const double h_e__meter[2],
    const double delta_h__meter,
    const double f__mhz,
    const double d__meter,
    const double A_ref__db,
    const int climate,
    const int mdvar,
    long *warnings
);

////////////////////////////////////////////////////////////////////////////////
// Private Functions
// TODO-TEMPLATE: Add other/internal functions here (no need for "PROPLIB_API")
std::string GetReturnStatus(const int code);

}  // namespace ITS
