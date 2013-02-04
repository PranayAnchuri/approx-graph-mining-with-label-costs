#pragma once

/*
 * Contains the adjacency representation of the graph,
 * vertex to label map, set of labels, set of edges in the
 * graph
 */

#include "Globals.hpp"

class Store {
    public:
        Store(){};
        void read_db(string file);
        void read_cost(string file);
        void get_frequent_vertices(types::cost_t alpha, int minsup);
        inline types::cost_t get_alpha() { return alpha;}
        inline int get_minsup() { return minsup;}
    private:
        void add_vertex(int id, types::label_t label);
        void add_edge(int id1, int id2);
        types::graph_t gr;
        types::vmap_t vmap;
        types::lmap_t lmap;
        vector<types::cost_t> simvals;
        map<types::label_t, types::vlist_t> l1pats;
        int num_labels;
        int minsup;
        types::cost_t alpha;
};
