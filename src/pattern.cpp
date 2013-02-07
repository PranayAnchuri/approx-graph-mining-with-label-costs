#include "pattern.hpp"

void pattern::add_fwd_vertex(types::label_t lab) {
    pat_vmap[pat_vmap.size()] = lab;
}

void pattern::add_edge(int id1, int id2) {
    pat_edges.push_back(ii(id1, id2));
}

void pattern::get_vertices(types::pat_vlist_t& vertices) {
    REP(i, 0, pat_vmap.size()) {
        vertices.push_back(i);
    }
}

void pattern::get_edges(types::pat_elist_t& edges) {
    edges = pat_edges;
}

string pattern::to_string() {
    stringstream ss;
    tr(pat_vmap, it) {
        ss << " v " << it->first << " " <<it->second << endl;
    }
    tr(pat_edges, it) {
        ss << "e " << it->first << " " << it->second << endl;
    }
    return ss.str();
}

types::graph_t pattern::get_adj() {
    // get the pattern graph in adjacency list format
    types::graph_t adj;
    tr(pat_edges,it) {
       adj[it->first].insert(it->second);
       adj[it->second].insert(it->first);
    }
    return adj;
}

types::pat_elist_t pattern::get_edge_path(const types::pat_vertex_t& id){
    // Traverse all the edges in the pattern atleast once starting from id
    types::graph_t adj = get_adj();
    stack<types::pat_vertex_t> st; // always try for an edge from top of the stack
    st.push(id);
    int nedges = pat_edges.size();
    typedef pair<types::pat_vertex_t, types::pat_vertex_t> pat_edge_t;
    set<pat_edge_t> visited; // edges visited till now
    types::pat_elist_t path;
    while(nedges>0) {
        // see if you can visit a new edge from the top
        types::pat_vertex_t t = st.top();
        bool found = false;
        tr(adj[t],it) {
            if(!present(visited,ii(t,*it)) && !present(visited, ii(*it,t))) {
                // new edge visited
                st.push(*it);
                nedges--;
                found=true;
                visited.insert(pat_edge_t(t,*it));
                path.push_back(pat_edge_t(t,*it));
                break;
            }
        }
        if(!found)
            st.pop();
        // if the stack is empty find a new starting position that has been
        // visited already
        if(st.empty()) {
            tr(pat_edges,it) {
                if(!present(visited,*it)) {
                    st.push(it->first);
                    st.push(it->second);
                    break;
                }
            }
        }
    }
    return path;
}

vector<types::pat_vertex_t> pattern::get_vertices() const {
    // return the vertices present in the pattern
    vector<types::pat_vertex_t> vertices;
    tr(pat_vmap, it) {
        vertices.push_back(it->first);
    }
    return vertices;
}
map<types::pat_vertex_t, map<int,KhopLabel> > pattern::get_hops() const {
    vector<types::pat_vertex_t> vertices = get_vertices();
    map<types::pat_vertex_t, map<int,KhopLabel> > hopsets;
    Hops::get_hops(pat_edges, vertices, pat_vmap,  hopsets);
    return hopsets;
}
