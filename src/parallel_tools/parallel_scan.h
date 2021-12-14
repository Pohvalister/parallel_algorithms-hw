#ifndef PARALLEL_QSORT_PARALLEL_SCAN_H
#define PARALLEL_QSORT_PARALLEL_SCAN_H
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <vector>
#include <cstdint>

// exclusive inplace parallel scan для части массива длины степени 2-ки, начало с позиции start, возвращает сумму всех элементов
template<typename T>
T parallel_scan_pow2(std::vector<T> & data, const int & start, const std::size_t & pow2){
	int end = start + (1<<pow2);
	//upwards
	for (std::size_t i = 1; i <= pow2; i++){
		size_t jump = 1<<i;
#pragma grainsize 1
		cilk_for(int j = start + jump; j <= end; j += jump){
			data[j - 1] += data[j - jump/2 - 1];
		}
	}
	T total = data[end - 1];
	data[end - 1] = 0;
	//downwards
	for (std::size_t i = pow2; i > 0; i--){
		size_t jump = 1<<i;
#pragma grainsize 1
		cilk_for(int j = start + jump; j <= end; j+=jump){
			std::swap(data[j - 1], data[j - jump/2 - 1]);
			data[j - 1] += data[j - jump/2 - 1];
		}
	}
	return total;
}

std::size_t bytesOf(std::size_t num){
	std::size_t counter=0;
	while(num!=0){
		num>>=1;
		counter++;
	}
	return counter;
}

template<typename T>
T parallel_scan(std::vector<T> & data){//exclusive_inplace scan
	std::size_t bytes_count = bytesOf(data.size());//!!!

	T accumulated = 0;
	std::size_t offset = 0;
	for(std::size_t i = 0; i < bytes_count; i++)
		if (data.size() & 1 << i){
			T tmp = accumulated;
			data[offset] += accumulated;
			accumulated = parallel_scan_pow2(data, offset, i);
			data[offset] = tmp;
			offset += 1<<i;
		}

    return accumulated;
}


#endif //PARALLEL_QSORT_PARALLEL_SCAN_H
