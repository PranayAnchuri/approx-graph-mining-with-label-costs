#include "Globals.hpp"
#include "config.hpp"
#include "preprocess.hpp"
#include "walks.hpp"

Logger* init_logger(string file) {
    Logger *lg = Logger::get_logger("MAIN");
    std::filebuf *fbuf = new std::filebuf();
    fbuf->open(file.c_str(), std::ios::out | std::ios::trunc);
    Logger::set_global_streambuf(fbuf, true);
    //INFO(*lg, "test message: " << var);
    //DEBUG(*lg, "test message: " << tst);
    return lg; 
}

int main(int argc, char** argv)
{
    init_logger(argv[2]);
    Config conf(argv[1]);
    Store st;
    read_inp(conf, st);
    preprocess(conf, st);
    rwalk::walks(st);
    return 0;
}
