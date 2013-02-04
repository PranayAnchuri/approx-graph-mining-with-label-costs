#include "store.hpp"

void Store::add_vertex(int id, types::label_t label) {
    vmap[id] = label;
    lmap[label].push_back(id);
}

void Store::add_edge(int id1, int id2) {
    gr[id1].insert(id2);
    gr[id2].insert(id1);
}

void Store::read_db(string file) {
    // read the file "filename_"
    ifstream infile;
    infile.open(file.c_str());
    string line;
    vector<string> words;
    string token;
    int id1,id2;
    getline(infile,line); // read the header of the file
    while(getline(infile,line))
    {
        istringstream iss(line);
        while ( getline(iss, token, ' ') )
        {
            words.push_back(token);
        }
        if(words[0]=="#") {
            words.clear();
            continue;
        }
        if(words.size()==3 && words[0]=="v")
        {
            // if the line corresponds to a vertex
            int id = atoi(words[1].c_str());
            int label = atoi(words[2].c_str());
            add_vertex(id, label);
            cout<<id<<"and "<<label<<endl;
        }
        else if(words.size()==4 && ( words[0]=="e" || words[0]=="u"))
        {
            // if the line read corresponds to an edge
            id1 = atoi(words[1].c_str());
            id2 = atoi(words[2].c_str());
            add_edge(id1, id2);
        }
        words.clear();
    }
}

void Store::read_cost(string file) {
    char delim = ' ';
    ifstream infile;
    infile.open((char*)file.c_str());
    string line;
    vector<string> words;
    getline(infile,line); // read the header of the file
    split(line,words,'\t');
    vector<string> header = words;
    // Number of classes in the header of the file
    int rsize = words.size();
    num_labels = rsize;
    simvals.reserve(rsize*rsize);
    while(getline(infile,line)) {
        // words from the line
        split(line,words,delim);
        // Starting from 1 add the values to sim values 
        string key = words[0];
        REP(i,1,words.size()) {
            //simvalues_.push_back(1-convertToDouble(words[i]));
            double cost = convertToDouble(words[i]);
            cout<<cost<<endl;
            simvals.push_back(cost);
        }
        words.clear();
    } // While loop for the entire file
}

void Store::get_frequent_vertices(types::cost_t alpha, int minsup) {
    // for each vertex compute the cost of matching with other vertex
    REP(i,0,num_labels) {
        // Iterate over all the vertices in the graph
        types::vlist_t reps; // valid representatives of the label i
        tr(vmap, it) {
            // cost of the corresponding labels
            types::cost_t cost = simvals[num_labels*i+it->second];
            if(cost <= alpha) {
                reps.push_back(it->first);
            }
        }
        if( reps.size() >= minsup) {
            // label i is frequent
            l1pats[i] = reps;
        }
    }
}
