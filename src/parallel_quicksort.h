#ifndef PARALLEL_QSORT_QUICKSORTS_H
#define PARALLEL_QSORT_QUICKSORTS_H
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <vector>
#include <random>

#include "parallel_tools/parallel_filter.h"

/*
quicksort(a, l, r):
if (r - l) >1
    x <- a[rand(l,r)]
    left <- filter(a, l, r, []{y < x})
    middle <- filter(a, l, r, []{y == x})
    right <- filter (a, l, r, []{y > x})
    forkjoin(
        quicksort(left)
        quicksort(right)
    )
    merge left, middle, right
*/

template<typename T>
static void parallel_copy(const std::vector<T>& src, std::vector<T>& dst, std::size_t offset){

#pragma grainsize 1
	cilk_for(std::size_t i = 0; i < src.size(); i++){
		dst[offset + i] = src[i];
	}
}

template<typename T>
static void recur_parallel_qsort(std::vector<T>& data){//end points on las elem
	if (data.size() <= 1)
		return;

	T dividor = data[data.size()/2];
	std::vector<T> lesser_part = cilk_spawn parallel_filter<T>(data, [&dividor](T const& val){return val < dividor;});
	std::vector<T> equal_part = cilk_spawn parallel_filter<T>(data, [&dividor](T const& val){return val == dividor;});
	std::vector<T> bigger_part = cilk_spawn parallel_filter<T>(data, [&dividor](T const& val){return val > dividor;});
	cilk_sync;

	cilk_spawn {
		recur_parallel_qsort(lesser_part);
		parallel_copy(lesser_part, data, 0);
	};

	cilk_spawn{
		recur_parallel_qsort(bigger_part);
		parallel_copy(bigger_part, data, data.size() - bigger_part.size());
	};

	parallel_copy(equal_part, data, lesser_part.size());
	cilk_sync;

}

template<typename T>
void parallel_qsort(std::vector<T>& data){
	recur_parallel_qsort(data);
}

#endif //PARALLEL_QSORT_QUICKSORTS_H
