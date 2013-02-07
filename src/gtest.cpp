#include <gtest.h>
#include "Globals.hpp"
#include "khop.hpp"
#include "read_hop.hpp"

int my_argc;
char** my_argv;

TEST(Hop,HopRead) {
    Hops::db_hops_t dbhops;
    Hops::read_hop(string("../data/test.khops"), dbhops);
    cout << Hops::to_string(dbhops);
}

int main(int argc,char** argv) {
    ::testing::InitGoogleTest(&argc,argv);
    my_argc = argc;
    my_argv = argv;
    //Args prog_args;
    //parse_args(my_argc,my_argv, prog_args);
    //st.set_args(prog_args);
    return RUN_ALL_TESTS();
}
