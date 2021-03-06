#include "sub_step1.h"

void MkTrVlLambdaDat(int nset,
                     int nlambda,
                     const double* const lambda_arr,
                     string outdir)
{
    int nfile_src_id_te   = nset;
    int nfile_src_id_trvl = nset;

    char trvl_lambda_dat[kLineSize];
    sprintf(trvl_lambda_dat, "%s/trvl_lambda.dat", outdir.c_str());
    FILE* fp = fopen(trvl_lambda_dat, "w");
    
    for(int index_te = 0; index_te < nfile_src_id_te; index_te ++){
        for(int index_trvl = 0; index_trvl < nfile_src_id_trvl; index_trvl ++){
            char file_src_id_tr[kLineSize];
            char file_src_id_vl[kLineSize];
            sprintf(file_src_id_tr, "cv%2.2d-%2.2d-tr.csv",
                    index_te, index_trvl);
            sprintf(file_src_id_vl, "cv%2.2d-%2.2d-vl.csv",
                    index_te, index_trvl);
            for(int ilambda = 0; ilambda < nlambda; ilambda ++){
                fprintf(fp, "%s  %s  %d  %e\n",
                        file_src_id_tr,
                        file_src_id_vl,
                        ilambda,
                        lambda_arr[ilambda]);
            }
        }
    }
    fclose(fp);
}

void RunAucboostStep1(string csvfile,
                      string csv_version,
                      string src_id_dir,
                      int niter,
                      string outdir)
{
    printf(" --- RunAucboostStep1 --- \n");
    printf(" --- arg: RunAucboostStep1 ---- \n");
    printf(" csvfile = %s\n", csvfile.c_str());
    printf(" src_id_dir = %s\n", src_id_dir.c_str());
    printf(" niter = %d\n", niter);
    printf(" outdir = %s\n", outdir.c_str());
    printf(" --- arg: RunAucboostStep1 ---- \n");

    long nhsc_data = 0;
    HscData** hsc_data_arr = NULL;
    int nname = 0;
    string* name_arr = NULL;
    if("16051900" == csv_version){
        LoadCsvDat16051900(csvfile, &nhsc_data, &hsc_data_arr,
                           &nname, &name_arr);
    } else if("17112400" == csv_version){
        LoadCsvDat17112400(csvfile, &nhsc_data, &hsc_data_arr,
                           &nname, &name_arr);
    }
    printf("nhsc_data = %ld\n", nhsc_data);
    
    char trvl_lambda_dat[kLineSize];
    sprintf(trvl_lambda_dat, "%s/trvl_lambda.dat", outdir.c_str());
    string* lines_arr = NULL;
    long nline = 0;
    MiIolib::GenReadFileSkipComment(trvl_lambda_dat, &lines_arr, &nline);
    for(long iline = 0; iline < nline; iline ++){
        int nsplit = 0;
        string* split_arr = NULL;
        MiStr::GenSplit(lines_arr[iline], &nsplit, &split_arr);

        char src_id_tr[kLineSize];
        sprintf(src_id_tr, "%s/%s",
                src_id_dir.c_str(), split_arr[0].c_str());
        char src_id_vl[kLineSize];
        sprintf(src_id_vl, "%s/%s",
                src_id_dir.c_str(), split_arr[1].c_str());
        int    ilambda = atoi(split_arr[2].c_str());
        double lambda  = atof(split_arr[3].c_str());

        long nsrc_id_true_tr = 0;
        long nsrc_id_false_tr = 0;
        long nsrc_id_true_vl = 0;
        long nsrc_id_false_vl = 0;
        long* src_id_true_tr_arr = NULL;
        long* src_id_false_tr_arr = NULL;        
        long* src_id_true_vl_arr = NULL;
        long* src_id_false_vl_arr = NULL;        
        
        LoadSrcIdDat(src_id_tr,
                     &nsrc_id_true_tr, &src_id_true_tr_arr,
                     &nsrc_id_false_tr, &src_id_false_tr_arr);
        LoadSrcIdDat(src_id_vl,
                     &nsrc_id_true_vl, &src_id_true_vl_arr,
                     &nsrc_id_false_vl, &src_id_false_vl_arr);

        long nfeature_true_tr = 0;
        double** feature_true_tr_arr = NULL;
        long nfeature_false_tr = 0;
        double** feature_false_tr_arr = NULL;
        long nfeature_true_vl = 0;
        double** feature_true_vl_arr = NULL;
        long nfeature_false_vl = 0;
        double** feature_false_vl_arr = NULL;

        GetData(nhsc_data, hsc_data_arr, nsrc_id_true_tr, src_id_true_tr_arr,
                &nfeature_true_tr, &feature_true_tr_arr);
        GetData(nhsc_data, hsc_data_arr, nsrc_id_false_tr, src_id_false_tr_arr,
                &nfeature_false_tr, &feature_false_tr_arr);
        GetData(nhsc_data, hsc_data_arr, nsrc_id_true_vl, src_id_true_vl_arr,
                &nfeature_true_vl, &feature_true_vl_arr);
        GetData(nhsc_data, hsc_data_arr, nsrc_id_false_vl, src_id_false_vl_arr,
                &nfeature_false_vl, &feature_false_vl_arr);

        char tag_lambda[kLineSize];
        sprintf(tag_lambda, "lambda_%2.2d_%.1e",
                ilambda, lambda);
        char outdir_tr[kLineSize];
        sprintf(outdir_tr, "%s/%s/%s",
                outdir.c_str(), split_arr[0].c_str(), tag_lambda);
        printf("outdir_tr = %s\n", outdir_tr);
        if( MiIolib::TestFileExist(outdir_tr) ){
            char cmd[kLineSize];
            sprintf(cmd, "mkdir -p %s", outdir_tr);
            system(cmd);
            printf("cmd = %s\n", cmd);
        }

        Aucboost(hsc_data_arr[0]->GetNftr(),
                 feature_true_tr_arr, nfeature_true_tr,
                 feature_false_tr_arr, nfeature_false_tr,
                 feature_true_vl_arr, nfeature_true_vl,
                 feature_false_vl_arr, nfeature_false_vl,
                 name_arr,
                 niter,
                 lambda,
                 outdir_tr);

        delete [] split_arr;
        delete [] src_id_true_tr_arr;
        delete [] src_id_false_tr_arr;
        delete [] src_id_true_vl_arr;
        delete [] src_id_false_vl_arr;
        for(long idata = 0; idata < nfeature_true_tr; idata ++){
            delete [] feature_true_tr_arr[idata];
        }
        delete [] feature_true_tr_arr;

        for(long idata = 0; idata < nfeature_false_tr; idata ++){
            delete [] feature_false_tr_arr[idata];
        }
        delete [] feature_false_tr_arr;

        for(long idata = 0; idata < nfeature_true_vl; idata ++){
            delete [] feature_true_vl_arr[idata];
        }
        delete [] feature_true_vl_arr;

        for(long idata = 0; idata < nfeature_false_vl; idata ++){
            delete [] feature_false_vl_arr[idata];
        }
        delete [] feature_false_vl_arr;
    }
    delete [] lines_arr;
    delete [] name_arr;
    for(long idata = 0; idata < nhsc_data; idata ++){
        delete hsc_data_arr[idata];
    }
    delete hsc_data_arr;
    printf(" === RunAucboostStep1 === \n");    
}
