//
// step1.cc
//
// 2019.05.16 M.Morii
//

#include "mi_time.h"
#include "sub_step.h"
#include "sub_step1.h"
#include "arg_step1.h"

// global variable 
int g_flag_debug = 0;
int g_flag_help = 0;
int g_flag_verbose = 0;
  
int main(int argc, char* argv[])
{
    int status_prog = kRetNormal;

    double time_st = MiTime::GetTimeSec();
    
    ArgValStep1* argval = new ArgValStep1;
    argval->Init(argc, argv);
    argval->Print(stdout);

    FILE* fp_log = NULL;
    OpenLogfile(argval->GetOutdir(),
                "log",
                argval->GetProgname(),
                &fp_log);
    argval->Print(fp_log);
    
    int nlambda = 0;
    double* lambda_arr = NULL;
    LoadLambdaDat(argval->GetLambdaDat(), &nlambda, &lambda_arr);
    printf("nlambda = %d\n", nlambda);
    
    MkTrVlLambdaDat(argval->GetNset(), nlambda, lambda_arr, argval->GetOutdir());

    RunAucboostStep1(argval->GetCsvfile(),
                     argval->GetCsvVersion(),
                     argval->GetSourceIdDir(),
                     argval->GetNiter(),
                     argval->GetOutdir());
    
    int nfile_te   = argval->GetNset();
    int nfile_trvl = argval->GetNset();
    double* lambda_best_arr = new double [nfile_te];
    for(int index_te = 0; index_te < nfile_te; index_te ++){
        double* lambda_aucmax_arr = new double [nfile_trvl];
        for(int index_trvl = 0; index_trvl < nfile_trvl; index_trvl ++){
            char tetr_dir[kLineSize];
            sprintf(tetr_dir, "cv%2.2d-%2.2d-tr.csv", index_te, index_trvl);
            double* auc_arr = new double [nlambda];
            for(int ilambda = 0; ilambda < nlambda; ilambda ++){
                char lambda_dir[kLineSize];
                sprintf(lambda_dir, "lambda_%2.2d_%.1e", ilambda, lambda_arr[ilambda]);
                char auc_dat[kLineSize];
                sprintf(auc_dat, "%s/%s/%s/auc_te.dat", argval->GetOutdir().c_str(), tetr_dir, lambda_dir);
                int ntmp = 0;
                string* tmp_arr = NULL;
                MiIolib::GenReadFileOfHeadLine(auc_dat, &tmp_arr, &ntmp);
                auc_arr[ilambda] = atof(tmp_arr[0].c_str());
                delete [] tmp_arr;
            }
            int index_auc_max = MirMath::GetLocMax(nlambda, auc_arr);
            lambda_aucmax_arr[index_trvl] = lambda_arr[index_auc_max];

            char lambda_aucmax_file[kLineSize];
            sprintf(lambda_aucmax_file, "%s/%s/aucmax.dat", argval->GetOutdir().c_str(), tetr_dir);
            FILE* fp_aucmax = fopen(lambda_aucmax_file, "w");
            fprintf(fp_aucmax, "%d  %e\n", index_auc_max, lambda_aucmax_arr[index_trvl]);
            fclose(fp_aucmax);
            delete [] auc_arr;
        }
        lambda_best_arr[index_te] = MirMathUtil::GetAMeanLog10(nfile_trvl, lambda_aucmax_arr);
        
        char te_lambda_file[kLineSize];
        sprintf(te_lambda_file, "%s/cv%2.2d.lambda", argval->GetOutdir().c_str(), index_te);
        FILE* fp_te_lambda = fopen(te_lambda_file, "w");
        fprintf(fp_te_lambda, "%e\n", lambda_best_arr[index_te]);
        fclose(fp_te_lambda);
        delete [] lambda_aucmax_arr;
    }
    char lambda_best_file[kLineSize];
    sprintf(lambda_best_file, "%s/lambda.dat", argval->GetOutdir().c_str());
    FILE* fp_lambda_best = fopen(lambda_best_file, "w");
    for(int index_te = 0; index_te < nfile_te; index_te ++){    
        fprintf(fp_lambda_best, "%d  %.5e\n", index_te, lambda_best_arr[index_te]);
    }
    fclose(fp_lambda_best);
    delete [] lambda_best_arr;
    
    double time_ed = MiTime::GetTimeSec();
    printf("time_ed - time_st = %e\n", time_ed - time_st);
    
    return status_prog;
}






