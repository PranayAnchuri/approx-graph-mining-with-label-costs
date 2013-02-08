#include "khop.hpp"

bool KhopLabel::is_less(DerivedLabel* rhs) {
    KhopLabel* rhs_khop = (KhopLabel*)rhs;
}

types::cost_t KhopLabel::distance(KhopLabel& dbhop, const int& num_labels,\
                                    const vector<types::cost_t>& costvalues) {
    //  Construct the flow network and compute the max flow min cost algo
    // Create a node for each label present in the pattern multiset and
    // also database multiset
    int cnt = 1; // 0 is used for the source node
    int src = 0;
    int dest = hops.size() + dbhop.hops.size()+1;
    // maximum flow that can go out of the source vertex
    int src_out = 0;
    int des_out = 0;
    MinCostMaxFlow network(dest+1);
    // mapping of pattern and database labels to nodes in the network
    map<types::pat_vertex_t,int> pnodes;
    map<types::db_vertex_t,int> dnodes;
    // Edges from src to pattern labels
    tr(hops, it) {
        network.AddEdge(0,cnt,it->second,0);
        src_out += it->second;
        pnodes[it->first] = cnt;
        cnt++;
    }
    // Edges from the database labels to the dest 
    tr(dbhop.hops ,it) {
        network.AddEdge(cnt,dest,it->second,0);
        dnodes[it->first] = cnt;
        des_out += it->second;
        cnt++;
    }
    cout<<" Count" << cnt<<endl;
    // Edges from the pattenr vertices and the database vertices
    tr(hops, it) {
        types::cost_t minsim=1.0; // minimum similarity to vertex on other side
        tr(dbhop.hops ,it2) {
            int p = pnodes[it->first];
            int d = dnodes[it2->first];
            types::cost_t matchcost  = costvalues[it->first*num_labels+it2->first];
            network.AddEdge(p,d,it->second, matchcost);
        }
    }
    pair<types::cost_t, types::cost_t> res;
    res = network.GetMaxFlow(src,dest);
    // if it network satisfy the flow requirements
    if(res.first != src_out)
        return -1;
    return res.second;
} 

std::string KhopLabel::to_string() {
    std::stringstream ss;
    tr(hops, it) {
        ss << "(" << it->first << ", " << it->second << ") ";
    }
    return ss.str();
}
