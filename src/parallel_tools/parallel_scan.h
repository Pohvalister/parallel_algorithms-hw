#ifndef PARALLEL_QSORT_PARALLEL_SCAN_H
#define PARALLEL_QSORT_PARALLEL_SCAN_H
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <vector>
#include <cstdint>

template<typename T>
std::vector<T> parallel_scan(const std::vector<T> & data, T& lastVal){//exclusive_inplace scan

    std::size_t result_size = 1;
    int pow_counter = 0;
    while (result_size < data.size())
	{
		pow_counter++;
		result_size *= 2;
	}
    std::vector<T> result (result_size);// power of 2

#pragma grainsize 1
    cilk_for(std::size_t i = 0; i < result_size; i++){
		result[i] = (i < data.size() ? data[i] : 0);
	}
    //upwards
    for (std::size_t pow = 1; pow <= pow_counter; pow++) {
        int step = 1<<pow;
#pragma grainsize 1
        cilk_for(int i = step; i <= result_size; i = i + step){
            result[i] = result[i - 1] + result[i + step/2 - 1];// -1 because begin with i = 0
        }
    }

    lastVal = result[result_size - 1];
    result[result_size - 1] = 0;
    //downwards
    for (std::size_t pow = pow_counter; pow > 0; pow--){
        int step = 1<<pow;
#pragma grainsize 1
        cilk_for(std::size_t i = step; i <= result_size; i= i + step){
            std::swap(result[i-1], result[i - step/2 - 1]);
            result[i-1] += result[i - step/2 - 1];
        }
    }

    result.resize(data.size()); //removing power of 2
    return result;
}


#endif //PARALLEL_QSORT_PARALLEL_SCAN_H
