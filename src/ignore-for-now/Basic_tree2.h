// John Eblen
// June 13, 2006
// Basic tree class
// A basic tree data structure designed for the recursive general
// preprocessor to store the sets of nodes found that cannot belong in a
// clique.

#ifndef _BASIC_TREE_H
#define _BASIC_TREE_H 1

#include <map>

class basic_tree
{
  int i;
  basic_tree *parent;
  map<int, basic_tree> child;

  public:
  // Constructors
  basic_tree() {i=0; parent = 0;}
  basic_tree(int j) {i=j; parent = 0;}
  private:
  basic_tree(int j, basic_tree *btp) {i=j; parent = btp;}

  public:
  // Tree Building and Pruning
  void add_child(int j) {child[j] = basic_tree(j, this);}
  void delete_child(int j) {child.erase(j);}
  void delete_all_children() {child.clear();}

  // Accessor Methods
  void set(int j) {i=j;}
  int value() const {return i;}
  int num_children() const {return child.size();}

  // Tree Traversal
  basic_tree *up() {if (parent) return parent; return this;}
  basic_tree *down(int i)
  {
    if (child.find(i) == child.end()) return 0;
    return &(child[i]);
  }

  // Child Traversal
  // A thin wrapper around a child map iterator so user does not need to
  // worry about pairs.
  class child_ptr
  {
    map<int, basic_tree> &child_map;
    map<int, basic_tree>::iterator it;

    // Constructors and assignment
    public:
    child_ptr(basic_tree *btp)
    :child_map(btp->child),
     it(btp->child.begin())
    {}

    child_ptr &operator++() {it++; return *this;}
    child_ptr &operator--() {it--; return *this;}
    basic_tree *operator->() {return &(it->second);}
    bool end() const {return it == child_map.end();}
  };
};

#endif
