/*
 * A class numerical stats
 */
#pragma once


#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include "measure.hpp"

class Counter : public Measurement {
    public:
        Counter();
        Counter(int init);
        void add(int val=1);
        int get();
        virtual std::string to_string();
    private:
        int count;
        int multiple;
        static const int factor=10000;
};
