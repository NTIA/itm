# ITM — Rust Implementation

Pure Rust port of the [ITS Irregular Terrain Model (ITM)](../README.md) v1.4.0.
ITM predicts terrestrial radiowave propagation for frequencies between 20 MHz and 20 GHz using
the Longley-Rice algorithm, considering free-space loss, diffraction, and troposcatter.

## Contents

| Crate | Description |
|-------|-------------|
| [`itm`](itm/) | Library — call from your Rust code |
| [`itm-cli`](itm-cli/) | CLI binary — use without writing code |

---

## Quick Start

### Build

```sh
cargo build --release
```

Both crates are in a workspace, so this produces:
- `target/release/libITM.rlib` (or `.dll`/`.so` on the relevant platform)
- `target/release/itm-cli`

### Run the CLI

**Point-to-point (terrain profile):**

```sh
itm-cli p2p \
  --h-tx 15 --h-rx 3 \
  --pfl terrain.txt \
  --climate 5 --n0 301 --freq 3500 --pol 1 \
  --epsilon 15 --sigma 0.005 \
  --mdvar 12 --time 50 --location 50 --situation 50
```

**Area prediction (no terrain file):**

```sh
itm-cli area \
  --h-tx 10 --h-rx 1 \
  --tx-siting 0 --rx-siting 0 \
  --distance 16 --delta-h 0 \
  --climate 5 --n0 301 --freq 230 --pol 0 \
  --epsilon 15 --sigma 0.008 \
  --mdvar 0 --time 87 --location 50 --situation 50
```

**Example output:**

```
Basic Transmission Loss: 142.37 dB
Return code: 0
--- Intermediate Values ---
  Path distance:        16.000 km
  Free-space loss:      107.75 dB
  Reference atten:      34.62 dB
  Delta H:              0.00 m
  Surface refractivity: 301.0 N-units
  Eff. TX height:       10.000 m
  Eff. RX height:        1.000 m
  TX horizon dist:    1234.5 m
  RX horizon dist:     456.7 m
  Propagation mode:     Diffraction
```

---

## Library Usage

Add to `Cargo.toml`:

```toml
[dependencies]
itm = { path = "path/to/itm-rs/itm" }
```

### Point-to-Point

```rust
use itm;

// Build the terrain profile array in PFL format:
//   pfl[0] = N (number of points minus 1)
//   pfl[1] = dR (spacing in meters)
//   pfl[2..=N+2] = elevation above MSL in meters
let mut pfl = vec![0.0_f64; 102];
pfl[0] = 100.0;   // 101 points
pfl[1] = 300.0;   // 300 m spacing → 30 km path

let (rtn, a__db, warnings) = itm::itm_p2p_tls(
    15.0,   // h_tx__meter
    3.0,    // h_rx__meter
    &pfl,
    5,      // climate: Continental Temperate
    301.0,  // n_0: surface refractivity (N-units)
    3500.0, // f__mhz
    1,      // pol: vertical
    15.0,   // epsilon
    0.005,  // sigma (S/m)
    12,     // mdvar
    50.0,   // time %
    50.0,   // location %
    50.0,   // situation %
);

assert_eq!(rtn, itm::errors::SUCCESS);
println!("Loss: {a__db:.2} dB  warnings: 0x{warnings:04X}");
```

To also retrieve intermediate values, call `itm_p2p_tls_ex`:

```rust
let (rtn, a__db, warnings, iv) = itm::itm_p2p_tls_ex(/* same args */);
println!("Mode: {}", match iv.mode { 1 => "LOS", 2 => "Diffraction", _ => "Troposcatter" });
println!("Δh: {:.1} m", iv.delta_h__meter);
```

### Area Prediction

```rust
let (rtn, a__db, warnings) = itm::itm_area_tls(
    10.0,   // h_tx__meter
    1.0,    // h_rx__meter
    0,      // tx_siting_criteria: Random
    0,      // rx_siting_criteria: Random
    16.0,   // d__km
    0.0,    // delta_h__meter
    5,      // climate
    301.0,  // n_0
    230.0,  // f__mhz
    0,      // pol: horizontal
    15.0,   // epsilon
    0.008,  // sigma
    0,      // mdvar
    87.0,   // time %
    50.0,   // location %
    50.0,   // situation %
);
```

### Confidence/Reliability Variant

Prefer `_cr` functions when you want to specify confidence and reliability
instead of time/location/situation:

