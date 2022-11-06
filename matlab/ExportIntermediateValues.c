#include "mex.h"
#include "test.h"

void ExportIntermediateValues(struct IntermediateValues iValues, int index, mxArray *plhs[]) {
    // record intermediate value structure
    mwSize dims[2] = {1, 12};
    const char* field_names[] = {"parameter", "value"};
    plhs[index] = mxCreateStructArray(2, dims, NUMBER_OF_FIELDS, field_names);

    mxSetFieldByNumber(plhs[index], 0, 0, mxCreateString("theta_hzn_tx"));
    mxSetFieldByNumber(plhs[index], 0, 1, mxCreateDoubleScalar(iValues.theta_hzn[0]));
    mxSetFieldByNumber(plhs[index], 1, 0, mxCreateString("theta_hzn_rx"));
    mxSetFieldByNumber(plhs[index], 1, 1, mxCreateDoubleScalar(iValues.theta_hzn[1]));
    mxSetFieldByNumber(plhs[index], 2, 0, mxCreateString("d_hzn_tx__meter"));
    mxSetFieldByNumber(plhs[index], 2, 1, mxCreateDoubleScalar(iValues.d_hzn__meter[0]));
    mxSetFieldByNumber(plhs[index], 3, 0, mxCreateString("d_hzn_rx__meter"));
    mxSetFieldByNumber(plhs[index], 3, 1, mxCreateDoubleScalar(iValues.d_hzn__meter[1]));
    mxSetFieldByNumber(plhs[index], 4, 0, mxCreateString("h_e_tx__meter"));
    mxSetFieldByNumber(plhs[index], 4, 1, mxCreateDoubleScalar(iValues.h_e__meter[0]));
    mxSetFieldByNumber(plhs[index], 5, 0, mxCreateString("h_e_rx__meter"));
    mxSetFieldByNumber(plhs[index], 5, 1, mxCreateDoubleScalar(iValues.h_e__meter[1]));
    mxSetFieldByNumber(plhs[index], 6, 0, mxCreateString("N_s"));
    mxSetFieldByNumber(plhs[index], 6, 1, mxCreateDoubleScalar(iValues.N_s));
    mxSetFieldByNumber(plhs[index], 7, 0, mxCreateString("delta_h__meter"));
    mxSetFieldByNumber(plhs[index], 7, 1, mxCreateDoubleScalar(iValues.delta_h__meter));
    mxSetFieldByNumber(plhs[index], 8, 0, mxCreateString("A_ref__db"));
    mxSetFieldByNumber(plhs[index], 8, 1, mxCreateDoubleScalar(iValues.A_ref__db));
    mxSetFieldByNumber(plhs[index], 9, 0, mxCreateString("A_fs__db"));
    mxSetFieldByNumber(plhs[index], 9, 1, mxCreateDoubleScalar(iValues.A_fs__db));
    mxSetFieldByNumber(plhs[index], 10, 0, mxCreateString("d__km"));
    mxSetFieldByNumber(plhs[index], 10, 1, mxCreateDoubleScalar(iValues.d__km));
    mxSetFieldByNumber(plhs[index], 11, 0, mxCreateString("mode"));
    mxSetFieldByNumber(plhs[index], 11, 1, mxCreateDoubleScalar(iValues.mode));
}