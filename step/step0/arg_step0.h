#ifndef MORIIISM_HSC_STEP_STEP0_ARG_STEP0_H_
#define MORIIISM_HSC_STEP_STEP0_ARG_STEP0_H_

#include "mi_base.h"

class ArgValStep0 : public MiArgBase{
public:
    ArgValStep0() :
        MiArgBase(),
        progname_(""),
        csvfile_(""),
        csv_version_(""),
        nset_(0),
        outdir_(""),
        seed_(0) {}
    ~ArgValStep0(){
        Null();
    }
    void Init(int argc, char* argv[]);
    void Print(FILE* fp) const;

    string GetProgname() const {return progname_;};
    string GetCsvfile() const {return csvfile_;};
    string GetCsvVersion() const {return csv_version_;};
    int    GetNset() const {return nset_;};
    string GetOutdir() const {return outdir_;};
    int    GetSeed() const {return seed_;};

private:
    string progname_;
    string csvfile_;
    string csv_version_;
    int    nset_;
    string outdir_;
    int    seed_;

    void Null();
    void SetOption(int argc, char* argv[], option* long_options);
    void Usage(FILE* fp) const;
};

#endif // MORIIISM_HSC_STEP_STEP0_ARG_STEP0_H_
