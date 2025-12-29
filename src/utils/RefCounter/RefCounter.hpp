#pragma once
#include <iostream>
#include <string>

using namespace std;

class RefCounter {
    private:
        int count;
    public:
        RefCounter();

        int add();
        int dec();
        int get();
};