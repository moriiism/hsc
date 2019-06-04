#ifndef MORIIISM_HSC_HSCLIB_SUB_STEP0_H_
#define MORIIISM_HSC_HSCLIB_SUB_STEP0_H_

#include "mi_rand.h"
#include "mi_sort.h"

void SplitArrRand2(long narr, const long* const val_arr,
                   long num_of_first,
                   long* const narr1_ptr, long** const val1_arr_ptr,
                   long* const narr2_ptr, long** const val2_arr_ptr);

#endif // MORIIISM_HSC_HSCLIB_SUB_STEP0_H_