```rust
let (rtn, a__db, warnings) = itm::itm_p2p_cr(
    h_tx, h_rx, &pfl, climate, n_0, f__mhz, pol, epsilon, sigma,
    mdvar,
    50.0, // confidence %
    90.0, // reliability %
);
```

Extended (`_ex`) versions exist for all four variants:

| Function | Variability | Extras |
|---|---|---|
| `itm_p2p_tls` | time / location / situation | — |
| `itm_p2p_tls_ex` | time / location / situation | `IntermediateValues` |
| `itm_p2p_cr` | confidence / reliability | — |
| `itm_p2p_cr_ex` | confidence / reliability | `IntermediateValues` |
| `itm_area_tls` | time / location / situation | — |
| `itm_area_tls_ex` | time / location / situation | `IntermediateValues` |
| `itm_area_cr` | confidence / reliability | — |
| `itm_area_cr_ex` | confidence / reliability | `IntermediateValues` |

---

## Input Reference

### Common Inputs

| Parameter | Type | Units | Limits | Description |
|-----------|------|-------|--------|-------------|
| `h_tx__meter` | `f64` | m | 0.5 – 3000 | TX structural height |
| `h_rx__meter` | `f64` | m | 0.5 – 3000 | RX structural height |
| `climate` | `i32` | — | 1 – 7 | Radio climate (see table below) |
| `n_0` | `f64` | N-Units | 250 – 400 | Min monthly mean surface refractivity |
| `f__mhz` | `f64` | MHz | 20 – 20 000 | Frequency |
| `pol` | `i32` | — | 0, 1 | Polarization: 0 = horizontal, 1 = vertical |
| `epsilon` | `f64` | — | > 1 | Relative permittivity of ground |
| `sigma` | `f64` | S/m | > 0 | Ground conductivity |
| `mdvar` | `i32` | — | 0 – 33 | Mode of variability (see table below) |

**Radio climates:**

| Value | Enum variant | Description |
|-------|--------------|-------------|
| 1 | `Climate::Equatorial` | Equatorial |
| 2 | `Climate::ContinentalSubtropical` | Continental Subtropical |
| 3 | `Climate::MaritimeSubtropical` | Maritime Subtropical |
| 4 | `Climate::Desert` | Desert |
| 5 | `Climate::ContinentalTemperate` | Continental Temperate |
| 6 | `Climate::MaritimeTemperateOverLand` | Maritime Temperate (Over Land) |
| 7 | `Climate::MaritimeTemperateOverSea` | Maritime Temperate (Over Sea) |

**Modes of variability (`mdvar`):**

| Value | Description |
|-------|-------------|
| 0 | Single Message |
| 1 | Accidental |
| 2 | Mobile |
| 3 | Broadcast |
| +10 | Eliminate location variability |
| +20 | Eliminate direct situation variability |

### Point-to-Point Inputs

| Parameter | Type | Description |
|-----------|------|-------------|
| `pfl` | `&[f64]` | Terrain profile: `[N, dR, h₀, h₁, …, hₙ]` |

`pfl[0]` = number of terrain points minus 1 (N).  
`pfl[1]` = horizontal spacing in meters (dR).  
`pfl[2]` through `pfl[N+2]` = elevation above MSL in meters (N+1 values total).

### Area Mode Inputs

| Parameter | Type | Units | Description |
|-----------|------|-------|-------------|
| `d__km` | `f64` | km | Path distance (> 0) |
| `delta_h__meter` | `f64` | m | Terrain irregularity parameter (≥ 0) |
| `tx_siting_criteria` | `i32` | — | 0 = Random, 1 = Careful, 2 = Very Careful |
| `rx_siting_criteria` | `i32` | — | 0 = Random, 1 = Careful, 2 = Very Careful |

### Variability Inputs

| Parameter | Type | Limits | Description |
|-----------|------|--------|-------------|
| `time` | `f64` | 0 < x < 100 | Time percentage |
| `location` | `f64` | 0 < x < 100 | Location percentage |
| `situation` | `f64` | 0 < x < 100 | Situation percentage |
| `confidence` | `f64` | 0 < x < 100 | Confidence percentage (`_cr` functions) |
| `reliability` | `f64` | 0 < x < 100 | Reliability percentage (`_cr` functions) |

---

## Outputs

All functions return a tuple `(rtn: i32, a__db: f64, warnings: i32)`.  
`_ex` variants append `IntermediateValues` as a fourth element.

