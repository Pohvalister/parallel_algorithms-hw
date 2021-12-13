#ifndef PARALLEL_QSORT_PARALLEL_FILTER_H
#define PARALLEL_QSORT_PARALLEL_FILTER_H
#include <cilk/cilk.h>
#include "parallel_scan.h"
#include "parallel_map.h"

//filter : map into 0|1|0|0|1|... -> exclusive_scan -> insert with parallel_for
template<typename T>
std::vector<T> parallel_filter(const std::vector<T> & data, bool(*predicate)(const T&)){
    std::vector<int> mapped = parallel_map<T, int>(data, [&predicate](const T& val){return (predicate(val) ? 1 : 0);});
    T result_size;
	std::vector<T> scanned = parallel_scan(data, result_size);

	std::vector<T> result(result_size);
#pragma grainsize 1
	cilk_for(std::size_t i = 0; i < data.size(); i++){
		if (mapped[i] != 0)
			result[scanned[i]] = data[i];
	}
	return result;
}

#endif //PARALLEL_QSORT_PARALLEL_FILTER_H
