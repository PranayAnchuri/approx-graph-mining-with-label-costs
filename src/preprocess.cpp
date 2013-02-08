#include "preprocess.hpp"

void read_inp(Config& conf, Store& st) {
    string s("DB");
    st.read_db(conf.args[s]);
    st.read_cost(conf.args[string("SIM")]);
    Hops::read_hop(conf.args[string("HOPS")], st.db_hops);
    st.copy_numeric_args(conf.numeric_args);
}

void preprocess(Config& conf, Store& st) {
    st.get_frequent_vertices(st.get_alpha(), st.get_minsup());
}
