# ITM Command-line Driver #

This readme explains the use of the ITM command-line driver.  This is a supplemental software tool to allow a user to call the ITM DLL from the command-line with inputs specified in supporting file formats.

## Input Files ##

Inputs to the command-line driver are specified in an ASCII text file using the common `key,value` format.  Each line holds a single `key,value` combination, with the `key` representing the model input variable name and the `value` representing its value.  The driver will automatically detect whether ITM should be called specifying variability in time/location/situation or confidence/reliability based on the input parameters specified.

## Output Files ##

After parsing the inputs and calling ITM, the command-line driver will generate an output report file containing the results.  This results file contains inputs params, output params, and human-readable supporting documentation.

## Execution ##

Executing the ITM command-line driver requires specifying input arguments, defined in the below table:

| Flag    | Type   | Required? | Description  |
|---------|--------|-----------|--------------|
| `-i`    | string | True      | File specifying model input parameters in `key,value` format |
| `-o`    | string | True      | Filename where output results should be written |
| `-mode` | enum | True      | Specify the prediction mode of ITM.  <ul><li>`P2P` = Point-to-Point Mode</li><li>`AREA` = Area Mode</li></ul> |
| `-t`    | string | False     | File containing terrain information in PFL format.  Only needed if using point-to-point mode |
| `-dbg`  |        | False     | If flag is specified, the intermediate data structure values will be written to the output file |

Input arguments are not case sensitive and do not have to be specified in a certain order.  A generic example of calling the ITM command-line driver is:

```cmd
ITMDrvr.exe -i <input_file> -t <terrain_file> -o <output_file> -mode P2P
```

### Examples ###

| Input File | Terrain File | Output File | Arguments |
|------------|--------------|-------------|-----------|
| [`i_p2ptls.txt`](/cmd_examples/i_p2ptls.txt) | [`pfl.txt`](/cmd_examples/pfl.txt) | [`o_p2ptls.txt`](/cmd_examples/o_p2ptls.txt) | `-i i_p2ptls.txt -t pfl.txt -o o_p2ptls.txt -mode P2P -dbg` |
| [`i_p2pcr.txt`](/cmd_examples/i_p2pcr.txt) | [`pfl.txt`](/cmd_examples/pfl.txt) | [`o_p2pcr.txt`](/cmd_examples/o_p2pcr.txt) | `-i i_p2pcr.txt -t pfl.txt -o o_p2pcr.txt -mode P2P -dbg` |
| [`i_p2pcr_tbl.txt`](/cmd_examples/i_p2pcr_tbl.txt) | [`pfl.txt`](/cmd_examples/pfl.txt) | [`o_p2pcr_tbl.txt`](/cmd_examples/o_p2pcr_tbl.txt) | `-i i_p2pcr_tbl.txt -t pfl.txt -o o_p2pcr_tbl.txt -mode P2P -dbg` |
| [`i_areatls.txt`](/cmd_examples/i_areatls.txt) |         | [`o_areatls.txt`](/cmd_examples/o_areatls.txt) | `-i i_areatls.txt -o o_areatls.txt -mode AREA -dbg` |
| [`i_areacr.txt`](/cmd_examples/i_areacr.txt) |          | [`o_areacr.txt`](/cmd_examples/o_areacr.txt) | `-i i_areacr.txt -o o_areacr.txt -mode AREA -dbg` |
| [`i_areacr_tbl.txt`](/cmd_examples/i_areacr_tbl.txt) |      | [`o_areacr_tbl.txt`](/cmd_examples/o_areacr_tbl.txt) | `-i i_areacr_tbl.txt -o o_areacr_tbl.txt -mode AREA -dbg` |

## Command-line Driver Errors ##

In addition to the ITM DLL [errors](ERRORS_AND_WARNINGS.md), the ITM command-line driver defines the following list of return codes.

### General Errors ###

| Value | Const Name                        | Description  |
| ------|-----------------------------------|--------------|
|     0 | `SUCCESS`                         | Successful execution |
|  1001 | `DRVRERR__DLL_LOADING`            | Failed to load itm.dll |
|  1002 | `DRVRERR__MAJOR_VERSION_MISMATCH` | The major version number of the ITM driver and DLL do not match |
|  1003 | `DRVRERR__INVALID_OPTION`         | Unknown option specified |
|  1004 | `DRVRERR__GET_P2P_TLS_FUNC_LOADING` | Failed to load point-to-point TLS function |
|  1005 | `DRVRERR__GET_P2P_CR_FUNC_LOADING` | Failed to load point-to-point CR function |
|  1006 | `DRVRERR__GET_AREA_TLS_FUNC_LOADING` | Failed to load area TLS function |
|  1007 | `DRVRERR__GET_AREA_CR_FUNC_LOADING` | Failed to load area CR function |
|  1008 | `DRVRERR__PARSE_MODE_VALUE`       | Invalid value for `-mode` argument |

