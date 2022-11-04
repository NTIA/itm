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

#define NUMBER_OF_FIELDS (sizeof(field_names) / sizeof(*field_names))

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
    mwSize dims[2] = {1, 12};
    const char* field_names[] = {"parameter", "value"};
    plhs[3] = mxCreateStructArray(2, dims, NUMBER_OF_FIELDS, field_names);

    mxSetFieldByNumber(plhs[3], 0, 0, mxCreateString("theta_hzn_tx"));
    mxSetFieldByNumber(plhs[3], 0, 1, mxCreateDoubleScalar(iValues.theta_hzn[0]));
    mxSetFieldByNumber(plhs[3], 1, 0, mxCreateString("theta_hzn_rx"));
    mxSetFieldByNumber(plhs[3], 1, 1, mxCreateDoubleScalar(iValues.theta_hzn[1]));
    mxSetFieldByNumber(plhs[3], 2, 0, mxCreateString("d_hzn_tx__meter"));
    mxSetFieldByNumber(plhs[3], 2, 1, mxCreateDoubleScalar(iValues.d_hzn__meter[0]));
    mxSetFieldByNumber(plhs[3], 3, 0, mxCreateString("d_hzn_rx__meter"));
    mxSetFieldByNumber(plhs[3], 3, 1, mxCreateDoubleScalar(iValues.d_hzn__meter[1]));
    mxSetFieldByNumber(plhs[3], 4, 0, mxCreateString("h_e_tx__meter"));
    mxSetFieldByNumber(plhs[3], 4, 1, mxCreateDoubleScalar(iValues.h_e__meter[0]));
    mxSetFieldByNumber(plhs[3], 5, 0, mxCreateString("h_e_rx__meter"));
    mxSetFieldByNumber(plhs[3], 5, 1, mxCreateDoubleScalar(iValues.h_e__meter[1]));
    mxSetFieldByNumber(plhs[3], 6, 0, mxCreateString("N_s"));
    mxSetFieldByNumber(plhs[3], 6, 1, mxCreateDoubleScalar(iValues.N_s));
    mxSetFieldByNumber(plhs[3], 7, 0, mxCreateString("delta_h__meter"));
    mxSetFieldByNumber(plhs[3], 7, 1, mxCreateDoubleScalar(iValues.delta_h__meter));
    mxSetFieldByNumber(plhs[3], 8, 0, mxCreateString("A_ref__db"));
    mxSetFieldByNumber(plhs[3], 8, 1, mxCreateDoubleScalar(iValues.A_ref__db));
    mxSetFieldByNumber(plhs[3], 9, 0, mxCreateString("A_fs__db"));
    mxSetFieldByNumber(plhs[3], 9, 1, mxCreateDoubleScalar(iValues.A_fs__db));
    mxSetFieldByNumber(plhs[3], 10, 0, mxCreateString("d__km"));
    mxSetFieldByNumber(plhs[3], 10, 1, mxCreateDoubleScalar(iValues.d__km));
    mxSetFieldByNumber(plhs[3], 11, 0, mxCreateString("mode"));
    mxSetFieldByNumber(plhs[3], 11, 1, mxCreateDoubleScalar(iValues.mode));
}