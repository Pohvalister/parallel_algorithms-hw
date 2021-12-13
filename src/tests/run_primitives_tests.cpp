#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <functional>

#include "parallel_tools/parallel_filter.h"
#include "parallel_tools/parallel_map.h"
#include "parallel_tools/parallel_scan.h"
#include "sequentional_tools.h"

// preparations
template<typename T>
static std::string toStr(const T& val){
	return std::to_string(val);
}

// BASIC testings
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

TEST(stress_tests, parallel_smth){
	ASSERT_EQ(1, 1);
}

// STRESS testing