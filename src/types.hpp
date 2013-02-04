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
  typedef std::vector<int> vlist_t;
  typedef std::set<int> set_vlist_t;
  typedef std::map<int, label_t> vmap_t;
  typedef std::map<int, set_vlist_t > graph_t;
  typedef std::map<label_t, vlist_t> lmap_t;
  typedef double cost_t;

  // GAPPROX
  typedef std::vector<std::vector<int> > gapprox_embed_t;
} // namespace types

