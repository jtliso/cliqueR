#include "Preprocessor2.h"

bool Preprocessor::operator() (Graph &mygraph, int min_clique_size)
{
  bool success;
  Graph::Vertices v(mygraph);
  v.fill();
  success = operator() (v, min_clique_size);
  mygraph.remove_all_except(v);
  return success;
}

bool Preprocessor::operator() (Graph::Vertices &mygraph, int min_clique_size)
{
  int i;
  int prev_size, size;
  if (stop_criterion == NODE) size = mygraph.size();
  else size = mygraph.esize();

  for (i=0; i<alg.size(); ++i)
  {
    while(1)
    {
      if (prep(mygraph, min_clique_size, alg[i].level, *(alg[i].test),
                                         alg[i].cand_man, &eliminated_sets))
      {
        eliminated_sets.delete_all_children();
        mygraph.clear();
        return 1;
      }
      eliminated_sets.delete_all_children();

      // NOTE:  computing edge size is expensive for Graph::Vertices
      prev_size = size;
      if (stop_criterion == NODE) size = mygraph.size();
      else size = mygraph.esize();
      if ((prev_size * stop_ratio) <= size) break;
    }
  }

  return 0;
}

bool prep(Graph::Vertices &mygraph, int min_clique_size, int level,
          const Clique_Test &test_fun, const Candidate_Manager *parent_cand_man,
                                                    basic_tree *eliminated_sets)
{
  static int depth = 0;
  if (level < 0) return 1;
  if (level == 0) return test_fun(mygraph, min_clique_size);
  depth++;

  int cand;
  Graph *base_graph = mygraph.graph();

  Candidate_Manager *cand_man = parent_cand_man->create();
  cand_man->init(&mygraph);
  int counter = 0;
  while ((cand = cand_man->next(&mygraph)) != -1)
  {
    // If the test does not require building a subgraph (such as size
    // testing), we can be more efficient.  We can test first and then we
    // only need to build the neighborhood subgraph if the test fails and
    // level > 1.
    if (!test_fun.need_subgraph())
    {
      if (test_fun(mygraph, min_clique_size, cand))
      {
        mygraph.remove(cand);
        eliminated_sets->add_child(cand);
        continue;
      }

      if (level == 1) continue;
    }

    // Build Subgraph
    Graph::Vertices subgraph(mygraph);
    subgraph.remove_nnbrs(cand);
    subgraph.insert(cand);

    // Hopefully we can simply remove the node here.  First check simply
    // avoids running the test twice for tests where a subgraph is not
    // needed.
    if (test_fun.need_subgraph() && test_fun(subgraph, min_clique_size))
    {
      mygraph.remove(cand);
      eliminated_sets->add_child(cand);
      continue;
    }

    // Give up at lowest level
    if (level == 1) continue;

    // Otherwise, let's try harder by calling prep recursively
    else
    {
      eliminated_sets->add_child(cand);

      // prep called successfully, remove node and prune tree at the node.
      if (prep(subgraph, min_clique_size, level-1, test_fun, cand_man,
                                          eliminated_sets->down(cand)))
      {
        mygraph.remove(cand);
        eliminated_sets->down(cand)->delete_all_children();
      }

      // Complete failure - cannot eliminate this node nor any sets containing
      // this node.  Remove it from the tree.
      else if (eliminated_sets->down(cand)->num_children() == 0)
                                           eliminated_sets->delete_child(cand);

      // Partial success - cannot remove node but found sets containing it.
      // Thus, keep in tree and remove any edges, if possible
      else
      {
        basic_tree::child_ptr cp(eliminated_sets->down(cand));
        for (; !cp.end(); ++cp)
        {
          if (cp->num_children() == 0) base_graph->disconnect(cand, cp->value());
        }
      }
    }
  }

  // Before returning, restore deleted edges
  if (level > 1 && depth > 1)
  {
    basic_tree::child_ptr my_child(eliminated_sets);
    for (; !my_child.end(); ++my_child)
    {
      basic_tree::child_ptr my_grandchild
                            (eliminated_sets->down(my_child->value()));
      for (; !my_grandchild.end(); ++my_grandchild)
      {
        if (my_grandchild->num_children() == 0)
               base_graph->connect(my_child->value(), my_grandchild->value());
      }
    }
  }
  depth--;
  return test_fun(mygraph, min_clique_size);
}
