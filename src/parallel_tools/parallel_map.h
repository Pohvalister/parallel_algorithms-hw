#ifndef PARALLEL_QSORT_PARALLEL_MAP_H
#define PARALLEL_QSORT_PARALLEL_MAP_H

#include <cilk/cilk.h>
#include <vector>
#include <cstddef>

template<typename TIn, typename TOut>
std::vector<TOut> parallel_map(const std::vector<TIn> &data, TOut(*func)(const TIn &)) {
    std::vector<TOut> result(data.size());

#pragma grainsize 1
    cilk_for(std::size_t i = 0; i < data.size(); i++){
        result[i] = func(data[i]);
    }
    return result;
}

#endif //PARALLEL_QSORT_PARALLEL_MAP_H
