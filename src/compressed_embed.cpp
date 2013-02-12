#include "compressed_embed.hpp"

namespace LabelPruning{
    bool is_fwd(pattern& pat);
    map<types::pat_vertex_t, int> min_khop_level(pattern& pat);
    map<types::pat_vertex_t, types::set_vlist_t> compute_groups(pattern& pat);

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
            ss << " \n P V - " << it->first << " -- R ";
            tr(it->second, rep_it) {
                ss << rep_it->second.to_string();
            }
            //ss << std::endl;
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
    bool RepEmbedding::match_nbrs(const types::vlist_t& pat_nbrs, Store& st,\
                                    types::bare_embeds_t& invalid,\
                                    const types::db_vertex_t des) {
        // is there a 1-1 matching between the vertices in the pattern and the
        // vertices in the db
        int psize = pat_nbrs.size();
        types::set_vlist_t& db_nbrs = st.gr[des];
        int gsize = db_nbrs.size();
        if(psize > gsize)
            return false;
        VVI adj(psize,VI(gsize)); // edges in the bipartite matching
        int row=0;
        tr(pat_nbrs,pn) {
            int col=0;
            tr(db_nbrs, gn) {
                if(!present(invalid[*pn], *gn)) {
                    adj[row][col] = 1;
                }
                col++;
            }
            row++;
        }
        VI row_match(psize);
        VI col_match(gsize);
        int msize = MyMatch::BipartiteMatching(adj, row_match, col_match);
        return msize==psize;
    }

    bool RepEmbedding::prune_reps(pattern& pat, Store& st) {
        // prune the embeddings based on the pattern
        //INFO(*logger, "Embeddings before pruning " << to_string());
        pat_hops_t phops = pat.get_hops(); // key is the pat vertex and value is hop label;
        int minsup = st.get_minsup();
        // Dont modify this object for god's sake
        db_hops_t& dhops = st.db_hops;
        // Compute the neighbors for each vertex which is used in the nbr
        // matching
        types::pat_graph_t pat_adj = pat.get_adj_list();
        // Get max level for which pruning is done
        //int mxlevel = st.get_maxkhop();
        int maxlevel = pat.get_pat_size();
        int num_labels = st.get_num_labels();
        types::cost_t alpha = st.get_alpha();
        map<types::pat_vertex_t, set<types::db_vertex_t> > invalid;
        /*
         * Minimum level for each vertex at which the khop comparison
         * should begin
         */
        map<types::pat_vertex_t, int> levels = min_khop_level(pat);
        /*
         * Groups : compute orbits for the vertices in the pattern
         */
        map<types::pat_vertex_t, types::set_vlist_t> groups = compute_groups(pat);
        //throw runtime_error("groups computed");
        bool fwd = is_fwd(pat);
        // TODO: try rearranging the for loops
        for(int level=0; level < maxlevel ; level++) {
            tr(embeds, it) {
                /*
                 * check if this vertex is leader of any particular group
                 */
                if(!present(groups, it->first))
                    continue;
                /*
                 * Should we compare the khop labels at this level
                 */
                if(fwd) {
                    if(levels[it->first] != level)
                        continue;
                }
                else {
                if(levels[it->first] < level)
                    continue;
                }
                // key is the pattern vertex
                if(!present(phops[it->first], level))
                    continue;
                KhopLabel pat_hop = phops[it->first][level];
                types::pat_vertex_t pat_v = it->first;
                tr(it->second, rep_it) {
                    // if the database label doesnt dominates pattern label
                    if(!present(dhops[rep_it->first], level))
                        continue;
                    bool nbrs_match;
                    MEASURE("nbrs", nbrs_match = match_nbrs(pat_adj[it->first], st,\
                                                    invalid, rep_it->first));
                    if(!nbrs_match) {
                        //invalid[pat_v].insert(rep_it->first);
                        tr(groups[pat_v], vert) {
                            invalid[*vert].insert(rep_it->first);
                        }
                        CMEASURE("Nbr Pruning", 1);
                        continue;
                        //CMEASURE("MATCH", 1);
                    }
                    /*
                     * Returns -1 if the capacity constraints are not met
                     * total cost otherwise */
                    types::cost_t flowval;
                    MEASURE("Pat Hop", flowval = pat_hop.distance(dhops[rep_it->first][level],\
                            num_labels, st.simvals, st));
                    //if(level > 1)
                    //INFO(*logger, "S "<< pat_v << " D "<< rep_it->first<<" C "<<flowval << "Level " << level);
                    if( flowval < 0 || flowval > alpha){
                        // remove the vertex from the representative set
                        //invalid[pat_v].insert(rep_it->first);
                        tr(groups[pat_v], vert) {
                            invalid[*vert].insert(rep_it->first);
                        }
                        CMEASURE("Flow Pruning", 1);
                        //CMEASURE("FLOW", 1);
                        continue;
                    }

                    /**** check if there is a 1-1 mapping between the neighbors of ****/
                }
                // check if the hop label is still valid
                //if()
            }
            // remove invalid vertices and compute the support
            remove_invalid(invalid);
            int sup = compute_support();
            if(sup < minsup) {
                // terminate the process
                //INFO(*logger, "failed after pruning");
                //INFO(*logger, "embeddings " << to_string());
                return 0;
            }
        } // end of the for loop
        //INFO(*logger, "success after pruning " << to_string());
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
            bool res;
            MEASURE("Verify" , res = next_embeds->verify_support(st, pat, valid ));
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
        //INFO(*logger, "Compressed Embedding forward extension");
        RepEmbedding* next_embeds = new RepEmbedding();
        MEASURE("COPYING", *next_embeds = *this);
        // get the id of the next vertex that will be added to the embeddings
        types::pat_vertex_t des = pat.get_fwd_id();
        //INFO(*logger, "pat size" << pat.get_pat_size());
        // Add the embedding for the new vertex
        MEASURE("Add Reps", next_embeds->add_reps(des, st.get_rep(lab)));
        //throw std::runtime_error(" check");
        // prune the representative sets
        return pruning(next_embeds, st, pat);
    }

