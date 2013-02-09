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
        void push_mem(const string& memtype, const int& usage);
    private:
        void add_timer(string param);
        void add_counter(string param);
        map<string,Timer> timers;
        map<string,Counter> counters;
        map<string, vector<int> > mem_usage; // store mem usage before each max pat is enumerated
        std::string delim;
};

