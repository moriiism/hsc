#include "sub_step2.h"

void LoadLambdaArr(string lambda_file,
                   int* const nlambda_ptr,
                   double** const lambda_arr_ptr)
{

    string* lines_arr = NULL;
    long nline = 0;
    MiIolib::GenReadFileSkipComment(lambda_file,
                                    &lines_arr, &nline);
    int nlambda = nline;
    double* lambda_arr = new double [nlambda];
    for(long iline = 0; iline < nline; iline ++){
        int nsplit = 0;
        string* split_arr = NULL;
        MiStr::GenSplit(lines_arr[iline], &nsplit, &split_arr);
        lambda_arr[iline] = atof(split_arr[1].c_str());
        MiStr::DelSplit(split_arr);
    }
    delete [] lines_arr;
    *nlambda_ptr    = nlambda;
    *lambda_arr_ptr = lambda_arr;
}

void MkTrVlTeLambdaDat(int nset,
                       const double* const lambda_arr,
                       string outdir)
{
    int nfile_src_id_te = nset;
    char trvlte_lambda_dat[kLineSize];
    sprintf(trvlte_lambda_dat, "%s/trvlte_lambda.dat", outdir.c_str());
    FILE* fp = fopen(trvlte_lambda_dat, "w");
    for(int index_te = 0; index_te < nfile_src_id_te; index_te ++){
        char file_src_id_tr[kLineSize];
        char file_src_id_vl[kLineSize];
        char file_src_id_te[kLineSize];
        sprintf(file_src_id_tr, "cv%2.2d-00-tr.csv", index_te);
        sprintf(file_src_id_vl, "cv%2.2d-00-vl.csv", index_te);
        sprintf(file_src_id_te, "cv%2.2d-te.csv", index_te);

        fprintf(fp, "%s  %s  %s  %e\n",
                file_src_id_tr,
                file_src_id_vl,
                file_src_id_te,
                lambda_arr[index_te]);
    }
    fclose(fp);
}


void RunAucboostStep2(string csvfile,
                      string csv_version,
                      string src_id_dir,
                      int niter,
                      string outdir)
{
    printf(" --- RunAucboostStep2 --- \n");
    printf(" --- arg: RunAucboostStep2 ---- \n");
    printf(" csvfile = %s\n", csvfile.c_str());
    printf(" src_id_dir = %s\n", src_id_dir.c_str());
    printf(" niter = %d\n", niter);
    printf(" outdir = %s\n", outdir.c_str());
    printf(" --- arg: RunAucboostStep2 ---- \n");

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
    sprintf(trvl_lambda_dat, "%s/trvlte_lambda.dat", outdir.c_str());
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
        char src_id_te[kLineSize];
        sprintf(src_id_te, "%s/%s",
                src_id_dir.c_str(), split_arr[2].c_str());
        double lambda  = atof(split_arr[3].c_str());

        long nsrc_id_true_tr = 0;
        long nsrc_id_false_tr = 0;
        long nsrc_id_true_vl = 0;
        long nsrc_id_false_vl = 0;
        long nsrc_id_true_te = 0;
        long nsrc_id_false_te = 0;
        long* src_id_true_tr_arr = NULL;
        long* src_id_false_tr_arr = NULL;        
        long* src_id_true_vl_arr = NULL;
        long* src_id_false_vl_arr = NULL;
        long* src_id_true_te_arr = NULL;
        long* src_id_false_te_arr = NULL;        
        LoadSrcIdDat(src_id_tr,
                     &nsrc_id_true_tr, &src_id_true_tr_arr,
                     &nsrc_id_false_tr, &src_id_false_tr_arr);
        LoadSrcIdDat(src_id_vl,
                     &nsrc_id_true_vl, &src_id_true_vl_arr,
                     &nsrc_id_false_vl, &src_id_false_vl_arr);
        LoadSrcIdDat(src_id_te,
                     &nsrc_id_true_te, &src_id_true_te_arr,
                     &nsrc_id_false_te, &src_id_false_te_arr);


        long nsrc_id_true_trvl = nsrc_id_true_tr + nsrc_id_true_vl;
        long* src_id_true_trvl_arr = new long [nsrc_id_true_trvl];
        long isrc_tot = 0;
        for(long isrc = 0; isrc < nsrc_id_true_tr; isrc ++){
            src_id_true_trvl_arr[isrc_tot] = src_id_true_tr_arr[isrc];
            isrc_tot ++;
        }
        for(long isrc = 0; isrc < nsrc_id_true_vl; isrc ++){
            src_id_true_trvl_arr[isrc_tot] = src_id_true_vl_arr[isrc];
            isrc_tot ++;
        }

        long nsrc_id_false_trvl = nsrc_id_false_tr + nsrc_id_false_vl;
        long* src_id_false_trvl_arr = new long [nsrc_id_false_trvl];
        isrc_tot = 0;
        for(long isrc = 0; isrc < nsrc_id_false_tr; isrc ++){
            src_id_false_trvl_arr[isrc_tot] = src_id_false_tr_arr[isrc];
            isrc_tot ++;
        }
        for(long isrc = 0; isrc < nsrc_id_false_vl; isrc ++){
            src_id_false_trvl_arr[isrc_tot] = src_id_false_vl_arr[isrc];
            isrc_tot ++;
        }
        
        long nfeature_true_trvl = 0;
        double** feature_true_trvl_arr = NULL;
        long nfeature_false_trvl = 0;
        double** feature_false_trvl_arr = NULL;

        long nfeature_true_te = 0;
        double** feature_true_te_arr = NULL;
        long nfeature_false_te = 0;
        double** feature_false_te_arr = NULL;

        GetData(nhsc_data, hsc_data_arr, nsrc_id_true_trvl, src_id_true_trvl_arr,
                &nfeature_true_trvl, &feature_true_trvl_arr);
        GetData(nhsc_data, hsc_data_arr, nsrc_id_false_trvl, src_id_false_trvl_arr,
                &nfeature_false_trvl, &feature_false_trvl_arr);

        GetData(nhsc_data, hsc_data_arr, nsrc_id_true_te, src_id_true_te_arr,
                &nfeature_true_te, &feature_true_te_arr);
        GetData(nhsc_data, hsc_data_arr, nsrc_id_false_te, src_id_false_te_arr,
                &nfeature_false_te, &feature_false_te_arr);

        char outdir_te[kLineSize];
        sprintf(outdir_te, "%s/%s",
                outdir.c_str(), split_arr[2].c_str());
        printf("outdir_te = %s\n", outdir_te);
        if( MiIolib::TestFileExist(outdir_te) ){
            char cmd[kLineSize];
            sprintf(cmd, "mkdir -p %s", outdir_te);
            system(cmd);
            printf("cmd = %s\n", cmd);
        }

        Aucboost(hsc_data_arr[0]->GetNftr(),
                 feature_true_trvl_arr, nfeature_true_trvl,
                 feature_false_trvl_arr, nfeature_false_trvl,
                 feature_true_te_arr, nfeature_true_te,
                 feature_false_te_arr, nfeature_false_te,
                 name_arr,
                 niter,
                 lambda,
                 outdir_te);

        delete [] split_arr;
        delete [] src_id_true_tr_arr;
        delete [] src_id_false_tr_arr;
        delete [] src_id_true_vl_arr;
        delete [] src_id_false_vl_arr;
        delete [] src_id_true_te_arr;
        delete [] src_id_false_te_arr;
        delete [] src_id_true_trvl_arr;
        delete [] src_id_false_trvl_arr;
        
        for(long idata = 0; idata < nfeature_true_trvl; idata ++){
            delete [] feature_true_trvl_arr[idata];
        }
        delete [] feature_true_trvl_arr;

        for(long idata = 0; idata < nfeature_false_trvl; idata ++){
            delete [] feature_false_trvl_arr[idata];
        }
        delete [] feature_false_trvl_arr;

        for(long idata = 0; idata < nfeature_true_te; idata ++){
            delete [] feature_true_te_arr[idata];
        }
        delete [] feature_true_te_arr;

        for(long idata = 0; idata < nfeature_false_te; idata ++){
            delete [] feature_false_te_arr[idata];
        }
        delete [] feature_false_te_arr;
    }
    delete [] lines_arr;
    delete [] name_arr;
    for(long idata = 0; idata < nhsc_data; idata ++){
        delete hsc_data_arr[idata];
    }
    delete hsc_data_arr;
    printf(" === RunAucboostStep2 === \n");
}


