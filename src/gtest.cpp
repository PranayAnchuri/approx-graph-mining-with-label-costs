#include <gtest.h>
#include "Globals.hpp"
#include "khop.hpp"
#include "read_hop.hpp"
#include "pattern.hpp"
#include "store.hpp"
#include "preprocess.hpp"
#include "types.hpp"
#include "compressed_embed.hpp"

int my_argc;
char** my_argv;
Store st;
Config* conf;
Logger* logger = Logger::get_logger("MAIN");
Logger* stat_logger = Logger::get_logger("STAT");
pattern pat;

void initialize() {
    // initialize datastrucutures used by various tests
    pat.add_fwd_label(0);
    pat.add_fwd_label(1);
    pat.add_fwd_label(2);
    pat.add_fwd_label(3);
    pat.add_edge(0,1);
    pat.add_edge(0,3);
    pat.add_edge(3,1);
    pat.add_edge(1,2);
}

TEST(Label,HopRead) {
    Hops::db_hops_t dbhops;
    Hops::read_hop(string("../data/test.khops"), dbhops);
    cout << Hops::to_string(dbhops);
}

TEST(Label, HopComp) {
    // check if the pattern hops are computed correctly
    //INFO(*logger, pat.to_string());
    Hops::pat_hops_t pathops = pat.get_hops();
    KhopLabel lab = pathops[3][0];
    INFO(*logger, Hops::to_string(pathops));
    INFO(*logger, lab.to_string());
}

TEST(Label, HopDistance) {
    // compute the distance between two khop objects
    Hops::pat_hops_t pathops = pat.get_hops();
    INFO(*logger, Hops::to_string(pathops));
    Hops::db_hops_t dbhops = st.db_hops;
    INFO(*logger, Hops::to_string(dbhops));
    types::cost_t mcost = pathops[0][1].distance(dbhops[1][1], 4, st.simvals);
    INFO(*logger, "matching cost" << mcost);
    mcost = dbhops[1][0].distance(dbhops[5][0], 4, st.simvals);
    INFO(*logger, "matching cost" << mcost);
}

TEST(Label, NbrMatch) {
    // check if there is a one-to-one matching between the nbrs
    //RepEmbedding* embeds = new LabelPruning::RepEmbedding();
}

TEST(Offset, ORead) {
    // check if the offsets are read correctly and the initial vertices are
    // computed
    1+2;
}

int main(int argc,char** argv) {
    initialize();
    ::testing::InitGoogleTest(&argc,argv);
    my_argc = argc;
    my_argv = argv;
    conf = new Config(string(argv[1]));
    //read_inp(*conf, st);
    //Args prog_args;
    //parse_args(my_argc,my_argv, prog_args);
    //st.set_args(prog_args);
    return RUN_ALL_TESTS();
}
