
#include "walks.hpp"
#include "embedding.hpp"
#include "gapprox_embed.hpp"
#include "trash_keeper.hpp"
#include "pattern.hpp"
#include "compressed_embed.hpp"

namespace rwalk {
    void random_walk(Store& st);
    Embedding* fwd_extension(Janitor& jtr, pattern& pat, Embedding* embeds, Store& st);
    Embedding* back_extension(Janitor& jtr, pattern& pat, Embedding* embeds, Store& st);

    Embedding* initialize_walk(Store& st, pattern& pat) {
        types::label_t lab;
        types::vlist_t vlist;
        st.get_random_l1(lab, vlist);
        pat.add_fwd_vertex(lab);
        //INFO(*logger, lab << " and vlist size is" << vlist.size());
        /*
         * Create level 1 embeddings depending on the type of the algorithm that
         * we are running; gapprox_embedding to test GAPPROX method
         */
        Embedding* embeds;
        if(!true) {
            // complete enumeration of all the embeddings of the candidate
            // pattern
            embeds = new GApprox::GApproxEmbedding();
        }
        else {
            // Store the embeddings using the representative sets
            embeds = new LabelPruning::RepEmbedding();
        }
        embeds->init_embeddings(st, lab, st.l1pats[lab] );
        return embeds;
    }
    void walks(Store& st) {
        int numwalks = st.get_num_walks();
        for(int walk=0; walk < numwalks; walk++) {
            random_walk(st);
        }
    }
    void random_walk(Store& st) {
        pattern pat;
        Embedding* embeds = initialize_walk(st, pat);
        Logger* logger = st.get_logger();
        INFO(*logger, embeds->to_string());
        INFO(*logger, Hops::to_string(st.db_hops));
        Janitor jtr;
        int toss;
        while(true) {
            Embedding* result =  0; // next set of embeddings
            toss = st.myran.det_toss();
            vi tries;
            tries.push_back(toss); tries.push_back(1-toss);
            tr(tries,it) {
                if(*it) {
                    result = fwd_extension(jtr, pat, embeds,  st);
                }
                else {
                    result = back_extension(jtr, pat, embeds,  st);
                }
                if(result) {
                    embeds = result;
                    INFO(*logger, "after extension " << embeds->to_string());
                    break;
                }
            }
            if(!result)
                break;
        }
    }

    Embedding* fwd_extension(Janitor& jtr, pattern& pat, Embedding* embeds, Store& st) {
        /* Shuffle the list of vertices from which the extension is tried and
         * the labels with which the extensions are tried from a given vertex
         * in the pattern
         */
        if(!embeds)
            throw std::runtime_error(" Embedding is null");
        types::pat_vlist_t vertices;
        vector<types::label_t> labels;
        pat.get_vertices(vertices);
        st.get_labels(labels);
        st.myran.myshuffle(vertices);
        st.myran.myshuffle(labels);
        Logger* logger = st.get_logger();
        // Iterate over the complete set of possible extensions
        tr(vertices, it) {
            if(jtr.is_expired(*it))
                continue;
            tr(labels, it2) {
                if(jtr.is_failed_label(*it, *it2))
                    continue;
                else {
                    INFO(*logger, "src " << *it << "label " << *it2);
                    Embedding* extend_embed = embeds->extend_fwd(st, pat, *it,  *it2);
                    if(!extend_embed)
                        continue;
                    INFO(*logger, "computed extensions");
                    INFO(*logger, extend_embed->to_string());
                    int sup = extend_embed->compute_support();
                    INFO(*logger, "computed support");
                    if( ! !st.is_frequent(sup)) {
                        jtr.add_failed_label(*it, *it2);
                        INFO(*logger, "infrequent extension");
                        delete extend_embed;
                    }
                    else {
                        // update the set of embeddings
                        pat.add_fwd_vertex(*it2);
                        delete embeds;
                        INFO(*logger, "frequent extension");
                        return extend_embed;
                    }
                }
            }
            jtr.add_expired_vertex(*it);
        }
        return 0; // return a null pointer if none of the extensions are frquent
    }

    Embedding* back_extension(Janitor& jtr, pattern& pat, Embedding* embeds, Store& st) {
        types::pat_elist_t pat_edges;
        st.myran.myshuffle(pat_edges);
        tr(pat_edges, it) {
            if(jtr.is_back_fail(it->first, it->second))
                continue;
            else {
                int sup = 0; //TODO
                if(st.is_frequent(sup)) {
                }
                else {
                    return embeds;
                }
            }
        }
        return 0;
    }
}
