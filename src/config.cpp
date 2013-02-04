#include "config.hpp"

Config::Config(string inpfile) {
    this->inpfile = inpfile;
    read_config_file();
}

void Config::read_config_file() {
    ifstream inp(inpfile.c_str());
    string key, value;
    while (inp >> key >> value) {
        args[key] = value;
        if(utils::is_double(value.c_str())) {
            numeric_args[key] = convertToDouble(value);
        }
    }
}
