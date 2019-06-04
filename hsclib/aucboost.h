#ifndef MORIIISM_HSC_HSCLIB_AUCBOOST_H_
#define MORIIISM_HSC_HSCLIB_AUCBOOST_H_

#include "mib_blas.h"
#include "mir_math_util.h"

void Aucboost(int nftr,
              const double* const* const data_true_tr_arr, long ndata_true_tr,
              const double* const* const data_false_tr_arr, long ndata_false_tr,
              const double* const* const data_true_te_arr, long ndata_true_te,
              const double* const* const data_false_te_arr, long ndata_false_te,
              const string* const name_arr,
              int niter,
              double lambda,
              string outdir);

double GetPhi(double val);

double GetNot(double val);

void GenBFzMArr(int nftr,
                const double* const* const data_true_tr_arr, long ndata_true_tr,
                const double* const* const data_false_tr_arr, long ndata_false_tr,
                double*** const B_arr_ptr,
                int** const nb_arr_ptr,
                double*** const Fz_arr_ptr,
                double**** const M0_arr_ptr,
                double**** const M1_arr_ptr);

void DelBFzMArr(int nftr,
                double** B_arr,
                int* nb_arr,
                double** Fz_arr,
                double*** M0_arr,
                double*** M1_arr);

double GetScoreFuncVal(int npar,
                       const double* const a_arr,
                       const double* const b_arr,
                       const int* const p_arr,
                       const double* const alpha_arr,
                       int nftr, const double* const data_arr);

void GetMat01Arr(long n0, const double* const Mat0_arr,
                 long n1, const double* const Mat1_arr,
                 double* const Mat01_arr);


void MkBoostingParFile(int niter,
                       const double* const a_arr,
                       const double* const b_arr,
                       const int* const p_arr,
                       const double* const alpha_arr,
                       string outdir);

void MkRocFile(long n0, const double* const F0_arr,
               long n1, const double* const F1_arr,
               int npoint, string outdir, string tag);

void GenRoc(long n0, const double* const F0_arr,
            long n1, const double* const F1_arr,
            int npoint,
            double** const FPR_arr_ptr,
            double** const TPR_arr_ptr,
            double** const score_th_arr_ptr);

void MkAucFile(int niter, const double* const auc_arr,
               string outdir, string tag);

double GetAuc(long n0, const double* const F0_arr,
              long n1, const double* const F1_arr);

#endif // MORIIISM_HSC_HSCLIB_AUCBOOST_H_
