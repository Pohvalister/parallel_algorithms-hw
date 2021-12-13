#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <functional>

#include "parallel_tools/parallel_filter.h"
#include "parallel_tools/parallel_map.h"
#include "parallel_tools/parallel_scan.h"
#include "sequentional_tools.h"

template<typename T>
static std::string toStr(const T& val){
	return std::to_string(val);
}

class basic_tests : public ::testing::Test{
protected:
	void SetUp(){
		const int AMOUNT = 101;
		for (size_t i = 0; i < AMOUNT; i++)
			input.push_back(random());
	}
	std::vector<int> input;
};

class intensive_tests : public ::testing::Test{
protected:
	void SetUp(){
		const int BIG_AMOUNT = 100001;
		for (size_t i = 0; i < BIG_AMOUNT; i ++)
			big_input.push_back(1);//big_input.push_back(rand() * (rand() % 2 ? 1 : -1));
	}
	std::vector<int> big_input;
};

TEST_F(basic_tests, parallel_filter){
	std::function<bool(const int&)> greater = [](const int& val){return val > 0;};
	std::vector<int> parallel_output = parallel_filter(input, greater);
	std::vector<int> sequential_output = sequential_filter(input, greater);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);

}

TEST_F(basic_tests, parallel_map){
	std::function<std::string(const int&)> toStrFunc(toStr<int>);
	std::vector<std::string> parallel_output = parallel_map(input, toStrFunc);
	std::vector<std::string> sequential_output = sequential_map(input, toStrFunc);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}

TEST_F(basic_tests, parallel_scan){
	int lastP, lastS;

	std::vector<int> parallel_output = input;
	lastP = parallel_scan(parallel_output);
	std::vector<int> sequential_output = input;
	lastS = sequential_scan(sequential_output);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	ASSERT_EQ(lastP, lastS);
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}

TEST_F(basic_tests, empty_vector){
	std::vector<int> input(0);
	std::function<std::string(const int&)> toStrFunc(toStr<int>);
	std::function<bool(const int&)> greater = [](const int& val){return val > 0;};

	ASSERT_EQ(parallel_filter(input, greater).size(), 0);
	ASSERT_EQ(parallel_map(input, toStrFunc).size(), 0);
	int last;
	ASSERT_EQ(parallel_scan(input), 0);
}

TEST_F(intensive_tests, parallel_map){
	std::function<int(const int&)> inc = [](const int& val){return val + 1;};

	std::vector<int> parallel_output = parallel_map(big_input, inc);
	std::vector<int> sequential_output = sequential_map(big_input, inc);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}

TEST_F(intensive_tests, parallel_scan){
	int lastP, lastS;

	std::vector<int> parallel_output = big_input;
	lastP = parallel_scan(parallel_output);
	std::vector<int> sequential_output = big_input;
	lastS = sequential_scan(sequential_output);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	ASSERT_EQ(lastP, lastS);
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}
#include <chrono>
template <typename T>
int calc_time(std::vector<T> data, std::function<void(std::vector<T>&)> mapper){
	std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();
	mapper(data);
	std::chrono::steady_clock::time_point stop_timer = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(stop_timer - start_timer).count();
}

TEST(benchmarking, map){
	std::cout<< "Cilk workers: " << __cilkrts_get_nworkers() << '\n';
	const std::size_t AMOUNT = 10000000;
	srand(time(0));

	std::function<int(const int&)> inc = [](const int& val){return val + 1;};
	const std::size_t ITER = 10;
	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++)
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));

		std::vector<int> data2 = data1;
		std::function<void(std::vector<int>&)> s_sort = [&inc](std::vector<int>& data){ sequential_map(data, inc);};
		std::cout<< "seq: " << calc_time(data1, s_sort) << '\n';

		std::function<void(std::vector<int>&)> p_sort = [&inc](std::vector<int>& data){ parallel_map(data, inc);};
		std::cout<< "par: " << calc_time(data2, p_sort) << '\n';
	}
}


// STRESS testing