#include "preprocess.hpp"

void read_offsets(Config& conf, Store& st) {
    if( present(conf.args, string("MULTI")))
        st.set_multigraph();
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
            while (inp >> offset) {
                offsets.push_back(offset);
            }
            st.set_offsets(offsets);
        }
    }
}
void read_inp(Config& conf, Store& st) {
    string s("DB");
    st.read_db(conf.args[s]);
    INFO(*logger, "Read db" << st.get_num_labels());
    st.read_cost(conf.args[string("SIM")]);
    INFO(*logger, "Read sim");
    Hops::read_hop(conf.args[string("HOPS")], st.db_hops);
    INFO(*logger,"Read Hops");
    st.copy_numeric_args(conf.numeric_args);
    st.set_algo(convertToInt(conf.args["ALGO"]));
    //throw runtime_error("INP");
    // read the offsets file if mining from a set of graphs
    read_offsets(conf, st);
    INFO(*logger,"Read offsets");
}

void preprocess(Config& conf, Store& st) {
    st.get_frequent_vertices(st.get_alpha(), st.get_minsup());
}
