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
        // add the representatives for the first vertex
        embeds[0] = rep_s;
    }

    std::string RepEmbedding::to_string() {
        std::stringstream ss;
        tr(embeds, it) {
            ss << " P V - " << it->first << " -- R ";
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

    bool RepEmbedding::prune_reps(pattern& pat, Store& st) {
        // prune the embeddings based on the pattern
        pat_hops_t phops = pat.get_hops(); // key is the pat vertex and value is hop label
        int minsup = st.get_minsup();
        // Dont modify this object for god's sake
        db_hops_t& dhops = st.db_hops;
        // Get max level for which pruning is done
        //int mxlevel = st.get_maxkhop();
        int maxlevel = pat.get_pat_size();
        int num_labels = st.get_num_labels();
        types::cost_t alpha = st.get_alpha();
        map<types::pat_vertex_t, set<types::db_vertex_t> > invalid;
        for(int level=0; level < maxlevel; level++) {
            tr(embeds, it) {
                // key is the pattern vertex
                if(!present(phops[it->first], level))
                    continue;
                KhopLabel pat_hop = phops[it->first][level];
                types::pat_vertex_t pat_v = it->first;
                tr(it->second, rep_it) {
                    // if the database label doesnt dominates pattern label
                    if(!present(dhops[rep_it->first], level))
                        continue;
                    types::cost_t flowval = pat_hop.distance(dhops[rep_it->first][level], num_labels, st.simvals);
                    /*
                     * Returns -1 if the capacity constraints are not met
                     * total cost otherwise */
                    if( flowval < 0 || flowval > alpha)
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
        return 1;
    }


    RepEmbedding* RepEmbedding::pruning(RepEmbedding* next_embeds, Store& st, pattern& pat) {
        bool res = next_embeds->prune_reps(pat, st);
        if(!res) {
            delete next_embeds;
            return 0;
        }
        else {
            // post pruning
            //next_embeds->complete_enumeration(st, pat);
            types::bare_embeds_t valid;
            bool res = next_embeds->verify_support(st, pat, valid );
            if(!res) {
                delete next_embeds;
                return 0;
            }
            else{
                next_embeds->retain_only_valid(valid);
                return next_embeds;
            }
        }
    }

    Embedding* RepEmbedding::extend_fwd(Store& st, pattern& pat, types::pat_vertex_t src, \
            types::label_t lab) {
        // Add the candidate representatives for the next set and prune
        INFO(*logger, "Compressed Embedding forward extension");
        RepEmbedding* next_embeds = new RepEmbedding();
        *next_embeds = *this;
        // get the id of the next vertex that will be added to the embeddings
        types::pat_vertex_t des = pat.get_fwd_id();
        // Add the embedding for the new vertex
        next_embeds->add_reps(des, st.get_rep(lab));
        INFO(*logger, "Embeddings for the new vertex" );
        INFO(*logger, next_embeds->to_string());
        throw std::runtime_error(" check");
        // prune the representative sets
        return pruning(next_embeds, st, pat);
    }

    Embedding* RepEmbedding::extend_back(Store& st, pattern& pat, types::pat_vertex_t src, \
            types::pat_vertex_t des) {
        RepEmbedding* next_embeds = new RepEmbedding();
        *next_embeds = *this;
        // the candidate set of representative vertices dont change
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

    /***************************************** VERIFICATION *****************/

    void get_paths(map<types::pat_vertex_t, types::pat_elist_t>& paths, pattern& pat) {
        types::pat_vlist_t vertices;
        pat.get_vertices(vertices);
        tr(vertices, it) {
            paths[*it] = pat.get_edge_path(*it);
        }
    }

    void RepEmbedding::retain_only_valid(types::bare_embeds_t& valid) {
        // Retain only these k
        tr(embeds, patv) {
            typeof(embeds[patv->first].begin()) reps = embeds[patv->first].begin();
            while(reps != embeds[patv->first].end()) {
                // can we remove this candidate vertex
                if(!present(valid[patv->first], reps->first)) {
                    embeds[patv->first].erase(reps++);
                }
                else {
                    reps++;
                }
            }
        }
    }

    // Function to cover all the vertices in an embedding so that
    // complete enumeration from the corresponding vertices can
    // be avoided
    inline void add_valid(map<types::pat_vertex_t, types::db_vertex_t>& covered,\
            types::bare_embeds_t& valid) {
        tr(covered,it) {
            valid[it->first].insert(it->second);
        }
    }


    // try a complete enumeration from a candidate vertex
    bool RepEmbedding::enumerate(Store& st, pattern& pat, const vector<types::pat_edge_t>& path, int eindex, \
            types::cost_t threshold, const int& numlabels, vector<types::pat_edge_t>& epath, \
            map<types::pat_vertex_t,types::db_vertex_t>& covered, set<types::db_vertex_t>& tabu) {
        // try to enumerate a pattern starting from "vertex" and following the
        // epath is the exact path and covered are the mappings for the vertices
        if(threshold<0){
            return false;
        }
        if(eindex==path.size())
            return true;
        types::pat_edge_t ed = path[eindex];
        if(present(covered,ed.first) && present(covered,ed.second)) {
            // make sure that the edge is present in the graph and continue
            if(st.is_valid_edge( covered[ed.first], covered[ed.second])) {
                epath.push_back(ii(covered[ed.first], covered[ed.second]));
                return enumerate(st, pat, path,eindex+1,threshold, numlabels,\
                        epath, covered, tabu);
            }
            else {
                return false;
            }

        }
        else if(present(covered,ed.first) && !present(covered,ed.second)){
            // Fwd edge : get the neighbors of the current mapping 
            types::db_vertex_t v = covered[ed.first];
            types::label_t src_lab = pat.get_label(ed.second);
            // get the neighbors of v
            types::set_vlist_t ad = st.adjacent(v);
            // intersect adj list with the representatives of the vert ed.second
            //types::set_vlist_t common;
            //set_intersection(all(ad),all(em[ed.second]), inserter(common, common.end()));
            // recurse on each of the element
            tr(embeds[ed.second], rep) {
                if(!present(ad, rep->first ))
                    continue;
                types::label_t des_lab = st.get_label(rep->first);
                types::cost_t cost = st.simvals[src_lab*numlabels + des_lab];
                if(threshold<cost || present(tabu,rep->first)) {
                    continue;
                }
                epath.push_back(types::pat_edge_t(v,rep->first));
                covered[ed.second] = rep->first;
                tabu.insert(rep->first);
                bool res = enumerate(st,pat, path, eindex+1, threshold-cost, numlabels,  epath, covered, tabu);
                if(res)
                    return true;
                else {
                    epath.pop_back();
                    covered.erase(ed.second);
                    tabu.erase(rep->first);
                }
            }
            return false;
        }
        else {
            cerr << "Error :  cannot get the coverings of the vertices in the path"<<endl;
            abort();
        }
    }

    /* Verify the candidate representative vertices by exhaustive enumeration
     * valid is the compressed embedding without the Repr objects that are
     * verified to be valid
     */
    bool RepEmbedding::verify_support(Store& st, pattern& pat, types::bare_embeds_t& valid) {
        int numlabels = st.get_num_labels();
        int minsup = st.get_minsup();
        types::cost_t alpha = st.get_alpha();
        // 1) Get the paths that each vertex in the pattern has to follow
        map<types::pat_vertex_t, types::pat_elist_t > paths;
        get_paths(paths, pat);

        vector<types::cost_t>& costvalues = st.simvals; // TODO : change simvals to costvals

        // 2) Store the true approximate embeddings that are enumerated


        // 3) process all the representative sets
        tr(embeds, it) {
            // key is the pattern vertex and the value is the representative
            // vertices for the pattern vertex
            types::pat_vertex_t pat_v = it->first;
            tr(it->second, rep) { // key is db vertex and value is repr object
                if(present(valid[pat_v], rep->first))
                    continue; // vertex verified already
                // can we enumerate a pattern starting from *v
                // returns vertices if successful o/w empty
                map<types::pat_vertex_t, types::db_vertex_t> covered;
                vector<types::pat_edge_t> epath;
                covered[pat_v] = rep->first;
                // reduce the threshold if the label for the initial vertex also
                types::label_t patlabel = pat.get_label(pat_v);
                types::label_t vlabel = st.get_label(rep->first);
                double cost = costvalues[numlabels*patlabel + vlabel];
                set<types::db_vertex_t> tabu;
                tabu.insert(rep->first);
                bool res = enumerate(st, pat, paths[it->first], 0, alpha-cost, numlabels, epath, covered, tabu);
                if(res) {
                    add_valid(covered, valid);
                    // get the string corresponding to this
                    //if(st.num_embeds() > embeddings.size())
                    //embeddings.push_back(covered_to_string(covered, pat, st));
                }
            }

            int sup = compute_support();
            if(sup < minsup)
                return false;
        }
        return true;
    }
}
