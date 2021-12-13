#include <gtest/gtest.h>
#include <vector>

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


