#ifndef PARALLEL_QSORT_QUICKSORTS_H
#define PARALLEL_QSORT_QUICKSORTS_H
#include <vector>
#include <random>

template<typename T>
class Sorter{
public:
    void sort(std::vector<T> &data) = 0;
};

template<typename T>
class SequentialQSort: public Sorter<T>{
public:
    void sort(std::vector<T> &data) override;

private:
    void sort_portion(std::vector<T> &data, std::size_t l, std::size_t r)
    std::size_t partition(std::vector<T> &data, std::size_t l, std::size_t r);
};

template<typename T>
class ParallelQSort: public Sorter<T>{
    //ParallelQSort(int blockSize); random generator??
public:
    void sort(std::vector<T> &data) override;

};

//SequentialQSort Realizations
template<typename T>
void SequentialQSort<T>::sort(std::vector<T> &data){
    sort_portion(data, 0, data.size() - 1 );
}

template<typename T>
void SequentialQSort<T>::sort_portion(std::vector<T> &data, std::size_t l, std::size_t r) {
    if (l < r){
        std::size_t m = partition(&data, l, r);
        sort_portion(data, l , m);
        sort_portion(data, m+1, r);
    }
}

template<typename T>
std::size_t SequentialQSort<T>::partition(std::vector<T> &data, std::size_t l, std::size_t r) {
    T dividor = data[l];
    std::size_t i = l, j = r;
    while (i<=j){
        while (data[i] < dividor)
            i++;
        while (data[j] > dividor)
            j--;
        if (i >= j)
            break;
        std::swap(data[i++], data[j--]);
    }
    return j;
}

//ParallelQSort Realizations
/*
quicksort(a, l, r):
if (r - l) >1
    x <- a[rand(l,r)]
    left <- filter(a, l, r, []{y < x})
    middle <- filter(a, l, r, []{y == x})
    right <- filter (a, l, r, []{y > x})
    forkjoin(
        quicksort(left)
        quicksort(right)
    )

filter : map into 0|1|0|0|1|... -> exclusive_scan -> insert with parallel_for
*/
void ParallelQSort<T>::sort(std::vector<T> &data){
    if (data.size() <= 1)
        return;
    dividor <- data[0];
}

#endif //PARALLEL_QSORT_QUICKSORTS_H
