#![allow(non_snake_case)]
use itm::IntermediateValues;
use std::env;
use std::fs;
use std::process;

const HELP: &str = r#"ITS Irregular Terrain Model (ITM) — Rust CLI

USAGE:
    itm-cli <SUBCOMMAND> [OPTIONS]

SUBCOMMANDS:
    p2p     Point-to-point prediction (requires terrain profile file)
    area    Area prediction (no terrain file needed)

P2P OPTIONS (all required):
    --h-tx <m>          TX structural height, meters (0.5–3000)
    --h-rx <m>          RX structural height, meters (0.5–3000)
    --pfl <file>        Terrain profile file (plain text: N dR h0 h1 ... hN)
    --climate <1-7>     Radio climate (1=Equatorial … 7=MarTemSea)
    --n0 <N-units>      Surface refractivity (250–400)
    --freq <MHz>        Frequency in MHz (20–20000)
    --pol <0|1>         Polarization: 0=horizontal, 1=vertical
    --epsilon <val>     Relative permittivity (>1)
    --sigma <val>       Conductivity, S/m (>0)
    --mdvar <0-33>      Mode of variability
    --time <pct>        Time percentage (0–100 exclusive)
    --location <pct>    Location percentage (0–100 exclusive)
    --situation <pct>   Situation percentage (0–100 exclusive)

AREA OPTIONS (all required):
    --h-tx <m>          TX structural height, meters
    --h-rx <m>          RX structural height, meters
    --tx-siting <0-2>   TX siting criteria (0=random, 1=careful, 2=very careful)
    --rx-siting <0-2>   RX siting criteria
    --distance <km>     Path distance in km (>0)
    --delta-h <m>       Terrain irregularity parameter, meters (>=0)
    --climate <1-7>     Radio climate
    --n0 <N-units>      Surface refractivity
    --freq <MHz>        Frequency in MHz
    --pol <0|1>         Polarization
    --epsilon <val>     Relative permittivity
    --sigma <val>       Conductivity
    --mdvar <0-33>      Mode of variability
    --time <pct>        Time percentage
    --location <pct>    Location percentage
    --situation <pct>   Situation percentage

TERRAIN FILE FORMAT:
    First value: number of terrain points minus 1 (N)
    Second value: spacing in meters (dR)
    Remaining N+1 values: terrain elevations in meters above MSL

EXAMPLES:
    itm-cli p2p --h-tx 15 --h-rx 3 --pfl terrain.txt --climate 5 --n0 301
        --freq 3500 --pol 1 --epsilon 15 --sigma 0.005 --mdvar 12
        --time 50 --location 50 --situation 50

    itm-cli area --h-tx 10 --h-rx 1 --tx-siting 0 --rx-siting 0
        --distance 16 --delta-h 0 --climate 5 --n0 301 --freq 230
        --pol 0 --epsilon 15 --sigma 0.008 --mdvar 0
        --time 87 --location 50 --situation 50
"#;

fn usage_err(msg: &str) -> ! {
    eprintln!("Error: {msg}");
    eprintln!("Run `itm-cli --help` for usage.");
    process::exit(1);
}

fn parse_f64(val: &str, name: &str) -> f64 {
    val.parse::<f64>()
        .unwrap_or_else(|_| usage_err(&format!("--{name} must be a number, got '{val}'")))
}

fn parse_i32(val: &str, name: &str) -> i32 {
    val.parse::<i32>()
        .unwrap_or_else(|_| usage_err(&format!("--{name} must be an integer, got '{val}'")))
}

/// Parse a key=value pair from the argument iterator.
fn next_val<'a>(args: &mut impl Iterator<Item = &'a str>, flag: &str) -> &'a str {
    args.next()
        .unwrap_or_else(|| usage_err(&format!("--{flag} requires a value")))
}

/// Load a PFL terrain file.
/// Format: `N dR h0 h1 ... hN` (N+2 header values: pfl[0]=N, pfl[1]=dR, rest=heights)
fn load_pfl(path: &str) -> Vec<f64> {
    let text = fs::read_to_string(path)
        .unwrap_or_else(|e| usage_err(&format!("Cannot read terrain file '{path}': {e}")));
    let values: Vec<f64> = text
        .split_whitespace()
        .map(|s| {
            s.parse::<f64>()
                .unwrap_or_else(|_| usage_err(&format!("Non-numeric value '{s}' in terrain file")))
        })
        .collect();
    if values.len() < 3 {
        usage_err("Terrain file must have at least 3 values (N dR h0)");
    }
    let n = values[0] as usize;
    if values.len() != n + 2 {
        usage_err(&format!(
            "Terrain file declares N={n} points but has {} values (expected {})",
            values.len(),
            n + 2
        ));
    }
    values
}

