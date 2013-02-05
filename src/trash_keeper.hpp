/*
 * trash_keeper.h
 * Pattern in a vertex based pattern growth
 *  Created on: Feb 16, 2012
 *      Author: pranay
 *
 *      ************** stores the failed extensions, labels etc 
 */
#pragma once

#include "Globals.hpp"

class Janitor
{
    public:
        /****************** adding trash ***********************************/
        void add_back_fail(int src, int des)
        {
            back_fail.insert(ii(src, des)); // no back edge is possible for these set of vertices
        }
        void add_expired_vertex(int id)
        {
            expired_vertices.insert(id);
        }
        void add_failed_label(int id, types::label_t lab)
        {
            failed_labels[id].insert(lab);
        }
        bool is_back_fail(int src, int des)
        {
            return present(back_fail, ii(src, des)) || present(back_fail, ii(des, src));
        }
        bool is_expired(int id)
        {
            return present(expired_vertices, id);
        }
        bool is_failed_label(int id, types::label_t lab)
        {
            return present(failed_labels, id) && present(failed_labels[id], lab);
        }
    private:
        set<ii> back_fail; // pairs of dfs ids that cannot be connected by a back edge
        set<int> expired_vertices; // set of vertices 
        map<int, set<types::label_t> > failed_labels; // labels for which fwd extension is not possible fro0m a given id
};
