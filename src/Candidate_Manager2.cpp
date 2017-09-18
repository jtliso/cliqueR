#include "Candidate_Manager2.h"

void Candidate_Manager::init(Graph::Vertices *v, int min_clique_size,
                                                     Graph::Vertices *clique)
{
  next_cand = 0;
  v->node_array(cand_list);
}

int Candidate_Manager::next(Graph::Vertices *v, int min_clique_size,
                                                    Graph::Vertices *clique)
{
  for (; next_cand != cand_list.size(); ++next_cand)
         if (v->in_set(cand_list[next_cand])) return cand_list[next_cand++];
  return -1;
}


void Lazy_Low_Degree_Man::init(Graph::Vertices *v, int min_clique_size,
                                                       Graph::Vertices *clique)
{
  Candidate_Manager::init(v);
  sort(cand_list.begin(), cand_list.end(), Graph_vertices_deg_sort_asc(*v));
}

int Prep_High_Degree_Man::next(Graph::Vertices *v, int min_clique_size,
                                                       Graph::Vertices *clique)
{
  int deg, best_degree = -1;
  int size;
  int clique_size = clique->size();
  vector<int>::iterator cand, best_cand = cand_list.end();

  for (cand = cand_list.begin(); cand != cand_list.end();)
  {
    if (*cand == -1) {++cand; continue;}
    if (!v->in_set(*cand)) {*cand = -1; ++cand; continue;}

    deg = v->degree(*cand);
    size = v->size();

    // Preprocess nodes of too low or too high degree
    if (deg < min_clique_size - clique_size - 1)
    {
      v->remove(*cand);
      *cand = -1;
      ++cand;
      continue;
    }

    if (deg == size-1)
    {
      v->remove(*cand);
      clique->insert(*cand);
      ++clique_size;
      *cand = -1;
      ++cand;
      continue;
    }

    if (deg == size-2)
    {
      v->remove_nnbrs(*cand);
      v->remove(*cand);
      clique->insert(*cand);
      ++clique_size;
      *cand = -1;
      ++cand;
      continue;
    }

    if (deg > best_degree)
    {
      best_cand = cand;
      best_degree = deg;
    }

    ++cand;
  }

  if (best_cand == cand_list.end()) return -1;
  int cand_id = *best_cand;
  *best_cand = -1;

  // In rare cases, the n-2 rule may remove the best candidate.  In this
  // case, we just recompute the highest degree vertex without preprocessing
  // using the hdvertex built-in Graph library routine.
  if (!v->in_set(cand_id)) cand_id = v->hdvertex();
  return cand_id;
}

int Prep_Low_Degree_Man::next(Graph::Vertices *v, int min_clique_size,
                                                      Graph::Vertices *clique)
{
  int deg, best_degree = v->size();
  int size;
  int clique_size = clique->size();
  vector<int>::iterator cand, best_cand = cand_list.end();

  for (cand = cand_list.begin(); cand != cand_list.end();)
  {
    if (*cand == -1) {++cand; continue;}
    if (!v->in_set(*cand)) {*cand = -1; ++cand; continue;}

    deg = v->degree(*cand);
    size = v->size();

    // Preprocess nodes of too low or too high degree
    if (deg < min_clique_size - clique_size - 1)
    {
      v->remove(*cand);
      *cand = -1;
      ++cand;
      continue;
    }

    if (deg == size-1)
    {
      v->remove(*cand);
      clique->insert(*cand);
      ++clique_size;
      *cand = -1;
      ++cand;
      continue;
    }

    if (deg == size-2)
    {
      v->remove_nnbrs(*cand);
      v->remove(*cand);
      clique->insert(*cand);
      ++clique_size;
      *cand = -1;
      ++cand;
      continue;
    }

    if (deg < best_degree)
    {
      best_cand = cand;
      best_degree = deg;
    }

    ++cand;
  }

  if (best_cand == cand_list.end()) return -1;
  int cand_id = *best_cand;
  *best_cand = -1;

  // In rare cases, the n-2 rule may remove the best candidate.  In this
  // case, we just recompute the lowest degree vertex without preprocessing
  // using the ldvertex built-in Graph library routine.
  if (!v->in_set(cand_id)) cand_id = v->ldvertex();
  return cand_id;
}

void Lazy_High_Color_Man::init(Graph::Vertices *v, int min_clique_size,
                                                       Graph::Vertices *clique)
{
  Graph *base_graph = v->graph();
  node2color.clear();
  node2color.resize(base_graph->max_size());
  my_cand_list.resize(v->size());
  color_subgraph(*v, &node2color, &my_cand_list);
  next_cand = 0;
}

int Lazy_High_Color_Man::next(Graph::Vertices *v, int min_clique_size,
                                                      Graph::Vertices *clique)
{
  int cand;

  for (; next_cand < my_cand_list.size(); ++next_cand)
  {
    cand = my_cand_list[next_cand];
    if (v->in_set(cand))
    {
      if (clique->size() + node2color[cand] + 1 < min_clique_size) return -1;
      return cand;
    }
  }

  return -1;
}
