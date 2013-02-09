/*
 * Base class for all kinds of measurements
 */
#pragma once
#include <string>

class Measurement {
    public:
        virtual std::string to_string() =0;
};
