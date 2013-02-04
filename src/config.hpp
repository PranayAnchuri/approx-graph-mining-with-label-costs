#pragma once

#include "Globals.hpp"

/*
 * Accepts a file that contains all the parameters for the mining program
 * in key value format
 */
class Config {
    public:
        Config(string inpfile);
        map<string , string> args;
        map<string, types::double_t> numeric_args;
    private:
        string inpfile;
        void read_config_file();
};
