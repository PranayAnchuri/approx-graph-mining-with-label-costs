#include "multiset.hpp"

void MultiSet::add_element(const string& key,const int& cnt)
{
    // Add an element to the existing multiset
    elements_[key] += cnt;
}

int MultiSet::size() {
    return elements_.size();
}

string MultiSet::get_string() const {
    string st;
    tr(elements_,it) {
        st += " "+it->first+":"+convertInt(elements_.find(it->first)->second);
    }
    return st+"\n";
}

void MultiSet::add(const MultiSet& M)
{
    tr(M.elements_,it)
    {
        add_element(it->first,it->second);
    }
}

  pair<L,L> MultiSet::distance(const MultiSet& M,
          map<pair<string,string>, double>& simvalues, double threshold) const {
    //  Construct the flow network and compute the max flow min cost algo
    // Create a node for each label present in the pattern multiset and
    // also database multiset
    int cnt = 1; // 0 is used for the source node
    int src = 0;
    int dest = elements_.size() + M.elements_.size()+1;
    // maximum flow that can go out of the source vertex
    int src_out = 0;
    int des_out = 0;
    MinCostMaxFlow network(dest+1);
    //DBGVAR(cout,dest);
    // mapping of pattern and database labels to nodes in the network
    map<string,int> pnodes;
    map<string,int> dnodes;
    // Edges from src to pattern labels
    tr(elements_,it) {
        network.AddEdge(0,cnt,it->second,0);
        src_out += it->second;
        pnodes[it->first] = cnt;
        cnt++;
    }
    // Edges from the database labels to the dest 
    tr(M.elements_,it) {
        network.AddEdge(cnt,dest,it->second,0);
        dnodes[it->first] = cnt;
        des_out += it->second;
        cnt++;
    }
    // Edges from the pattenr vertices and the database vertices
    tr(elements_,it) {
        L minsim=1.0; // minimum similarity to vertex on other side
        tr(M.elements_,it2) {
            int p = pnodes[it->first];
            int d = dnodes[it2->first];
            // TODO : Use the label similarity from the input file
           //int labsim = it->first.compare(it2->first)==0 ? 0 : 1;
            //double labsim=0;
            double labsim = simvalues[make_pair(it->first,it2->first)];
            //********************** IMPORTANT****************
            // The input matrix we have is the similarity matrix not
            // the distance matrix replace 1-labsim with labsim if 
            // the input is a distance matrix
            network.AddEdge(p,d,it->second,1-labsim);
        }
    }
    pair<L,L> res;
    res = network.GetMaxFlow(src,dest);
    /*if(true || res.first != src_out) {
        res.first = 1;
    }*/
    //res.first = 1;
    //res.second = 0;
    return res;
} 

void MultiSet::print() {
    tr(elements_,it) {
        cout<<"("<<it->first<<","<<it->second<<")"<<endl;
    }
}
int MultiSet::get(string lab) {
    if(present(elements_,lab))
        return elements_[lab];
    else
        return -1;
}

bool MultiSet::match(const MultiSet& M, set<pair<string,string> >& matchpairs) {
    
        // 1) Get the total number of vertices needed on the pattern side
        map<string,int> offsets; // key is the label and the value is multiplicity
        int num = 0;
        int dnum = M.elements_.size(); // number of vertices on the database side
        tr(elements_, it) {
            offsets[it->first] = num;
            num+= it->second; 
        }
        if(num > dnum)
            return false;
        // Create the adj matrix
        VVI adj(num,VI(dnum));
        tr(elements_, pele) {
            int col=0;
            tr(M.elements_, dele) {
                // check if the elements can be matched
                if(present(matchpairs,make_pair(pele->first, dele->first))) {
                    int off = offsets[pele->first];
                    for(int i=0; i<pele->second;i++) {
                        // add the edges
                        adj[off+i][col] = 1;
                    }
                }
                col++;
            }
        }
        VI row_match(num);
        VI col_match(dnum);
        int msize = MyMatch::BipartiteMatching(adj, row_match, col_match);
        return msize==num;
}

