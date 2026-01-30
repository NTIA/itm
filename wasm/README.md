# ITM WebAssembly Build

This is the ITM (Irregular Terrain Model) library compiled to WebAssembly using Emscripten with Embind bindings.

## Files

- `itm.js` - Emscripten-generated JavaScript loader with Embind bindings
- `itm.wasm` - Compiled WebAssembly binary
- `demo.html` - Interactive browser demo
- `test-node.mjs` - Node.js test script
- `package.json` - npm package configuration

## Quick Start

### Browser Usage

```html
<script type="module">
  import ITM from './itm.js';
  
  const itm = await ITM();
  
  // Calculate area mode propagation loss
  const result = itm.ITM_AREA_TLS(
    30,    // TX height (m)
    3,     // RX height (m)
    0, 0,  // siting criteria
    50,    // distance (km)
    90,    // terrain irregularity (m)
    5,     // climate
    301,   // surface refractivity
    1000,  // frequency (MHz)
    1,     // polarization (vertical)
    15,    // epsilon
    0.005, // sigma
    3,     // mode variability
    50, 50, 50  // time, location, situation (%)
  );
  
  console.log('Loss:', result.A__db, 'dB');
</script>
```

### Node.js Usage

```javascript
import ITM from './itm.js';

const itm = await ITM();

const result = itm.ITM_AREA_TLS(
  30, 3, 0, 0, 50, 90, 5, 301, 1000, 1, 15, 0.005, 3, 50, 50, 50
);

console.log('Basic Transmission Loss:', result.A__db, 'dB');
console.log('Free Space Loss:', result.A_fs__db, 'dB');
console.log('Mode:', result.mode); // 1=LoS, 2=Diffraction, 3=Troposcatter
```

## Available Functions

The module exposes the following ITM functions through Embind:

### Area Prediction Mode

- **`ITM_AREA_TLS(...)`** - Area mode with time/location/situation variability
- **`ITM_AREA_CR(...)`** - Area mode with confidence/reliability

### Point-to-Point Mode

- **`ITM_P2P_TLS(...)`** - Point-to-point with time/location/situation
- **`ITM_P2P_CR(...)`** - Point-to-point with confidence/reliability

### Return Values

All main functions return an object with:
- `A__db` - Basic transmission loss (dB)
- `A_fs__db` - Free space loss (dB) 
- `d__km` - Path distance (km)
- `theta_hzn` - Horizon angles (array)
- `d_hzn__meter` - Horizon distances (array)
- `h_e__meter` - Effective heights (array)
- `delta_h__meter` - Terrain irregularity (m)
- `N_s` - Surface refractivity
- `A_ref__db` - Reference attenuation (dB)
- `mode` - Propagation mode (1=LoS, 2=Diffraction, 3=Troposcatter)
- `warnings` - Warning flags

## Interactive Demo

Open `demo.html` in a web browser (must be served via HTTP/HTTPS):

```bash
# Python
python3 -m http.server 8000

# Node.js
npx serve .

# Then visit: http://localhost:8000/demo.html
```

## Testing

Run the test suite:
```bash
node test-node.mjs
```

Expected output shows propagation loss calculations for various scenarios.

## Input Parameters

### Common Parameters (all modes)

- `h_tx__meter` - TX structural height (0.5-3000 m)
- `h_rx__meter` - RX structural height (0.5-3000 m)
- `climate` - Radio climate (1-7)
  - 1 = Equatorial
  - 2 = Continental Subtropical
  - 3 = Maritime Subtropical
  - 4 = Desert
  - 5 = Continental Temperate
  - 6 = Maritime Temperate Over Land
  - 7 = Maritime Temperate Over Sea
- `N_0` - Surface refractivity (250-400 N-Units)
- `f__mhz` - Frequency (20-20000 MHz)
- `pol` - Polarization (0=Horizontal, 1=Vertical)
- `epsilon` - Relative permittivity (>1)
- `sigma` - Conductivity (>0 S/m)
- `mdvar` - Mode of variability (0-3, +10 to eliminate location, +20 for situation)

### Area Mode Specific

- `d__km` - Path distance (km)
- `delta_h__meter` - Terrain irregularity (m)
- `tx_siting_criteria` - TX siting (0=Random, 1=Careful, 2=Very Careful)
- `rx_siting_criteria` - RX siting (0=Random, 1=Careful, 2=Very Careful)

### Point-to-Point Mode Specific

- `pfl` - Terrain profile array:
  - `[0]` = number of elevation points - 1
  - `[1]` = resolution in meters
  - `[i]` = elevation in meters

## Performance

- **Binary Size:** ~200-300 KB (uncompressed)
- **Gzipped:** ~80-120 KB
- **Load Time:** <200ms on modern connections
- **Execution:** Near-native C++ performance

## Browser Compatibility

- ✓ Chrome 57+
- ✓ Firefox 52+
- ✓ Safari 11+
- ✓ Edge 16+

## Node.js Compatibility

- Node.js 12+ with ES module support
- Use `.mjs` extension or `"type": "module"` in package.json

## Documentation

For complete ITM documentation, see:
- [Main README](../README.md)
- [Error Codes & Warnings](../ERRORS_AND_WARNINGS.md)
- [Command-line Tool](../cmdREADME.md)

## License

See the main repository LICENSE file.

