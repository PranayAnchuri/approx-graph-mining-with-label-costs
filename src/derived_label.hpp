/*
 * Base class for derived labels that help in pruning the representative
 * sets for the vertices in the pattern
 */

#pragma once

#include "types.hpp"

class DerivedLabel {
    public:
        virtual bool is_less(DerivedLabel* rhs) = 0;
};


