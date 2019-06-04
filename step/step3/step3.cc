//
// step3.cc
//
// 2019.05.16 M.Morii
//

#include "mi_time.h"
#include "sub_step.h"
#include "sub_step3.h"
#include "arg_step3.h"

// global variable 
int g_flag_debug = 0;
int g_flag_help = 0;
int g_flag_verbose = 0;
  
int main(int argc, char* argv[])
{
    int status_prog = kRetNormal;

    double time_st = MiTime::GetTimeSec();
    
    ArgValStep3* argval = new ArgValStep3;
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
    
    MkTrVlTeLambdaDat(argval->GetNset(),
                      nlambda, lambda_arr,
                      argval->GetOutdir());
    
    RunAucboostStep3(argval->GetCsvfile(),
                     argval->GetCsvVersion(),
                     argval->GetSourceIdDir(),
                     argval->GetNiter(),
                     argval->GetOutdir());

    int nfile_te   = argval->GetNset();
    double* lambda_aucmax_arr = new double [nfile_te];
    for(int index_te = 0; index_te < nfile_te; index_te ++){
        char te_dir[kLineSize];
        sprintf(te_dir, "cv%2.2d-te.csv", index_te);
        double* auc_arr = new double [nlambda];
        for(int ilambda = 0; ilambda < nlambda; ilambda ++){
            char lambda_dir[kLineSize];
            sprintf(lambda_dir, "lambda_%2.2d_%.1e", ilambda, lambda_arr[ilambda]);
            char auc_dat[kLineSize];
            sprintf(auc_dat, "%s/%s/%s/auc_te.dat", argval->GetOutdir().c_str(), te_dir, lambda_dir);
            int ntmp = 0;
            string* tmp_arr = NULL;
            MiIolib::GenReadFileOfHeadLine(auc_dat, &tmp_arr, &ntmp);
            auc_arr[ilambda] = atof(tmp_arr[0].c_str());
            delete [] tmp_arr;
        }
        int index_auc_max = MirMath::GetLocMax(nlambda, auc_arr);
        lambda_aucmax_arr[index_te] = lambda_arr[index_auc_max];

        char lambda_aucmax_file[kLineSize];
        sprintf(lambda_aucmax_file, "%s/%s/aucmax.dat", argval->GetOutdir().c_str(), te_dir);
        FILE* fp_aucmax = fopen(lambda_aucmax_file, "w");
        fprintf(fp_aucmax, "%d  %e\n", index_auc_max, lambda_aucmax_arr[index_te]);
        fclose(fp_aucmax);
        delete [] auc_arr;
    }

    double lambda_mean = MirMathUtil::GetAMeanLog10(nfile_te, lambda_aucmax_arr);
    char lambda_mean_file[kLineSize];
    sprintf(lambda_mean_file, "%s/lambda.dat", argval->GetOutdir().c_str());
    FILE* fp_lambda_mean = fopen(lambda_mean_file, "w");
    fprintf(fp_lambda_mean, "%.5e\n", lambda_mean);
    fclose(fp_lambda_mean);
    
    double time_ed = MiTime::GetTimeSec();
    printf("time_ed - time_st = %e\n", time_ed - time_st);
    
    return status_prog;
}






