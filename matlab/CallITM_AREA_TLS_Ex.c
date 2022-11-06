#include "mex.h"
#include "test.h"

// define external public function
extern int ITM_AREA_TLS_Ex(double h_tx__meter, double h_rx__meter, 
                        int tx_site_criteria, int rx_site_criteria, 
                        double d__km, double delta_h__meter, int climate, 
                        double N_0, double f__mhz, int pol, double epsilon, 
                        double sigma, int mdvar, double time, 
                        double location, double situation, double *A__db, 
                        long *warnings);

/*=============================================================================
 |
 |  Description:  The ITS Irregular Terrain Model (ITM).  This function
 |                exposes area mode functionality, with variability
 |                specified with time/location/situation (TLS)
 |
 |        Input:  nrhs  - Number of inputs
 |                prhs  - Array of input values
 |
 |       Output:  nlhs  - Number of ouputs
 |                plhs  - Array of output values
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void CallITM_AREA_TLS_Ex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // validate inputs
    CallITM_AREA_VALIDATION(nrhs, prhs, "MATLAB:ITM:CallITM_AREA_TLS_Ex", AREA_MODE_TLS);

    // parse inputs
    double h_tx__meter = mxGetScalar(prhs[1]);
    double h_rx__meter = mxGetScalar(prhs[2]);
    int tx_site_criteria = (int)mxGetScalar(prhs[3]);
    int rx_site_criteria = (int)mxGetScalar(prhs[4]);
    double d__km = mxGetScalar(prhs[5]);
    double delta_h__meter = mxGetScalar(prhs[6]);
    int climate = (int)mxGetScalar(prhs[7]);
    double N_0 = mxGetScalar(prhs[8]);
    double f__mhz = mxGetScalar(prhs[9]);
    int pol = (int)mxGetScalar(prhs[10]);
    double epsilon = mxGetScalar(prhs[11]);
    double sigma = mxGetScalar(prhs[12]);
    int mdvar = (int)mxGetScalar(prhs[13]);
    double time = mxGetScalar(prhs[14]);
    double location = mxGetScalar(prhs[15]);
    double situation = mxGetScalar(prhs[16]);

    // call external C library function
    double A__db;
    long warnings;
    struct IntermediateValues iValues;
    int rtn = ITM_AREA_TLS_Ex(h_tx__meter, h_rx__meter, 
                              tx_site_criteria, rx_site_criteria, 
                              d__km, delta_h__meter, climate, 
                              N_0, f__mhz, pol, epsilon, 
                              sigma, mdvar, time, 
                              location, situation, &A__db, 
                              &warnings, &iValues);
    
    // record result
    plhs[0] = mxCreateDoubleScalar(rtn);
    plhs[1] = mxCreateDoubleScalar(A__db);
    plhs[2] = mxCreateDoubleScalar(warnings);

    // record intermediate value structure
    ExportIntermediateValues(iValues, 3, plhs);
}