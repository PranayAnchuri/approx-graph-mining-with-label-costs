#include "pattern.hpp"


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
    // gives a set based adjacency list
    types::graph_t adj;
    tr(pat_edges,it) {
       adj[it->first].insert(it->second);
       adj[it->second].insert(it->first);
    }
    return adj;
}

types::pat_graph_t pattern::get_adj_list() const {
    types::pat_graph_t adj;
    tr(pat_edges,it) {
       adj[it->first].push_back(it->second);
       adj[it->second].push_back(it->first);
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

types::pat_vlist_t pattern::get_vertices() const {
    // return the vertices present in the pattern
    vector<types::pat_vertex_t> vertices;
    tr(pat_vmap, it) {
        vertices.push_back(it->first);
    }
    return vertices;
}

map<types::pat_vertex_t, map<int,KhopLabel> > pattern::get_hops() const {
    types::pat_vlist_t vertices = get_vertices();
    map<types::pat_vertex_t, map<int,KhopLabel> > hopsets;
    Hops::get_hops(pat_edges, vertices, pat_vmap,  hopsets);
    return hopsets;
}

void pattern::add_fwd(const types::pat_vertex_t& src, const types::label_t& lab) {
    types::pat_vertex_t des = pat_vmap.size();
    pat_vmap[des] = lab;
    pat_edges.push_back(make_pair(src, des));
}

void pattern::add_back(const types::pat_vertex_t& src, const types::pat_vertex_t& des) {
    pat_edges.push_back(make_pair(src, des));
}

void pattern::undo_fwd() {
    // remove the last vertex that was added to the pattern and also the
    // corresponding edge
    types::pat_vertex_t prev_id = get_fwd_id();
    assert(prev_id>=0);
    pat_vmap.erase(prev_id);
    pat_edges.pop_back();
}

void pattern::undo_back() {
    pat_edges.pop_back();
}

void pattern::set_sup(const int& sup){
    this->pat_sup = sup;
}
