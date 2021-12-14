#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

#include "parallel_quicksort.h"
#include "parallel_tools/sequentional_tools.h"

TEST(basic_tests, simple_usage){
	std::vector<int> data = {10, 20, -3, 4};
	parallel_qsort(data);
	ASSERT_EQ(data[0], -3);
	ASSERT_EQ(data[1], 4);
	ASSERT_EQ(data[2], 10);
	ASSERT_EQ(data[3], 20);
}

TEST(basic_tests, parallel_qsort_filter){
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
TEST(basic_tests, parallel_qsort_inplace){
	const int AMOUNT = 101;
	std::vector<int> parallel_data;
	for (std::size_t i = 0; i < AMOUNT; i++)
		parallel_data.push_back(rand());
	std::vector<int> sequentional_data = parallel_data;

	parallel_qsort_inplace(parallel_data);
	sequential_qsort(sequentional_data);

	ASSERT_EQ(parallel_data.size(), AMOUNT);
	ASSERT_EQ(parallel_data.size(), sequentional_data.size());
	for (std::size_t i = 0; i < parallel_data.size(); i++)
		ASSERT_EQ(parallel_data[i], sequentional_data[i]);
}

class intensive_tests : public ::testing::Test{
protected:
	void SetUp(){

		srand(time(0));
		for (std::size_t i = 0; i < AMOUNT; i++)
			data.push_back( rand() * (rand() % 2 ? 1 : -1));

		example = data;
		std::sort(example.begin(), example.end());
	}
	const std::size_t AMOUNT = 100000;
	const std::size_t ITER = 100;

	std::vector<int> example;
	std::vector<int> data;
};
TEST_F(intensive_tests, parallel_qsort_filter){
	for (std::size_t i = 0; i < ITER; i++){
		std::random_shuffle(data.begin(), data.end());
		parallel_qsort(data);

		for (std::size_t i = 0; i < AMOUNT; i++)
			ASSERT_EQ(data[i], example[i]);
	}
}
TEST_F(intensive_tests, parallel_qsort_inplace){
	for (std::size_t i = 0; i < ITER; i++){
		std::random_shuffle(data.begin(), data.end());
		parallel_qsort_inplace(data);

		for (std::size_t i = 0; i < AMOUNT; i++)
			ASSERT_EQ(data[i], example[i]);
	}
}

#include <chrono>
class benchmarking : public ::testing::Test{
protected:
	void SetUp(){
		std::cout<< "\nCilk workers: " << __cilkrts_get_nworkers() << '\n';
		srand(time(0));
	}

	const std::size_t AMOUNT = 100'000'000;
	const std::size_t ITER = 5;
	std::function<void(std::vector<int>&)> s_sort = [](std::vector<int>& data){ sequential_qsort(data);};

	template <typename T>
	int calc_time(std::vector<T> data, std::function<void(std::vector<T>&)> sorter){
		std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();
		sorter(data);
		std::chrono::steady_clock::time_point stop_timer = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(stop_timer - start_timer).count();
	}
};


TEST_F(benchmarking, parallel_qsort_filter){
	std::function<void(std::vector<int>&)> p_sort = [](std::vector<int>& data){ parallel_qsort(data);};

	int seq_time = 0, par_time = 0;
	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++){
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));
		}

		std::vector<int> data2 = data1;
		seq_time+= calc_time(data1, s_sort);
		par_time+= calc_time(data2, p_sort);
	}
	std::cout<< "seq: " << seq_time << ", par_f: " << par_time << '\n';
}

TEST_F(benchmarking, parallel_qsort_inplace){
	std::function<void(std::vector<int>&)> p_sort = [](std::vector<int>& data){ parallel_qsort_inplace(data);};
	int seq_time = 0, par_time = 0;
	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++){
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));
		}

		std::vector<int> data2 = data1;
		seq_time+= calc_time(data1, s_sort);
		par_time+= calc_time(data2, p_sort);
	}
	std::cout<< "seq: " << seq_time << ", par_i: " << par_time << '\n';
}

TEST_F(benchmarking, parallel_qsort_inplace_blocked){

	for (int i = 10; i <= 10'000'000; i*=10){
		std::function<void(std::vector<int>&)> p_sort = [&i](std::vector<int>& data){ parallel_qsort_inplace(data, i);};
		int seq_time = 0, par_time = 0;
		for (std::size_t i = 0; i < ITER; i++){
			std::vector<int> data1;
			for (std::size_t i = 0; i < AMOUNT; i++){
				data1.push_back(rand() * (rand() % 2 ? 1 : -1));
			}

			std::vector<int> data2 = data1;
			seq_time+= calc_time(data1, s_sort);
			par_time+= calc_time(data2, p_sort);
		}
		std::cout<< "seq: " << seq_time << ", par_i_b_" << i <<": " << par_time << '\n';
	}
}