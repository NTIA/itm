# Irregular Terrain Model (ITM) #

[![NTIA/ITS PropLib][proplib-badge]][proplib-link]
[![GitHub Release][gh-releases-badge]][gh-releases-link]
[![GitHub Actions Unit Test Status][gh-actions-test-badge]][gh-actions-test-link]
[![C++ API Reference][gh-actions-docs-badge]][gh-pages-docs-link]
[![GitHub Issues][gh-issues-badge]][gh-issues-link]
[![DOI][doi-badge]][doi-link]

[proplib-badge]: https://img.shields.io/badge/PropLib-badge?label=%F0%9F%87%BA%F0%9F%87%B8%20NTIA%2FITS&labelColor=162E51&color=D63E04
[proplib-link]: https://ntia.github.io/propagation-library-wiki
[gh-actions-test-badge]: https://img.shields.io/github/actions/workflow/status/NTIA/itm/ctest.yml?branch=master&logo=cmake&label=Build%2FTests&labelColor=162E51
[gh-actions-test-link]: https://github.com/NTIA/itm/actions/workflows/ctest.yml
[gh-actions-docs-badge]: https://img.shields.io/github/actions/workflow/status/NTIA/itm/doxygen.yml?branch=master&logo=c%2B%2B&label=Docs&labelColor=162E51
[gh-pages-docs-link]: https://ntia.github.io/ITM
[gh-releases-badge]: https://img.shields.io/github/v/release/NTIA/ITM?logo=github&label=Release&labelColor=162E51&color=D63E04
[gh-releases-link]: https://github.com/NTIA/ITM/releases
[gh-issues-badge]: https://img.shields.io/github/issues/NTIA/ITM?logo=github&label=Issues&labelColor=162E51
[gh-issues-link]: https://github.com/NTIA/ITM/issues
[doi-badge]: https://zenodo.org/badge/218981682.svg
[doi-link]: https://zenodo.org/badge/latestdoi/218981682

This repository contains the NTIA/ITS implementation of the Irregular Terrain Model (ITM).
ITM predicts terrestrial radiowave propagation for frequencies between 20 MHz and 20 GHz based on
electromagnetic theory and empirical models developed by Anita Longley and Phil Rice. Propagation
mechanisms considered include free space loss, diffraction, and troposcatter. Specifically, ITM
predicts attenuation as a function of distance (greater than 1 km), terminal heights, meteorological
conditions, terrain effects, and the variability of the signal in time and in space.

Additional bindings to the shared library built from this repository are provided
for .NET, MATLAB®, and Python® in the following repositories:

* [NTIA/ITM-dotnet](https://github.com/NTIA/itm-dotnet)
* [NTIA/ITM-matlab](https://github.com/NTIA/itm-matlab)
* [NTIA/ITM-python](https://github.com/NTIA/itm-python)

**Note**: Version 1.5 of this code base is functionally identical to version 1.2.2 of the FORTRAN
source, which has been archived [here](https://github.com/NTIA/itm-longley-rice). ITS plans to apply
all future ITM updates to this C++ code base.

## Getting Started ##

To get started using this library, refer to
[its page on the **NTIA/ITS Propagation Library Wiki**](https://ntia.github.io/propagation-library-wiki/models/ITM/).
There, you will find installation instructions, usage information, and code
examples for all supported languages.

An executable is also provided which can be used to run the functions provided
by this library using plain text input and output files. Installation and usage
details for the command-line driver are also provided on
[the wiki](https://ntia.github.io/propagation-library-wiki/models/ITM/driver).

If you're a developer and would like to contribute to or extend this repository,
you will find comprehensive documentation of this C++ code
[here](https://ntia.github.io/ITM), and a guide for contributors
[here](CONTRIBUTING.md).

<!-- TODO: Old README information is commented below. This should be migrated to the PropLib Wiki -->
<!-- 
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

-->

## Configure and Build ##

The software is designed to be built into a DLL (or corresponding `.so` or `.dylib`
library for non-Windows systems). A CMake build configuration and presets are
provided for cross-platform builds. Presets provide default sets of compiler flags,
and additional set default CMake options to control which parts of the project are
build. Below are a few examples of how this project can be built using provided presets.

```cmd
# From this repository's root directory, try one of the following command pairs:

# "Release" configurations compile the library and driver, build docs, and configure tests:
cmake --preset release64
cmake --build --preset release64

# "Debug" configurations skip building the docs, driver, and driver tests:
cmake --preset debug64
cmake --build --preset debug64

# Additional options can override presets:
cmake --preset debug64 -DBUILD_DRIVER=ON

# "DocsOnly" configurations only build the docs:
cmake --preset docsOnly
cmake --build --preset docsOnly
```

Note that this repository makes use of several
[Git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
to reference dependencies used for running unit tests and building documentation.
In order to do either, ensure the required submodules are cloned by running:

```cmd
# From this repository's root directory
git submodule init
git submodule update
```

## Running Tests ##

If you've configured tests when building the project, for example by using one of
the "Release" or "Debug" CMake presets, you can run the included unit tests as follows:

```cmd
ctest --preset release64
```

## References ##

* [ITS Propagation Library Wiki](https://ntia.github.io/propagation-library-wiki)
* [`ITS.Propagation.ITM` C++ API Reference](https://ntia.github.io/ITM)
* G.A. Hufford, A.G. Longley, W.A. Kissick, [A Guide to the Use of the ITS Irregular Terrain Model in the Area Prediction Mode](https://www.its.bldrdoc.gov/publications/details.aspx?pub=2091), NTIA Technical Report TR-82-100, April 1982.
* G.A. Hufford, [The ITS Irregular Terrain Model, version 1.2.2 Algorithm](https://www.its.bldrdoc.gov/media/50676/itm_alg.pdf).
* G.A. Hufford, [1985 ITM Memo](https://www.its.bldrdoc.gov/media/50675/Hufford_1985_Memo.pdf), Dr. George Hufford's 1985 Memo describing the changes to ITM version 1.2.1 (dated April, 1979) in ITM version 1.2.2 (dated September, 1984)
* G.A. Hufford, [The Irregular Terrain Model](https://www.its.bldrdoc.gov/media/50674/itm.pdf), The "definitive" representation of the ITS Irregular Terrain Model. It contains both the source code and rather extensive documentation.
* A.G. Longley and P.L. Rice, [Prediction of Tropospheric Radio Transmission Loss Over Irregular Terrain: A Computer Method - 1968](https://www.its.bldrdoc.gov/publications/details.aspx?pub=2784), NTIA Technical Report ERL 79-ITS 67, July 1968.

## License ##

See [`LICENSE.md`](./LICENSE.md).

MATLAB is a registered trademark of The MathWorks, Inc. See
[mathworks.com/trademarks](https://mathworks.com/trademarks) for a list of additional trademarks.

"Python" and the Python logos are trademarks or registered trademarks of the Python Software Foundation, used by the National Telecommunications and Information Administration with permission from the Foundation.

## Contact ##

For technical questions, contact <code@ntia.gov>.

## Disclaimer ##

Certain commercial equipment, instruments, or materials are identified in this project were used for the convenience of the developers. In no case does such identification imply recommendation or endorsement by the National Telecommunications and Information Administration, nor does it imply that the material or equipment identified is necessarily the best available for the purpose.
