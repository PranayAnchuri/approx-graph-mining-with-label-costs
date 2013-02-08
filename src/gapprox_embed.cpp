#include "gapprox_embed.hpp"

namespace GApprox{
    GApproxEmbedding::GApproxEmbedding() {
        supfunc = &GApproxEmbedding::min_node_sup;
    }
    void GApproxEmbedding::init_embeddings(Store& st, const types::label_t& lab, \
            const types::vlist_t& vids) {
        int num_labels = st.get_num_labels();
        tr(vids, it) {
            types::db_vertex_t rep_label = st.get_label(*it);
            types::cost_t cost = st.simvals[lab*num_labels + rep_label];
            types::vlist_t vlist;
            vlist.push_back(*it);
            embeds.push_back(make_pair(cost, vlist));
        }
    }
    std::string GApproxEmbedding::to_string() {
        std::stringstream ss;
        for(int i=0; i< embeds.size(); i++) {
            ss << endl;
            ss << "Cost " << embeds[i].first<< " - ";
            for(int vertex=0; vertex<embeds[i].second.size(); vertex++) {
                ss << embeds[i].second[vertex] << " ";
            }
        }
        return ss.str();
    }

    Embedding* GApproxEmbedding::extend_fwd(Store& st, pattern& pat, types::pat_vertex_t src, \
                                            types::label_t lab) {
        GApproxEmbedding* next_embeds = new GApproxEmbedding();
        int num_labels = st.get_num_labels();
        types::cost_t max_cost = st.get_alpha();

        Logger* logger = st.get_logger();

        // iterate over all the existing embeddings
        tr(embeds, it) {
            // budget used up already
            types::cost_t curr_cost = it->first;
            // iterate over all the representative vertices for the new label
            tr(st.l1pats[lab], rep) {
                // the vertex is not used up already in the embedding
                if( cpresent(it->second, *rep))
                    continue;
                // there is an edge between the vertices in the original db
                if(!st.is_valid_edge(it->second[src], *rep)) {
                    continue;
                }
                // label of the new vertex that will be added to the embedding
                types::label_t src_label = st.get_label(*rep);
                // cost between label in the pattern and label of the new vertex
                INFO(*logger, "getting label cost");
                types::cost_t cost = st.simvals[num_labels*src_label + lab];
                INFO(*logger, " cost " << cost );
                // copy the existing embedding
                types::pat_vlist_t embed_vertices = it->second;
                // push the new vertex
                embed_vertices.push_back(*rep);
                // check if the total cost is still in limits
                if(curr_cost + cost <= max_cost) {
                    next_embeds->add_embedding(make_pair(curr_cost+cost, embed_vertices));
                }
            }
        }
        return (Embedding*)next_embeds;
    }

    Embedding* GApproxEmbedding::extend_back(Store& st, pattern& pat, types::pat_vertex_t src, \
                                            types::pat_vertex_t des) {
        // for each embedding check if there exists an edge between src and back
        // mappings of the pattern vertex
        GApproxEmbedding* next_embeds = new GApproxEmbedding();
        tr(embeds, it) {
            types::pat_vertex_t src_v = it->second[src];
            types::pat_vertex_t des_v = it->second[des];
            // check if there is an edge between src_v and des_v
            if(st.is_valid_edge(src_v, des_v)) {
                // add the embedding to the next set of embeddings
                next_embeds->add_embedding(*it);
            }
        }
        return (Embedding*)next_embeds;
    }
    int GApproxEmbedding::min_node_sup(map<types::pat_vertex_t, types::set_vlist_t>& unique_reps) {
        vector<int> sizes;
        tr(unique_reps, it) {
            sizes.push_back(it->second.size());
        }
        if(sizes.empty())
            return 0;
        int sup =  *min_element(all(sizes));
        return sup;
    }

    int GApproxEmbedding::compute_support() {
        map<types::pat_vertex_t, types::set_vlist_t> unique_reps;
        tr(embeds, it) {
            for(int index =0 ; index<it->second.size(); index++) {
                unique_reps[index].insert(it->second[index]);
            }
        }
        return (this->*supfunc)(unique_reps);
    }
}
