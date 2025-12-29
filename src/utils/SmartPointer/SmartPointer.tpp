#pragma once
#include <iostream>

#include "SmartPointer.hpp"
#include "../RefCounter/RefCounter.hpp"

using namespace std;

template<typename T>
SmartPointer<T>::SmartPointer(T *newPointer) {
    pointer = newPointer;
    counter = new RefCounter();
    counter->add();
}

template<typename T>
SmartPointer<T>::SmartPointer(const SmartPointer &other) {
    pointer = other.pointer;
    counter = other.counter;
    counter->add();
}

template<typename T>
SmartPointer<T>::~SmartPointer() {
    if(counter->dec() == 0) {
        delete pointer;
        delete counter;
    }
}

template<typename T>
SmartPointer<T>& SmartPointer<T>::operator = (const SmartPointer &other) {
    if(this == &other) return *this;

    if(counter->dec() == 0) {
        delete pointer;
        delete counter;
    }

    pointer = other.pointer;
    counter = other.counter;
    counter->add();

    return *this;
} 

template<typename T>
T* SmartPointer<T>::operator -> () const {
    return pointer;
} 

template<typename T>
T& SmartPointer<T>::operator * () const {
    return *pointer;
} 