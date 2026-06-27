/// Terrain irregularity of a path of length `d__meter`.
pub fn terrain_roughness(d__meter: f64, delta_h__meter: f64) -> f64 {
    delta_h__meter * (1.0 - 0.8 * (-d__meter / 50_000.0).exp())
}
