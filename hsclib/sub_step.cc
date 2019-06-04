#include "sub_step.h"

void OpenLogfile(string outdir,
                 string outfile_head,
                 string progname,
                 FILE** const fp_log_ptr)
{
    char logfile[kLineSize];
    if( MiIolib::TestFileExist(outdir) ){
        char cmd[kLineSize];
        sprintf(cmd, "mkdir -p %s", outdir.c_str());
        system(cmd);
    }
    sprintf(logfile, "%s/%s_%s.log",
            outdir.c_str(),
            outfile_head.c_str(),
            progname.c_str());
    FILE* fp_log = fopen(logfile, "w");
    MiIolib::Printf2(fp_log, "-----------------------------\n");
    *fp_log_ptr = fp_log;
}


void LoadLambdaDat(string lambda_dat,
                   int* const nlambda_ptr,
                   double** const lambda_arr_ptr)
{
    // read lambda data
    string* lines_arr = NULL;
    long nline = 0;
    MiIolib::GenReadFileSkipComment(lambda_dat,
                                    &lines_arr, &nline);
    if(1 != nline){
        abort();
    }
    printf("nline = %ld\n", nline);    
    printf("%s\n", lines_arr[0].c_str());

    int nsplit = 0;
    string* split_arr = NULL;
    MiStr::GenSplit(lines_arr[0], &nsplit, &split_arr);
    long nlambda  = atoi(split_arr[0].c_str());
    double lambda_lo    = atof(split_arr[1].c_str());
    double lambda_up    = atof(split_arr[2].c_str());
    string lambda_scale = split_arr[3];
    MiStr::DelSplit(split_arr);
    double* lambda_arr = new double[nlambda];
    MirBinning::GetPoint(nlambda, lambda_lo, lambda_up, lambda_scale, lambda_arr);

    *nlambda_ptr    = nlambda;
    *lambda_arr_ptr = lambda_arr;
}
