
///////////////////////////////////////////////
// LABELS

#define LBL__SUCCESS                                "Success - No Errors"
#define LBL__SUCCESS_WITH_WARNINGS                  "Success - but with warnings"
#define LBL__NO_WARNINGS                            "No Warnings"

// Modes of propagation
#define LBL__LINE_OF_SITE                           "Line of Sight"
#define LBL__DIFFRACTION                            "Diffraction"
#define LBL__TROPOSCATTER                           "Troposcatter"

// Polarization
#define LBL__POLARIZATION_HORIZONTAL                "Horizontal"
#define LBL__POLARIZATION_VERTICAL                  "Vertical"

// Terminal siting criteria
#define LBL__SITING_CRITERIA_RANDOM                 "Random"
#define LBL__SITING_CRITERIA_CAREFUL                "Careful"
#define LBL__SITING_CRITERIA_VERY_CAREFUL           "Very Careful"

// Radio climates
#define LBL__CLIMATE_EQUATORIAL                     "Equatorial"
#define LBL__CLIMATE_CONTINENTAL_SUBTROPICAL        "Continental Subtropical"
#define LBL__CLIMATE_MARITIME_SUBTROPICAL           "Maritime Subtropical"
#define LBL__CLIMATE_DESERT                         "Desert"
#define LBL__CLIMATE_CONTINENTAL_TEMPERATE          "Continental Temperate"
#define LBL__CLIMATE_MARITIME_TEMPERATE_OVER_LAND   "Maritime Temperate over Land"
#define LBL__CLIMATE_MARITIME_TEMPERATE_OVER_SEA    "Maritime Temperate over Sea"

// Mode of variabilities
#define LBL__MDVAR_SINGLE_MESSAGE_MODE              "Single Message Mode"
#define LBL__MDVAR_ACCIDENTAL_MODE                  "Accidental Mode"
#define LBL__MDVAR_MOBILE_MODE                      "Mobile Mode"
#define LBL__MDVAR_BROADCAST_MODE                   "Broadcast Mode"

// Errors
#define LBL__ERROR_INVALID_VALUE                    "Invalid Value"
#define LBL__ERROR_TX_TERMINAL_HEIGHT               "TX terminal height is out of range"
#define LBL__ERROR_RX_TERMINAL_HEIGHT               "RX terminal height is out of range"
#define LBL__ERROR_INVALID_RADIO_CLIMATE            "Invalid value for radio climate"
#define LBL__ERROR_INVALID_TIME                     "Time percentage is out of range"
#define LBL__ERROR_INVALID_LOCATION                 "Location percentage is out of range"
#define LBL__ERROR_INVALID_SITUATION                "Situation percentage is out of range"
#define LBL__ERROR_INVALID_CONFIDENCE               "Confidence percentage is out of range"
#define LBL__ERROR_INVALID_RELIABILITY              "Reliability percentage is out of range"
#define LBL__ERROR_REFRACTIVITY                     "Refractivity is out of range"
#define LBL__ERROR_FREQUENCY                        "Frequency is out of range"
#define LBL__ERROR_POLARIZATION                     "Invalid value for polarization"
#define LBL__ERROR_EPSILON                          "Epsilon is out of range"
#define LBL__ERROR_SIGMA                            "Sigma is out of range"
#define LBL__ERROR_GROUND_IMPEDANCE                 "The imaginary portion of the complex impedance is larger than the real portion"
#define LBL__ERROR_MDVAR                            "Invalid value for mode of variability"
#define LBL__ERROR_EFFECTIVE_EARTH                  "Internally computed effective earth radius is invalid"
#define LBL__ERROR_PATH_DISTANCE                    "Path distance is out of range"
#define LBL__ERROR_DELTA_H                          "Delta H (terrain irregularity parameter) is out of range"
#define LBL__ERROR_TX_SITING_CRITERIA               "Invalid value for TX siting criteria"
#define LBL__ERROR_RX_SITING_CRITERIA               "Invalid value for RX siting criteria"
#define LBL__ERROR_SURFACE_REFRACTIVITY_SMALL       "Internally computed surface refractivity value is too small"
#define LBL__ERROR_SURFACE_REFRACTIVITY_LARGE       "Internally computed surface refractivity value is too large"

// Warnings
#define LBL__WARN_TX_TERMINAL_HEIGHT                "TX terminal height is near its limits"
#define LBL__WARN_RX_TERMINAL_HEIGHT                "RX terminal height is near its limits"
#define LBL__WARN_FREQUENCY                         "Frequency is near its limits"
#define LBL__WARN_PATH_DISTANCE_TOO_BIG_1           "Path distance is near its upper limit"
#define LBL__WARN_PATH_DISTANCE_TOO_BIG_2           "Path distance is large - care must be taken with result"
#define LBL__WARN_PATH_DISTANCE_TOO_SMALL_1         "Path distance is near its lower limit"
#define LBL__WARN_PATH_DISTANCE_TOO_SMALL_2         "Path distance is small - care must be taken with result"
#define LBL__WARN_TX_HORIZON_ANGLE                  "TX horizon angle is large - small angle approximations could break down"
#define LBL__WARN_RX_HORIZON_ANGLE                  "RX horizon angle is large - small angel approximations could break down"
#define LBL__WARN_TX_HORIZON_DISTANCE_1             "TX horizon distance is less than 1/10 of the smooth earth horizon distance"
#define LBL__WARN_RX_HORIZON_DISTANCE_1             "RX horizon distance is less than 1/10 of the smooth earth horizon distance"
#define LBL__WARN_TX_HORIZON_DISTANCE_2             "TX horizon distance is greater than 3 times the smooth earth horizon distance"
#define LBL__WARN_RX_HORIZON_DISTANCE_2             "RX horizon distance is greater than 3 times the smooth earth horizon distance"
#define LBL__WARN_EXTREME_VARIABILITIES             "One of the provided variabilities is located far in the tail of its distribution"
#define LBL__WARN_SURFACE_REFRACTIVITY              "Internally computed surface refractivity value is small - care must be taken with result"