| Field | Units | Description |
|-------|-------|-------------|
| `rtn` | — | Return/error code (0 = success) |
| `a__db` | dB | Basic transmission loss |
| `warnings` | — | Warning bitmask (see below) |

### `IntermediateValues`

```rust
pub struct IntermediateValues {
    pub theta_hzn: [f64; 2],     // horizon angles (radians)
    pub d_hzn__meter: [f64; 2],  // horizon distances (m)
    pub h_e__meter: [f64; 2],    // effective heights (m)
    pub n_s: f64,                 // surface refractivity (N-Units)
    pub delta_h__meter: f64,      // terrain irregularity (m)
    pub a_ref__db: f64,           // reference attenuation (dB)
    pub a_fs__db: f64,            // free-space loss (dB)
    pub d__km: f64,               // path distance (km)
    pub mode: i32,                // 1=LOS, 2=Diffraction, 3=Troposcatter
}
```

---

## Error Codes and Warnings

### Error Codes (`itm::errors::*`)

| Constant | Value | Meaning |
|----------|-------|---------|
| `SUCCESS` | 0 | No error |
| `SUCCESS_WITH_WARNINGS` | 1 | Completed with warnings |
| `ERROR__TX_TERMINAL_HEIGHT` | 1000 | TX height out of range |
| `ERROR__RX_TERMINAL_HEIGHT` | 1001 | RX height out of range |
| `ERROR__INVALID_RADIO_CLIMATE` | 1002 | Climate not 1–7 |
| `ERROR__INVALID_TIME` | 1003 | Time % not in (0, 100) |
| `ERROR__INVALID_LOCATION` | 1004 | Location % not in (0, 100) |
| `ERROR__INVALID_SITUATION` | 1005 | Situation % not in (0, 100) |
| `ERROR__INVALID_CONFIDENCE` | 1006 | Confidence % not in (0, 100) |
| `ERROR__INVALID_RELIABILITY` | 1007 | Reliability % not in (0, 100) |
| `ERROR__REFRACTIVITY` | 1008 | N₀ not in [250, 400] |
| `ERROR__FREQUENCY` | 1009 | Frequency not in [20, 20 000] MHz |
| `ERROR__POLARIZATION` | 1010 | Polarization not 0 or 1 |
| `ERROR__EPSILON` | 1011 | Relative permittivity ≤ 1 |
| `ERROR__SIGMA` | 1012 | Conductivity ≤ 0 |
| `ERROR__GROUND_IMPEDANCE` | 1013 | Ground impedance invalid |
| `ERROR__MDVAR` | 1014 | mdvar not in [0, 33] |
| `ERROR__EFFECTIVE_EARTH` | 1016 | Effective earth radius error |
| `ERROR__PATH_DISTANCE` | 1017 | Path distance ≤ 0 |
| `ERROR__DELTA_H` | 1018 | delta_h < 0 |
| `ERROR__TX_SITING_CRITERIA` | 1019 | TX siting not 0/1/2 |
| `ERROR__RX_SITING_CRITERIA` | 1020 | RX siting not 0/1/2 |
| `ERROR__SURFACE_REFRACTIVITY_SMALL` | 1021 | Computed surface refractivity too small |
| `ERROR__SURFACE_REFRACTIVITY_LARGE` | 1022 | Computed surface refractivity too large |

### Warning Flags (`itm::warnings::*`)

Warnings are a bitmask — multiple flags may be set simultaneously.
Check with `warnings & itm::warnings::WARN_<NAME> != 0`.

| Bit | Meaning |
|-----|---------|
| `0x0001` | TX terminal height out of recommended range |
| `0x0002` | RX terminal height out of recommended range |
| `0x0004` | Frequency out of recommended range |
| `0x0008` | Path distance > 1000 km |
| `0x0010` | Path distance > 2000 km |
| `0x0020` | Path distance below minimum for antenna heights |
| `0x0040` | Path distance < 1 km |
| `0x0080` | TX horizon angle > 200 mrad |
| `0x0100` | RX horizon angle > 200 mrad |
| `0x0200` | TX horizon distance < 10% of smooth-earth value |
| `0x0400` | RX horizon distance < 10% of smooth-earth value |
| `0x0800` | TX horizon distance > 3× smooth-earth value |
| `0x1000` | RX horizon distance > 3× smooth-earth value |
| `0x2000` | Extreme variabilities |
| `0x4000` | Surface refractivity out of recommended range |

