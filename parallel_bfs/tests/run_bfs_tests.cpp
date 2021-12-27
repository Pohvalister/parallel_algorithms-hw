#include <gtest/gtest.h>
#include "parray.hpp"
#include "datapar.hpp"
#include <vector>

#include "graph_tools.h"
#include "../parallel_bfs.h"

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
}

TEST(basic_tests, parallel_like_sequential){
	std::vector<std::vector<int>> graph = generate_random_graph(10,0.4);

	std::vector<int> seq_distances = sequential_bfs(0, graph);
	std::vector<int> par_distances = parallel_bfs(0, graph);
	ASSERT_EQ(seq_distances.size(), par_distances.size());
	for (int i = 0; i < seq_distances.size(); i++){
		ASSERT_EQ(seq_distances[i], par_distances[i]);
	}
}

TEST(benchmarking, intensive_trials){

}

#include <chrono>

int calc_time(const std::vector<std::vector<int>>& graph, std::function<void(int, std::vector<std::vector<int>>)> bfs_func){
	std::chrono::steady_clock::time_point start_timer = std::chrono::steady_clock::now();
	bfs_func(0, graph);
	std::chrono::steady_clock::time_point stop_timer = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(stop_timer - start_timer).count();
}

TEST(benchmarking, cube500_solving){
	std::function<void(int, std::vector<std::vector<int>>)> par_bfs = [](int start, const std::vector<std::vector<int>>&data){ parallel_bfs(start, data);};
	std::function<void(int, std::vector<std::vector<int>>)> seq_bfs = [](int start, const std::vector<std::vector<int>>&data){ sequential_bfs(start, data);};
	int seq_time = 0, par_time=0;
	for (int i = 0; i < 3; i++){
		auto graph = generate_cubed_graph(500);
		seq_time += calc_time(graph,seq_bfs);
		par_time += calc_time(graph, par_bfs);
	}
	std::cout<<"seq: " << seq_time << ", par: " << par_time << '\n';
}
