#ifndef PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
#define PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
/*
на вход - список смежности
non_deterministic_parallel_BFS:
frontier(i) -> frontier(i + 1)
чтобы не повторялось несколько раз - имеем массив, была ли положена вершина в сет


for i = s ... :
 //here need scan : deg|f_i[i].deg|f_i[2].deg|f_i[3].deg|...|f_i[size].de
f_i+1 = new int(сумма всех соседей кажой вершины, -1)

pfor v = 1...|f_i|:
	for vu из E: //<- pfor?
		if a[u].cas(0,1):
			f_i+1[] = u
		f_i+1 = filter(f_i, x >= 0)

 bfs - расстояние от начальной до всех остальных вершин
*/



#endif //PARALLEL_QSORT_PARALLEL_BFS_PARALLEL_BFS_H
