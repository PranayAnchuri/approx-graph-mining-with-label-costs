/*
 * Collect statistics for the experiment
 */

#pragma once

#include "Globals.hpp"
#include "timer.hpp"
#include "Counter.hpp"

extern Logger* logger;
extern Logger* stat_logger;
class Stats {
    public:
        // start and stop timer for a parameter
        Stats();
        void start(const string& param);
        void stop(const string& param);
        void store(const string& param);
        void inc(string param, const int& val);
        std::string to_string();
    private:
        void add_timer(string param);
        void add_counter(string param);
        map<string,Timer> timers;
        map<string,Counter> counters;
        std::string delim;
};

