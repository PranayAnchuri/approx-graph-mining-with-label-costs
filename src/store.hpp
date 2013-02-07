
/*
 * Contains the adjacency representation of the graph,
 * vertex to label map, set of labels, set of edges in the
 * graph
 */

#pragma once

#include "Globals.hpp"
#include "repr.hpp"
#include "read_hop.hpp"
#include "khop.hpp"

class Store {
    public:
        Store();
        void read_db(string file);
        void read_cost(string file);
        void get_frequent_vertices(types::cost_t alpha, int minsup);
        types::cost_t get_alpha() { return alpha;}
        int get_minsup(){return minsup;}
        int get_num_labels() { return num_labels;}
        int get_num_walks() {return walks;}
        types::label_t get_label(const types::db_vertex_t& id) { return vmap[id];}
        void get_labels(vector<types::label_t>& labels) { labels = freq_labels;}
        bool is_valid_edge(const int& id1, const int& id2) { return present(gr[id1], id2);}
        void get_random_l1(types::label_t& lab, types::vlist_t& vlist); // for initializing random walk
        void copy_numeric_args(map<string, types::double_t>& numeric_args);
        inline map<types::db_vertex_t, Repr> get_rep(const types::label_t lab) { return freq_reps[lab];}
        map<types::label_t, types::vlist_t> l1pats;
        vector<types::cost_t> simvals;
        Logger* get_logger() { return logger;}
        Prand myran;
        inline bool is_frequent(int sup) { return sup >= minsup; }
        types::set_vlist_t adjacent(const types::db_vertex_t& v) { return gr[v];}
        void read_hops(string file);
        map<types::db_vertex_t, map<int, KhopLabel> > db_hops;
    private:
        void add_vertex(const int& id, const types::label_t& label);
        void add_edge(const int& id1, const int& id2);
        types::graph_t gr;
        types::vmap_t vmap;
        types::lmap_t lmap;
        vector<types::label_t> freq_labels;
        map<types::label_t, map<types::db_vertex_t, Repr> > freq_reps;
        int num_labels;
        int minsup;
        int walks;
        types::cost_t alpha;
        Logger* logger;
};
