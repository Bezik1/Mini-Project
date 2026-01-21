#pragma once

#include <string>

using namespace std;

/**
 * @brief Template class providing an implementation of the Heap Sort algorithm.
 * 
 * @details This class contains static methods to perform in-place sorting on an array of any type T
 * that supports comparison operators. Heap Sort has a time complexity of O(n log n) and 
 * does not require additional memory allocation.
 * 
 * @tparam T the type of elements to be sorted. Must implement `operator>` or `operator<`.
 */
template<typename T>
class HeapSort {
public:
        /**
         * @brief Sorts an array using the Heap Sort algorithm.
         * 
         * @details The process consists of two main phases:
         * I. Building a max-heap from the input data.
         * II. Repeatedly extracting the maximum element from the heap and restoring the heap property.
         * 
         * @param array pointer to the first element of the array to be sorted.
         * @param size the number of elements in the array.
         * 
         * @return void
         */
        static void sort(T* array, int size);

    private:
        /**
         * @brief Maintains the max-heap property for a subtree rooted at a given index.
         * 
         * @details This function compares the element at `index` with its left and right children.
         * If a child is larger, it swaps the elements and continues the process downward (iteratively)
         * until the max-heap property is restored for the entire subtree.
         * 
         * @param array pointer to the array representing the heap.
         * @param size the current size of the heap to consider.
         * @param index the index of the root of the subtree to be heapified.
         * 
         * @return void
         */
        static void heapify(T* array, int size, int index);

};

#include "HeapSort.tpp"