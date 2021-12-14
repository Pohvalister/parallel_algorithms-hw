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

class intensive_tests : public ::testing::Test{
protected:
	void SetUp(){
		const int BIG_AMOUNT = 100001;
		for (size_t i = 0; i < BIG_AMOUNT; i ++){
			big_input.push_back(rand() * (rand() % 2 ? 1 : -1));
			big_ones.push_back(1);
		}
	}
	std::vector<int> big_ones;
	std::vector<int> big_input;
};

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
class benchmarking : public ::testing::Test{
protected:
	void SetUp(){
		std::cout<< "\nCilk workers: " << __cilkrts_get_nworkers() << '\n';
		srand(time(0));
	}
	const std::size_t AMOUNT = 100000000;
	const std::size_t ITER = 5;

	template <typename T>
	int calc_time(std::vector<T> data, std::function<void(std::vector<T>&)> mapper)
	{
		std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();
		mapper(data);
		std::chrono::steady_clock::time_point stop_timer = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(stop_timer - start_timer).count();
	}
};


TEST_F(benchmarking, map){
	std::function<int(const int&)> inc = [](const int& val){return val + 1;};

	std::function<void(std::vector<int>&)> s_map = [&inc](std::vector<int>& data){ sequential_map(data, inc);};
	std::function<void(std::vector<int>&)> p_map = [&inc](std::vector<int>& data){ parallel_map(data, inc);};

	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++)
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));

		std::vector<int> data2 = data1;

		std::cout<< "seq map: " << calc_time(data1, s_map) << '\n';
		std::cout<< "par map: " << calc_time(data2, p_map) << '\n';
	}
}

TEST_F(benchmarking, scan){
	std::function<void(std::vector<int>&)> s_scan = [](std::vector<int>& data){ sequential_scan(data);};
	std::function<void(std::vector<int>&)> p_scan = [](std::vector<int>& data){ parallel_scan(data);};

	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++)
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));
		std::vector<int> data2 = data1;

		std::cout<< "seq scan: " << calc_time(data1, s_scan) << '\n';
		std::cout<< "par scan: " << calc_time(data2, p_scan) << '\n';
	}
}

TEST_F(benchmarking, filter){
	std::function<bool(const int&)> positive = [](const int& val){return val > 0;};

	std::function<void(std::vector<int>&)> s_filter = [&positive](std::vector<int>& data){ sequential_filter(data, positive);};
	std::function<void(std::vector<int>&)> p_filter = [&positive](std::vector<int>& data){ parallel_filter(data, positive);};

	for (std::size_t i = 0; i < ITER; i++){
		std::vector<int> data1;
		for (std::size_t i = 0; i < AMOUNT; i++)
			data1.push_back( rand() * (rand() % 2 ? 1 : -1));
		std::vector<int> data2 = data1;

		std::cout<< "seq filter: " << calc_time(data1, s_filter) << '\n';
		std::cout<< "par filter: " << calc_time(data2, p_filter) << '\n';
	}
}


// STRESS testing