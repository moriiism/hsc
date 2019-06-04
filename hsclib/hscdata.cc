#include "hscdata.h"

void HscData::Init(int nftr)
{
    nftr_    = nftr;
    ftr_arr_ = new double [nftr];
    for(int iftr = 0; iftr < nftr; iftr ++){
        ftr_arr_[iftr] = 0.0;
    }
    true_false_ = 0;
}

void HscData::Set16051900(double mag,
                          double magerr,
                          double elongation_norm,
                          double fwhm_norm,
                          double significance_abs,
                          double residual,
                          double psffit_sigma_ratio,
                          double psffit_peak_ratio,
                          double frac_det,
                          double density,
                          double density_good,
                          double bapsf,
                          double psf)
{
    int iftr = 0;
    ftr_arr_[iftr] = mag;                iftr ++;
    ftr_arr_[iftr] = magerr;             iftr ++;
    ftr_arr_[iftr] = elongation_norm;    iftr ++;
    ftr_arr_[iftr] = fwhm_norm;          iftr ++;
    ftr_arr_[iftr] = significance_abs;   iftr ++;
    ftr_arr_[iftr] = residual;           iftr ++;
    ftr_arr_[iftr] = psffit_sigma_ratio; iftr ++;
    ftr_arr_[iftr] = psffit_peak_ratio;  iftr ++;
    ftr_arr_[iftr] = frac_det;           iftr ++;
    ftr_arr_[iftr] = density;            iftr ++;
    ftr_arr_[iftr] = density_good;       iftr ++;
    ftr_arr_[iftr] = bapsf;              iftr ++;
    ftr_arr_[iftr] = psf;                iftr ++;
}

void HscData::Set17112400(double magerr,
                          double elongation_norm,
                          double fwhm_norm,
                          double significance_abs,
                          double residual,
                          double psffit_sigma_ratio,
                          double psffit_peak_ratio,
                          double frac_det,
                          double density,
                          double density_good,
                          double bapsf,
                          double sigmapsf)
{
    int iftr = 0;
    ftr_arr_[iftr] = magerr;             iftr ++;
    ftr_arr_[iftr] = elongation_norm;    iftr ++;
    ftr_arr_[iftr] = fwhm_norm;          iftr ++;
    ftr_arr_[iftr] = significance_abs;   iftr ++;
    ftr_arr_[iftr] = residual;           iftr ++;
    ftr_arr_[iftr] = psffit_sigma_ratio; iftr ++;
    ftr_arr_[iftr] = psffit_peak_ratio;  iftr ++;
    ftr_arr_[iftr] = frac_det;           iftr ++;
    ftr_arr_[iftr] = density;            iftr ++;
    ftr_arr_[iftr] = density_good;       iftr ++;
    ftr_arr_[iftr] = bapsf;              iftr ++;
    ftr_arr_[iftr] = sigmapsf;           iftr ++;
}

void HscData::Copy(const HscData* const org)
{
    if(this == org) {abort();}
    if(NULL == org) {abort();}

    Null();

    nftr_    = org->nftr_;
    ftr_arr_ = new double [org->nftr_];
    for(int iftr = 0; iftr < nftr_; iftr ++){
        ftr_arr_[iftr] = org->ftr_arr_[iftr];
    }
    true_false_ = org->true_false_;
}

HscData* const HscData::Clone() const
{
    HscData* obj_new = new HscData;
    obj_new->Copy(this);
    return obj_new;
}

void HscData::Print16051900(FILE* fp) const
{
    int iftr = 0;
    fprintf(fp, "mag                  = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "magerr               = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "elongation_norm      = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "fwhm_norm            = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "significance_abs     = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "residual             = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "psffit_sigma_ratio   = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "psffit_peak_ration   = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "frac_det             = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "density              = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "density_good         = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "bapsf                = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "psf                  = %e\n", ftr_arr_[iftr]); iftr ++;

    fprintf(fp, "nftr_                = %d\n", nftr_);
    fprintf(fp, "true_false_          = %d\n", true_false_);
}

void HscData::Print17112400(FILE* fp) const
{
    int iftr = 0;
    fprintf(fp, "magerr               = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "elongation_norm      = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "fwhm_norm            = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "significance_abs     = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "residual             = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "psffit_sigma_ratio   = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "psffit_peak_ration   = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "frac_det             = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "density              = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "density_good         = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "bapsf                = %e\n", ftr_arr_[iftr]); iftr ++;
    fprintf(fp, "sigmapsf             = %e\n", ftr_arr_[iftr]); iftr ++;

    fprintf(fp, "nftr_                = %d\n", nftr_);
    fprintf(fp, "true_false_          = %d\n", true_false_);
}

// private

void HscData::Null()
{
    if(NULL != ftr_arr_){
        delete [] ftr_arr_;
    }
    nftr_        = 0;
    true_false_  = 0;
}



