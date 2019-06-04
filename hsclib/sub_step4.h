#ifndef MORIIISM_HSC_HSCLIB_SUB_STEP4_H_
#define MORIIISM_HSC_HSCLIB_SUB_STEP4_H_

#include "hscdata.h"
#include "aucboost.h"

void RunAucboostStep4(string csvfile,
                      string csv_version,
                      int niter,
                      double lambda,
                      string outdir);

#endif // MORIIISM_HSC_HSCLIB_SUB_STEP4_H_
