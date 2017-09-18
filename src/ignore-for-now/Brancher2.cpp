#include "Brancher2.h"

int Brancher::operator() (Graph &mygraph, Graph::Vertices &maximum_clique) const
{
  // Depending on the implementation, a thread may or may not want to start
  // branching after par_init.
  if (par_imp)
    if (par_init(mygraph, maximum_clique) == NO_RUN_AFTER_INIT)
                                             return maximum_clique.size();
  Graph::Vertices v(mygraph);
  v.fill();
  find_mcl(v, maximum_clique);
  return maximum_clique.size();
}

int Brancher::operator() (Graph::Vertices &mygraph,
                                          Graph::Vertices &maximum_clique) const
{
  // Depending on the implementation, a thread may or may not want to start
  // branching after par_init.
  if (par_imp)
    if (par_init(*(mygraph.graph()), maximum_clique) == NO_RUN_AFTER_INIT)
                                              return maximum_clique.size();
  find_mcl(mygraph, maximum_clique);
  return maximum_clique.size();
}

void Brancher::find_mcl(Graph::Vertices mygraph,
                        Graph::Vertices &maximum_clique,
                        Graph::Vertices *clique_so_far) const
{
  static int big_count = 0;
  static int depth = 0;
  ++depth;

  int num_nodes_needed;
  int cand;
  Graph *base_graph = mygraph.graph();

  // Create local copies of current clique information
  Graph::Vertices current_clique(*base_graph);
  current_clique.clear();
  if (clique_so_far != 0) current_clique.insert(*clique_so_far);

  // Branch-level Interleaved and Degree Preprocessing
  if (branch_ipp)
  {
    num_nodes_needed = maximum_clique.size() + 1 - current_clique.size();
    if (num_nodes_needed > 1) (*branch_ipp)(mygraph, num_nodes_needed);
  }

  if (branch_dpp)
  {
    Degree_Preprocessor(mygraph, current_clique);
    update_maximum_clique(maximum_clique, current_clique);
  }

  Candidate_Manager *cand_man = branch_cand_man->create();
  cand_man->init(&mygraph, maximum_clique.size() + 1, &current_clique);
  update_maximum_clique(maximum_clique, current_clique);
  while ((cand = cand_man->next(&mygraph, maximum_clique.size() + 1,
                                                     &current_clique)) != -1)
  {
    ++big_count;

    if (search_best_to_worst)
    {
      // Assume cand is in clique
      current_clique.insert(cand);
      update_maximum_clique(maximum_clique, current_clique);
      Graph::Vertices tmpgraph(mygraph);
      tmpgraph.remove_nnbrs(cand);
      tmpgraph.remove(cand);

      if (par_imp)
      {
        if (par_find_mcl(tmpgraph, maximum_clique, &current_clique, depth) ==
                                                          HALT_SEARCH) return;
      }
      else find_mcl(tmpgraph, maximum_clique, &current_clique);

      if (par_imp)
        if (par_checkin(tmpgraph, maximum_clique, &current_clique, depth) ==
                                                         HALT_SEARCH) return;

      // Assume cand is not in clique
      current_clique.remove(cand);
      mygraph.remove(cand);
    }

    else
    {
      // Assume cand is not in clique
      update_maximum_clique(maximum_clique, current_clique);
      mygraph.remove(cand);

      if (par_imp)
      {
        if (par_find_mcl(mygraph, maximum_clique, &current_clique, depth) ==
                                                         HALT_SEARCH) return;
      }
      else find_mcl(mygraph, maximum_clique, &current_clique);

      if (par_imp)
        if (par_checkin(mygraph, maximum_clique, &current_clique, depth) ==
                                                        HALT_SEARCH) return;

      // Assume cand is in clique
      current_clique.insert(cand);
      update_maximum_clique(maximum_clique, current_clique);
      mygraph.remove_nnbrs(cand);
    }

    // Node-level Interleaved and Degree Preprocessing
    if (node_ipp)
    {
      num_nodes_needed = maximum_clique.size() + 1 - current_clique.size();
      if (num_nodes_needed > 1) (*node_ipp)(mygraph, num_nodes_needed);
    }

    if (node_dpp)
    {
      Degree_Preprocessor(mygraph, current_clique);
      update_maximum_clique(maximum_clique, current_clique);
    }

    // Simple search tree pruning
    if (mygraph.size() + current_clique.size() <= maximum_clique.size()) break;
  }

  delete cand_man;
  update_maximum_clique(maximum_clique, current_clique);

  --depth;
  // if (depth == 0) cerr << "Number of iterations:  " << big_count << endl;
}

void Brancher::update_maximum_clique(Graph::Vertices &m, Graph::Vertices &c)
                                                                       const
{
  if (c.size() > m.size())
  {
    m.clear();
    m.insert(c);
  }
}
