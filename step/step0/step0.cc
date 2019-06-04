//
// step0.cc
//
// 2019.05.16 M.Morii
//

#include "mi_time.h"
#include "hscdata.h"
#include "sub_step.h"
#include "sub_step0.h"
#include "arg_step0.h"

// global variable 
int g_flag_debug = 0;
int g_flag_help = 0;
int g_flag_verbose = 0;
  
int main(int argc, char* argv[])
{
    int status_prog = kRetNormal;

    double time_st = MiTime::GetTimeSec();
    
    ArgValStep0* argval = new ArgValStep0;
    argval->Init(argc, argv);
    argval->Print(stdout);

    FILE* fp_log = NULL;
    OpenLogfile(argval->GetOutdir(),
                "log",
                argval->GetProgname(),
                &fp_log);
    argval->Print(fp_log);
    
    MiRand::Init(argval->GetSeed());
    
    long nhsc_data = 0;
    HscData** hsc_data_arr = NULL;
    int nname = 0;
    string* name_arr = NULL;
    if("16051900" == argval->GetCsvVersion()){
        LoadCsvDat16051900(argval->GetCsvfile(), &nhsc_data, &hsc_data_arr,
                           &nname, &name_arr);
    } else if("17112400" == argval->GetCsvVersion()){
        LoadCsvDat17112400(argval->GetCsvfile(), &nhsc_data, &hsc_data_arr,
                           &nname, &name_arr);
    }
    printf("nhsc_data = %ld\n", nhsc_data);

    long ntrue = 0;
    long nfalse = 0;
    for(long isrc = 0; isrc < nhsc_data; isrc ++){
        if(0 == hsc_data_arr[isrc]->GetTrueFalse()){
            nfalse ++;
        } else if(1 == hsc_data_arr[isrc]->GetTrueFalse()){
            ntrue ++;
        } else {
            abort();
        }
    }
    printf("ntrue = %ld, nfalse = %ld\n", ntrue, nfalse);

    long* id_true_arr  = new long [ntrue];
    long* id_false_arr = new long [nfalse];
    long ifalse = 0;
    long itrue = 0;
    for(long isrc = 0; isrc < nhsc_data; isrc ++){
        if(0 == hsc_data_arr[isrc]->GetTrueFalse()){
            id_false_arr[ifalse] = isrc;
            ifalse ++;
        } else if(1 == hsc_data_arr[isrc]->GetTrueFalse()){
            id_true_arr[itrue] = isrc;
            itrue ++;
        } else {
            abort();
        }
    }

    int nsplit = 3;
    for(int iset = 0; iset < argval->GetNset(); iset ++){
        printf("iset = %d\n", iset);
        //
        // --- split into (test data) and (training data + validation data)
        //
        
        //
        // true data
        //
        long nid_true_te = 0;
        long* id_true_te_arr = NULL;
        long nid_true_trvl = 0;
        long* id_true_trvl_arr = NULL;
        long num_of_first = round( ntrue / nsplit);
        SplitArrRand2(ntrue, id_true_arr,
                      num_of_first,
                      &nid_true_te, &id_true_te_arr,
                      &nid_true_trvl, &id_true_trvl_arr);
        //
        // bogus data
        //
        long nid_false_te = 0;
        long* id_false_te_arr = NULL;
        long nid_false_trvl = 0;
        long* id_false_trvl_arr = NULL;
        num_of_first = round( nfalse / nsplit);
        SplitArrRand2(nfalse, id_false_arr,
                      num_of_first,
                      &nid_false_te, &id_false_te_arr,
                      &nid_false_trvl, &id_false_trvl_arr);
        //
        // test data
        //
        char te_file[kLineSize];
        sprintf(te_file, "%s/cv%2.2d-te.csv", argval->GetOutdir().c_str(), iset);
        FILE* fp_te = fopen(te_file, "w");
        for(long isrc = 0; isrc < nid_true_te; isrc ++){
            fprintf(fp_te, "%-7ld  %d \n",
                    id_true_te_arr[isrc], 1);
        }
        for(long isrc = 0; isrc < nid_false_te; isrc ++){
            fprintf(fp_te, "%-7ld  %d \n",
                    id_false_te_arr[isrc], 0);
        }
        
        for(int jset = 0; jset < argval->GetNset(); jset ++){
            //
            // true data
            //
            long nid_true_tr = 0;
            long* id_true_tr_arr = NULL;
            long nid_true_vl = 0;
            long* id_true_vl_arr = NULL;
            long num_of_first = round( nid_true_trvl / 2.0);
            SplitArrRand2(nid_true_trvl, id_true_trvl_arr,
                          num_of_first,
                          &nid_true_tr, &id_true_tr_arr,
                          &nid_true_vl, &id_true_vl_arr);
            //
            // bogus data
            //
            long nid_false_tr = 0;
            long* id_false_tr_arr = NULL;
            long nid_false_vl = 0;
            long* id_false_vl_arr = NULL;
            num_of_first = round( nid_false_trvl / 2.0);
            SplitArrRand2(nid_false_trvl, id_false_trvl_arr,
                          num_of_first,
                          &nid_false_tr, &id_false_tr_arr,
                          &nid_false_vl, &id_false_vl_arr);
            
            //
            // training data
            //
            char tr_file[kLineSize];
            sprintf(tr_file, "%s/cv%2.2d-%2.2d-tr.csv", argval->GetOutdir().c_str(), iset, jset);
            FILE* fp_tr = fopen(tr_file, "w");
            for(long isrc = 0; isrc < nid_true_tr; isrc ++){
                fprintf(fp_tr, "%-7ld  %d \n",
                        id_true_tr_arr[isrc], 1);
            }
            for(long isrc = 0; isrc < nid_false_tr; isrc ++){
                fprintf(fp_tr, "%-7ld  %d \n",
                        id_false_tr_arr[isrc], 0);
            }

            //
            // validation data
            //
            char vl_file[kLineSize];
            sprintf(vl_file, "%s/cv%2.2d-%2.2d-vl.csv", argval->GetOutdir().c_str(), iset, jset);
            FILE* fp_vl = fopen(vl_file, "w");
            for(long isrc = 0; isrc < nid_true_vl; isrc ++){
                fprintf(fp_vl, "%-7ld  %d \n",
                        id_true_vl_arr[isrc], 1);
            }
            for(long isrc = 0; isrc < nid_false_vl; isrc ++){
                fprintf(fp_vl, "%-7ld  %d \n",
                        id_false_vl_arr[isrc], 0);
            }


        }
    }
    
    double time_ed = MiTime::GetTimeSec();
    printf("time_ed - time_st = %e\n", time_ed - time_st);
    
    return status_prog;
}




