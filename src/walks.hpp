#pragma once
#include "Globals.hpp"
#include "store.hpp"

/*
 * Pick a random vertex from the set of frequent
 * vertices
 */
namespace rwalk {
    map<types::label_t, types::vlist_t> initialize_walk(Store& st);
    void walks(Store& st);
    //Logger* logger = Logger::get_logger("MAIN");
}
