#ifndef PARALLEL_QSORT_PARALLEL_BFS_GRAPH_TOOLS_H
#define PARALLEL_QSORT_PARALLEL_BFS_GRAPH_TOOLS_H
#include <vector>
#include <queue>

//вход - список смежности графов
//выход - расстояния вершин от начальной
std::vector<int> sequential_bfs(int start_node, const std::vector<std::vector<int>>& edges_lists){
	std::vector<int> distances(edges_lists.size(), -1);
	distances[start_node] = 0;

	std::queue<int> nodes_to_consider;
	nodes_to_consider.push(start_node);

	while(!nodes_to_consider.empty()){
		int curr_node = nodes_to_consider.front();
		nodes_to_consider.pop();

		for (int node_connected : edges_lists[curr_node])
			if (distances[node_connected] == -1){
				distances[node_connected] = distances[curr_node] + 1;
				nodes_to_consider.push(node_connected);
			}
	}
	return distances;
}

// Graph generators
std::vector<std::vector<int>> generate_random_graph(int nodes_size, int edge_appear_probability){
	std::vector<std::vector<int>> graph(nodes_size, std::vector<int>());
	for (int i = 0; i < nodes_size; i++)
		for (int j = i + 1; j < nodes_size; j++)
			if (rand() % 2){
				graph[i].push_back(j);
				graph[j].push_back(i);
			}

	return graph;
}

int generate_num_via_coords(int cube_side_size, int x, int y, int z){
	return x + cube_side_size * y + cube_side_size * cube_side_size * z;
}
std::vector<std::vector<int>> generate_cubed_graph(int cube_side_size){
	std::vector<std::vector<int>> graph(cube_side_size * cube_side_size * cube_side_size, std::vector<int>());
	for (int i = 0; i < cube_side_size; i++)
		for (int j = 0; j < cube_side_size; j++)
			for (int k = 0; k < cube_side_size; k++){
				int node_num = generate_num_via_coords(cube_side_size, i, j, k);
				std::vector<int> nums_to_connect;
				if (i != cube_side_size - 1)
					nums_to_connect.push_back(generate_num_via_coords(cube_side_size, i+1, j, k));
				if (j != cube_side_size - 1)
					nums_to_connect.push_back(generate_num_via_coords(cube_side_size, i, j+1, k));
				if (k != cube_side_size - 1)
					nums_to_connect.push_back(generate_num_via_coords(cube_side_size, i, j, k+1));
				for (int num : nums_to_connect){
					graph[num].push_back(node_num);
					graph[node_num].push_back(num);
				}
			}
	return graph;
}
#endif //PARALLEL_QSORT_PARALLEL_BFS_GRAPH_TOOLS_H
