/*
 * Class that deals with the similarity between labels
 */

#pragma once

#include "Globals.hpp"

class Sim
{
    public:
        Sim(string f):filename_(f) {}
        void set_input(string f);
        map<pair<string, string>, double> read_mat(char delim=' ');
        Sim() {}
        // Return the similarity between the labels l1 and l2;
        double lab_sim(string l1, string l2);
        int num_labels();
        double sum_sim();
        double get_sim(string l1, string l2);
        void print_labsim();
        int sim_size();
        void get_sim(vd& values);
        void get_indices(map<string, int> indices);
    private:
            string filename_;
            // Key is the label and the value is the row of the label in sim mat
            map<string, int> labindex_;
            map<pair<string, string>, double> costs;
            // Label similarity values
            vd simvalues_;
            // Number of labels in the entire graph
            int labels_;
};
