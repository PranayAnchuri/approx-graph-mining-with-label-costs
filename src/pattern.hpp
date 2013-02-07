#pragma once

#include "Globals.hpp"

class pattern {
    public:
        void add_fwd_vertex(types::label_t label);
        void add_edge(int id1, int id2);
        void get_vertices(types::pat_vlist_t& vertices);
        void get_edges(types::pat_elist_t& edges);
        // call this before adding the next fwd vertex to the pattern
        types::pat_vertex_t get_fwd_id() const { return pat_vmap.size();} 
        types::vmap_t get_vmap() { return pat_vmap;}
        string to_string();
        types::pat_elist_t get_edge_path(const types::pat_vertex_t& id);
        types::label_t get_label(const types::pat_vertex_t& pat_v) { return pat_vmap[pat_v];}
    private:
        map<int, types::label_t> pat_vmap;
        types::pat_elist_t pat_edges;
        types::graph_t get_adj();
};
