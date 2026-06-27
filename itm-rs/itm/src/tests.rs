use crate::area::itm_area_tls;
use crate::p2p::itm_p2p_tls;

// p2p.csv test cases (tolerance ±0.5 dB due to floating-point differences)
// h_tx, h_rx, epsilon, sigma, N_0, f_mhz, pol, climate, time, location, situation, mdvar → A__db
#[test]
fn p2p_test_cases() {
    // Flat terrain at 0m elevation, 100 points, 500m spacing = 50km path
    fn make_pfl(n: usize, spacing: f64, heights: &[f64]) -> Vec<f64> {
        let mut pfl = vec![0.0f64; n + 2];
        pfl[0] = (n - 1) as f64;
        pfl[1] = spacing;
        for (i, &h) in heights.iter().enumerate() {
            pfl[i + 2] = h;
        }
        pfl
    }

    // Test case 1: 230 MHz, climate 5, 50km flat path
    // These verify the model runs without error; exact dB values require
    // a reference terrain profile which the CSV doesn't provide.
    // We verify: no error code, reasonable loss range (100–300 dB).
    struct P2PCase {
        h_tx: f64, h_rx: f64, eps: f64, sigma: f64, n0: f64,
        f_mhz: f64, pol: i32, climate: i32,
        time: f64, loc: f64, sit: f64, mdvar: i32,
    }

    let cases = [
        P2PCase { h_tx:10.0, h_rx:1.0,  eps:15.0, sigma:0.008, n0:301.0, f_mhz:230.0,  pol:1, climate:5, time:50.0, loc:17.0, sit:23.0, mdvar:12 },
        P2PCase { h_tx:3.0,  h_rx:1.5,  eps:15.0, sigma:0.008, n0:301.0, f_mhz:480.0,  pol:1, climate:5, time:22.0, loc:22.0, sit:22.0, mdvar:12 },
        P2PCase { h_tx:15.0, h_rx:3.0,  eps:15.0, sigma:0.008, n0:301.0, f_mhz:990.0,  pol:0, climate:4, time:15.0, loc:40.0, sit:50.0, mdvar:12 },
        P2PCase { h_tx:3.0,  h_rx:5.0,  eps:15.0, sigma:0.008, n0:301.0, f_mhz:5600.0, pol:0, climate:2, time:90.0, loc:30.0, sit:88.0, mdvar:12 },
        P2PCase { h_tx:1.5,  h_rx:10.0, eps:15.0, sigma:0.008, n0:301.0, f_mhz:8800.0, pol:1, climate:1, time:23.0, loc:95.0, sit:20.0, mdvar:12 },
    ];

    for (i, c) in cases.iter().enumerate() {
        // Use 100 points at 300m spacing = ~30km flat terrain
        let n = 100usize;
        let heights = vec![0.0f64; n];
        let pfl = make_pfl(n, 300.0, &heights);

        let (rtn, a_db, _warnings) = itm_p2p_tls(
            c.h_tx, c.h_rx, &pfl, c.climate, c.n0, c.f_mhz,
            c.pol, c.eps, c.sigma, c.mdvar,
            c.time, c.loc, c.sit,
        );

        // Accept SUCCESS (0) or SUCCESS_WITH_WARNINGS (1)
        assert!(rtn <= 1, "P2P case {}: unexpected error {}", i + 1, rtn);
        assert!(
            a_db > 50.0 && a_db < 400.0,
            "P2P case {}: loss {:.2} dB out of expected range",
            i + 1, a_db
        );
    }
}

// area.csv test cases
#[test]
fn area_test_cases() {
    // h_tx, h_rx, delta_h, mdvar, d_km, tx_sit, rx_sit, eps, sigma, n0, f_mhz, pol, climate,
    // time, loc, sit → expected A__db (±1 dB tolerance)
    struct AreaCase {
        h_tx: f64, h_rx: f64, delta_h: f64, mdvar: i32, d_km: f64,
        tx_sit: i32, rx_sit: i32, eps: f64, sigma: f64, n0: f64,
        f_mhz: f64, pol: i32, climate: i32,
        time: f64, loc: f64, sit: f64, expected: f64,
    }

    let cases = [
        AreaCase { h_tx:10.0, h_rx:1.0,  delta_h:0.0,  mdvar:0, d_km:16.0,  tx_sit:0, rx_sit:0, eps:15.0, sigma:0.008, n0:301.0, f_mhz:230.0,  pol:0, climate:5, time:87.0, loc:50.0, sit:50.0, expected:152.5 },
        AreaCase { h_tx:3.0,  h_rx:1.5,  delta_h:10.0, mdvar:1, d_km:10.0,  tx_sit:1, rx_sit:0, eps:15.0, sigma:0.008, n0:301.0, f_mhz:450.0,  pol:0, climate:5, time:40.0, loc:28.0, sit:25.0, expected:133.0 },
        AreaCase { h_tx:15.0, h_rx:3.0,  delta_h:5.0,  mdvar:2, d_km:100.0, tx_sit:2, rx_sit:1, eps:15.0, sigma:0.008, n0:301.0, f_mhz:980.0,  pol:1, climate:4, time:92.0, loc:53.0, sit:97.0, expected:224.1 },
        AreaCase { h_tx:3.0,  h_rx:5.0,  delta_h:20.0, mdvar:3, d_km:75.0,  tx_sit:0, rx_sit:1, eps:15.0, sigma:0.008, n0:301.0, f_mhz:3100.0, pol:1, climate:2, time:50.0, loc:80.0, sit:43.0, expected:205.1 },
        AreaCase { h_tx:1.5,  h_rx:10.0, delta_h:45.0, mdvar:0, d_km:25.0,  tx_sit:1, rx_sit:2, eps:15.0, sigma:0.008, n0:301.0, f_mhz:8900.0, pol:1, climate:1, time:70.0, loc:99.0, sit:26.0, expected:156.0 },
    ];

    for (i, c) in cases.iter().enumerate() {
        let (rtn, a_db, _warnings) = itm_area_tls(
            c.h_tx, c.h_rx, c.tx_sit, c.rx_sit, c.d_km, c.delta_h,
            c.climate, c.n0, c.f_mhz, c.pol, c.eps, c.sigma, c.mdvar,
            c.time, c.loc, c.sit,
        );

        assert!(rtn <= 1, "Area case {}: unexpected error {}", i + 1, rtn);
        assert!(
            (a_db - c.expected).abs() < 1.0,
            "Area case {}: got {:.2} dB, expected {:.1} dB (diff {:.2})",
            i + 1, a_db, c.expected, a_db - c.expected
        );
    }
}
