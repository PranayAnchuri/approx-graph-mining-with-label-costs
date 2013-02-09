#include "Globals.hpp"
#include "config.hpp"
#include "preprocess.hpp"
#include "walks.hpp"
#include "logger.hpp"


Logger* stat_logger;
Logger* logger;
void init_logger(string logfile, string statfile) {
    stat_logger = new Logger("STAT", "%n %t %f-%L\t", LG_TRACE, 0, statfile.c_str());
    logger = new Logger("MAIN", "%n %t %f-%L\t", LG_OFF, 3, logfile.c_str());
}

int main(int argc, char** argv)
{
    init_logger(argv[2], argv[3]);
    Config conf(argv[1]);
    Store st;
    read_inp(conf, st);
    preprocess(conf, st);
    rwalk::walks(st);
    return 0;
}
