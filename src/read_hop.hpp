/*
 * Read the database multisets from a file instead of computing it each and every time
 */

#pragma once

#include "Globals.hpp"
#include "types.hpp"
#include "khop.hpp"

namespace Hops {
    typedef map<int, KhopLabel> level_hops_t;
    typedef map<types::db_vertex_t, level_hops_t> db_hops_t;
    typedef map<types::pat_vertex_t, level_hops_t> pat_hops_t;

    void read_hop(string filename, db_hops_t& dbhops);
    void get_hops(const types::pat_elist_t& edges, const types::pat_vlist_t& vertices,\
                                const types::vmap_t& labels, pat_hops_t& hopsets);
    std::string to_string(db_hops_t& dbhops);
}
