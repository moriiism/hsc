#ifndef MORIIISM_HSC_STEP_STEP2_ARG_STEP2_H_
#define MORIIISM_HSC_STEP_STEP2_ARG_STEP2_H_

#include "mi_base.h"

class ArgValStep2 : public MiArgBase{
public:
    ArgValStep2() :
        MiArgBase(),
        progname_(""),
        csvfile_(""),
        csv_version_(""),
        source_id_dir_(""),
        lambda_file_(""),
        niter_(0),
        nset_(0),
        outdir_("") {}
    ~ArgValStep2(){
        Null();
    }
    void Init(int argc, char* argv[]);
    void Print(FILE* fp) const;

    string GetProgname() const {return progname_;};
    string GetCsvfile() const {return csvfile_;};
    string GetCsvVersion() const {return csv_version_;};
    string GetSourceIdDir() const {return source_id_dir_;};
    string GetLambdaFile() const {return lambda_file_;};
    int    GetNiter() const {return niter_;};
    int    GetNset() const {return nset_;};
    string GetOutdir() const {return outdir_;};

private:
    string progname_;
    string csvfile_;
    string csv_version_;
    string source_id_dir_;
    string lambda_file_;
    int    niter_;
    int    nset_;
    string outdir_;

    void Null();
    void SetOption(int argc, char* argv[], option* long_options);
    void Usage(FILE* fp) const;
};

#endif // MORIIISM_HSC_STEP_STEP2_ARG_STEP2_H_
