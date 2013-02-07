#include "read_hop.hpp"

namespace Hops {
    std::string to_string(db_hops_t& dbhops) {
        std::stringstream ss;
        tr(dbhops, it) {
            ss << " Vertex " << it->first <<endl;
            tr(it->second, lhops) {
                ss << "\t Level " << lhops->first << " - " << lhops->second.to_string() << std::endl;
            }
        }
        return ss.str();
    }

    void read_hop(string filename, db_hops_t& dbhops) {

        ifstream infile;
        infile.open((char*)filename.c_str());
        string line;
        string token;
        types::db_vertex_t id = 0;
        while(getline(infile,line)) {
            istringstream iss(line);
            vector<string> words;
            while ( getline(iss, token, ' ') ) {
                words.push_back(token);
            }
            if(words.size()==1) {
                // start of the next vertex in the graph
                id = convertToInt(words[0]);
            }
            else {
                // read the next level multiset for this vertex
                int level = convertToInt(words[0]);
                for(int i=1;i<words.size();i++) {
                    vector<string> sp;
                    split(words[i],sp,':');
                    types::label_t label = convertToInt(sp[0]);
                    int count = convertToInt(sp[1]);
                    dbhops[id][level].insert(label, count);
                }
            }
        }
    }

    void get_adj(const types::pat_elist_t& edges, types::pat_graph_t& pat_gr) {
        tr(edges,it) {
            pat_gr[it->first].push_back(it->second);
            pat_gr[it->second].push_back(it->first);
        }
    }

    void bfs( const types::pat_vertex_t& src, const types::vmap_t& labels, types::pat_graph_t& adj, pat_hops_t& hopsets) {

        queue<types::pat_vertex_t> q;
        q.push(-1);
        q.push(src);
        // add the 0 level multiset which is the label of the source
        hopsets[src][0].insert(labels.find(src)->second, 1);
        int level=0;
        set<int> marked;
        marked.insert(src);
        while(!q.empty()) {
            types::pat_vertex_t e = q.front();
            q.pop();
            if (e==-1) {
                if(q.size()>0)
                    q.push(-1);
                level++;
            }
            else {
                // insert the neighbors of the node
                tr(adj[e],it) {
                    if(!present(marked,*it)) {
                        q.push(*it);
                        hopsets[src][level].insert(labels.find(*it)->second, 1);
                        marked.insert(*it);
                    }
                }
            }
        }
    }

    void get_hops(const types::pat_elist_t& edges, const types::pat_vlist_t& vertices,\
                                const types::vmap_t& labels, pat_hops_t& hopsets) {
        // get the graph in the form of an adjacency list
        types::pat_graph_t adj;
        get_adj(edges,adj);
        for(typeof(vertices.begin()) it=vertices.begin(); it<vertices.end();it++) {
            bfs(*it, labels, adj, hopsets);
        }
    }
}