void LoadCsvDat16051900(string csv_dat,
                        long* const ndata_ptr,
                        HscData*** const hsc_data_arr_ptr,
                        int* const nname_ptr,
                        string** const name_arr_ptr)
{
    string* lines_arr = NULL;
    long nline = 0;
    MiIolib::GenReadFileSkipComment(csv_dat, &lines_arr, &nline);
    int nsplit0 = 0;
    string* split0_arr = NULL;
    MiStr::GenSplit(lines_arr[0], &nsplit0, &split0_arr, ",");
    int nname = 13;
    string* name_arr = new string [nname];
    name_arr[0] = split0_arr[2];
    name_arr[1] = split0_arr[3];
    name_arr[2] = split0_arr[4];
    name_arr[3] = split0_arr[5];
    name_arr[4] = split0_arr[6];
    name_arr[5] = split0_arr[7];
    name_arr[6] = split0_arr[8];
    name_arr[7] = split0_arr[9];
    name_arr[8] = split0_arr[10];
    name_arr[9] = split0_arr[11];
    name_arr[10] = split0_arr[12];
    name_arr[11] = split0_arr[13];
    name_arr[12] = split0_arr[14];

   
    long ndata = nline - 1;

    
    HscData** hsc_data_arr = new HscData* [ndata];
    for(long iline = 1; iline < nline; iline ++){
        int nsplit = 0;
        string* split_arr = NULL;
        MiStr::GenSplit(lines_arr[iline], &nsplit, &split_arr, ",");

        long idata = iline - 1;
        hsc_data_arr[idata] = new HscData;
        hsc_data_arr[idata]->Init(13);
        hsc_data_arr[idata]->Set16051900(atof(split_arr[2].c_str()),
                                         atof(split_arr[3].c_str()),
                                         atof(split_arr[4].c_str()),
                                         atof(split_arr[5].c_str()),
                                         atof(split_arr[6].c_str()),
                                         atof(split_arr[7].c_str()),
                                         atof(split_arr[8].c_str()),
                                         atof(split_arr[9].c_str()),
                                         atof(split_arr[10].c_str()),
                                         atof(split_arr[11].c_str()),
                                         atof(split_arr[12].c_str()),
                                         atof(split_arr[13].c_str()),
                                         atof(split_arr[14].c_str()));
        hsc_data_arr[idata]->SetTrueFalse(atoi(split_arr[15].c_str()));
        MiStr::DelSplit(split_arr);
    }
    delete [] lines_arr;
    *ndata_ptr = ndata;
    *hsc_data_arr_ptr = hsc_data_arr;
    *nname_ptr = nname;
    *name_arr_ptr = name_arr;
}

void LoadCsvDat17112400(string csv_dat,
                        long* const ndata_ptr,
                        HscData*** const hsc_data_arr_ptr,
                        int* const nname_ptr,
                        string** const name_arr_ptr)
{
    string* lines_arr = NULL;
    long nline = 0;
    MiIolib::GenReadFileSkipComment(csv_dat, &lines_arr, &nline);
    int nsplit0 = 0;
    string* split0_arr = NULL;
    MiStr::GenSplit(lines_arr[0], &nsplit0, &split0_arr, ",");
    int nname = 12;
    string* name_arr = new string [nname];
    name_arr[0] = split0_arr[3];
    name_arr[1] = split0_arr[4];
    name_arr[2] = split0_arr[5];
    name_arr[3] = split0_arr[6];
    name_arr[4] = split0_arr[7];
    name_arr[5] = split0_arr[8];
    name_arr[6] = split0_arr[9];
    name_arr[7] = split0_arr[10];
    name_arr[8] = split0_arr[11];
    name_arr[9] = split0_arr[12];
    name_arr[10] = split0_arr[13];
    name_arr[11] = split0_arr[14];


    long ndata = nline - 1;
    
    
    HscData** hsc_data_arr = new HscData* [ndata];
    for(long iline = 0; iline < nline; iline ++){
        int nsplit = 0;
        string* split_arr = NULL;
        MiStr::GenSplit(lines_arr[iline], &nsplit, &split_arr, ",");

        long idata = iline - 1;        
        hsc_data_arr[idata] = new HscData;
        hsc_data_arr[idata]->Init(12);
        hsc_data_arr[idata]->Set17112400(atof(split_arr[3].c_str()),
                                         atof(split_arr[4].c_str()),
                                         atof(split_arr[5].c_str()),
                                         atof(split_arr[6].c_str()),
                                         atof(split_arr[7].c_str()),
                                         atof(split_arr[8].c_str()),
                                         atof(split_arr[9].c_str()),
                                         atof(split_arr[10].c_str()),
                                         atof(split_arr[11].c_str()),
                                         atof(split_arr[12].c_str()),
                                         atof(split_arr[13].c_str()),
                                         atof(split_arr[14].c_str()));
        hsc_data_arr[idata]->SetTrueFalse(atoi(split_arr[15].c_str()));
        MiStr::DelSplit(split_arr);
    }
    delete [] lines_arr;
    *ndata_ptr = ndata;    
    *hsc_data_arr_ptr = hsc_data_arr;
    *nname_ptr = nname;
    *name_arr_ptr = name_arr;
}

