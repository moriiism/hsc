#include "aucboost.h"

// data_true_tr_arr[SRC][FTR]

// data_arr: ndata * nftr matrix
void Aucboost(int nftr,
              const double* const* const data_true_tr_arr, long ndata_true_tr,
              const double* const* const data_false_tr_arr, long ndata_false_tr,
              const double* const* const data_true_te_arr, long ndata_true_te,
              const double* const* const data_false_te_arr, long ndata_false_te,
              const string* const name_arr,
              int niter,
              double lambda,
              string outdir)
{
    printf(" --- aucboost ---- \n");
    printf("nftr   = %d\n", nftr);
    printf("ndata_true_tr = %ld\n", ndata_true_tr);
    printf("ndata_false_tr = %ld\n", ndata_false_tr);    
    printf("ndata_true_te = %ld\n", ndata_true_te);
    printf("ndata_false_te = %ld\n", ndata_false_te);    
    printf("niter   = %d\n", niter);
    printf("lambda  = %e\n", lambda);
    printf("outdir  = %s\n", outdir.c_str());
    printf(" --- aucboost ---- \n");

    long n0 = ndata_false_tr;
    long n1 = ndata_true_tr;
    long n0_te = ndata_false_te;
    long n1_te = ndata_true_te;

    // score value of each input data
    double* F0_arr = new double [n0];
    double* F1_arr = new double [n1];
    for(long idata = 0; idata < n0; idata ++){
        F0_arr[idata] = 0.0;
    }
    for(long idata = 0; idata < n1; idata ++){
        F1_arr[idata] = 0.0;
    }
    
    printf("--- GenBFzMArr\n");
    double** B_arr = NULL;
    int* nb_arr = NULL;
    double** Fz_arr = NULL;
    double*** M0_arr = NULL;
    double*** M1_arr = NULL;
    GenBFzMArr(nftr,
               data_true_tr_arr, ndata_true_tr,
               data_false_tr_arr, ndata_false_tr,
               &B_arr, &nb_arr, &Fz_arr,
               &M0_arr, &M1_arr);
    printf("=== GenBFzMArr\n");
    
    double* a_arr = new double [niter]; // +-1
    double* b_arr = new double [niter]; // threshold
    int*    p_arr = new int    [niter]; // index of feature
    double* alpha_arr = new double [niter]; // weight
    for(int iiter = 0; iiter < niter; iiter++){
        a_arr[iiter] = 0.0;
        b_arr[iiter] = 0.0;
        p_arr[iiter] = 0;
        alpha_arr[iiter] = 0.0;
    }
    double* auc_te_arr = new double [niter];
    double* auc_tr_arr = new double [niter];
    
    // boosting loop
    for(int iiter = 0; iiter < niter; iiter++){
        // printf("iiter (niter) = %d (%d)\n", iiter, niter);

        double* F01_arr = new double [n0 * n1];
        double* phi_arr = new double [n0 * n1];
        for(long iarr = 0; iarr < n0; iarr++){
            for(long jarr = 0; jarr < n1; jarr++){
                long ij_index = iarr + jarr * n0;
                F01_arr[ij_index] = F1_arr[jarr] - F0_arr[iarr];
                phi_arr[ij_index] = GetPhi(F01_arr[ij_index]);
            }
        }
        double** d_arr = new double* [nftr];
        for(int iftr = 0; iftr < nftr; iftr ++){
            d_arr[iftr] = new double[nb_arr[iftr]];
            for(int ib = 0; ib < nb_arr[iftr]; ib ++){
                
                double* M01_arr = new double [n0 * n1];
                for(long iarr = 0; iarr < n0; iarr++){
                    for(long jarr = 0; jarr < n1; jarr++){
                        long ij_index = iarr + jarr * n0;
                        M01_arr[ij_index] = M1_arr[iftr][ib][jarr] - M0_arr[iftr][ib][iarr];
                    }
                }
                d_arr[iftr][ib] = ddot_(n0 * n1, phi_arr, 1, M01_arr, 1);
                d_arr[iftr][ib] /= (n0 * n1);

                // regularization term
                double reg = Fz_arr[iftr][ib]
                    - 3 * Fz_arr[iftr][ib + 1]
                    + 3 * Fz_arr[iftr][ib + 2]
                    - Fz_arr[iftr][ib + 3];
                d_arr[iftr][ib] -= 2.0 * lambda * reg;

                delete [] M01_arr;
            }
        }

        double* dmin_arr = new double [nftr];
        double* dmax_arr = new double [nftr];
        int* index_dmin_arr = new int [nftr];
        int* index_dmax_arr = new int [nftr];
        for(int iftr = 0; iftr < nftr; iftr ++){
            dmin_arr[iftr] = MirMath::GetMin(nb_arr[iftr], d_arr[iftr]);
            dmax_arr[iftr] = MirMath::GetMax(nb_arr[iftr], d_arr[iftr]);
            index_dmin_arr[iftr] = MirMath::GetLocMin(nb_arr[iftr], d_arr[iftr]);
            index_dmax_arr[iftr] = MirMath::GetLocMax(nb_arr[iftr], d_arr[iftr]);
        }
        double dmin = MirMath::GetMin(nftr, dmin_arr);
        double dmax = MirMath::GetMax(nftr, dmax_arr);
        int index_dmin = MirMath::GetLocMin(nftr, dmin_arr);
        int index_dmax = MirMath::GetLocMax(nftr, dmax_arr);

        int ib_index = 0;
        if(dmax >= -dmin){
            a_arr[iiter] = 1;
            p_arr[iiter] = index_dmax;
            ib_index = index_dmax_arr[index_dmax];
        } else{
            a_arr[iiter] = -1;
            p_arr[iiter] = index_dmin;
            ib_index = index_dmin_arr[index_dmin];
        }

        b_arr[iiter] = B_arr[p_arr[iiter]][ib_index];

        // for alpha update
        double al = 0.0;
        if(iiter < 3){
            al = 1.0;
        } else {
            double* tmp_arr = new double [3];
            tmp_arr[0] = alpha_arr[iiter - 3];
            tmp_arr[1] = alpha_arr[iiter - 2];
            tmp_arr[2] = alpha_arr[iiter - 1];
            al = MirMath::GetMedian(3, tmp_arr);
            delete [] tmp_arr;
        }
        double al0 = 0.0;
        int inewton = 0;


        double* M01_arr = new double [n0 * n1];
        for(long iarr = 0; iarr < n0; iarr++){
            for(long jarr = 0; jarr < n1; jarr++){
                long ij_index = iarr + jarr * n0;
                M01_arr[ij_index] =
                    M1_arr[p_arr[iiter]][ib_index][jarr] -
                    M0_arr[p_arr[iiter]][ib_index][iarr];
            }
        }
        
        while( fabs( al - al0) > 1.0e-6 && inewton < 5){
            al0 = al;
            double d1 = 0.0;
            double d2 = 0.0;
            for(long iarr = 0; iarr < n0; iarr++){
                for(long jarr = 0; jarr < n1; jarr++){
                    long ij_index = iarr + jarr * n0;
                    double exponent = F01_arr[ij_index]
                        + al0 * a_arr[iiter] * M01_arr[ij_index];
                    double phi = GetPhi(exponent);
                    d1 += phi * a_arr[iiter] * M01_arr[ij_index];
                    d2 -= exponent * phi * pow(M01_arr[ij_index], 2);
                }
            }
            d1 /= n0 * n1;
            d2 /= n0 * n1;
            double dw1 = 2 * ( a_arr[iiter]
                        * ( Fz_arr[p_arr[iiter]][ib_index] - 3 * Fz_arr[p_arr[iiter]][ib_index + 1]
                            + 3 * Fz_arr[p_arr[iiter]][ib_index + 2] - Fz_arr[p_arr[iiter]][ib_index + 3] )
                        + 2 * al0);
            double dw2 = 4.0;
            al = al0 - (d1 - lambda * dw1) / (d2 - lambda * dw2);
            inewton ++;
        }
        delete [] M01_arr;
        
        alpha_arr[iiter] = al;
        
        if(1 == a_arr[iiter]){
            for(long iarr = 0; iarr < n0; iarr++){
                F0_arr[iarr] += al * M0_arr[p_arr[iiter]][ib_index][iarr];
            }
            for(long jarr = 0; jarr < n1; jarr++){
                F1_arr[jarr] += al * M1_arr[p_arr[iiter]][ib_index][jarr];
            }
            int nfz = nb_arr[p_arr[iiter]] + 3;
            for(int ifz = ib_index + 2; ifz < nfz; ifz ++){
                Fz_arr[p_arr[iiter]][ifz] += al;
            }
        } else{
            for(long iarr = 0; iarr < n0; iarr++){
                F0_arr[iarr] += al * GetNot(M0_arr[p_arr[iiter]][ib_index][iarr]);
            }
            for(long jarr = 0; jarr < n1; jarr++){
                F1_arr[jarr] += al * GetNot(M1_arr[p_arr[iiter]][ib_index][jarr]);
            }
            for(int ifz = 0; ifz <= ib_index + 1; ifz ++){
                Fz_arr[p_arr[iiter]][ifz] += al;
            }
        }


        int npoint = 1000;
        double* F0_te_arr = new double [n0_te];
        double* F1_te_arr = new double [n1_te];
        for(long idata = 0; idata < n0_te; idata ++){
            F0_te_arr[idata] = GetScoreFuncVal(iiter + 1,
                                               a_arr, b_arr, p_arr, alpha_arr,
                                               nftr, data_false_te_arr[idata]);
        }
        for(long idata = 0; idata < n1_te; idata ++){
            F1_te_arr[idata] = GetScoreFuncVal(iiter + 1,
                                               a_arr, b_arr, p_arr, alpha_arr,
                                               nftr, data_true_te_arr[idata]);
        }
        auc_te_arr[iiter] = GetAuc(n0_te, F0_te_arr,
                                   n1_te, F1_te_arr);
        auc_tr_arr[iiter] = GetAuc(n0, F0_arr,
                                   n1, F1_arr);
        MkRocFile(n0_te, F0_te_arr, n1_te, F1_te_arr, npoint, outdir, "te");
        MkRocFile(n0, F0_arr, n1, F1_arr, npoint, outdir, "tr");
        
        printf("%s: iiter = %2.2d: auc_tr = %f: auc_te = %f: "
               "a, b, p, alpha = %+3.3f  %+3.3f  %2d  %+3.3f \n",
               outdir.c_str(), iiter, auc_tr_arr[iiter], auc_te_arr[iiter],
               a_arr[iiter], b_arr[iiter], p_arr[iiter], alpha_arr[iiter]);


        delete [] F01_arr;
        delete [] phi_arr;
        for(int iftr = 0; iftr < nftr; iftr ++){
            delete [] d_arr[iftr];
        }
        delete [] d_arr;

        delete [] dmin_arr;
        delete [] dmax_arr;
        delete [] index_dmin_arr;
        delete [] index_dmax_arr;

        delete [] F0_te_arr;
        delete [] F1_te_arr;
    }

    int npoint = 1000;
    double* F0_te_arr = new double [n0_te];
    double* F1_te_arr = new double [n1_te];
    for(long idata = 0; idata < n0_te; idata ++){
        F0_te_arr[idata] = GetScoreFuncVal(niter,
                                           a_arr, b_arr, p_arr, alpha_arr,
                                           nftr, data_false_te_arr[idata]);
    }
    for(long idata = 0; idata < n1_te; idata ++){
        F1_te_arr[idata] = GetScoreFuncVal(niter,
                                           a_arr, b_arr, p_arr, alpha_arr,
                                           nftr, data_true_te_arr[idata]);
    }
    MkRocFile(n0_te, F0_te_arr, n1_te, F1_te_arr, npoint, outdir, "te");
    delete [] F0_te_arr;
    delete [] F1_te_arr;
    
    MkBoostingParFile(niter, a_arr, b_arr, p_arr, alpha_arr, outdir);
    MkRocFile(n0, F0_arr, n1, F1_arr, npoint, outdir, "tr");
    
    
    MkAucFile(niter, auc_te_arr, outdir, "te");
    MkAucFile(niter, auc_tr_arr, outdir, "tr");
    

    delete [] F0_arr;
    delete [] F1_arr;
    
    DelBFzMArr(nftr, B_arr, nb_arr, Fz_arr,
               M0_arr, M1_arr);
    delete [] a_arr;
    delete [] b_arr;
    delete [] p_arr;
    delete [] alpha_arr;

    delete [] auc_te_arr;
    delete [] auc_tr_arr;

    printf("=== aucboost ===\n");
}

