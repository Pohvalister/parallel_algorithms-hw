#ifndef PARALLEL_QSORT_PARALLEL_MAP_H
#define PARALLEL_QSORT_PARALLEL_MAP_H
#include <cilk/cilk.h>
#include <vector>
#include <cstddef>

template<typename TIn, typename TOut>
std::vector<TOut> parallel_map(const std::vector<TIn> &data, std::function<TOut(const TIn&)>func) {//TOut(*func)(const TIn &)
    std::vector<TOut> result(data.size());

#pragma grainsize 10000
    cilk_for(int i = 0; i < data.size(); i++){
        result[i] = func(data[i]);
    }
    return result;
}

#endif //PARALLEL_QSORT_PARALLEL_MAP_H
