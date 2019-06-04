//
// step2.cc
//
// 2019.05.16 M.Morii
//

#include "mi_time.h"
#include "sub_step.h"
#include "sub_step2.h"
#include "arg_step2.h"

// global variable 
int g_flag_debug = 0;
int g_flag_help = 0;
int g_flag_verbose = 0;
  
int main(int argc, char* argv[])
{
    int status_prog = kRetNormal;

    double time_st = MiTime::GetTimeSec();
    
    ArgValStep2* argval = new ArgValStep2;
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
    LoadLambdaArr(argval->GetLambdaFile(),
                  &nlambda, &lambda_arr);
    MkTrVlTeLambdaDat(argval->GetNset(),
                      lambda_arr,
                      argval->GetOutdir());
    RunAucboostStep2(argval->GetCsvfile(),
                     argval->GetCsvVersion(),
                     argval->GetSourceIdDir(),
                     argval->GetNiter(),
                     argval->GetOutdir());
    MkRocAve(argval->GetNset(), argval->GetOutdir());
    
    double time_ed = MiTime::GetTimeSec();
    printf("time_ed - time_st = %e\n", time_ed - time_st);
    
    return status_prog;
}