### Input File Parsing Errors ###

| Value | Const Name                        | Description  |
| ------|-----------------------------------|--------------|
|  1101 | `DRVRERR__PARSE_HTX`              | Unable to parse transmitter height |
|  1102 | `DRVRERR__PARSE_HRX`              | Unable to parse receiver height |
|  1103 | `DRVRERR__PARSE_CLIMATE`          | Unable to parse climate |
|  1104 | `DRVRERR__PARSE_N0`               | Unable to parse refractivity |
|  1105 | `DRVRERR__PARSE_FREQ`             | Unable to parse frequency |
|  1106 | `DRVRERR__PARSE_POL`              | Unable to parse polarization |
|  1107 | `DRVRERR__PARSE_EPSILON`          | Unable to parse relative permittivity |
|  1108 | `DRVRERR__PARSE_SIGMA`            | Unable to parse conductivity |
|  1109 | `DRVRERR__PARSE_MDVAR`            | Unable to parse mode of variability |
|  1110 | `DRVRERR__PARSE_TIME`             | Unable to parse time |
|  1111 | `DRVRERR__PARSE_LOCATION`         | Unable to parse location |
|  1112 | `DRVRERR__PARSE_SITUATION`        | Unable to parse situation |
|  1113 | `DRVRERR__PARSE_CONFIDENCE`       | Unable to parse confidence |
|  1114 | `DRVRERR__PARSE_RELIABILITY`      | Unable to parse reliability |
|  1115 | `DRVRERR__PARSE_TX_SITE`          | Unable to parse transmitter siting criteria |
|  1116 | `DRVRERR__PARSE_RX_SITE`          | Unable to parse receiver siting criteria |
|  1117 | `DRVRERR__PARSE_DKM`              | Unable to parse path distance |
|  1118 | `DRVRERR__PARSE_DELTAH`           | Unable to parse terrain irregularity parameter |
|  1119 | `DRVRERR__PARSE_INCOMPLETE_DISTANCES` | Invalid specification of multiple distance values |

### Validation Errors ###

Driver validation errors occur when required input parameters are missing.  The validation of the parameters values does not occur until _after_ they are passed to ITM.

| Value | Const Name                        | Description  |
| ------|-----------------------------------|--------------|
|  1201 | `DRVRERR__VALIDATION_MODE`        | Prediction mode is not specified |
|  1202 | `DRVRERR__VALIDATION_IN_FILE`     | Input parameter file is not specified |
|  1203 | `DRVRERR__VALIDATION_OUT_FILE`    | Output file is not specified |
|  1204 | `DRVRERR__VALIDATION_TERRAIN_FILE` | Terrain file is not specified |
|  1205 | `DRVRERR__VALIDATION_HTX`         | Transmitter height (`h_tx__meter`) is required but missing from input parameter file |
|  1206 | `DRVRERR__VALIDATION_HRX`         | Receiver height (`h_rx__meter`) is required but missing from input parameter file |
|  1207 | `DRVRERR__VALIDATION_CLIMATE`     | Radio climate (`climate`) is required but missing from input parameter file |
|  1208 | `DRVRERR__VALIDATION_N0`          | Refractivity (`N_0`) is required but missing from input parameter file |
|  1209 | `DRVRERR__VALIDATION_FMHZ`        | Frequency (`f__mhz`) is required but missing from input parameter file |
|  1210 | `DRVRERR__VALIDATION_POL`         | Polarization (`pol`) is required but missing from input parameter file |
|  1211 | `DRVRERR__VALIDATION_EPSILON`     | Relative permittivity (`epsilon`) is required but missing from input parameter file |
|  1212 | `DRVRERR__VALIDATION_SIGMA`       | Conductivity (`sigma`) is required but missing from input parameter file |
|  1213 | `DRVRERR__VALIDATION_MDVAR`       | Mode of variability (`mdvar`) is required but missing from input parameter file |
|  1214 | `DRVRERR__VALIDATION_TLS_AND_CR`  | Provided both time/location/situation parameters and confidence/reliability |
|  1215 | `DRVRERR__VALIDATION_TLS_OR_CR`   | Must provide time/location/situation parameters or confidence/reliability |
|  1216 | `DRVRERR__VALIDATION_HTX_SITE`    | Transmitter siting criteria (`tx_siting_criteria`) is required but missing from input parameter file |
|  1217 | `DRVRERR__VALIDATION_RTX_SITE`    | Receiver siting criteria (`rx_siting_criteria`) is required but missing from input parameter file |
|  1218 | `DRVRERR__VALIDATION_DKM`         | Path distance (`d__km`) is required but missing from input parameter file |
|  1219 | `DRVRERR__VALIDATION_DELTAH`      | Terrain irregularity parameter (`delta_h`) is required but missing from input parameter file |
