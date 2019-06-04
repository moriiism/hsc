#ifndef MORIIISM_HSC_HSCLIB_HSCDATA_H_
#define MORIIISM_HSC_HSCLIB_HSCDATA_H_

#include "mi_iolib.h"

class HscData : public MiObject{
public:
    explicit HscData(string title = "") :
        MiObject("HscData", title),
        ftr_arr_(NULL),
        nftr_(0),
        true_false_(0) {}
    ~HscData(){
        Null();
    }
    void Init(int nftr);
    void Set16051900(double mag,
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
                     double psf);
    void Set17112400(double magerr,
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
                     double sigmapsf);
    void SetTrueFalse(int true_false) {true_false_ = true_false;};

    void Copy(const HscData* const org);
    HscData* const Clone() const;
    void Print16051900(FILE* fp) const;
    void Print17112400(FILE* fp) const;
    int GetNftr() const {return nftr_;};
    int GetTrueFalse() const {return true_false_;};
    double GetFtrArrElm(int icol) const {return ftr_arr_[icol];};
    
private:
    // ftr: feature
    
    double* ftr_arr_;
    int     nftr_;
    int     true_false_;
    
    void Null();
};

void LoadCsvDat16051900(string csv_dat,
                        long* const ndata_ptr,
                        HscData*** const hsc_data_arr_ptr,
                        int* const nname_ptr,
                        string** const name_arr_ptr);
void LoadCsvDat17112400(string csv_dat,
                        long* const ndata_ptr,
                        HscData*** const hsc_data_arr_ptr,
                        int* const nname_ptr,
                        string** const name_arr_ptr);

// for step1, 2, 3
void LoadSrcIdDat(string src_id_dat,
                  long* const nsrc_id_true_ptr,
                  long** const src_id_true_arr_ptr,
                  long* const nsrc_id_false_ptr,
                  long** const src_id_false_arr_ptr);

// for step1, 2, 3
void GetData(long nhsc_data, const HscData* const* const hsc_data_arr,
             long nsrc_id, const long* const src_id_arr,
             long* const nfeature_ptr,
             double*** const feature_arr_ptr);

// for step4
void GetDataNoSel(long nhsc_data, const HscData* const* const hsc_data_arr,
                  long* const nfeature_true_ptr,
                  double*** const feature_true_arr_ptr,
                  long* const nfeature_false_ptr,
                  double*** const feature_false_arr_ptr);

#endif // MORIIISM_HSC_HSCLIB_HSCDATA_H_
