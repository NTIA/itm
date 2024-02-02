# ITS Irregular Terrain Model (ITM) #

This code repository contains the ITS Irregular Terrain Model (ITM). ITM predicts terrestrial radiowave propagation for frequencies between 20 MHz and 20 GHz based on electromagnetic theory and empirical models developed by Anita Longley and Phil Rice. Propagation mechanisms considered include free space loss, diffraction, and troposcatter. Specifically, ITM predicts attenuation as a function of distance (greater than 1 km), terminal heights, meteorological conditions, terrain effects, and the variability of the signal in time and in space.

**Note**: Version 1.3 of this code base is functionally identical to version 1.2.2 of the FORTRAN source, which has been archived [here](https://github.com/NTIA/itm-longley-rice).  ITS plans to apply all future ITM updates to this C++ code base.

## Quick Start ##

Users of ITM have two options to immediately begin using ITM:

1. The ITM software is made available to the user as a precompiled DLL in the [Release](https://github.com/NTIA/itm/releases) page.  A user can add this DLL to their own software project as a dependency and call ITM through its function definitions.  Additionally, a C#/.NET software wrapper is provided.  Distribution and updates of this wrapper code are provided through the published [NuGet package](https://github.com/NTIA/itm/packages).

2. A supporting command-line driver is also included in this repository, with its own corresponding [readme](cmdREADME.md).  This tool allows a user to call ITM without any programming required through the use of ASCII input/output files.

A select set of example inputs and outputs are provided for testing purposes.  This is not a comprehensive validation test set.  For ITM in Point-to-Point Prediction Mode, [p2p.csv](p2p.csv) defines the set of inputs and outputs with [pfls.csv](pfls.csv) containing the corresponding set of terrain profiles.  For Area Prediction Mode, inputs and outputs are defined in [area.csv](area.csv). 

## Inputs ##

ITM can be called in either Area Prediction Mode or Point-to-Point Prediction Mode.  Additionally, variabilities can be defined by either time/location/situation or confidence/reliability.  The below inputs are organized accordingly.

### Common Inputs ###

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `h_tx__meter`     | double | meter | 0.5 <= `h_tx__meter` <= 3000 | Structural height of the TX |
| `h_rx__meter`     | double | meter | 0.5 <= `h_rx__meter` <= 3000 | Structural height of the RX |
| `climate`         | int    |       | enum         | Radio climate of the path  <ul><li>1 = Equatorial</li><li>2 = Continental  Subtropical</li><li>3 = Maritime Subtropical</li><li>4 = Desert</li><li>5 = Continental Temperate</li><li>6 = Maritime Temperate Over Land</li><li>7 = Maritime Temperate Over Sea</li></ul>|
| `N_0`             | double | N-Units | 250 <= `N_0` <= 400 | Minimum monthly mean surface refractivity reduced to sea level |
| `f__mhz`          | double | MHz   | 20 <= `f__mhz` <= 20000 | Frequency |
| `pol`             | int    |       | enum          | Polarization  <ul><li>0 = Horizontal</li><li>1 = Vertical</li></ul> |
| `epsilon`         | double |       | 1 < `epsilon` | Relative permittivity |
| `sigma`           | double | S/m   | 0 < `sigma`   | Conductivity |
| `mdvar`           | int    |       | enum          | Mode of variability <ul><li>0 = Single Message Mode</li><li>1 = Accidental Mode</il><li>2 = Mobile Mode</li><li>3 = Broadcast Mode</li></ul> Additionally, +10 if location variability is to be eliminated, and +20 if direct situation variability is to be eliminated |

### Point-to-Point Mode Specific Inputs ###

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `pfl`             | double[] |     |              | Terrain profile data in PFL format, from TX to RX <ul><li>`pfl[0]` : Number of elevation points - 1</li><li>`pfl[1]` : Resolution, in meters</li><li>`pfl[i]` : Elevation above sea level, in meters</li></ul> |

### Area Mode Inputs ###

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `d__km`           | double | km    | 0 < `d__km`  | Path distance |
| `delta_h__meter`  | double | meter | 0 <= `delta_h__meter` | Terrain irregularity parameter |
| `tx_siting_criteria` | int |       | enum         | Siting criteria of TX <ul><li>0 = Random</li><li>1 = Careful</li><li>2 = Very Careful</li></ul>|
| `rx_siting_criteria` | int |       | enum         | Siting criteria of RX <ul><li>0 = Random</li><li>1 = Careful</li><li>2 = Very Careful</li></ul>|

### Variability Inputs ###

A user either supplies values for time/location/situation or confidence/reliability.

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `time`            | double |       | 0 < `time` < 100 | Time variability |
| `location`        | double |       | 0 < `location` < 100 | Location variability |
| `situation`       | double |       | 0 < `situation` < 100 | Situation variability |
| `confidence`      | double |       | 0 < `confidence` < 100 | Confidence variability |
| `reliability`     | double |       | 0 < `reliability` < 100 | Reliability variability |

## Outputs ##

| Variable      | Type   | Units | Description |
|---------------|--------|-------|-------------|
| `A__db`       | double | dB    | Basic transmission loss |
| `warnings`    | int    |       | Warning flags |

## Intermediate Values ##

Internal intermediate values can be extracted from ITM via functions that are suffixed with `_Ex`.  These functions will populate the `IntermediateValues` data structure with intermediate values from the ITM calculations.

| Variable         | Type      | Units       | Description |
|------------------|-----------|-------------|-------------|
| `theta_hzn`      | double[]  | radians     | Terminal horizon angles |
| `d_hzn__meter`   | double[]  | meter       | Terminal horizon distances |
| `h_e__meter`     | double[]  | meter       | Effective terminal heights |
| `N_s`            | double    | N-Units     | Surface refractivity |
| `delta_h__meter` | double    | meter       | Terrain irregularity parameter |
| `A_ref__db`      | double    | dB          | Reference attenuation |
| `A_fs__db`       | double    | dB          | Free space basic transmission loss |
| `d__km`          | double    | km          | Path distance |
| `mode`           | int       |             | Mode of propagation <ul><li>1 = Line of Sight</li><li>2 = Diffraction</li><li>3 = Troposcatter</li></ul>|

## Error Codes and Warning Flags ##

ITM supports a defined list of error codes and warning flags.  A complete list can be found [here](ERRORS_AND_WARNINGS.md).

## Notes on Code Style ##

* In general, variables follow the naming convention in which a single underscore denotes a subscript (pseudo-LaTeX format), and where a double underscore is followed by the units, i.e. h_tx__meter.
* Variables are named to match their corresponding mathematical variables in the underlying references.
* Wherever possible, equation numbers and source documentation are provided.

## Configure and Build ##

### C++ Software ###

The software is designed to be built into a DLL (or corresponding library for non-Windows systems).  The source code can be built for any OS that supports the standard C++ libraries.  A Visual Studio 2019 project file is provided for Windows users to support the build process and configuration.

### C#/.NET Wrapper Software ###

The .NET support of ITM consists of a simple pass-through wrapper around the native DLL.  It is compiled to target .NET Framework 4.7.2.

## References ##

* G.A. Hufford, A.G. Longley, W.A. Kissick, [A Guide to the Use of the ITS Irregular Terrain Model in the Area Prediction Mode](https://www.its.bldrdoc.gov/publications/details.aspx?pub=2091), NTIA Technical Report TR-82-100, April 1982.
* G.A. Hufford, [The ITS Irregular Terrain Model, version 1.2.2 Algorithm](https://www.its.bldrdoc.gov/media/50676/itm_alg.pdf).
* G.A. Hufford, [1985 ITM Memo](https://www.its.bldrdoc.gov/media/50675/Hufford_1985_Memo.pdf), Dr. George Hufford's 1985 Memo describing the changes to ITM version 1.2.1 (dated April, 1979) in ITM version 1.2.2 (dated September, 1984)
* G.A. Hufford, [The Irregular Terrain Model](https://www.its.bldrdoc.gov/media/50674/itm.pdf), The "definitive" representation of the ITS Irregular Terrain Model. It contains both the source code and rather extensive documentation.
* A.G. Longley and P.L. Rice, [Prediction of Tropospheric Radio Transmission Loss Over Irregular Terrain: A Computer Method - 1968](https://www.its.bldrdoc.gov/publications/details.aspx?pub=2784), NTIA Technical Report ERL 79-ITS 67, July 1968.

## Contact ##

For technical questions about ITM, contact Paul McKenna, pmckenna@ntia.gov.
