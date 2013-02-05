/*
 * Base class for the embeddings
 * Subclass this to store the embeddings in other format
 */
#pragma once

#include "Globals.hpp"
#include "store.cpp"

class Embedding{
    public:
        Embedding();
        virtual void init_embeddings(Store& st, const types::label_t& lab,\
                const types::vlist_t& vids);
        virtual Embedding* extend_fwd(Store& st, types::label_t);
        virtual int compute_support();
        virtual std::string to_string();
    private:
        /*virtual void extend_embeddings(const types::label_t& lab,\
                const types::vlist_t& vids) =0;
        virtual int compute_support() = 0;
        virtual void undo_embeddings() = 0;*/
};
