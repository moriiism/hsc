#ifndef MORIIISM_HSC_HSCLIB_SUB_STEP_H_
#define MORIIISM_HSC_HSCLIB_SUB_STEP_H_

#include "mi_iolib.h"
#include "mir_binning.h"

void OpenLogfile(string outdir,
                 string outfile_head,
                 string progname,
                 FILE** const fp_log_ptr);

void LoadLambdaDat(string lambda_dat,
                   int* const nlambda_ptr,
                   double** const lambda_arr_ptr);

#endif // MORIIISM_HSC_HSCLIB_SUB_STEP_H_
