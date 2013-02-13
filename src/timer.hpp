/*
 * starts and stops the timer
 */
#pragma once

#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include "measure.hpp"
#include "Globals.hpp"

#define microsec 1000000.0

extern Logger* logger;
extern Logger* stat_logger;
class Timer : public Measurement {

    public:
        Timer();
        void start();
        void stop();
        double time();
        void store(); // push the current time in to the vector
        std::string to_string(bool detailed=false);
    private:
        struct timeval start_time;
        struct timeval stop_time;
        bool  running;
        double curr_time;
        std::vector<double> stored_times;
};
