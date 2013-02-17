
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
        pat.add_fwd_label(lab);
        //INFO(*logger, lab << " and vlist size is" << vlist.size());
        /*
         * Create level 1 embeddings depending on the type of the algorithm that
         * we are running; gapprox_embedding to test GAPPROX method
         */
        Embedding* embeds;
        bool algo = st.get_algo();
        if(!algo) {
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
            MEASURE("Walk", random_walk(st));
        }
    }
    void random_walk(Store& st) {
        pattern pat;
        Embedding* embeds = initialize_walk(st, pat);
        //Logger* logger = st.get_logger();
        //INFO(*logger, embeds->to_string());
        //INFO(*logger, Hops::to_string(st.db_hops));
        Janitor jtr;
        int toss;
        int physical = get_physicalmem();
        int virt = get_virtualmem();
        INFO(*logger, "Num Labels" << st.get_num_labels());
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
                    //INFO(*logger, "after extension " << embeds->to_string());
                    virt = get_virtualmem();
                    physical = get_physicalmem();
                    break;
                }
            }
            if(!result)
                break;
        }
        st.stat->push_mem("Physical Memory", physical);
        st.stat->push_mem("Virtual Memory", virt);
        st.add_max_pat(pat);
	INFO(*stat_logger, pat.to_string());
    }

    Embedding* fwd_extension(Janitor& jtr, pattern& pat, Embedding* embeds, Store& st) {
        /* Shuffle the list of vertices from which the extension is tried and
         * the labels with which the extensions are tried from a given vertex
         * in the pattern
         */
        if(pat.get_pat_size() > 8)
            return 0;
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
                    pat.add_fwd(*it, *it2);
                    Embedding* extend_embed = embeds->extend_fwd(st, pat, *it,  *it2);
                    if(!extend_embed) {
                        jtr.add_failed_label(*it, *it2);
                        pat.undo_fwd();
                        continue;
                    }
                    INFO(*logger, "computed extensions");
                    int sup = extend_embed->compute_support(st.offsets);
                    INFO(*logger, "computed support" << sup);
                    INFO(*logger, extend_embed->to_string());
                    if( !st.is_frequent(sup)) {
                        jtr.add_failed_label(*it, *it2);
                        pat.undo_fwd();
                        INFO(*logger, "infrequent extension");
                        delete extend_embed;
                    }
                    else {
                        // update the set of embeddings
                        //pat.add_fwd_vertex(*it2);
                        delete embeds;
                        INFO(*logger, "frequent extension");
                        //INFO(*stat_logger, "Max pat \n" << pat.to_string());
                        return extend_embed;
                    }
                }
            }
            jtr.add_expired_vertex(*it);
        }
        return 0; // return a null pointer if none of the extensions are frquent
    }

    Embedding* back_extension(Janitor& jtr, pattern& pat, Embedding* embeds, Store& st) {
        if(pat.get_pat_size() > 8)
            return 0;
        types::pat_elist_t pat_edges;
        st.myran.myshuffle(pat_edges);
        tr(pat_edges, it) {
            if(jtr.is_back_fail(it->first, it->second))
                continue;
            else {
                // add edge to the pattern
                INFO(*logger, "Back extension " << it->first << " , " << it->second);
                pat.add_back(it->first, it->second);
                Embedding* extend_embed =\
                           embeds->extend_back(st, pat, it->first, it->second);
                if(!extend_embed) {
                    jtr.add_back_fail(it->first, it->second);
                    pat.undo_back();
                    continue;
                }
                // if set offsets is a vector of vertex boundaries
                int sup = extend_embed->compute_support(st.offsets);
                if(!st.is_frequent(sup)) {
                    jtr.add_back_fail(it->first, it->second);
                    INFO(*logger, "frequent extension");
                    pat.undo_back();
                    delete extend_embed;
                }
                else {
                    delete embeds;
                    INFO(*logger, "frequent extension");
                    return extend_embed;
                }
            }
        }
        return 0;
    }
}