fn print_result(rtn: i32, a__db: f64, warnings: i32, inter: Option<&IntermediateValues>) {
    println!("Basic Transmission Loss: {:.2} dB", a__db);
    println!("Return code: {rtn}");
    if warnings != 0 {
        println!("Warnings: 0x{warnings:04X}");
        print_warnings(warnings);
    }
    if let Some(iv) = inter {
        println!("\n--- Intermediate Values ---");
        println!("  Path distance:        {:.3} km", iv.d__km);
        println!("  Free-space loss:      {:.2} dB", iv.a_fs__db);
        println!("  Reference atten:      {:.2} dB", iv.a_ref__db);
        println!("  Delta H:              {:.2} m", iv.delta_h__meter);
        println!("  Surface refractivity: {:.1} N-units", iv.n_s);
        println!(
            "  Eff. TX height:       {:.3} m", iv.h_e__meter[0]
        );
        println!(
            "  Eff. RX height:       {:.3} m", iv.h_e__meter[1]
        );
        println!(
            "  TX horizon dist:      {:.1} m", iv.d_hzn__meter[0]
        );
        println!(
            "  RX horizon dist:      {:.1} m", iv.d_hzn__meter[1]
        );
        let mode_str = match iv.mode {
            1 => "Line of Sight",
            2 => "Diffraction",
            3 => "Troposcatter",
            _ => "Unknown",
        };
        println!("  Propagation mode:     {mode_str}");
    }
}

fn print_warnings(w: i32) {
    let flags = [
        (0x0001, "TX terminal height out of recommended range"),
        (0x0002, "RX terminal height out of recommended range"),
        (0x0004, "Frequency out of recommended range"),
        (0x0008, "Path distance > 1000 km"),
        (0x0010, "Path distance > 2000 km"),
        (0x0020, "Path distance below minimum for antenna heights"),
        (0x0040, "Path distance < 1 km"),
        (0x0080, "TX horizon angle > 200 mrad"),
        (0x0100, "RX horizon angle > 200 mrad"),
        (0x0200, "TX horizon distance < 10% of smooth-earth value"),
        (0x0400, "RX horizon distance < 10% of smooth-earth value"),
        (0x0800, "TX horizon distance > 3x smooth-earth value"),
        (0x1000, "RX horizon distance > 3x smooth-earth value"),
        (0x2000, "Extreme variabilities"),
        (0x4000, "Surface refractivity out of recommended range"),
    ];
    for (mask, desc) in &flags {
        if w & mask != 0 {
            println!("  [WARN] {desc}");
        }
    }
}

fn run_p2p(args: &[String]) {
    let mut h_tx = None::<f64>;
    let mut h_rx = None::<f64>;
    let mut pfl_path = None::<String>;
    let mut climate = None::<i32>;
    let mut n_0 = None::<f64>;
    let mut f_mhz = None::<f64>;
    let mut pol = None::<i32>;
    let mut epsilon = None::<f64>;
    let mut sigma = None::<f64>;
    let mut mdvar = None::<i32>;
    let mut time = None::<f64>;
    let mut location = None::<f64>;
    let mut situation = None::<f64>;

    let mut iter = args.iter().map(String::as_str);
    while let Some(flag) = iter.next() {
        let val = next_val(&mut iter, flag);
        match flag {
            "--h-tx"      => h_tx      = Some(parse_f64(val, "h-tx")),
            "--h-rx"      => h_rx      = Some(parse_f64(val, "h-rx")),
            "--pfl"       => pfl_path  = Some(val.to_string()),
            "--climate"   => climate   = Some(parse_i32(val, "climate")),
            "--n0"        => n_0       = Some(parse_f64(val, "n0")),
            "--freq"      => f_mhz     = Some(parse_f64(val, "freq")),
            "--pol"       => pol       = Some(parse_i32(val, "pol")),
            "--epsilon"   => epsilon   = Some(parse_f64(val, "epsilon")),
            "--sigma"     => sigma     = Some(parse_f64(val, "sigma")),
            "--mdvar"     => mdvar     = Some(parse_i32(val, "mdvar")),
            "--time"      => time      = Some(parse_f64(val, "time")),
            "--location"  => location  = Some(parse_f64(val, "location")),
            "--situation" => situation = Some(parse_f64(val, "situation")),
            other => usage_err(&format!("Unknown flag for p2p: {other}")),
        }
    }

    macro_rules! req {
        ($opt:expr, $name:literal) => {
            $opt.unwrap_or_else(|| usage_err(concat!("--", $name, " is required for p2p mode")))
        };
    }

    let pfl = load_pfl(&req!(pfl_path, "pfl"));

    let (rtn, a__db, warnings, inter) = itm::itm_p2p_tls_ex(
        req!(h_tx, "h-tx"),
        req!(h_rx, "h-rx"),
        &pfl,
        req!(climate, "climate"),
        req!(n_0, "n0"),
        req!(f_mhz, "freq"),
        req!(pol, "pol"),
        req!(epsilon, "epsilon"),
        req!(sigma, "sigma"),
        req!(mdvar, "mdvar"),
        req!(time, "time"),
        req!(location, "location"),
        req!(situation, "situation"),
    );

    print_result(rtn, a__db, warnings, Some(&inter));
}

