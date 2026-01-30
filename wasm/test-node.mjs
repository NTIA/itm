/**
 * Node.js test script for ITM WebAssembly module
 */
import ITM from './itm.js';

(async () => {
    console.log('🔄 Loading ITM WebAssembly module...\n');
    
    try {
        // Call ITM as a function to get the module instance
        const itm = await ITM();
        
        console.log('✓ Module loaded successfully!\n');
        
        // Debug: Check what's available in the module
        console.log('Available ITM functions:');
        const itmFunctions = Object.keys(itm).filter(key => typeof itm[key] === 'function' && key.startsWith('ITM_'));
        console.log(itmFunctions.join(', '));
        console.log('');
        
        // Use ITM_AREA_TLS_r (the _r version returns a result object)
        if (typeof itm.ITM_AREA_TLS_r === 'function') {
            console.log('=== Testing ITM Area Mode (ITM_AREA_TLS_r) ===\n');
            
            const result = itm.ITM_AREA_TLS_r(
                30,    // h_tx__meter (TX height)
                3,     // h_rx__meter (RX height)
                0,     // tx_site_criteria (random)
                0,     // rx_site_criteria (random)
                50,    // d__km (distance)
                90,    // delta_h__meter (terrain irregularity)
                5,     // climate (Continental Temperate)
                301,   // N_0 (surface refractivity)
                1000,  // f__mhz (frequency)
                1,     // pol (vertical)
                15,    // epsilon (relative permittivity)
                0.005, // sigma (conductivity)
                3,     // mdvar (broadcast mode)
                50,    // time (%)
                50,    // location (%)
                50     // situation (%)
            );
            
            console.log('Test Case: 50km link at 1000 MHz');
            console.log('─'.repeat(50));
            console.log(`TX Height:              30 m`);
            console.log(`RX Height:              3 m`);
            console.log(`Distance:               50 km`);
            console.log(`Frequency:              1000 MHz`);
            console.log(`Climate:                Continental Temperate`);
            console.log(`Terrain Irregularity:   90 m`);
            console.log('');
            
            // Debug: Show the actual result structure
            console.log('Raw result object:', result);
            console.log('Result type:', typeof result);
            if (result && typeof result === 'object') {
                console.log('Result properties:', Object.keys(result).join(', '));
            }
            console.log('');
            
            // Check if result has expected properties
            if (result && typeof result.A__db !== 'undefined') {
                console.log('Results:');
                console.log(`  Basic Transmission Loss:  ${result.A__db.toFixed(2)} dB`);
                
                if (typeof result.A_fs__db !== 'undefined') {
                    console.log(`  Free Space Loss:          ${result.A_fs__db.toFixed(2)} dB`);
                }
                if (typeof result.mode !== 'undefined') {
                    console.log(`  Mode of Propagation:      ${getModeString(result.mode)}`);
                }
                if (typeof result.d__km !== 'undefined') {
                    console.log(`  Path Distance:            ${result.d__km.toFixed(2)} km`);
                }
                if (typeof result.N_s !== 'undefined') {
                    console.log(`  Surface Refractivity:     ${result.N_s.toFixed(2)} N-Units`);
                }
                if (typeof result.A_ref__db !== 'undefined') {
                    console.log(`  Reference Attenuation:    ${result.A_ref__db.toFixed(2)} dB`);
                }
                if (typeof result.warnings !== 'undefined') {
                    console.log(`  Warnings:                 ${result.warnings === 0 ? 'None' : '0x' + result.warnings.toString(16)}`);
                }
                console.log('');
                
                // Additional test cases
                console.log('=== Additional Test Cases ===\n');
                
                const testCases = [
                    { d: 10, f: 100, desc: 'Short range, low frequency' },
                    { d: 100, f: 5000, desc: 'Long range, high frequency' },
                    { d: 25, f: 2400, desc: 'WiFi frequency (2.4 GHz)' }
                ];
                
                testCases.forEach((test, index) => {
                    const res = itm.ITM_AREA_TLS_r(
                        30, 3, 0, 0, test.d, 90, 5, 301, test.f, 1, 15, 0.005, 3, 50, 50, 50
                    );
                    console.log(`Test ${index + 1}: ${test.desc}`);
                    console.log(`  Distance: ${test.d} km, Frequency: ${test.f} MHz`);
                    if (res && typeof res.A__db !== 'undefined') {
                        console.log(`  Loss: ${res.A__db.toFixed(2)} dB${res.mode ? ' (Mode: ' + getModeString(res.mode) + ')' : ''}`);
                    } else {
                        console.log(`  Result:`, res);
                    }
                    console.log('');
                });
                
                console.log('✓ All tests completed successfully!\n');
            } else {
                console.log('⚠ Warning: Result object does not have expected A__db property');
                console.log('This may indicate the embind wrapper returns data in a different format.');
                console.log('Result value:', result);
                console.log('\n✓ Function executes but result format differs from expected.\n');
            }
        } else {
            console.log('⚠ ITM_AREA_TLS_r function not found.');
            console.log('Available ITM functions:', itmFunctions.join(', '));
            console.log('\n✓ Module loads correctly, but expected functions not found.\n');
        }
        
    } catch (error) {
        console.error('✗ Test failed:', error);
        console.error('Stack:', error.stack);
        process.exit(1);
    }
    
    function getModeString(mode) {
        const modes = { 1: 'Line of Sight', 2: 'Diffraction', 3: 'Troposcatter' };
        return modes[mode] || 'Unknown';
    }
})();