double GetPhi(double val)
{
    double phi = exp( - val * val / 2.0) / sqrt(2.0 * M_PI);
    return(phi);
}

double GetNot(double val)
{
    double ans = val * (-1) + 1;
    return(ans); 
}

void GenBFzMArr(int nftr,
                const double* const* const data_true_tr_arr, long ndata_true_tr,
                const double* const* const data_false_tr_arr, long ndata_false_tr,
                double*** const B_arr_ptr,
                int** const nb_arr_ptr,
                double*** const Fz_arr_ptr,
                double**** const M0_arr_ptr,
                double**** const M1_arr_ptr)
{
    int n0_quantile = 10; // false
    int n1_quantile = 30; // true
    double* prob0_arr = new double [n0_quantile];
    double* prob1_arr = new double [n1_quantile];
    for(int iqtl = 0; iqtl < n0_quantile; iqtl ++){
        prob0_arr[iqtl] = (double) (iqtl + 1) / n0_quantile;
        printf("%f ", prob0_arr[iqtl]);
    }
    printf("\n");
    for(int iqtl = 0; iqtl < n1_quantile; iqtl ++){
        prob1_arr[iqtl] = (double) (iqtl + 1) / n1_quantile;
        printf("%f ", prob1_arr[iqtl]);
    }
    printf("\n");
    
    double** B0_arr = new double* [nftr];
    double** B1_arr = new double* [nftr];
    for(int iftr = 0; iftr < nftr; iftr++){
        double* data_true_tmp = new double[ndata_true_tr];
        for(long idata = 0; idata < ndata_true_tr; idata ++){
            data_true_tmp[idata] = data_true_tr_arr[idata][iftr];
        }
        double* data_false_tmp = new double[ndata_false_tr];
        for(long idata = 0; idata < ndata_false_tr; idata ++){
            data_false_tmp[idata] = data_false_tr_arr[idata][iftr];
        }

        B0_arr[iftr] = new double [n0_quantile];
        B1_arr[iftr] = new double [n1_quantile];
        for(int iqtl = 0; iqtl < n0_quantile; iqtl ++){
            B0_arr[iftr][iqtl] = 0.0;
        }
        for(int iqtl = 0; iqtl < n1_quantile; iqtl ++){
            B1_arr[iftr][iqtl] = 0.0;
        }
        TMath::Quantiles(ndata_false_tr, n0_quantile, data_false_tmp,
                         B0_arr[iftr], prob0_arr, kFALSE);
        TMath::Quantiles(ndata_true_tr, n1_quantile, data_true_tmp,
                         B1_arr[iftr], prob1_arr, kFALSE);
        //MirMathUtil::GetQuantile(ndata_false_tr, data_false_tmp,
        //                         n0_quantile,
        //                         &B0_arr[iftr]);
        //MirMathUtil::GetQuantile(ndata_true_tr, data_true_tmp,
        //                         n1_quantile,
        //                         &B1_arr[iftr]);
        delete [] data_true_tmp;
        delete [] data_false_tmp;
    }

    delete [] prob0_arr;
    delete [] prob1_arr;
    
    double** B_arr  = new double* [nftr];
    int* nb_arr    = new int [nftr];
    for(int iftr = 0; iftr < nftr; iftr++){
        int ntmp = n0_quantile + n1_quantile;
        double* tmp_arr = new double [ntmp];
        int itmp = 0;
        for(int iqtl = 0; iqtl < n0_quantile; iqtl ++){
            tmp_arr[itmp] = B0_arr[iftr][iqtl];
            itmp ++;
        }
        for(int iqtl = 0; iqtl < n1_quantile; iqtl ++){
            tmp_arr[itmp] = B1_arr[iftr][iqtl];
            itmp ++;
        }
        long nuniq = 0;
        double* uniq_arr = NULL;
        double epsilon = 1e-10;
        MirMathUtil::Uniq(ntmp, tmp_arr,
                          &nuniq, &uniq_arr,
                          epsilon);
        nb_arr[iftr] = nuniq;
        B_arr[iftr] = new double [nuniq];
        for(long iuniq = 0; iuniq < nuniq; iuniq ++){
            B_arr[iftr][iuniq] = uniq_arr[iuniq];
        }
        delete [] tmp_arr;
        delete [] uniq_arr;
    }
    for(int iftr = 0; iftr < nftr; iftr++){
        delete [] B0_arr[iftr];
        delete [] B1_arr[iftr];
    }
    delete [] B0_arr;
    delete [] B1_arr;

    // dump
    for(int iftr = 0; iftr < nftr; iftr++){
        printf("iftr = %d : nb = %d : ", iftr, nb_arr[iftr]);
        for(long iuniq = 0; iuniq < nb_arr[iftr]; iuniq ++){
            printf("%.3f ", B_arr[iftr][iuniq]);
        }
        printf("\n");
    }

    // Fz_arr
    double** Fz_arr  = new double* [nftr];
    for(int iftr = 0; iftr < nftr; iftr++){
        int nfz = nb_arr[iftr] + 3;
        Fz_arr[iftr] = new double [nfz];
        for(int ifz = 0; ifz < nfz; ifz ++){
            Fz_arr[iftr][ifz] = 0.0;
        }
    }

    long n0 = ndata_false_tr;
    long n1 = ndata_true_tr;
    
    // M0_arr
    double*** M0_arr = new double** [nftr];
    for(int iftr = 0; iftr < nftr; iftr++){
        M0_arr[iftr] = new double* [nb_arr[iftr]];
        for(int ib = 0; ib < nb_arr[iftr]; ib ++){
            M0_arr[iftr][ib] = new double [n0];
            for(long idata = 0; idata < n0; idata ++){
                if(data_false_tr_arr[idata][iftr] >= B_arr[iftr][ib]){
                    M0_arr[iftr][ib][idata] = 1.0;
                } else {
                    M0_arr[iftr][ib][idata] = 0.0;
                }
            }
        }
    }

    // M1_arr
    double*** M1_arr = new double** [nftr];
    for(int iftr = 0; iftr < nftr; iftr++){
        M1_arr[iftr] = new double* [nb_arr[iftr]];
        for(int ib = 0; ib < nb_arr[iftr]; ib ++){
            M1_arr[iftr][ib] = new double [n1];
            for(long idata = 0; idata < n1; idata ++){
                if(data_true_tr_arr[idata][iftr] >= B_arr[iftr][ib]){
                    M1_arr[iftr][ib][idata] = 1.0;
                } else {
                    M1_arr[iftr][ib][idata] = 0.0;
                }
            }
        }
    }


    *B_arr_ptr = B_arr;
    *nb_arr_ptr = nb_arr;
    *Fz_arr_ptr = Fz_arr;
    *M0_arr_ptr = M0_arr;
    *M1_arr_ptr = M1_arr;
}

