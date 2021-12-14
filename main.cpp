#include <iostream>

#include "parallel_quicksort.h"

int main() {
	std::vector<int> tmp = {10, 20, -3, 4};
	parallel_qsort(tmp);
	for (auto val : tmp){
		std::cout<< val << ' ';
	}
    return 0;
}
