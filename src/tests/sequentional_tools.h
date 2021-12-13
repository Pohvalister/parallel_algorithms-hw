#ifndef PARALLEL_QSORT_SRC_TESTS_SEQUENTIONAL_TOOLS_H
#define PARALLEL_QSORT_SRC_TESTS_SEQUENTIONAL_TOOLS_H

// primitives
template<typename T>
std::vector<T> sequential_filter(const std::vector<T>& data, std::function <bool(const T&)>predicate){
	std::vector<T> result;
	for (const T & value : data)
		if (predicate(value))
			result.push_back(value);

	return result;
}

template<typename TIn, typename TOut>
std::vector<TOut> sequential_map(const std::vector<TIn> &data, std::function<TOut(const TIn&)>func){
	std::vector<TOut> result;
	for (const TIn & value : data)
		result.push_back(func(value));

	return result;
}

template<typename T>
T sequential_scan(std::vector<T> & data){//exclusive_inplace scan
	if (data.empty())
		return 0;

	T sum = 0;
	for (std::size_t i = 0; i < data.size(); i++){
		std::swap(sum, data[i]);
		sum += data[i];
	}
	return sum;
}

// qsorting funcs
template<typename T>
static std::size_t partition(std::vector<T>& data, std::size_t start, std::size_t end){
	T dividor = data[(start + end) /2];
	std::size_t i = start, j = end;
	while (i < j){
		while (data[i]< dividor)
			i++;
		while (data[j] > dividor)
			j--;
		if (i >= j)
			break;
		std::swap(data[i], data[j]);
		i++;
		j--;
	}
	return j;
}

template<typename T>
static void recur_sequential_qsort(std::vector<T>& data, std::size_t start, std::size_t end){//end points on las elem
	if (start >= end)
		return;
	std::size_t split = partition(data, start, end);
	recur_sequential_qsort(data, start, split);
	recur_sequential_qsort(data, split + 1, end);
}

template<typename T>
void sequential_qsort(std::vector<T>& data){
	recur_sequential_qsort(data, 0, data.size() - 1);
}



#endif //PARALLEL_QSORT_SRC_TESTS_SEQUENTIONAL_TOOLS_H
