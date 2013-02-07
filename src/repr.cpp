#include "repr.hpp"

Repr::Repr(const types::db_vertex_t& vid, const types::cost_t& cost) {
    this->vid = vid;
    this->costs[0] = cost;
    initcost = cost;
}

bool Repr::operator==(const Repr& r) const {
    return vid == r.vid;
}

bool Repr::operator<(const Repr& r) const {
    return vid < r.vid;
}

void Repr::replace(const int& level, const types::cost_t& cost) {
    // replace the cost of level with new cost
    this->costs[level] = cost;
}

void Repr::add(const int& level, const types::cost_t& inc) {
    // increase the cost of matching a given level
    this->costs[level]+= inc;
}

std::string Repr::to_string() const {
    std::stringstream ss;
    ss << vid <<  " ";
    return ss.str();
}
