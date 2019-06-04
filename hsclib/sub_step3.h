#ifndef MORIIISM_HSC_HSCLIB_SUB_STEP3_H_
#define MORIIISM_HSC_HSCLIB_SUB_STEP3_H_

#include "hscdata.h"
#include "aucboost.h"

void MkTrVlTeLambdaDat(int nset,
                       int nlambda,
                       const double* const lambda_arr,
                       string outdir);

void RunAucboostStep3(string csvfile,
                      string csv_version,
                      string src_id_dir,
                      int niter,
                      string outdir);

#endif // MORIIISM_HSC_HSCLIB_SUB_STEP3_H_
