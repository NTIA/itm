fn h0_curve(j: usize, r: f64) -> f64 {
    const A: [f64; 5] = [25.0, 80.0, 177.0, 395.0, 705.0];
    const B: [f64; 5] = [24.0, 45.0, 68.0, 80.0, 105.0];
    10.0 * (1.0 + A[j] * (1.0 / r).powi(4) + B[j] * (1.0 / r).powi(2)).log10()
}

/// Troposcatter frequency gain function H_0().
pub fn h0_function(r: f64, eta_s: f64) -> f64 {
    let eta_s = eta_s.clamp(1.0, 5.0);
    let i = eta_s as usize;
    let q = eta_s - i as f64;
    let result = h0_curve(i - 1, r);
    if q != 0.0 {
        (1.0 - q) * result + q * h0_curve(i, r)
    } else {
        result
    }
}
