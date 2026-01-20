#include "HeapSort.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

template<typename T>
void HeapSort<T>::sort(T* array, int size) {
    if (size <= 1) return;

    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(array, size, i);
    }

    for (int i = size - 1; i > 0; i--) {
        swap(array[0], array[i]);
        heapify(array, i, 0);
    }
}

template<typename T>
void HeapSort<T>::heapify(T* array, int size, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && array[left] > array[largest])
        largest = left;

    if (right < size && array[right] > array[largest])
        largest = right;

    if (largest != index) {
        swap(array[index], array[largest]);
        heapify(array, size, largest);
    }
}