void DelBFzMArr(int nftr,
                double** B_arr,
                int* nb_arr,
                double** Fz_arr,
                double*** M0_arr,
                double*** M1_arr)
{
    for(int iftr = 0; iftr < nftr; iftr++){
        delete [] B_arr[iftr];
    }
    delete [] B_arr;

    for(int iftr = 0; iftr < nftr; iftr++){
        delete [] Fz_arr[iftr];
    }
    delete Fz_arr;

    for(int iftr = 0; iftr < nftr; iftr++){
        for(int ib = 0; ib < nb_arr[iftr]; ib ++){
            delete [] M0_arr[iftr][ib];
        }
        delete [] M0_arr[iftr];
    }
    delete [] M0_arr;

    for(int iftr = 0; iftr < nftr; iftr++){
        for(int ib = 0; ib < nb_arr[iftr]; ib ++){
            delete [] M1_arr[iftr][ib];
        }
        delete [] M1_arr[iftr];
    }
    delete [] M1_arr;

    delete [] nb_arr;
}

double GetScoreFuncVal(int npar,
                       const double* const a_arr,
                       const double* const b_arr,
                       const int* const p_arr,
                       const double* const alpha_arr,
                       int nftr, const double* const data_arr)
{
    double val = 0.0;
    for(int ipar = 0; ipar < npar; ipar++){
        double hev = 0.0;
        if(data_arr[ p_arr[ipar] ] - b_arr[ipar] >= 0){
            hev = 1.0;
        } else {
            hev = 0.0;
        }
        val += alpha_arr[ipar]
            * (a_arr[ipar] * hev + (1.0 - a_arr[ipar])/2.0);
    }
    return(val);
}

