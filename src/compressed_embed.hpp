/*index*
 * Store the embedddings as a set of representative vertices for each vertex in
 * the pattern
 */
#pragma once

#include "embedding.hpp"
#include "Globals.hpp"
#include "types.hpp"
#include "repr.hpp"
#include "khop.hpp"
#include "read_hop.hpp"
#include "mymatch.hpp"

extern Logger* logger;
namespace LabelPruning {
    // Representative embedding
    class RepEmbedding : public Embedding {
        public:
        RepEmbedding();
        virtual void init_embeddings(Store& st, const types::label_t& lab, \
                const types::vlist_t& vids);
        virtual std::string to_string();
        virtual int compute_support(const types::offsets_t& offsets = types::offsets_t());
        int min_node_sup(map<types::pat_vertex_t, types::set_vlist_t>& unique_reps);
        virtual Embedding* extend_fwd(Store& st, pattern& pat,\
                types::pat_vertex_t src, types::label_t lab);
        virtual Embedding* extend_back(Store& st, pattern& pat, types::pat_vertex_t src, \
                                            types::pat_vertex_t back);
        void add_reps(const types::pat_vertex_t& pat_v, const map<types::db_vertex_t, Repr>& rep );

        private:
        // key is the pattern vertex and the value is the rep object
        void remove_invalid(map<types::pat_vertex_t, set<types::db_vertex_t> >& invalid);
        bool prune_reps(pattern& pat, Store& st);
        void retain_only_valid(types::bare_embeds_t& valid);
        bool verify_support(Store& st, pattern& pat, types::bare_embeds_t& valid);
        int sup_with_offsets(const types::offsets_t& offsets);
        int sup_without_offsets();
        RepEmbedding* pruning(RepEmbedding* next_embeds, Store& st, pattern& pat);
        bool enumerate(Store& st, pattern& pat, const vector<types::pat_edge_t>& path, int eindex, \
                                     types::cost_t threshold, const int& numlabels, vector<types::pat_edge_t>& epath,\
                                    map<types::pat_vertex_t,types::db_vertex_t>& covered, set<types::db_vertex_t>& tabu);
        /*bool match_nbrs(const types::vlist_t& pat_nbrs,\
                                        const types::bare_embeds_t& invalid,\
                                        const types::db_vertex_t des);*/
        bool match_nbrs(const types::vlist_t& pat_nbrs, Store& st,\
                                    types::bare_embeds_t& invalid,\
                                    const types::db_vertex_t des);
            // Store the candidate representative vertices for each vertex in the
            // pattern
        typedef map<types::db_vertex_t, Repr> RepSet;
        typedef map<types::pat_vertex_t, RepSet> RepEmbeds;
        RepEmbeds embeds;
        // check this http://bit.ly/Xd for function pointers in objects
        typedef int (RepEmbedding::*fptr)(map<types::pat_vertex_t, \
                types::set_vlist_t>& unique_reps);
        fptr supfunc;
        typedef map<types::pat_vertex_t, map<int, KhopLabel> > pat_hops_t;
        typedef map<types::db_vertex_t , map<int, KhopLabel> > db_hops_t;
    };
}
