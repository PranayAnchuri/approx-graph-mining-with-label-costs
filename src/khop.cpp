#include "khop.hpp"
#include "store.hpp"

bool KhopLabel::is_less(DerivedLabel* rhs) {
    KhopLabel* rhs_khop = (KhopLabel*)rhs;
}

types::cost_t KhopLabel::distance(KhopLabel& dbhop, const int& num_labels,\
                                    const vector<types::cost_t>& costvalues , const Store& st) {
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
    if(src_out > des_out)
        return -1;
    // Edges from the pattenr vertices and the database vertices
    /*
     * For each label in the pattern side, multiply the least of 
     * matching this label with a label on the db side with the
     * actual multiplicity of the pattern label
     * The sum of such costs for each label is a lower bound on the total
     * cost of matching
     */
    types::cost_t tot_mincost=0;
    types::cost_t alpha = st.get_alpha();
    tr(hops, it) {
        types::cost_t lab_min_cost=1;
        tr(dbhop.hops ,it2) {
            int p = pnodes[it->first];
            int d = dnodes[it2->first];
            types::cost_t matchcost  = costvalues[it->first*num_labels+it2->first];
            if(matchcost < lab_min_cost)
                lab_min_cost = matchcost;
            network.AddEdge(p,d,it->second, matchcost);
        }
        tot_mincost += lab_min_cost* it->second;
        if(tot_mincost > alpha)
            return -1;
    }
    pair<types::cost_t, types::cost_t> res;
     MEASURE("Flow Computation", res = network.GetMaxFlow(src,dest));
     CMEASURE("# FLow Computations", 1);
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