void GetMat01Arr(long n0, const double* const Mat0_arr,
                 long n1, const double* const Mat1_arr,
                 double* const Mat01_arr)
{
    for(long iarr = 0; iarr < n0; iarr++){
        for(long jarr = 0; jarr < n1; jarr++){
            long ij_index = iarr + jarr * n0;
            Mat01_arr[ij_index] = Mat1_arr[jarr] - Mat0_arr[iarr];
        }
    }
}

void MkBoostingParFile(int niter,
                       const double* const a_arr,
                       const double* const b_arr,
                       const int* const p_arr,
                       const double* const alpha_arr,
                       string outdir)
{
    char boosting_par_file[kLineSize];
    sprintf(boosting_par_file, "%s/boosting_par.dat", outdir.c_str());
    FILE* fp_boosting_par = fopen(boosting_par_file, "w");
    for(int ipar = 0; ipar < niter; ipar ++){
        fprintf(fp_boosting_par, "%+3.3f  %+3.3f  %2d  %+3.3f\n",
                a_arr[ipar], b_arr[ipar], p_arr[ipar], alpha_arr[ipar]);
    }
    fclose(fp_boosting_par);
}


void MkRocFile(long n0, const double* const F0_arr,
               long n1, const double* const F1_arr,
               int npoint, string outdir, string tag)
{
    double* FPR_arr = NULL;
    double* TPR_arr = NULL;
    double* score_th_arr = NULL;
    GenRoc(n0, F0_arr,
           n1, F1_arr,
           npoint,
           &FPR_arr, &TPR_arr, &score_th_arr);
        
    char roc_file[kLineSize];
    sprintf(roc_file, "%s/roc_%s.dat", outdir.c_str(), tag.c_str());
    FILE* fp_roc = fopen(roc_file, "w");
    for(long ipoint = 0; ipoint < npoint; ipoint ++){
        fprintf(fp_roc, "%e %e\n", FPR_arr[ipoint], TPR_arr[ipoint]);
    }
    fclose(fp_roc);
    delete [] FPR_arr;
    delete [] TPR_arr;
    delete [] score_th_arr;
}

