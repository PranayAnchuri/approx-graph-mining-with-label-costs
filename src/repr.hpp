/*
 * class for storing the information corresponding to a representative vertex
 */
#pragma once

#include "types.hpp"
#include<iostream>
#include<sstream>

class Repr {
    public:
    Repr(const types::db_vertex_t& vid, const types::cost_t& cost);
    bool operator==(const Repr& r) const;
    bool operator<(const Repr& r) const;
    void add(const int& level, const types::cost_t& inc);
    void replace(const int& level, const types::cost_t& cost );
    types::db_vertex_t get_key() const {return vid;}
    inline types::cost_t get_cost() const {return initcost;}
    inline types::cost_t max_cost() const {return maxcost;}
    std::string to_string() const;
    private:
        types::db_vertex_t vid;// this is the vertex in the database
        std::map<int, types::cost_t> costs; // cost of matching at each level
        types::cost_t initcost;
        types::cost_t maxcost;
};
