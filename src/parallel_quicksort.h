#ifndef PARALLEL_QSORT_QUICKSORTS_H
#define PARALLEL_QSORT_QUICKSORTS_H
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <vector>
#include <random>

#include "parallel_tools/parallel_filter.h"
#include "parallel_tools/sequentional_tools.h"

/*quicksort(a, l, r):
	if (r - l) > 1
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
static void parallel_copy(const std::vector<T>& src, std::vector<T>& dst, int offset){
#pragma grainsize 1
	cilk_for(int i = 0; i < src.size(); i++){
		dst[offset + i] = src[i];
	}
}

template<typename T>
static void recur_parallel_qsort(std::vector<T>& data){//end points on las elem
	if (data.size() <= 1)
		return;

	T dividor = data[0];
	std::vector<T> lesser_part = cilk_spawn parallel_filter<T>(data, [&dividor](T const& val){return val < dividor;});
	std::vector<T> equal_part = cilk_spawn parallel_filter<T>(data, [&dividor](T const& val){return val == dividor;});
	std::vector<T> bigger_part = parallel_filter<T>(data, [&dividor](T const& val){return val > dividor;});
	cilk_sync;

	cilk_spawn {
		recur_parallel_qsort(lesser_part);
		parallel_copy(lesser_part, data, 0);
	};
	cilk_spawn parallel_copy(equal_part, data, lesser_part.size());
	recur_parallel_qsort(bigger_part);
	parallel_copy(bigger_part, data, data.size() - bigger_part.size());
	cilk_sync;
}

template<typename T>
void parallel_qsort(std::vector<T>& data){
	recur_parallel_qsort(data);
}

template<typename T>
void recur_parallel_qsort_inplace(std::vector<T>& data, int start_pos, int end_pos){
	if (start_pos >= end_pos)
		return;
	int split = partition(data, start_pos, end_pos);
	cilk_spawn recur_parallel_qsort_inplace(data, start_pos, split);
	recur_parallel_qsort_inplace(data, split + 1, end_pos);
	cilk_sync;
}


template<typename T>//without filter
void parallel_qsort_inplace(std::vector<T>& data){
	recur_parallel_qsort_inplace(data, 0, data.size() - 1);
}


template<typename T>
void recur_parallel_qsort_inplace(std::vector<T>& data, int start_pos, int end_pos, int block_size){
	if (start_pos >= end_pos)
		return;
	else if (start_pos + block_size >= end_pos){
		recur_sequential_qsort(data, start_pos, end_pos);
		return;
	}
	int split = partition(data, start_pos, end_pos);
	cilk_spawn recur_parallel_qsort_inplace(data, start_pos, split);
	recur_parallel_qsort_inplace(data, split + 1, end_pos);
	cilk_sync;
}

template<typename T>//with seq BLOCKS
void parallel_qsort_inplace(std::vector<T>& data, int block_size){
	recur_parallel_qsort_inplace(data, 0, data.size() - 1, block_size);
}

#endif //PARALLEL_QSORT_QUICKSORTS_H
