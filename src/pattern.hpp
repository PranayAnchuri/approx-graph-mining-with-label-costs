#pragma once

#include "Globals.hpp"

class pattern {
    public:
        void add_fwd_vertex(types::label_t label);
        void add_edge(int id1, int id2);
        void get_vertices(types::pat_vlist_t& vertices);
        void get_edges(types::pat_elist_t& edges);
    private:
        map<int, types::label_t> pat_vmap;
        types::pat_elist_t pat_edges;
};
