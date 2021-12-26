#ifndef PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
#define PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H

#include "parray.hpp"
#include "datapar.hpp"
#include <vector>
#include <cstdint>
#include <queue>
#include <atomic>
#include <functional>
/*
на вход - список смежности
non_deterministic_parallel_BFS:
frontier(i) -> frontier(i + 1)
чтобы не повторялось несколько раз - имеем массив, была ли положена вершина в сет (== есть ли она в выходном массиве)


for i = s ... :
//here need scan : deg|f_i[i].deg|f_i[2].deg|f_i[3].deg|...|f_i[size].deg
	f_i+1 = new int(сумма всех соседей кажой вершины, -1)

	pfor v = 1...|f_i|:
		pfor vu из E: //<- pfor?
			if a[u].cas(0,1):
				f_i+1[] = u
	f_i+1 = filter(f_i, x >= 0)

 bfs - расстояние от начальной до всех остальных вершин
*/
using namespace pasl::pctl;

std::vector<int> parallel_bfs(int start_node, const std::vector<std::vector<int>>& edges_lists){
	parray<std::atomic<int>> atomic_distances(edges_lists.size(), -1);
	parray<int> frontier = {start_node};

	atomic_distances[start_node] = 0;
	while(frontier.size()){
		//размеры вершин для создания нового frontier
		parray<int> fr_nodes_sizes(frontier.size(),[&frontier, &edges_lists, &atomic_distances](int node_id){
			return edges_lists[frontier[node_id]].size();
		});

		parray<int> sizes = scan(fr_nodes_sizes.begin(), fr_nodes_sizes.end(),(int) 0,  [](int x, int y){
			return x + y;
		},
		scan_type::forward_inclusive_scan);

		parray<int> new_frontier(sizes[sizes.size() - 1], -1);

		parallel_for(int(0), (int)frontier.size(), [&](int fr_id){//&frontier, &edges_lists, &sizes , &new_frontier, &atomic_distances
			int curr_node = frontier[fr_id];
		  parallel_for(int(0), (int)edges_lists[curr_node].size(),[&](int edge_id){
				int new_dist = atomic_distances[curr_node] + 1;
				int new_node = edges_lists[curr_node][edge_id];
				int tmp = -1;
				if (std::atomic_compare_exchange_strong(&(atomic_distances[new_node]), &tmp, new_dist)){}
					new_frontier[sizes[fr_id] + edge_id];
		  });
		});
		frontier = filter(new_frontier.begin(), new_frontier.end(), [](int node_id){
			return node_id!=-1;
		});
	}
	std::vector<int> result_distances(atomic_distances.size());
	parallel_for(int(0), (int)atomic_distances.size(), [&atomic_distances, &result_distances](int id){
		result_distances[id] = atomic_distances[id];
	});
	return result_distances;
}

#endif //PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
