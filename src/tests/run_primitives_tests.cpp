#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <functional>

#include "parallel_tools/parallel_filter.h"
#include "parallel_tools/parallel_map.h"
#include "parallel_tools/parallel_scan.h"

// preparations
template<typename T>
std::vector<T> sequential_filter(const std::vector<T>& data, bool(*predicate)(const T&)){
	std::vector<T> result;
	for (const T & value : data)
		if (predicate(value))
			result.push_back(value);

	return result;
}

template<typename TIn, typename TOut>
std::vector<TOut> sequential_map(const std::vector<TIn> &data, TOut(*func)(const TIn &)){
	std::vector<TOut> result;
	for (const TIn & value : data)
		result.push_back(func(value));

	return result;
}

template<typename T>
std::vector<T> sequential_scan(const std::vector<T> & data, T& lastVal){//exclusive_inplace scan
	std::vector<T> result;
	if (data.empty()){
		lastVal = 0;
		return result;
	}
	T sum = 0;
	for (const T& value : data){
		result.push_back(sum);
		sum += value;
	}
	lastVal = sum;
	return result;
}

template<typename T, T Comp>
bool greater(const T& val){
	return val < Comp;
}

template<typename T>
std::string toStr(const T& val){
	return std::to_string(val);
}

// BASIC testings
class basic_tests : public ::testing::Test{
protected:
	void SetUp(){
		const int AMOUNT = 100;
		for (size_t i = 0; i < AMOUNT; i++)
			input.push_back(random());
	}
	std::vector<int> input;
};

TEST_F(basic_tests, parallel_filter){
	std::vector<int> parallel_output = parallel_filter(input, greater<int, 0>);
	std::vector<int> sequential_output = sequential_filter(input, greater<int, 0>);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}

TEST_F(basic_tests, parallel_map){
	std::function<std::string(const int&)> toStrFunc(toStr<int>);
	std::vector<std::string> parallel_output = parallel_map(input, toStrFunc);
	std::vector<std::string> sequential_output = parallel_map(input, toStrFunc);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}

TEST_F(basic_tests, sequential_scan){
	int lastP, lastS;
	std::vector<int> parallel_output = parallel_scan(input, lastP);
	std::vector<int> sequential_output = parallel_scan(input, lastS);

	ASSERT_EQ(parallel_output.size(), sequential_output.size());
	ASSERT_EQ(lastP, lastS);
	for (std::size_t i = 0; i < parallel_output.size(); i++)
		ASSERT_EQ(parallel_output[i], sequential_output[i]);
}


TEST_F(basic_tests, empty_vector){
	std::vector<int> input(0);
	std::function<std::string(const int&)> toStrFunc(toStr<int>);

	ASSERT_EQ(parallel_filter(input, greater<int, 0>).size(), 0);
	ASSERT_EQ(parallel_map(input, toStrFunc).size(), 0);
	int last;
	ASSERT_EQ(parallel_scan(input, last).size(), 0);
}

// STRESS testing