#include "sim.hpp"

double Sim::lab_sim(string l1, string l2)
{
    return !l1.compare(l2);
}
void Sim::set_input(string f)
{
    filename_ = f;
}
map<pair<string, string>, double > Sim::read_mat(char delim)
{
    ifstream infile;
    infile.open((char*)filename_.c_str());
    string line;
    vector<string> words;
    getline(infile, line); // read the header of the file
    split(line, words, delim);
    vector<string> header = words;
    // Number of classes in the header of the file
    int rsize = words.size();
    simvalues_.reserve(rsize*rsize);
    labels_ = rsize; 
    while(getline(infile, line)) {
        // words from the line
        split(line, words, delim);
        // Starting from 1 add the values to sim values 
        string key = words[0];
        REP(i, 1, words.size()) {
            //simvalues_.push_back(1-convertToDouble(words[i]));
            double cost = convertToDouble(words[i]);
            string val = header[i-1];
            simvalues_.push_back(cost);
            costs[make_pair(key, val)] = cost;
        }
        words.clear();
    } // While loop for the entire file
    return costs;
}

int Sim::num_labels()
{
    return labindex_.size();
}
double Sim::sum_sim()
{
    double s = 0;
    tr(simvalues_, it)
        s+=*it;
    return s;
}
double Sim::get_sim(string l1, string l2)
{
    int id1 = labindex_[l1];
    int id2 = labindex_[l2];
    double x = simvalues_[id1*labels_+id2];
    return x;
}
void Sim::print_labsim() {
    //print_list(simvalues_);
    //print_map(labindex_);
}

int Sim::sim_size() {
    return simvalues_.size();
}


void Sim::get_sim(vd& values) {
    values = simvalues_;
}
void Sim::get_indices(map<string, int> indices) {
    indices = labindex_;
}
