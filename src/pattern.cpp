#include "pattern.hpp"

void pattern::add_fwd_vertex(types::label_t lab) {
    pat_vmap[pat_vmap.size()] = lab;
}

void pattern::add_edge(int id1, int id2) {
    pat_edges.push_back(ii(id1, id2));
}