void GenRoc(long n0, const double* const F0_arr,
            long n1, const double* const F1_arr,
            int npoint,
            double** const FPR_arr_ptr,
            double** const TPR_arr_ptr,
            double** const score_th_arr_ptr)
{
    long n01 = n0 + n1;
    double* F01_arr = new double [n01];
    long idata = 0;
    for(long iarr = 0; iarr < n0; iarr ++){
        F01_arr[idata] = F0_arr[iarr];
        idata ++;
    }
    for(long iarr = 0; iarr < n1; iarr ++){
        F01_arr[idata] = F1_arr[iarr];
        idata ++;
    }
    double F01_min = MirMath::GetMin(n01, F01_arr);
    double F01_max = MirMath::GetMax(n01, F01_arr);
    delete [] F01_arr;

    long nbin = npoint - 1;
    double delta = (F01_max - F01_min) / (nbin - 1);
    
    double* FPR_arr = new double [npoint];
    double* TPR_arr = new double [npoint];
    double* score_th_arr = new double [npoint];
    
    double F01_lo = F01_min - delta / 2.0;
    // double F01_up = F01_max + delta / 2.0;
    for(long ipoint = 0; ipoint < npoint; ipoint ++){
        double F01_th = F01_lo + delta * ipoint;
        FPR_arr[ipoint] = 0.0;
        for(long idata = 0; idata < n0; idata ++){
            if(F01_th <= F0_arr[idata]){
                FPR_arr[ipoint] ++;
            }
        }
        FPR_arr[ipoint] /= n0;

        TPR_arr[ipoint] = 0.0;
        for(long idata = 0; idata < n1; idata ++){
            if(F01_th <= F1_arr[idata]){
                TPR_arr[ipoint] ++;
            }
        }
        TPR_arr[ipoint] /= n1;
        score_th_arr[ipoint] = F01_th;
    }

    *FPR_arr_ptr = FPR_arr;
    *TPR_arr_ptr = TPR_arr;
    *score_th_arr_ptr = score_th_arr;
}

