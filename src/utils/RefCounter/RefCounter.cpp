#include <iostream>

#include "RefCounter.hpp"

using namespace std;

RefCounter::RefCounter() {
    count = 0;
}

int RefCounter::add() {
    return ++count;
}

int RefCounter::dec() {
    return --count;
}

int RefCounter::get() {
    return count;
}