#include "mex.h"
#include "test.h"

const char *err_InvalidNumberOfInputs = ":InvalidNumberOfInputs";
const char *err_InvalidInputType = ":InvalidInputType";
const char *base_name = "MATLAB:ITM:";

const char* msg_start = "Value of '";
const char* msg_end = "' must be a number";

const char* base_area_vars[] = {"h_tx__meter", 
                                "h_rx__meter", 
                                "tx_site_criteria",
                                "rx_site_criteria",
                                "d__km",
                                "delta_h__meter",
                                "climate",
                                "N_0",
                                "f__mhz",
                                "pol",
                                "epsilon",
                                "sigma",
                                "mdvar"
                               };
const char* tls_area_vars[] = {"time", "location", "situation"};
const char* cr_area_vars[] = {"confidence", "reliability"};

CallITM_AREA_VALIDATION(int nrhs, const mxArray *prhs[], const char* fn_name, int mode){
    char *root_str, *full_id;
    root_str = mxCalloc(strlen(base_name) + strlen(fn_name) + 1, sizeof(char));

    strcat(root_str, base_name);
    strcat(root_str, fn_name);

    // validate input arguments
    if (nrhs != 16+1) { // +1 cause first array element is function name
        // build complete errID
        full_id = mxCalloc(strlen(root_str) + strlen(err_InvalidNumberOfInputs), sizeof(char));
        strcat(full_id, root_str);
        strcat(full_id, err_InvalidNumberOfInputs);

        mexErrMsgIdAndTxt(full_id, 
                          "Function takes 16 input.\nITM_AREA_TLS_Ex("
                          "double h_tx__meter, double h_rx__meter, "
                          "int tx_site_criteria, int rx_site_criteria, "
                          "double d__km, double delta_h__meter, "
                          "int climate, double N_0, double f__mhz, "
                          "int pol, double epsilon, double sigma, "
                          "int mdvar, double time, double location, "
                          "double situation)");
    }
    else
    {
        char *full_msg;

        // build complete errID
        full_id = mxCalloc(strlen(root_str) + strlen(err_InvalidInputType), sizeof(char));
        strcat(full_id, root_str);
        strcat(full_id, err_InvalidInputType);

        // loop through base area mode variables
        int baselen = sizeof(base_area_vars) / sizeof(base_area_vars[0]);
        for (int i = 0; i < baselen; i++)
        {
            if (!mxIsNumeric(prhs[i + 1])) {
                // build error msg
                full_msg = mxCalloc(strlen(msg_start) + strlen(base_area_vars[i]) + strlen(msg_end) + 1, sizeof(char));
                strcat(full_msg, msg_start);
                strcat(full_msg, base_area_vars[i]);
                strcat(full_msg, msg_end);

                // throw error
                mexErrMsgIdAndTxt(full_id, full_msg);
            }
        }

        if (mode == AREA_MODE_TLS) {
            // loop through TLS area mode variables
            int tlslen = sizeof(tls_area_vars) / sizeof(tls_area_vars[0]);
            for (int i = baselen; i < tlslen + baselen; i++)
            {
                if (!mxIsNumeric(prhs[i + 1])) {
                    // build error msg
                    full_msg = mxCalloc(strlen(msg_start) + strlen(tls_area_vars[i - baselen]) + strlen(msg_end) + 1, sizeof(char));
                    strcat(full_msg, msg_start);
                    strcat(full_msg, tls_area_vars[i - baselen]);
                    strcat(full_msg, msg_end);
    
                    // throw error
                    mexErrMsgIdAndTxt(full_id, full_msg);
                }
            }
        }
        else {
            // loop through CR area mode variables
            int crlen = sizeof(cr_area_vars) / sizeof(cr_area_vars[0]);
            for (int i = baselen; i < crlen + baselen; i++)
            {
                if (!mxIsNumeric(prhs[i + 1])) {
                    // build error msg
                    full_msg = mxCalloc(strlen(msg_start) + strlen(cr_area_vars[i - baselen]) + strlen(msg_end) + 1, sizeof(char));
                    strcat(full_msg, msg_start);
                    strcat(full_msg, cr_area_vars[i - baselen]);
                    strcat(full_msg, msg_end);
    
                    // throw error
                    mexErrMsgIdAndTxt(full_id, full_msg);
                }
            }
        }
    }

    // no errors
    mxFree(root_str);
    mxFree(full_id);
}