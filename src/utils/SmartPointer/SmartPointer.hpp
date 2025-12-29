#pragma once
#include <iostream>

#include "../RefCounter/RefCounter.hpp"

/**
 * @brief Smart Pointer class is responsible for easy manipulation of data allocated on heap.
 * User, while using smart pointers is not obligated to manually delete them after their usage 
 * is done. Instead these objects are being destroyed at the end of the scope they were created in.
 * 
 * @tparam T type of the data on which this class is pointing at.
 * 
 * @property *counter it's the pointer to RefCounter class object, which is responsible for remembering
 * current number of pointers for specified value.
 * 
 * @property *pointer points to heap allocated memory.
 * 
 * @note Alternative approach to the same problem as in Garbage Collector in many programming
 * languages. It solves the problem of resolving the memory allocated on heap after their usage
 * is done.
 */
template <typename T>
class SmartPointer {
    private:
        RefCounter *counter;
        T *pointer;
    public:
        SmartPointer(T *newPointer);
        SmartPointer(const SmartPointer &other);
        /**
         * @brief delete current pointer.
         * 
         * @details Deletion is done by decreasing `counter`, which
         * stores information about current number of pointers, which points
         * to specified data. After this decrease `Smart Pointer` class checks,
         * wheter value of the `counter` is 0 and if it is it deletes pointer and
         * counter.
         */
        ~SmartPointer();
        
        /**
         * @brief Assigns other `Smart Pointer` to `*this` one.
         * 
         * @details It is done by decreasing number of references pointing at
         * current data. After that we check wheter ref number is 0, and if it is
         * then we delete `counter` and `pointer`. Next we assign reference and pointer
         * of `other` pointer to this one and we increase number of references.
         * 
         * @return address of `this` object.
         */
        SmartPointer<T>& operator = (const SmartPointer &other);
        
        /**
         * @brief It's the operator that is used in order to access the pointed object
         * via it's reference.
         * 
         * @return reference to the pointed object.
         */
        T& operator * () const;
        /**
         * @brief It's the operator that is used in order to get the
         * stored inside pointer.
         * 
         * @return pointer.
         */
        T* operator -> () const;
};

#include "SmartPointer.tpp"