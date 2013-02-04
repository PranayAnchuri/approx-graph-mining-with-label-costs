#pragma once

#include "Globals.hpp"

class pattern {
    public:
        void add_fwd_vertex(types::label_t label);
        void add_edge(int id1, int id2);
    private:
        map<int, types::label_t> pat_vmap;
        vector<pair<int,int> > pat_edges;
};
