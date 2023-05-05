#ifndef GSORT_H_
#define GSORT_H_

#include <vector>
#include <string>

//Ave time:O(n^2)  Space:O(1)  stable
template <typename T>
std::vector<T> Bubblesort(std::vector<T> vec);

//Ave time:O(n^2)  Space:O(1)  non stable
template <typename T>
std::vector<T> Selectionsort(std::vector<T> vec);

//Ave time:O(n^2)  Space:O(1)  stable
template <typename T>
std::vector<T> Insertsort(std::vector<T> vec);

//Ave time:O(n log n)  Space:O(1)  non stable
template <typename T>
std::vector<T> Shellesort(std::vector<T> vec);

//Ave time:O(n log^2 n)  Space:O(1)  stable
template <typename T>
std::vector<T> Mergesort(std::vector<T> vec);

//Ave time:O(n log n)  Space:O(log n)  non stable
template <typename T>
std::vector<T> Quicksort(std::vector<T> vec);

//Ave time:O(n log^2 n)  Space:O(1)  non stable
template <typename T>
std::vector<T> Heapsort(std::vector<T> vec);

//Ave time:O(n+m)  Space:O(n+m)  stable
template <typename T>
std::vector<T> Countingsort(std::vector<T> vec);

//Ave time:O(n)  Space:O(m)  stable
template <typename T>
std::vector<T> Bucketsort(std::vector<T> vec);

//Ave time:O(k*n)  Space:O(n^2)  stable
template <typename T>
std::vector<T> Radixsort(std::vector<T> vec);

#endif


