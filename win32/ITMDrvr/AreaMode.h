

//
// FUNCTIONS
///////////////////////////////////////////////
int ParseAreaInputFile(const char* in_file, AreaParams* area_params);
int ValidateAreaInputs(AreaParams* area_params);
int CallAreaMode(DrvrParams* params, AreaParams* area_params, IntermediateValues* inter_vals, 
    vector<double>* A__db, vector<double>* A_fs__db, long* warnings);
void WriteAreaInputs(FILE* fp, AreaParams* area_params);
int LoadAreaFunctions(HINSTANCE hLib);
