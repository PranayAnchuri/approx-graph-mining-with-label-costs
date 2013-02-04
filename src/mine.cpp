#include "Globals.hpp"
#include "config.hpp"
#include "preprocess.hpp"

int main(int argc,char** argv)
{
    Config conf(argv[1]);
    Store st;
    preprocess(conf, st);
    return 0;
}
