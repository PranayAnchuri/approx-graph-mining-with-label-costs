#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

namespace types {


  typedef int integer_t;
  typedef integer_t int_t;
  typedef unsigned int unsigned_integer_t;
  typedef unsigned_integer_t uint_t;
  typedef uint_t symbol_t;
  typedef double double_t;
  typedef char char_t;
  typedef char * charp_t;
  typedef float float_t;
  typedef long  long_t;
  typedef unsigned long unsigned_long_t;
  typedef unsigned long ulong_t;
  typedef bool bool_t;
  typedef std::string string_t;

  typedef void * void_ptr_t;


  typedef int label_t;
  typedef int pat_vertex_t;
  typedef int db_vertex_t;
  typedef std::vector<types::db_vertex_t> vlist_t;
  typedef std::set<types::db_vertex_t> set_vlist_t;
  typedef std::map<int, label_t> vmap_t;
  typedef std::map<int, set_vlist_t > graph_t;
  typedef std::map<label_t, vlist_t> lmap_t;
  typedef double cost_t;

  typedef std::vector<pat_vertex_t> pat_vlist_t;
  typedef std::vector<std::pair<pat_vertex_t, pat_vertex_t> > pat_elist_t;
  typedef std::pair<pat_vertex_t, pat_vertex_t> pat_edge_t;
  // Pattern in the form of a graph 
  typedef std::map<pat_vertex_t, pat_vlist_t> pat_graph_t;
  // store just the representative vertices without the Repr object
  typedef std::map<types::pat_vertex_t, types::set_vlist_t> bare_embeds_t;
  // GAPPROX

  // one embedding
  typedef std::pair<types::cost_t, pat_vlist_t> gapprox_em_t;
  // list of all embeddings for a given pattern
  typedef std::vector<gapprox_em_t> gapprox_embed_t;
} // namespace types