void MkRocAve(int nset, string outdir)
{
    Gd2dFunc** func_roc_arr = new Gd2dFunc*[nset];
    for(int iset = 0; iset < nset; iset ++){
        char roc_file[kLineSize];
        sprintf(roc_file, "%s/cv%2.2d-te.csv/roc_te.dat",
                outdir.c_str(), iset);
        long ndata = 0;
        string* data_line_arr = NULL;
        MiIolib::GenReadFileSkipComment(roc_file,
                                        &data_line_arr, &ndata);
        printf("iset: %d: ndata = %ld\n", iset, ndata);
        double* fpr_arr = new double [ndata];
        double* tpr_arr = new double [ndata];
        for(long idata = 0; idata < ndata; idata ++){
            int nsplit = 0;
            string* split_arr = NULL;
            MiStr::GenSplit(data_line_arr[idata],
                            &nsplit, &split_arr, " ");
            fpr_arr[idata] = atof(split_arr[0].c_str());
            tpr_arr[idata] = atof(split_arr[1].c_str());
            delete [] split_arr;
        }
        delete [] data_line_arr;

        GraphDataNerr2d* gd2d_roc = new GraphDataNerr2d;
        gd2d_roc->Init(ndata);
        gd2d_roc->SetXvalArr(ndata, fpr_arr);
        gd2d_roc->SetOvalArr(ndata, tpr_arr);
        gd2d_roc->Sort();

        func_roc_arr[iset] = new Gd2dFunc;
        func_roc_arr[iset]->InitSetGd2d(gd2d_roc);

        char qdpout[kLineSize];
        sprintf(qdpout, "%s/roc_%2.2d.qdp", outdir.c_str(), iset);
        MirQdpTool::MkQdp(gd2d_roc, qdpout, "x,y");

        delete [] fpr_arr;
        delete [] tpr_arr;
        delete gd2d_roc;
    }

    // make averaged roc file
    char outroc[kLineSize];
    sprintf(outroc, "%s/roc_ave.dat", outdir.c_str());
    FILE* fp_roc = fopen(outroc, "w");
    int nfpr = 300;
    double fpr_lo = 0.0;
    double fpr_up = 1.0;
    double delta_fpr = (fpr_up - fpr_lo) / nfpr;
    for(int ifpr = 0; ifpr < nfpr; ifpr ++){
        double fpr = fpr_lo + (ifpr + 0.5) * delta_fpr;
        double ave = 0.0;
        for(int iset = 0; iset < nset; iset ++){
            double fpr_arr[1];
            fpr_arr[0] = fpr;
            ave += func_roc_arr[iset]->Eval(fpr_arr, NULL);
        }
        ave /= nset;
        fprintf(fp_roc, "%e  %e\n", fpr, ave);
    }
    fclose(fp_roc);

}

