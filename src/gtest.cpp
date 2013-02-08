#include <gtest.h>
#include "Globals.hpp"
#include "khop.hpp"
#include "read_hop.hpp"
#include "pattern.hpp"
#include "store.hpp"
#include "preprocess.hpp"
#include "types.hpp"

int my_argc;
char** my_argv;
Store st;
Config* conf;
Logger* logger = Logger::get_logger("MAIN");

TEST(Hop,HopRead) {
    Hops::db_hops_t dbhops;
    Hops::read_hop(string("../data/test.khops"), dbhops);
    cout << Hops::to_string(dbhops);
}

TEST(Hop, HopComp) {
    // check if the pattern hops are computed correctly
    pattern pat;
    pat.add_fwd_vertex(0);
    pat.add_fwd_vertex(2);
    pat.add_fwd_vertex(5);
    pat.add_edge(0,2);
    pat.add_edge(1,2);
    pat.add_edge(0,1);
    cout << pat.to_string();
    types::pat_vlist_t pat_v;
    pat_v.push_back(0);
    pat_v.push_back(1);
    pat_v.push_back(2);
    Hops::pat_hops_t pathops;
    types::pat_elist_t pat_e;
    pat.get_edges(pat_e);
    types::vmap_t pat_vmap = pat.get_vmap();
    Hops::get_hops(pat_e , pat_v, pat_vmap, pathops );
    cout << Hops::to_string(pathops);
}

TEST(Hop, HopDistance) {
    // compute the distance between two khop objects
    pattern pat;
    pat.add_fwd_vertex(0);
    pat.add_fwd_vertex(1);
    pat.add_fwd_vertex(2);
    pat.add_fwd_vertex(3);
    pat.add_edge(0,1);
    pat.add_edge(0,3);
    pat.add_edge(1,3);
    pat.add_edge(1,2);
    Hops::pat_hops_t pathops = pat.get_hops();
    INFO(*logger, Hops::to_string(pathops));
    Hops::db_hops_t dbhops = st.db_hops;
    INFO(*logger, Hops::to_string(dbhops));
    types::cost_t mcost = pathops[0][1].distance(dbhops[1][1], 4, st.simvals);
    INFO(*logger, "matching cost" << mcost);
    mcost = dbhops[1][0].distance(dbhops[5][0], 4, st.simvals);
    INFO(*logger, "matching cost" << mcost);
}

int main(int argc,char** argv) {
    ::testing::InitGoogleTest(&argc,argv);
    my_argc = argc;
    my_argv = argv;
    conf = new Config(string(argv[1]));
    read_inp(*conf, st);
    //Args prog_args;
    //parse_args(my_argc,my_argv, prog_args);
    //st.set_args(prog_args);
    return RUN_ALL_TESTS();
}