// for step 1, 2, 3

void LoadSrcIdDat(string src_id_dat,
                  long* const nsrc_id_true_ptr,
                  long** const src_id_true_arr_ptr,
                  long* const nsrc_id_false_ptr,
                  long** const src_id_false_arr_ptr)
{
    string* lines_arr = NULL;
    long nline = 0;
    MiIolib::GenReadFileSkipComment(src_id_dat, &lines_arr, &nline);
    long ntrue = 0;
    long nfalse = 0;
    for(long iline = 0; iline < nline; iline ++){
        int nsplit = 0;
        string* split_arr = NULL;
        MiStr::GenSplit(lines_arr[iline], &nsplit, &split_arr);
        int true_false = atoi(split_arr[1].c_str());
        if(0 == true_false){
            nfalse ++;
        } else if (1 == true_false){
            ntrue ++;
        } else {
            abort();
        }
        MiStr::DelSplit(split_arr);
    }

    long* src_id_true_arr = new long [ntrue];
    long* src_id_false_arr = new long [nfalse];
    long itrue = 0;    
    long ifalse = 0;
    for(long iline = 0; iline < nline; iline ++){
        int nsplit = 0;
        string* split_arr = NULL;
        MiStr::GenSplit(lines_arr[iline], &nsplit, &split_arr);
        int true_false = atoi(split_arr[1].c_str());
        if(0 == true_false){
            src_id_false_arr[ifalse] = atoi(split_arr[0].c_str());
            ifalse ++;
        } else if (1 == true_false){
            src_id_true_arr[itrue] = atoi(split_arr[0].c_str());
            itrue ++;
        } else {
            abort();
        }
        MiStr::DelSplit(split_arr);
    }
    delete [] lines_arr;

    *nsrc_id_true_ptr = ntrue;
    *nsrc_id_false_ptr = nfalse;
    *src_id_true_arr_ptr = src_id_true_arr;
    *src_id_false_arr_ptr = src_id_false_arr;
}

// for step 1, 2, 3

void GetData(long nhsc_data, const HscData* const* const hsc_data_arr,
             long nsrc_id, const long* const src_id_arr,
             long* const nfeature_ptr,
             double*** const feature_arr_ptr)
{
    long nfeature = nsrc_id;
    double** feature_arr = new double* [nsrc_id];
    for(long isrc = 0; isrc < nsrc_id; isrc ++){
        int nftr = hsc_data_arr[0]->GetNftr();
        feature_arr[isrc] = new double[nftr];
        for(int icol = 0; icol < nftr; icol ++){
            feature_arr[isrc][icol] =
                hsc_data_arr[src_id_arr[isrc]]->GetFtrArrElm(icol);
        }
    }
    *nfeature_ptr = nfeature;
    *feature_arr_ptr = feature_arr;
}

// for step 4

void GetDataNoSel(long nhsc_data, const HscData* const* const hsc_data_arr,
                  long* const nfeature_true_ptr,
                  double*** const feature_true_arr_ptr,
                  long* const nfeature_false_ptr,
                  double*** const feature_false_arr_ptr)
{
    long nfeature_true = 0;
    long nfeature_false = 0;
    for(long isrc = 0; isrc < nhsc_data; isrc ++){
        if(0 == hsc_data_arr[isrc]->GetTrueFalse()){
            nfeature_false ++;
        } else if(1 == hsc_data_arr[isrc]->GetTrueFalse()){
            nfeature_true ++;
        } else {
            abort();
        }
    }

    double** feature_true_arr = new double* [nfeature_true];        
    double** feature_false_arr = new double* [nfeature_false];
    int nftr = hsc_data_arr[0]->GetNftr();
    long ifeature_true = 0;
    long ifeature_false = 0;    
    for(long isrc = 0; isrc < nhsc_data; isrc ++){
        if(0 == hsc_data_arr[isrc]->GetTrueFalse()){
            feature_false_arr[ifeature_false] = new double[nftr];
            for(int icol = 0; icol < nftr; icol ++){
                feature_false_arr[ifeature_false][icol] =
                    hsc_data_arr[isrc]->GetFtrArrElm(icol);
            }
            ifeature_false ++;
        } else if(1 == hsc_data_arr[isrc]->GetTrueFalse()){
            feature_true_arr[ifeature_true] = new double[nftr];
            for(int icol = 0; icol < nftr; icol ++){
                feature_true_arr[ifeature_true][icol] =
                    hsc_data_arr[isrc]->GetFtrArrElm(icol);
            }
            ifeature_true ++;
        } else {
            abort();
        }
    }

    *nfeature_true_ptr = nfeature_true;
    *feature_true_arr_ptr = feature_true_arr;
    *nfeature_false_ptr = nfeature_false;
    *feature_false_arr_ptr = feature_false_arr;
    
}
