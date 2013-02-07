#include "compressed_embed.hpp"

namespace LabelPruning{
    RepEmbedding::RepEmbedding() {
        supfunc = &RepEmbedding::min_node_sup;
    }
    void RepEmbedding::init_embeddings(Store& st, const types::label_t& lab, \
            const types::vlist_t& vids) {
        int num_labels = st.get_num_labels();
        RepSet rep_s; // _s for the set of representative vertices
        tr(vids, it) {
            types::db_vertex_t rep_label = st.get_label(*it);
            types::cost_t cost = st.simvals[lab*num_labels + rep_label];
            rep_s.insert(make_pair(*it, Repr(*it, cost)));
        }
    }

    std::string RepEmbedding::to_string() {
        std::stringstream ss;
        tr(embeds, it) {
            ss << " P Vertex - " << it->first << " ";
            tr(it->second, rep_it) {
                ss << rep_it->second.to_string();
            }
            ss << std::endl;
        }
        return ss.str();
    }

    void RepEmbedding::add_reps(const types::pat_vertex_t& pat_v, const map<types::db_vertex_t, Repr>& rep ) {
        embeds[pat_v] = rep;
    }

    void RepEmbedding::remove_invalid(map<types::pat_vertex_t, set<types::db_vertex_t> >& invalid) {
        tr(invalid, it) {
            tr(it->second, db_vertices) {
                embeds[it->first].erase(*db_vertices);
            }
        }
    }

    bool RepEmbedding::prune_reps(const pattern& pat, Store& st) {
        // prune the embeddings based on the pattern
        pat_hops_t phops; // key is the pat vertex and value is hop label
        db_hops_t dhops;
        int minsup = st.get_minsup();
        //pat.get_hops(phops);
        //db_hops_t dhops = st.get_db_hops();
        // Get max level for which pruning is done
        //int mxlevel = st.get_maxkhop();
        int maxlevel = 0;
        map<types::pat_vertex_t, set<types::db_vertex_t> > invalid;
        for(int level=0; level < maxlevel; level++) {
            tr(embeds, it) {
                // key is the pattern vertex
                KhopLabel pat_hop;
                types::pat_vertex_t pat_v = it->first;
                tr(it->second, rep_it) {
                    // if the database label doesnt dominates pattern label
                    if( !pat_hop.is_less(&dhops[rep_it->first])) {
                        // remove the vertex from the representative set
                        invalid[pat_v].insert(rep_it->first);
                    }
                    // check if the hop label is still valid
                    //if()
                }
            }
            // remove invalid vertices and compute the support
            remove_invalid(invalid);
            int sup = compute_support();
            if(sup < minsup) {
                // terminate the process
                return 0;
            }
        }
        return 1;
    }


    RepEmbedding* RepEmbedding::pruning(RepEmbedding* next_embeds, Store& st, const pattern& pat) {
        bool res = next_embeds->prune_reps(pat, st);
        int minsup = st.get_minsup();
        if(!res) {
            delete next_embeds;
            return 0;
        }
        else {
            // post pruning
            //next_embeds->complete_enumeration(st, pat);
            if(next_embeds->compute_support() < minsup) {
                delete next_embeds;
                return 0;
            }
            else
                return next_embeds;
        }
    }

    Embedding* RepEmbedding::extend_fwd(Store& st, const pattern& pat, types::pat_vertex_t src, \
                                            types::label_t lab) {
        // Add the candidate representatives for the next set and prune
        RepEmbedding* next_embeds = new RepEmbedding();
        *next_embeds = *this;
        // get the id of the next vertex that will be added to the embeddings
        types::pat_vertex_t des = pat.get_fwd_id();
        // Add the embedding for the new vertex
        next_embeds->add_reps(src, st.get_rep(lab));
        // prune the representative sets
        return pruning(next_embeds, st, pat);
    }

    Embedding* RepEmbedding::extend_back(Store& st, const pattern& pat, types::pat_vertex_t src, \
                                            types::pat_vertex_t des) {
        RepEmbedding* next_embeds = new RepEmbedding();
        *next_embeds = *this;
        // prune the representative vertices
        return pruning(next_embeds, st, pat);
    }

    int RepEmbedding::min_node_sup(map<types::pat_vertex_t, types::set_vlist_t>& unique_reps) {
        vector<int> sizes;
        tr(unique_reps, it) {
            sizes.push_back(it->second.size());
        }
        if(sizes.empty())
            return 0;
        int sup =  *min_element(all(sizes));
        return sup;
    }

    int RepEmbedding::compute_support() {
        map<types::pat_vertex_t, types::set_vlist_t> unique_reps;
        tr(embeds, it) {
            tr(it->second, rep_it) {
                unique_reps[it->first].insert(rep_it->first);
            }
        }
        return (this->*supfunc)(unique_reps);
    }
}
