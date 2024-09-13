#include <complex>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>

using namespace std;

// Export the DLL functions as "C" and not C++
#define DLLEXPORT extern "C" __declspec(dllexport)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define DIM(x, y) (((x) > (y)) ? (x - y) : (0))

#define PI                                      3.1415926535897932384
#define SQRT2                                   sqrt(2)
#define a_0__meter                              6370e3
#define a_9000__meter                           9000e3
#define THIRD                                   1.0 / 3.0

#define MODE__P2P                               0
#define MODE__AREA                              1


/////////////////////////////
// Data Structures

struct IntermediateValues
{
    double theta_hzn[2];        // Terminal horizon angles
    double d_hzn__meter[2];     // Terminal horizon distances, in meters
    double h_e__meter[2];       // Terminal effective heights, in meters
    double N_s;                 // Surface refractivity, in N-Units
    double delta_h__meter;      // Terrain irregularity parameter, in meters
    double A_ref__db;           // Reference attenuation, in dB
    double A_fs__db;            // Free space basic transmission loss, in dB
    double d__km;               // Path distance, in km
    int mode;                   // Mode of propagation value
};

/////////////////////////////
// Main ITM Functions

DLLEXPORT int ITM_P2P_TLS(const double h_tx__meter, const double h_rx__meter, const double pfl[], int climate, const double N_0, const double f__mhz,
    const int pol, const double epsilon, const double sigma, int mdvar, const double time, const double location, const double situation,
    double *A__db, long *warnings);
DLLEXPORT int ITM_P2P_TLS_Ex(const double h_tx__meter, const double h_rx__meter, const double pfl[], int climate, const double N_0, const double f__mhz,
    const int pol, const double epsilon, const double sigma, int mdvar, const double time, const double location, const double situation,
    double *A__db, long *warnings, IntermediateValues *interValues);
DLLEXPORT int ITM_P2P_CR(const double h_tx__meter, const double h_rx__meter, const double pfl[], int climate, const double N_0, const double f__mhz,
    const int pol, const double epsilon, const double sigma, int mdvar, const double confidence, const double reliability,
    double *A__db, long *warnings);
DLLEXPORT int ITM_P2P_CR_Ex(const double h_tx__meter, const double h_rx__meter, const double pfl[], int climate, const double N_0, const double f__mhz,
    const int pol, const double epsilon, const double sigma, int mdvar, const double confidence, const double reliability,
    double *A__db, long *warnings, IntermediateValues *interValues);
DLLEXPORT int ITM_AREA_TLS(const double h_tx__meter, const double h_rx__meter, const int tx_site_criteria, const int rx_site_criteria, const double d__km,
    const double delta_h__meter, int climate, const double N_0, double f__mhz, const int pol, const double epsilon, const double sigma,
    int mdvar, const double time, const double location, const double situation, double *A__db, long *warnings);
DLLEXPORT int ITM_AREA_TLS_Ex(const double h_tx__meter, const double h_rx__meter, const int tx_site_criteria, const int rx_site_criteria, const double d__km,
    const double delta_h__meter, int climate, const double N_0, const double f__mhz, const int pol, const double epsilon, const double sigma,
    int mdvar, const double time, const double location, const double situation, double *A__db, long *warnings, IntermediateValues *interValues);
DLLEXPORT int ITM_AREA_CR(const double h_tx__meter, const double h_rx__meter, const int tx_site_criteria, const int rx_site_criteria, const double d__km,
    const double delta_h__meter, int climate, const double N_0, const double f__mhz, const int pol, const double epsilon, const double sigma,
    int mdvar, const double confidence, const double reliability, double *A__db, long *warnings);
DLLEXPORT int ITM_AREA_CR_Ex(const double h_tx__meter, const double h_rx__meter, const int tx_site_criteria, const int rx_site_criteria, const double d__km,
    const double delta_h__meter, int climate, const double N_0, const double f__mhz, const int pol, const double epsilon, const double sigma,
    int mdvar, const double confidence, const double reliability, double *A__db, long *warnings, IntermediateValues *interValues);

