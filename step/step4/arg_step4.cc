#include "arg_step4.h"

// public

void ArgValStep4::Init(int argc, char* argv[])
{
    progname_ = "step4";

    option long_options[] = {
        {"debug",      required_argument, NULL, 'd'},
        {"help",       required_argument, NULL, 'h'},
        {"verbose",    required_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    // long option default
    SetOption(argc, argv, long_options);

    printf("ArgVal::Init: # of arg = %d\n", argc - optind);
    int narg = 5;
    if (argc - optind != narg){
        printf("# of arguments must be %d.\n", narg);
        Usage(stdout);
    }
    int iarg = optind;
    csvfile_       = argv[iarg]; iarg++;
    csv_version_   = argv[iarg]; iarg++;
    lambda_        = atof(argv[iarg]); iarg++;
    niter_         = atoi(argv[iarg]); iarg++;
    outdir_        = argv[iarg]; iarg++;
}

void ArgValStep4::Print(FILE* fp) const
{
    fprintf(fp, "%s: g_flag_debug   : %d\n", __func__, g_flag_debug);
    fprintf(fp, "%s: g_flag_help    : %d\n", __func__, g_flag_help);
    fprintf(fp, "%s: g_flag_verbose : %d\n", __func__, g_flag_verbose);
    
    fprintf(fp, "%s: progname_      : %s\n", __func__, progname_.c_str());
    fprintf(fp, "%s: csvfile_       : %s\n", __func__, csvfile_.c_str());
    fprintf(fp, "%s: csv_version_   : %s\n", __func__, csv_version_.c_str());
    fprintf(fp, "%s: lambda_        : %e\n", __func__, lambda_);
    fprintf(fp, "%s: niter_         : %d\n", __func__, niter_);
    fprintf(fp, "%s: outdir_        : %s\n", __func__, outdir_.c_str());
}

// private

void ArgValStep4::Null()
{
    progname_      = "";
    csvfile_       = "";
    csv_version_   = "";    
    lambda_        = 0.0;
    niter_         = 0;
    outdir_        = "";
}

void ArgValStep4::SetOption(int argc, char* argv[], option* long_options)
{
    if(0 < g_flag_verbose){
        MPrintInfo("start...");
    }
    // option default
    g_flag_debug   = 0;
    g_flag_help    = 0;
    g_flag_verbose = 0;
    while (1) {
        int option_index = 0;
        int retopt = getopt_long(argc, argv, "dhv",
                                 long_options, &option_index);
        if(-1 == retopt)
            break;
        switch (retopt) {
        case 0:
            // long option
            break;
        case 'd':
            g_flag_debug = atoi(optarg);
            printf("%s: g_flag_debug = %d\n", __func__, g_flag_debug);
            break;
        case 'h':
            g_flag_help = atoi(optarg);
            printf("%s: g_flag_help = %d\n", __func__, g_flag_help);
            if(0 != g_flag_help){
                Usage(stdout);
            }
            break;
        case 'v':
            g_flag_verbose = atoi(optarg);
            printf("%s: g_flag_verbose = %d\n", __func__, g_flag_verbose);
            break;
        case '?':
            printf("%s: retopt (= %c) is invalid flag.\n",
                   __func__, retopt);
            Usage(stdout);
            break;

        default:
            printf("%s: error: getopt returned character code 0%o ??\n",
                   __func__, retopt);
            abort();
        }
    }
    if(0 < g_flag_verbose){
        MPrintInfo("done.");
    }
}

void ArgValStep4::Usage(FILE* fp) const
{
    fprintf(fp,
            "usage: %s [--help (0)] [--verbose (0)] [--debug (0)] "
            "csvfile  csv_version  lambda  niter  outdir \n",
            progname_.c_str());
    abort();
}
