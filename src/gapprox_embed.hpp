#include "embedding.hpp"
#include "Globals.hpp"
#include "types.hpp"
#include "pattern.hpp"

extern Logger* logger;

namespace GApprox {
    class GApproxEmbedding : public Embedding {
        public:
        GApproxEmbedding();
        virtual void init_embeddings(Store& st, const types::label_t& lab, \
                const types::vlist_t& vids);
        virtual std::string to_string();
        virtual int compute_support(const types::offsets_t& = types::offsets_t());
        int min_node_sup(map<types::pat_vertex_t, types::set_vlist_t>& unique_reps);
        virtual Embedding* extend_fwd(Store& st,  pattern& pat,\
                types::pat_vertex_t src, types::label_t lab);
        virtual Embedding* extend_back(Store& st, pattern& pat, types::pat_vertex_t src, \
                                            types::pat_vertex_t back);
        virtual inline void add_embedding(const types::gapprox_em_t em) { embeds.push_back(em);}
        private:
        types::gapprox_embed_t embeds;
        int sup_without_offsets();
        int sup_with_offsets(const types::offsets_t& offsets);
        // check this http://bit.ly/Xd for function pointers in objects
        typedef int (GApproxEmbedding::*fptr)(map<types::pat_vertex_t, \
                types::set_vlist_t>& unique_reps);
        typedef int (GApproxEmbedding::*db_sup_t)(map<types::pat_vertex_t, \
                types::set_vlist_t>& unique_reps, const types::offsets_t& off );
        fptr supfunc;
        db_sup_t db_sup_func; // compute the support from a database of graphs
    };
}