void MkAucFile(int niter, const double* const auc_arr,
               string outdir, string tag)
{
    char auc_iter_file[kLineSize];
    sprintf(auc_iter_file, "%s/auc-iter_%s.dat", outdir.c_str(), tag.c_str());
    FILE* fp = fopen(auc_iter_file, "w");
    for(int ipar = 0; ipar < niter; ipar ++){
        fprintf(fp, "%e\n", auc_arr[ipar]);
    }
    fclose(fp);

    char auc_file[kLineSize];
    sprintf(auc_file, "%s/auc_%s.dat", outdir.c_str(), tag.c_str());
    fp = fopen(auc_file, "w");
    fprintf(fp, "%e\n", auc_arr[niter - 1]);
    fclose(fp);
}


double GetAuc(long n0, const double* const F0_arr,
              long n1, const double* const F1_arr)
{
    double auc = 0.0;
    for(long iarr = 0; iarr < n0; iarr++){
        for(long jarr = 0; jarr < n1; jarr++){
            double hev = 0.0;
            if(F1_arr[jarr] - F0_arr[iarr] > 0){
                hev = 1.0;
            } else if(F1_arr[jarr] - F0_arr[iarr] == 0){
                hev = 0.5;
            } else {
                hev = 0.0;
            }
            auc += hev;
        }
    }
    auc /= (n0 * n1);
    return(auc);
}

