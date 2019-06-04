#ifndef MORIIISM_HSC_HSCLIB_SUB_STEP2_H_
#define MORIIISM_HSC_HSCLIB_SUB_STEP2_H_

#include "mir_qdp_tool.h"
#include "mifc_graph2d.h"
#include "hscdata.h"
#include "aucboost.h"


void LoadLambdaArr(string lambda_file,
                   int* const nlambda_ptr,
                   double** const lambda_arr_ptr);

void MkTrVlTeLambdaDat(int nset,
                       const double* const lambda_arr,
                       string outdir);

void RunAucboostStep2(string csvfile,
                      string csv_version,
                      string src_id_dir,
                      int niter,
                      string outdir);

void MkRocAve(int nset, string outdir);

#endif // MORIIISM_HSC_HSCLIB_SUB_STEP2_H_