/////////////////////////////
// ITM Helper Functions

DLLEXPORT double ComputeDeltaH(const double pfl[], const double d_start__meter, const double d_end__meter);
DLLEXPORT double DiffractionLoss(const double d__meter, const double d_hzn__meter[2], const double h_e__meter[2], const complex<double> Z_g,
    const double a_e__meter, const double delta_h__meter, const double h__meter[2], const int mode, const double theta_los, const double d_sML__meter, const double f__mhz);
DLLEXPORT double FFunction(const double td);
DLLEXPORT void FindHorizons(const double pfl[], const double a_e__meter, const double h__meter[2], double theta_hzn[2], double d_hzn__meter[2]);
DLLEXPORT double FreeSpaceLoss(const double d__meter, const double f__mhz);
DLLEXPORT double FresnelIntegral(const double v2);
DLLEXPORT double H0Function(const double r, double eta_s);
DLLEXPORT double HeightFunction(const double x__km, const double K);
DLLEXPORT void InitializeArea(const int site_criteria[2], const double gamma_e, const double delta_h__meter,
    const double h__meter[2], double h_e__meter[2], double d_hzn__meter[2], double theta_hzn[2]);
DLLEXPORT void InitializePointToPoint(const double f__mhz, const double h_sys__meter, const double N_0, const int pol, const double epsilon, 
    const double sigma, complex<double> *Z_g, double *gamma_e, double *N_s);
DLLEXPORT double InverseComplementaryCumulativeDistributionFunction(const double q);
DLLEXPORT double KnifeEdgeDiffraction(const double d__meter, const double f__mhz, const double a_e__meter, const double theta_los, const double d_hzn__meter[2]);
DLLEXPORT void LinearLeastSquaresFit(const double pfl[], const double d_start, const double d_end, double *fit_y1, double *fit_y2);
DLLEXPORT double LineOfSightLoss(const double d__meter, const double h_e__meter[2], const complex<double> Z_g, const double delta_h__meter,
    const double M_d, const double A_d0, const double d_sML__meter, const double f__mhz);
DLLEXPORT int LongleyRice(const double theta_hzn[2], const double f__mhz, const complex<double> Z_g, const double d_hzn__meter[2], const double h_e__meter[2], 
    const double gamma_e, const double N_s, const double delta_h__meter, const double h__meter[2], const double d__meter, const int mode, double *A_ref__db, 
    long *warnings, int *propmode);
DLLEXPORT void QuickPfl(const double pfl[], const double gamma_e, const double h__meter[2], double theta_hzn[2], double d_hzn__meter[2], 
    double h_e__meter[2], double *delta_h__meter, double *d__meter);
DLLEXPORT double SigmaHFunction(const double delta_h__meter);
DLLEXPORT double SmoothEarthDiffraction(const double d__meter, const double f__mhz, const double a_e__meter, const double theta_los, 
    const double d_hzn__meter[2], const double h_e__meter[2], const complex<double> Z_g);
DLLEXPORT double TerrainRoughness(const double d__meter, const double delta_h__meter);
DLLEXPORT double TroposcatterLoss(const double d__meter, const double theta_hzn[2], const double d_hzn__meter[2], const double h_e__meter[2], 
    const double a_e__meter, const double N_s, const double f__mhz, const double theta_los, double *h0);
DLLEXPORT int ValidateInputs(const double h_tx__meter, const double h_rx__meter, const int climate, const double time,
    const double location, const double situation, const double N_0, const double f__mhz, const int pol,
    const double epsilon, const double sigma, const int mdvar, long *warnings);
DLLEXPORT double Variability(const double time, const double location, const double situation, const double h_e__meter[2], const double delta_h__meter,
    const double f__mhz, const double d__meter, const double A_ref__db, int climate, int mdvar, long *warnings);
