#include "pattern.hpp"

void pattern::add_fwd_vertex(types::label_t lab) {
    pat_vmap[pat_vmap.size()] = lab;
}

void pattern::add_edge(int id1, int id2) {
    pat_edges.push_back(ii(id1, id2));
}

void pattern::get_vertices(types::pat_vlist_t& vertices) {
    REP(i, 0, pat_vmap.size()) {
        vertices.push_back(i);
    }
}

void pattern::get_edges(types::pat_elist_t& edges) {
    edges = pat_edges;
}

string pattern::to_string() {
    stringstream ss;
    tr(pat_vmap, it) {
        ss << " v " << it->first << " " <<it->second << endl;
    }
    tr(pat_edges, it) {
        ss << "e " << it->first << " " << it->second << endl;
    }
    return ss.str();
}
