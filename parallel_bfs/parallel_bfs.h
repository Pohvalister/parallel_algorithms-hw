#ifndef PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
#define PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H

#include "parray.hpp"
#include "datapar.hpp"
#include <vector>
#include <cstdint>
#include <atomic>

using namespace pasl::pctl;

std::vector<int> parallel_bfs(int start_node, const std::vector<std::vector<int>>& edges_lists){
	const int NOT_REACHED = -1;
	parray<int> distances(edges_lists.size(), NOT_REACHED);
	distances[start_node] = 0;
	parray<int> frontier = {start_node};

	int curr_distance = 0;
	while(frontier.size()){
		curr_distance++;
		parray<int> fr_nodes_sizes(frontier.size(),[&frontier, &edges_lists](int node_id){
			return edges_lists[frontier[node_id]].size();
		});

		parray<int> node_sizes = scan(fr_nodes_sizes.begin(), fr_nodes_sizes.end(), 0, [](int x, int y){
			return x + y;
		},
		scan_type::forward_inclusive_scan);

		parray<int> new_frontier(node_sizes[node_sizes.size() - 1], NOT_REACHED);

		parallel_for(0, (int)frontier.size(), [&](int fr_id){//&frontier, &edges_lists, &sizes , &new_frontier, &atomic_distances
			int curr_node = frontier[fr_id];
		  parallel_for(0, (int)edges_lists[curr_node].size(),[&](int edge_id){
				int new_node = edges_lists[curr_node][edge_id];
				int curr_place = node_sizes[fr_id] - edge_id - 1;
				int to_check = NOT_REACHED;
				if (__atomic_compare_exchange_n(&(distances[new_node]), &to_check, curr_distance ,false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
					new_frontier[curr_place] = new_node;
		  });
		});
		frontier = filter(new_frontier.begin(), new_frontier.end(), [&NOT_REACHED](int node_id){
			return node_id != NOT_REACHED;
		});
	}
	std::vector<int> result(distances.size());
	parallel_for(0, (int)distances.size(),[&](int ind){
		result[ind] = distances[ind];
	});
	return result;
}
#endif //PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
