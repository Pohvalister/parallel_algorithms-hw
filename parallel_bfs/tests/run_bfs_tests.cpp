#include <gtest/gtest.h>
#include "parray.hpp"
#include "datapar.hpp"
#include <vector>

#include "graph_tools.h"

void parse_coorded_num(int num, int cube_side_size, int& x, int& y, int& z){
	x = num % cube_side_size;
	num /= cube_side_size;
	y = num % cube_side_size;
	num /= cube_side_size;
	z = num;
}

TEST(basic_tests, cube_generation){
	const int CUBE_SIZE = 10;
	std::vector<std::vector<int>> graph = generate_cubed_graph(CUBE_SIZE);
	ASSERT_EQ(graph.size(), CUBE_SIZE * CUBE_SIZE * CUBE_SIZE);

	std::vector<int> distances = sequential_bfs(0,graph);
	ASSERT_EQ(graph.size(), distances.size());
	for (int i = 0; i < distances.size(); i++){
		int x, y, z;
		parse_coorded_num(i, CUBE_SIZE, x, y, z);
		ASSERT_EQ(distances[i], x + y + z);
	}

	pasl::pctl::parray<int> const& arr(1);
	ASSERT_EQ(1, 1);
}
