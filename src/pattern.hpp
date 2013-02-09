#pragma once

#include "Globals.hpp"
#include "khop.hpp"
#include "read_hop.hpp"

extern Logger* logger;
class pattern {
    public:
        void add_fwd_label(const types::label_t& label)\
                        { pat_vmap[pat_vmap.size()] = label;}
        void add_edge(int id1, int id2);
        void get_vertices(types::pat_vlist_t& vertices);
        void get_edges(types::pat_elist_t& edges);
        void add_fwd(const types::pat_vertex_t& src, const types::label_t& lab);
        void add_back(const types::pat_vertex_t& src, const types::pat_vertex_t& des);
        // call this after adding the next fwd vertex to the pattern
        types::pat_vertex_t get_fwd_id() const { return pat_vmap.size()-1;} 
        types::vmap_t get_vmap() { return pat_vmap;}
        string to_string();
        types::pat_elist_t get_edge_path(const types::pat_vertex_t& id);
        types::label_t get_label(const types::pat_vertex_t& pat_v) { return pat_vmap[pat_v];}
        types::pat_vlist_t get_vertices() const;
        map<types::pat_vertex_t, map<int,KhopLabel> > get_hops() const;
        int get_pat_size() const { return pat_vmap.size();}
        void set_sup(const int& sup);
        void undo_fwd();
        inline void undo_back();
        types::pat_graph_t get_adj_list() const;
    private:
        types::graph_t get_adj();
        map<int, types::label_t> pat_vmap;
        types::pat_elist_t pat_edges;
        int pat_sup;
};
