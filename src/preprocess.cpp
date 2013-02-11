#include "preprocess.hpp"

void read_offsets(Config& conf, Store& st) {
    if(st.is_multigraph()) {
        // read the offset file
        if(!present(conf.args, string("OFFSET"))) {
            throw runtime_error("Offset file required");
        }
        else {
            // read the offset file
            ifstream inp(conf.args[string("OFFSET")].c_str());
            int offset;
            types::offsets_t offsets;
            string val;
            while (inp >> offset >> val) {
                offsets.push_back(offset);
            }
            st.set_offsets(offsets);
        }
    }
}
void read_inp(Config& conf, Store& st) {
    string s("DB");
    st.read_db(conf.args[s]);
    st.read_cost(conf.args[string("SIM")]);
    Hops::read_hop(conf.args[string("HOPS")], st.db_hops);
    st.copy_numeric_args(conf.numeric_args);
    st.set_algo(convertToInt(conf.args["ALGO"]));
    //throw runtime_error("INP");
    // read the offsets file if mining from a set of graphs
    //read_offsets(conf, st);
}

void preprocess(Config& conf, Store& st) {
    st.get_frequent_vertices(st.get_alpha(), st.get_minsup());
}
