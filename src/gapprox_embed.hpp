#include "embedding.hpp"
#include "Globals.hpp"
#include "types.hpp"

namespace GApprox {
    static Logger* logger = Logger::get_logger("MAIN");
    class GApproxEmbedding : public Embedding {
        public:
        GApproxEmbedding();
        virtual void init_embeddings(Store& st, const types::label_t& lab, \
                const types::vlist_t& vids);
        virtual std::string to_string();
        virtual int compute_support();
        int min_node_sup(map<types::pat_vertex_t, types::set_vlist_t>& unique_reps);
        virtual Embedding* extend_fwd(Store& st, \
                types::pat_vertex_t src, types::label_t lab);
        virtual Embedding* extend_back(Store& st, types::pat_vertex_t src, \
                                            types::pat_vertex_t back);
        virtual inline void add_embedding(const types::gapprox_em_t em) { embeds.push_back(em);}
        private:
        types::gapprox_embed_t embeds;
        // check this http://bit.ly/Xd for function pointers in objects
        typedef int (GApproxEmbedding::*fptr)(map<types::pat_vertex_t, \
                types::set_vlist_t>& unique_reps);
        fptr supfunc;
    };
}
