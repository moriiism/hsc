#include "sub_step4.h"

void RunAucboostStep4(string csvfile,
                      string csv_version,
                      int niter,
                      double lambda,
                      string outdir)
{
    printf(" --- RunAucboostStep4 --- \n");
    printf(" --- arg: RunAucboostStep4 ---- \n");
    printf(" csvfile = %s\n", csvfile.c_str());
    printf(" niter = %d\n", niter);
    printf(" lambda = %e\n", lambda);
    printf(" outdir = %s\n", outdir.c_str());
    printf(" --- arg: RunAucboostStep4 ---- \n");

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
    

    long nfeature_true = 0;
    double** feature_true_arr = NULL;
    long nfeature_false = 0;
    double** feature_false_arr = NULL;
    GetDataNoSel(nhsc_data, hsc_data_arr, 
                 &nfeature_true, &feature_true_arr,
                 &nfeature_false, &feature_false_arr);

    printf("nfeature_true = %ld\n", nfeature_true);
    printf("nfeature_false = %ld\n", nfeature_false);

    Aucboost(hsc_data_arr[0]->GetNftr(),
             feature_true_arr, nfeature_true,
             feature_false_arr, nfeature_false,
             feature_true_arr, nfeature_true,
             feature_false_arr, nfeature_false,
             name_arr,
             niter,
             lambda,
             outdir);

    for(long idata = 0; idata < nfeature_true; idata ++){
        delete [] feature_true_arr[idata];
    }
    delete [] feature_true_arr;

    for(long idata = 0; idata < nfeature_false; idata ++){
        delete [] feature_false_arr[idata];
    }
    delete [] feature_false_arr;

    delete [] name_arr;
    for(long idata = 0; idata < nhsc_data; idata ++){
        delete hsc_data_arr[idata];
    }
    delete hsc_data_arr;
    printf(" === RunAucboostStep4 === \n");
}