---

## Terrain File Format (CLI)

The `--pfl` file is plain text, whitespace-separated:

```
N dR h0 h1 h2 ... hN
```

- `N` — number of terrain points **minus 1** (integer)
- `dR` — horizontal spacing in meters (float)
- `h0` … `hN` — terrain elevations above MSL in meters (**N+1 values**)

The file must contain exactly `N + 2` numeric values.

**Example** — 5-point profile, 1000 m spacing:

```
4 1000 100.0 120.5 98.3 110.0 105.7
```

---

## Migrating from C++

The Rust API is a direct translation of the C++ DLL interface.
Parameter names and semantics are identical; only the calling convention changes.

### C++ → Rust mapping

| C++ | Rust |
|-----|------|
| `ITM_P2P_TLS(h_tx, h_rx, pfl, ...)` | `itm::itm_p2p_tls(h_tx, h_rx, &pfl, ...)` |
| `ITM_P2P_TLS_Ex(h_tx, h_rx, pfl, ..., &iv)` | `itm::itm_p2p_tls_ex(h_tx, h_rx, &pfl, ...)` → returns `(rtn, a__db, warn, iv)` |
| `ITM_AREA_TLS(...)` | `itm::itm_area_tls(...)` |
| `ITM_AREA_TLS_Ex(..., &iv)` | `itm::itm_area_tls_ex(...)` → returns `(rtn, a__db, warn, iv)` |

Key differences:
- **No output parameters** — Rust functions return tuples instead of mutating `&A__db`, `&warnings`, `&iv`.
- **`pfl` is a slice** — pass `&pfl` rather than a raw pointer; the library reads `pfl[0]` as N internally.
- **`IntermediateValues` is returned by value** — no need to pre-allocate and pass a pointer.
- **Error codes are `i32` constants** in `itm::errors`, not a C enum.

### Before (C++)

```cpp
double A__db;
int warnings;
IntermediateValues iv;

int rtn = ITM_P2P_TLS_Ex(
    h_tx, h_rx, pfl, climate, N_0, f__mhz, pol,
    epsilon, sigma, mdvar, time, location, situation,
    &A__db, &warnings, &iv);
```

### After (Rust)

```rust
let (rtn, a__db, warnings, iv) = itm::itm_p2p_tls_ex(
    h_tx, h_rx, &pfl, climate, n_0, f__mhz, pol,
    epsilon, sigma, mdvar, time, location, situation,
);
```

---

## Migrating from .NET / NuGet

The C# wrapper called the native DLL with `ref` output parameters.
The Rust library returns a plain tuple — no `ref`, no `out`.

### Before (C#)

```csharp
double A_db;
int warnings;
IntermediateValues iv;

int rtn = ITM.ITM_P2P_TLS_Ex(
    h_tx, h_rx, pfl, climate, N_0, f_mhz, pol,
    epsilon, sigma, mdvar, time, location, situation,
    out A_db, out warnings, out iv);
```

### After (Rust, called from C# via FFI or replaced entirely)

If replacing with pure Rust:

```rust
let (rtn, a__db, warnings, iv) = itm::itm_p2p_tls_ex(/* same numeric args */);
```

If you need to expose the Rust library back to C# as a DLL, wrap the functions with
`#[no_mangle] pub extern "C"` signatures that match the original DLL exports.

---

## Running Tests

```sh
cargo test
```

The test suite (`itm/src/tests.rs`) covers five P2P and five area scenarios across
the 230 MHz – 8.9 GHz range, verifying both successful completion and loss values
within ±1 dB of reference outputs.

---

## References

- G.A. Hufford, A.G. Longley, W.A. Kissick, [A Guide to the Use of the ITS Irregular Terrain Model in the Area Prediction Mode](https://www.its.bldrdoc.gov/publications/details.aspx?pub=2091), NTIA TR-82-100, 1982.
- G.A. Hufford, [The ITS Irregular Terrain Model, version 1.2.2 Algorithm](https://www.its.bldrdoc.gov/media/50676/itm_alg.pdf).
- A.G. Longley and P.L. Rice, [Prediction of Tropospheric Radio Transmission Loss Over Irregular Terrain](https://www.its.bldrdoc.gov/publications/details.aspx?pub=2784), NTIA ERL 79-ITS 67, 1968.
