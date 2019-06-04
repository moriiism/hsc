#ifndef MORIIISM_HSC_STEP_STEP4_ARG_STEP4_H_
#define MORIIISM_HSC_STEP_STEP4_ARG_STEP4_H_

#include "mi_base.h"

class ArgValStep4 : public MiArgBase{
public:
    ArgValStep4() :
        MiArgBase(),
        progname_(""),
        csvfile_(""),
        csv_version_(""),
        lambda_(0.0),
        niter_(0),
        outdir_("") {}
    ~ArgValStep4(){
        Null();
    }
    void Init(int argc, char* argv[]);
    void Print(FILE* fp) const;

    string GetProgname() const {return progname_;};
    string GetCsvfile() const {return csvfile_;};
    string GetCsvVersion() const {return csv_version_;};
    double GetLambda() const {return lambda_;};
    int    GetNiter() const {return niter_;};
    string GetOutdir() const {return outdir_;};

private:
    string progname_;
    string csvfile_;
    string csv_version_;
    double lambda_;
    int    niter_;
    string outdir_;

    void Null();
    void SetOption(int argc, char* argv[], option* long_options);
    void Usage(FILE* fp) const;
};

#endif // MORIIISM_HSC_STEP_STEP4_ARG_STEP4_H_
