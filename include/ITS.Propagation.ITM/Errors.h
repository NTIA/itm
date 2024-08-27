
///////////////////////////////////////////////
// ERROR CODES

#define SUCCESS                                 0
#define NO_WARNINGS                             0
#define SUCCESS_WITH_WARNINGS                   1

#define ERROR__TX_TERMINAL_HEIGHT               1000    // TX terminal height is out of range
#define ERROR__RX_TERMINAL_HEIGHT               1001    // RX terminal height is out of range
#define ERROR__INVALID_RADIO_CLIMATE            1002    // Invalid value for radio climate
#define ERROR__INVALID_TIME                     1003    // Time percentage is out of range
#define ERROR__INVALID_LOCATION                 1004    // Location percentage is out of range
#define ERROR__INVALID_SITUATION                1005    // Situation percentage is out of range
#define ERROR__INVALID_CONFIDENCE               1006    // Confidence percentage is out of range
#define ERROR__INVALID_RELIABILITY              1007    // Reliability percentage is out of range
#define ERROR__REFRACTIVITY                     1008    // Refractivity is out of range
#define ERROR__FREQUENCY                        1009    // Frequency is out of range
#define ERROR__POLARIZATION                     1010    // Invalid value for polarization
#define ERROR__EPSILON                          1011    // Epsilon is out of range
#define ERROR__SIGMA                            1012    // Sigma is out of range
#define ERROR__GROUND_IMPEDANCE                 1013    // The imaginary portion of the complex impedance is larger than the real portion
#define ERROR__MDVAR                            1014    // Invalid value for mode of variability
#define ERROR__EFFECTIVE_EARTH                  1016    // Internally computed effective earth radius is invalid
#define ERROR__PATH_DISTANCE                    1017    // Path distance is out of range
#define ERROR__DELTA_H                          1018    // Delta H (terrain irregularity parameter) is out of range
#define ERROR__TX_SITING_CRITERIA               1019    // Invalid value for TX siting criteria
#define ERROR__RX_SITING_CRITERIA               1020    // Invalid value for RX siting criteria
#define ERROR__SURFACE_REFRACTIVITY_SMALL       1021    // Internally computed surface refractivity value is too small
#define ERROR__SURFACE_REFRACTIVITY_LARGE       1022    // Internally computed surface refractivity value is too large
