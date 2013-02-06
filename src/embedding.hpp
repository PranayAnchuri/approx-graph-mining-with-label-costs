/*
 * Base class for the embeddings
 * Subclass this to store the embeddings in other format
 */
#pragma once

#include "Globals.hpp"
#include "store.hpp"

class Embedding{
    public:
        Embedding(){};
        virtual void init_embeddings(Store& st, const types::label_t& lab, \
                const types::vlist_t& vids) = 0;
        virtual Embedding* extend_fwd(Store& st, \
                types::pat_vertex_t src, types::label_t lab) = 0;
        virtual Embedding* extend_back(Store& st, types::pat_vertex_t src, \
                                            types::pat_vertex_t back) = 0;
        virtual int compute_support() = 0;
        virtual std::string to_string() = 0;
        void test();
    private:
        /*virtual void extend_embeddings(const types::label_t& lab, \
                const types::vlist_t& vids) =0;
        virtual int compute_support() = 0;
        virtual void undo_embeddings() = 0;*/
};
