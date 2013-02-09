#include "store.hpp"


Store::Store() {
    //logger = Logger::get_logger("MAIN");
    myran.set_seed(42);
    stat = new Stats();
}

/*types::cost_t Store::get_alpha() { return alpha;}
int Store::get_minsup() { return minsup;}
int Store::get_num_walks() { return walks;}
types::label_t Store::get_label(int id) { return vmap[id];}
int Store::get_num_labels() { return num_labels;}
bool Store::is_valid_edge(const int& id1, const int& id2) { return present(gr[id1], id2);}*/

void Store::add_vertex(const int& id, const types::label_t& label) {
    vmap[id] = label;
    lmap[label].push_back(id);
}

void Store::add_edge(const int& id1, const int& id2) {
    gr[id1].insert(id2);
    gr[id2].insert(id1);
}

void Store::get_random_l1(types::label_t& lab, types::vlist_t& vlist) {
    typeof(l1pats.begin()) it = l1pats.begin();
    int r = myran.det_number(l1pats.size());
    advance(it, r);
    lab = it->first;
    vlist = it->second;
}

void Store::copy_numeric_args(map<string, types::double_t>& numeric_args) {
    tr(numeric_args, it) {
        //INFO(*dump_logger, "Num Arg " << it->first << " Val is "<< it->second);
        if(!it->first.compare("MINSUP"))
            minsup = it->second;
        else if(!it->first.compare("ALPHA"))
            alpha = it->second;
        else if(!it->first.compare("WALKS"))
            walks = it->second;
        else if(!it->first.compare("SEED"))
            myran.set_seed(it->second);
    }
}

void Store::read_db(string file) {
    // read the file "filename_"
    ifstream infile;
    infile.open(file.c_str());
    string line;
    vector<string> words;
    string token;
    int id1, id2;
    getline(infile, line); // read the header of the file
    while(getline(infile, line))
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
            //cout<<id<<"and "<<label<<endl;
        }
        else if(words.size()==4 && ( words[0]=="e" || words[0]=="u"))
        {
            // if the line read corresponds to an edge
            id1 = atoi(words[1].c_str());
            id2 = atoi(words[2].c_str());
            add_edge(id1, id2);
            add_edge(id2, id1);
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
    getline(infile, line); // read the header of the file
    split(line, words, delim);
    vector<string> header = words;
    // Number of classes in the header of the file
    int rsize = words.size();
    this->num_labels = rsize;
    INFO(*logger, num_labels);
    simvals.reserve(rsize*rsize);
    while(getline(infile, line)) {
        // words from the line
        split(line, words, delim);
        // Starting from 1 add the values to sim values 
        string key = words[0];
        REP(i, 1, words.size()) {
            //simvalues_.push_back(1-convertToDouble(words[i]));
            double cost = convertToDouble(words[i]);
            simvals.push_back(cost);
        }
        words.clear();
    } // While loop for the entire file
}

void Store::read_hops(string file) {
    // read the khops of the database graph
    Hops::read_hop(file, db_hops);
}

void Store::init_stats() {
}

void Store::end_stats() {
    INFO(*stat_logger, "---\t Statistics\n" << stat->to_string());
    INFO(*stat_logger, "---\t Max Pats");
    tr(maxpats, it) {
        INFO(*stat_logger, it->to_string());
    }
}

void Store::get_frequent_vertices(types::cost_t alpha, int minsup) {
    // for each vertex compute the cost of matching with other vertex
    INFO(*logger, "Level 1 Vertices with alpha and minsup" << alpha << " and " << minsup);
    REP(i, 0, num_labels) {
        // Iterate over all the vertices in the graph
        types::vlist_t reps; // valid representatives of the label i
        map<types::db_vertex_t, Repr> rep_ob; // objects of rep type
        tr(vmap, it) {
            // cost of the corresponding labels
            types::cost_t cost = simvals[num_labels*i+it->second];
            if(cost <= alpha) {
                reps.push_back(it->first);
                rep_ob.insert( make_pair(it->first, Repr(it->first, cost)));
            }
        }
        if( reps.size() >= minsup) {
            // label i is frequent
            l1pats[i] = reps;
            freq_labels.push_back(i);
            freq_reps[i] = rep_ob;
            INFO(*logger, i << " reps size is " << reps.size());
        }
    }
}

/*void Store::get_labels(vector<types::label_t>& labels) {
    labels = freq_labels;
}*/

