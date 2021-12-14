#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

#include "parallel_quicksort.h"
#include "sequentional_tools.h"

TEST(basic_tests, simple_usage){
	std::vector<int> data = {10, 20, -3, 4};
	parallel_qsort(data);
	ASSERT_EQ(data[0], -3);
	ASSERT_EQ(data[1], 4);
	ASSERT_EQ(data[2], 10);
	ASSERT_EQ(data[3], 20);
}

TEST(basic_tests, algos_equality){
	const int AMOUNT = 101;
	std::vector<int> parallel_data;
	for (std::size_t i = 0; i < AMOUNT; i++)
		parallel_data.push_back(rand());
	std::vector<int> sequentional_data = parallel_data;

	parallel_qsort(parallel_data);
	sequential_qsort(sequentional_data);

	ASSERT_EQ(parallel_data.size(), AMOUNT);
	ASSERT_EQ(parallel_data.size(), sequentional_data.size());
	for (std::size_t i = 0; i < parallel_data.size(); i++)
		ASSERT_EQ(parallel_data[i], sequentional_data[i]);
}

TEST(intensive_tests, big_amount){
	const std::size_t AMOUNT = 100000;
	srand(time(0));
	std::vector<int> data;
	for (std::size_t i = 0; i < AMOUNT; i++)
		data.push_back( rand() * (rand() % 2 ? 1 : -1));

	std::vector<int> example = data;
	std::sort(example.begin(), example.end());

	const std::size_t ITER = 100;
	for (std::size_t i = 0; i < ITER; i++){
		std::random_shuffle(data.begin(), data.end());
		parallel_qsort(data);

		for (std::size_t i = 0; i < AMOUNT; i++)
			ASSERT_EQ(data[i], example[i]);
	}
}

#include <chrono>

template <typename T>
int calc_time(std::vector<T> data, std::function<void(std::vector<T>&)> sorter){
	std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();
	sorter(data);
	std::chrono::steady_clock::time_point stop_timer = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(stop_timer - start_timer).count();
}

TEST(intensive_tests, benchmarking){
	std::cout<< "Cilk workers: " << __cilkrts_get_nworkers() << '\n';
	const std::size_t AMOUNT = 1000000;
	srand(time(0));

	const std::size_t ITER = 5;
	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++)
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));

		std::vector<int> data2 = data1;
		std::function<void(std::vector<int>&)> s_sort = [](std::vector<int>& data){ sequential_qsort(data);};
		std::cout<< "seq: " << calc_time(data1, s_sort) << '\n';

		std::function<void(std::vector<int>&)> p_sort = [](std::vector<int>& data){ parallel_qsort(data);};
		std::cout<< "par: " << calc_time(data2, p_sort) << '\n';
	}
}