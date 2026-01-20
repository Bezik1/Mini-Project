#pragma once

#include <string>

using namespace std;

template<typename T>
class HeapSort {
    public:
        static void sort(T* array, int size);
    private:
        static void heapify(T* array, int size, int index);

};

#include "HeapSort.tpp"