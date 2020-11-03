# Return Codes and Warning Flags #

## Return Codes ##

ITM supports the following list of return codes.

| Value | Const Name                       | Description  |
| ------|----------------------------------|--------------|
|     0 | `SUCCESS`                        | Successful execution |
|     1 | `SUCCESS_WITH_WARNINGS`          | Successful execution, but warning flags set |
|  1000 | `ERROR__TX_TERMINAL_HEIGHT`      | TX terminal height is out of range |
|  1001 | `ERROR__RX_TERMINAL_HEIGHT`      | RX terminal height is out of range |
|  1002 | `ERROR__INVALID_RADIO_CLIMATE`   | Invalid value for radio climate |
|  1003 | `ERROR__INVALID_TIME`            | Time percentage is out of range |
|  1004 | `ERROR__INVALID_LOCATION`        | Location percentage is out of range |
|  1005 | `ERROR__INVALID_SITUATION`       | Situation percentage is out of range |
|  1006 | `ERROR__INVALID_CONFIDENCE`      | Confidence percentage is out of range |
|  1007 | `ERROR__INVALID_RELIABILITY`     | Reliability percentage is out of range |
|  1008 | `ERROR__REFRACTIVITY`            | Refractivity is out of range |
|  1009 | `ERROR__FREQUENCY`               | Frequency is out of range |
|  1010 | `ERROR__POLARIZATION`            | Invalid value for polarization |
|  1011 | `ERROR__EPSILON`                 | Epsilon is out of range |
|  1012 | `ERROR__SIGMA`                   | Sigma is out of range |
|  1013 | `ERROR__GROUND_IMPEDANCE`        | The imaginary portion of the complex impedance is larger than the real portion |
|  1014 | `ERROR__MDVAR`                   | Invalid value for mode of variability |
|  1016 | `ERROR__EFFECTIVE_EARTH`         | Internally computed effective earth radius is invalid |
|  1017 | `ERROR__PATH_DISTANCE`           | Path distance is out of range |
|  1018 | `ERROR__DELTA_H`                 | Delta H (terrain irregularity parameter) is out of range |
|  1019 | `ERROR__TX_SITING_CRITERIA`      | Invalid value for TX siting criteria |
|  1020 | `ERROR__RX_SITING_CRITERIA`      | Invalid value for RX siting criteria |
|  1021 | `ERROR__SURFACE_REFRACTIVITY_SMALL`    | Internally computed surface refractivity value is too small |
|  1022 | `ERROR__SURFACE_REFRACTIVITY_LARGE`    | Internally computed surface refractivity value is too large |

## Warning Flags ##

In addition to return codes, ITM supports return warning flags to the caller.  Warning flags are bitwise set, allowing the ITM model to return multiple warnings for a single function call.  Warning flags only need to be parsed if a return code of `SUCCESS_WITH_WARNINGS` is returned by a function call.

| Value  | Const Name                       | Description  |
| -------|----------------------------------|--------------|
| 0x0000 | `NO_WARNINGS`                    | No warning flags |
| 0x0001 | `WARN__TX_TERMINAL_HEIGHT`       | TX terminal height is near its limits |
| 0x0002 | `WARN__RX_TERMINAL_HEIGHT`       | RX terminal height is near its limits |
| 0x0004 | `WARN__FREQUENCY`                | Frequency is near its limits |
| 0x0008 | `WARN__PATH_DISTANCE_TOO_BIG_1`  | Path distance is near its upper limit |
| 0x0010 | `WARN__PATH_DISTANCE_TOO_BIG_2`  | Path distance is large - care must be taken with result |
| 0x0020 | `WARN__PATH_DISTANCE_TOO_SMALL_1` | Path distance is near its lower limit |
| 0x0040 | `WARN__PATH_DISTANCE_TOO_SMALL_2` | Path distance is small - care must be taken with result
| 0x0080 | `WARN__TX_HORIZON_ANGLE`         | TX horizon angle is large - small angle approximations could break down |
| 0x0100 | `WARN__RX_HORIZON_ANGLE`         | RX horizon angle is large - small angle approximations could break down |
| 0x0200 | `WARN__TX_HORIZON_DISTANCE_1`    | TX horizon distance is less than 1/10 of the smooth earth horizon distance |
| 0x0400 | `WARN__RX_HORIZON_DISTANCE_1`    | RX horizon distance is less than 1/10 of the smooth earth horizon distance |
| 0x0800 | `WARN__TX_HORIZON_DISTANCE_2`    | TX horizon distance is greater than 3 times the smooth earth horizon distance |
| 0x1000 | `WARN__RX_HORIZON_DISTANCE_2`    | RX horizon distance is greater than 3 times the smooth earth horizon distance |
| 0x2000 | `WARN__EXTREME_VARIABILITIES`    | One of the provided variabilities is located far in the tail of its distribution |
| 0x4000 | `WARN__SURFACE_REFRACTIVITY`     | Internally computed surface refractivity value is small - care must be taken with result |
