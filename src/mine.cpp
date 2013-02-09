#include "Globals.hpp"
#include "config.hpp"
#include "preprocess.hpp"
#include "walks.hpp"
#include "logger.hpp"


Logger* stat_logger;
Logger* logger;
void init_logger(string logfile, string statfile) {
    //std::filebuf *fbuf2 = new std::filebuf();
    //fbuf->open(statfile.c_str(), std::ios::out | std::ios::trunc);
    //fbuf2->open(logfile.c_str(), std::ios::out | std::ios::trunc);
    //Logger::set_global_streambuf(fbuf, true);
    //stat_logger->set_streambuf(fbuf);
    //logger->set_streambuf(fbuf2);
    //Logger::Logger(string logger_name, const char *log_head_format, log_level_t loglevel, int default_log_level, const ostream &output)
    stat_logger = new Logger("STAT", "%n %t %f-%L\t", LG_TRACE, 0, statfile.c_str());
    logger = new Logger("MAIN", "%n %t %f-%L\t", LG_TRACE, 0, logfile.c_str());

    //INFO(*lg, "test message: " << var);
    //DEBUG(*lg, "test message: " << tst);
    //return logger; 
}

int main(int argc, char** argv)
{
    init_logger(argv[2], argv[3]);
    INFO(*stat_logger,"test");
    //throw runtime_error("error");
    Config conf(argv[1]);
    Store st;
    read_inp(conf, st);
    preprocess(conf, st);
    rwalk::walks(st);
    return 0;
}
