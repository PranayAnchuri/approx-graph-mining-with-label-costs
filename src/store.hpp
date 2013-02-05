#pragma once

/*
 * Contains the adjacency representation of the graph,
 * vertex to label map, set of labels, set of edges in the
 * graph
 */

#include "Globals.hpp"

class Store {
    public:
        Store();
        void read_db(string file);
        void read_cost(string file);
        void get_frequent_vertices(types::cost_t alpha, int minsup);
        inline types::cost_t get_alpha() { return alpha;}
        inline int get_minsup() { return minsup;}
        inline int get_num_walks() { return walks;}
        inline types::label_t get_label(int id) { return vmap[id];}
        inline void get_labels(vector<types::label_t>& labels);
        inline int get_num_labels() { return num_labels;}
        void get_random_l1(types::label_t& lab, types::vlist_t& vlist); // for initializing random walk
        void copy_numeric_args(map<string,types::double_t>& numeric_args);
        map<types::label_t, types::vlist_t> l1pats;
        vector<types::cost_t> simvals;
        Prand myran;
    private:
        void add_vertex(const int& id, const types::label_t& label);
        void add_edge(const int& id1, const int& id2);
        types::graph_t gr;
        types::vmap_t vmap;
        types::lmap_t lmap;
        vector<types::label_t> freq_labels;
        int num_labels;
        int minsup;
        int walks;
        types::cost_t alpha;
        Logger* logger;
};
