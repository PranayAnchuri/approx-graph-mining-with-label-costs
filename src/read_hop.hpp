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
    std::string to_string(db_hops_t& dbhops);
}