    Embedding* RepEmbedding::extend_back(Store& st, pattern& pat, types::pat_vertex_t src, \
            types::pat_vertex_t des) {
        RepEmbedding* next_embeds = new RepEmbedding();
        MEASURE("COPYING", *next_embeds = *this);
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

    int RepEmbedding::compute_support(const types::offsets_t& offsets) {
        if(offsets.empty()) {
            return sup_without_offsets();
        }
        else {
            return sup_with_offsets(offsets);
        }
    }
    int RepEmbedding::sup_without_offsets() {
        map<types::pat_vertex_t, types::set_vlist_t> unique_reps;
        tr(embeds, it) {
            tr(it->second, rep_it) {
                unique_reps[it->first].insert(rep_it->first);
            }
        }
        return (this->*supfunc)(unique_reps);
    }
    int RepEmbedding::sup_with_offsets(const types::offsets_t& offsets) {
        map<types::pat_vertex_t, types::set_vlist_t> unique_reps;
        tr(embeds, it) {
            tr(it->second, rep_it) {
                //unique_reps[it->first].insert(rep_it->first);
                types::offsets_t::const_iterator lb =\
                                lower_bound(all(offsets), rep_it->first);
                int gid = lb - offsets.begin();
                unique_reps[it->first].insert(gid);
            }
        }
        return (this->*supfunc)(unique_reps);
    }

    /*
     *
     ******************* Optimizations ********************/
    bool is_fwd(pattern& pat) {
        // return true if the last edge added to the pattern is a fwd edge
        /*
         * Compute the adjacency list size of the "2nd" entry of the last edge
         * if len(adjlist) == 1 return true and false o/w
         */
        types::pat_edge_t last_edge = pat.get_last_edge();
        types::graph_t gr = pat.get_adj();
        types::pat_vertex_t des = last_edge.second;
        return gr[des].size() == 1;
    }

    map<types::pat_vertex_t, int> min_khop_level(pattern& pat) {
        // for which get where the comparison should begin
        /*
         * For each vertex in the pattern get the shortest distance to the
         * forward vertex; the minimum khop level is the just the shortest
         * distance
         */
        bool fwd = is_fwd(pat);
        map<types::pat_vertex_t, int> minlevel;
        types::pat_edge_t last_edge = pat.get_last_edge();
        types::pat_vlist_t pat_vlist = pat.get_vertices();
        tr(pat_vlist, it) {
            if(fwd) {
                minlevel[*it] = pat.sdist(*it, last_edge.second);
            }
            else {
                int dist1 = pat.sdist(*it, last_edge.first);
                int dist2 = pat.sdist(*it, last_edge.second);
                minlevel[*it] = min(dist1, dist2);
            }
        }
        return minlevel;
    }
    /*
     * Compute orbits of the vertices in the pattern
     * Returns a map where key is the vertex in the pattern
     * and value is the set of vertices for which this vertex
     * is responsible
     */
    map<types::pat_vertex_t, types::set_vlist_t> compute_groups(pattern& pat) {
        types::graph_t gr = pat.get_adj();
        set<types::pat_vertex_t> leafnodes;
        // return this map
        map<types::pat_vertex_t, types::set_vlist_t> ret;
        tr(gr, patv) {
            if(patv->second.size() == 1)
                leafnodes.insert(patv->first);
        }
        tr(gr, patv) {
            // divide the adjacency list into groups
            types::set_vlist_t adjlist = patv->second;
            types::set_vlist_t::const_iterator adjlist_it = adjlist.begin();
            // groups : key is the pat label and value is list of vertices with
            // same label
            map<types::label_t, types::set_vlist_t> groups;
            while(adjlist_it != adjlist.end()) {
                if(!present(leafnodes, *adjlist_it)) {
                    adjlist.erase(adjlist_it++);
                }
                else {
                    types::label_t nbr_label = pat.get_label(*adjlist_it);
                    groups[nbr_label].insert(*adjlist_it);
                    ++adjlist_it;
                }
            }
            // for each group compute the leader
            tr(groups, grp) {
                if(grp->second.size() <= 1)
                    continue;
                types::set_vlist_t::const_iterator grp_it = grp->second.begin();
                types::pat_vertex_t leader = *grp_it;
                grp_it++;
                while(grp_it != grp->second.end()) {
                    // put this vertex in the list for leader and delete this
                    // vertex from the ret
                    ret[leader].insert(*grp_it);
                    ret.erase(*grp_it);
                    ++grp_it;
                }
                ret[leader].insert(leader);
            }
            // now find the groups
        }
        return ret;
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
        INFO(*logger, "verifying " << pat.to_string());

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
                bool res;
                MEASURE("Enumerate", res = enumerate(st, pat, paths[it->first], 0, alpha-cost, numlabels, epath, covered, tabu));
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
