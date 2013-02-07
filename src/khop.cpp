#include "khop.hpp"

bool KhopLabel::is_less(DerivedLabel* rhs) {
    KhopLabel* rhs_khop = (KhopLabel*)rhs;
}

std::string KhopLabel::to_string() {
    std::stringstream ss;
    tr(hops, it) {
        ss << "(" << it->first << ", " << it->second << ") ";
    }
    return ss.str();
}
