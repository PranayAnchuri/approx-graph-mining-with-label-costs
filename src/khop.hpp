#pragma once
#include "derived_label.hpp"
#include "types.hpp"
#include "Globals.hpp"

/*
 * Compare the khop labels of the pattern and the database vertices
 */

class KhopLabel : public DerivedLabel {
    public:
        virtual bool is_less(DerivedLabel* rhs);
        inline void insert(types::label_t lab, int count) { hops.insert(make_pair(lab, count));}
        map<types::label_t, int> hops; // key is the label and value is the multiplicity
        std::string to_string();
};
