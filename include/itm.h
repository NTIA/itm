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

DLLEXPORT int ITM_P2P_TLS(double h_tx__meter, double h_rx__meter, double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
    double *A__db, long *warnings);
DLLEXPORT int ITM_P2P_TLS_Ex(double h_tx__meter, double h_rx__meter, double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
    double *A__db, long *warnings, IntermediateValues *interValues);
DLLEXPORT int ITM_P2P_CR(double h_tx__meter, double h_rx__meter, double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
    double *A__db, long *warnings);
DLLEXPORT int ITM_P2P_CR_Ex(double h_tx__meter, double h_rx__meter, double pfl[], int climate, double N_0, double f__mhz,
    int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
    double *A__db, long *warnings, IntermediateValues *interValues);
DLLEXPORT int ITM_AREA_TLS(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double time, double location, double situation, double *A__db, long *warnings);
DLLEXPORT int ITM_AREA_TLS_Ex(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double time, double location, double situation, double *A__db, long *warnings, IntermediateValues *interValues);
DLLEXPORT int ITM_AREA_CR(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double confidence, double reliability, double *A__db, long *warnings);
DLLEXPORT int ITM_AREA_CR_Ex(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria, double d__km,
    double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
    int mdvar, double confidence, double reliability, double *A__db, long *warnings, IntermediateValues *interValues);

/////////////////////////////
// ITM Helper Functions

DLLEXPORT double ComputeDeltaH(double pfl[], double d_start__meter, double d_end__meter);
DLLEXPORT double DiffractionLoss(double d__meter, double d_hzn__meter[2], double h_e__meter[2], complex<double> Z_g,
    double a_e__meter, double delta_h__meter, double h__meter[2], int mode, double theta_los, double d_sML__meter, double f__mhz);
DLLEXPORT double FFunction(double td);
DLLEXPORT void FindHorizons(double pfl[], double a_e__meter, double h__meter[2], double theta_hzn[2], double d_hzn__meter[2]);
DLLEXPORT double FreeSpaceLoss(double d__meter, double f__mhz);
DLLEXPORT double FresnelIntegral(double v2);
DLLEXPORT double H0Function(double r, double eta_s);
DLLEXPORT double HeightFunction(double x__km, double K);
DLLEXPORT void InitializeArea(int site_criteria[2], double gamma_e, double delta_h__meter,
    double h__meter[2], double h_e__meter[2], double d_hzn__meter[2], double theta_hzn[2]);
DLLEXPORT void InitializePointToPoint(double f__mhz, double h_sys__meter, double N_0, int polarization, double epsilon, 
    double sigma, complex<double> *Z_g, double *gamma_e, double *N_s);
DLLEXPORT double InverseComplementaryCumulativeDistributionFunction(double q);
DLLEXPORT double KnifeEdgeDiffraction(double d__meter, double f__mhz, double a_e__meter, double theta_los, double d_hzn__meter[2]);
DLLEXPORT void LinearLeastSquaresFit(double pfl[], double d_start, double d_end, double *fit_y1, double *fit_y2);
DLLEXPORT double LineOfSightLoss(double d__meter, double h_e__meter[2], complex<double> Z_g, double delta_h__meter,
    double M_d, double A_d0, double d_sML__meter, double f__mhz);
DLLEXPORT int LongleyRice(double theta_hzn[2], double f__mhz, complex<double> Z_g, double d_hzn__meter[2], double h_e__meter[2], 
    double gamma_e, double N_s, double delta_h__meter, double h__meter[2], double d__meter, int mode, double *A_ref__db, 
    long *warnings, int *propmode);
DLLEXPORT void QuickPfl(double pfl[], double gamma_e, double h__meter[2], double theta_hzn[2], double d_hzn__meter[2], 
    double h_e__meter[2], double *delta_h__meter, double *d__meter);
DLLEXPORT double SigmaHFunction(double delta_h__meter);
DLLEXPORT double SmoothEarthDiffraction(double d__meter, double f__mhz, double a_e__meter, double theta_los, 
    double d_hzn__meter[2], double h_e__meter[2], complex<double> Z_g);
DLLEXPORT double TerrainRoughness(double d__meter, double delta_h__meter);
DLLEXPORT double TroposcatterLoss(double d__meter, double theta_hzn[2], double d_hzn__meter[2], double h_e__meter[2], 
    double a_e__meter, double N_s, double f__mhz, double theta_los, double *h0);
DLLEXPORT int ValidateInputs(double h_tx__meter, double h_rx__meter, int climate, double time,
    double location, double situation, double N_0, double f__mhz, int pol,
    double epsilon, double sigma, int mdvar, long *warnings);
DLLEXPORT double Variability(double time, double location, double situation, double h_e__meter[2], double delta_h__meter,
    double f__mhz, double d__meter, double A_ref__db, int climate, int mdvar, long *warnings);
