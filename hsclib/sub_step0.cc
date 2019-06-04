#include "sub_step0.h"

//
// Split an array into two, randomly
//
//
// [in]  double  val_arr
// [in]  int     num1    : length of first output vector
//

void SplitArrRand2(long narr, const long* const val_arr,
                   long num_of_first,
                   long* const narr1_ptr, long** const val1_arr_ptr,
                   long* const narr2_ptr, long** const val2_arr_ptr)
{
    double* ran_arr = new double [narr];
    for(long iran = 0; iran < narr; iran ++){
        ran_arr[iran] = MiRand::Uniform();
    }
    long* index_arr = new long [narr];
    MiSort::Sort(narr, ran_arr, index_arr, 0);

    long narr1 = num_of_first;
    long narr2 = narr - num_of_first;
    long* val1_arr = new long [narr1];
    long* val2_arr = new long [narr2];
    for(long iarr = 0; iarr < narr1; iarr ++){
        val1_arr[iarr] = val_arr[index_arr[iarr]];
    }
    for(long iarr = 0; iarr < narr2; iarr ++){
        val2_arr[iarr] = val_arr[index_arr[narr1 + iarr]];
    }

    delete [] ran_arr;
    delete [] index_arr;

    *narr1_ptr = narr1;
    *narr2_ptr = narr2;
    *val1_arr_ptr = val1_arr;
    *val2_arr_ptr = val2_arr;
}