fn run_area(args: &[String]) {
    let mut h_tx = None::<f64>;
    let mut h_rx = None::<f64>;
    let mut tx_siting = None::<i32>;
    let mut rx_siting = None::<i32>;
    let mut distance = None::<f64>;
    let mut delta_h = None::<f64>;
    let mut climate = None::<i32>;
    let mut n_0 = None::<f64>;
    let mut f_mhz = None::<f64>;
    let mut pol = None::<i32>;
    let mut epsilon = None::<f64>;
    let mut sigma = None::<f64>;
    let mut mdvar = None::<i32>;
    let mut time = None::<f64>;
    let mut location = None::<f64>;
    let mut situation = None::<f64>;

    let mut iter = args.iter().map(String::as_str);
    while let Some(flag) = iter.next() {
        let val = next_val(&mut iter, flag);
        match flag {
            "--h-tx"      => h_tx      = Some(parse_f64(val, "h-tx")),
            "--h-rx"      => h_rx      = Some(parse_f64(val, "h-rx")),
            "--tx-siting" => tx_siting = Some(parse_i32(val, "tx-siting")),
            "--rx-siting" => rx_siting = Some(parse_i32(val, "rx-siting")),
            "--distance"  => distance  = Some(parse_f64(val, "distance")),
            "--delta-h"   => delta_h   = Some(parse_f64(val, "delta-h")),
            "--climate"   => climate   = Some(parse_i32(val, "climate")),
            "--n0"        => n_0       = Some(parse_f64(val, "n0")),
            "--freq"      => f_mhz     = Some(parse_f64(val, "freq")),
            "--pol"       => pol       = Some(parse_i32(val, "pol")),
            "--epsilon"   => epsilon   = Some(parse_f64(val, "epsilon")),
            "--sigma"     => sigma     = Some(parse_f64(val, "sigma")),
            "--mdvar"     => mdvar     = Some(parse_i32(val, "mdvar")),
            "--time"      => time      = Some(parse_f64(val, "time")),
            "--location"  => location  = Some(parse_f64(val, "location")),
            "--situation" => situation = Some(parse_f64(val, "situation")),
            other => usage_err(&format!("Unknown flag for area: {other}")),
        }
    }

    macro_rules! req {
        ($opt:expr, $name:literal) => {
            $opt.unwrap_or_else(|| usage_err(concat!("--", $name, " is required for area mode")))
        };
    }

    let (rtn, a__db, warnings, inter) = itm::itm_area_tls_ex(
        req!(h_tx, "h-tx"),
        req!(h_rx, "h-rx"),
        req!(tx_siting, "tx-siting"),
        req!(rx_siting, "rx-siting"),
        req!(distance, "distance"),
        req!(delta_h, "delta-h"),
        req!(climate, "climate"),
        req!(n_0, "n0"),
        req!(f_mhz, "freq"),
        req!(pol, "pol"),
        req!(epsilon, "epsilon"),
        req!(sigma, "sigma"),
        req!(mdvar, "mdvar"),
        req!(time, "time"),
        req!(location, "location"),
        req!(situation, "situation"),
    );

    print_result(rtn, a__db, warnings, Some(&inter));
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 || args[1] == "--help" || args[1] == "-h" {
        print!("{HELP}");
        return;
    }

    match args[1].as_str() {
        "p2p"  => run_p2p(&args[2..]),
        "area" => run_area(&args[2..]),
        other  => usage_err(&format!("Unknown subcommand '{other}'. Use 'p2p' or 'area'.")),
    }
}
