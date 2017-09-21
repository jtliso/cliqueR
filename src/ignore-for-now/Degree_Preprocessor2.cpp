#include "Degree_Preprocessor2.h"

int Degree_Preprocessor(Graph::Vertices &mygraph, Graph::Vertices &clique,
                                                               int *ldnode)
{
  int num_added = 0;
  int deg, size;
  int lowest_degree = mygraph.size();
  Graph::Vertices::Vex_ptr vp(mygraph);

  if (ldnode != 0) *ldnode = -1;
  for (; !vp.end(); ++vp)
  {
    deg = mygraph.degree(*vp);
    size = mygraph.size();

    // n-1 test:  Vertex connected to all others must be in clique
    if (deg == size-1)
    {
      mygraph.remove(*vp);
      clique.insert(*vp);
      ++num_added;
    }

    // n-2 test:  Vertex connected to all but one must be in clique - delete
    // the single non-neighbor.
    else if (deg == size-2)
    {
      mygraph.remove_nnbrs(*vp);
      mygraph.remove(*vp);
      clique.insert(*vp);
      ++num_added;
    }

    else if (deg < lowest_degree)
    {
      if (ldnode != 0) *ldnode = *vp;
      lowest_degree = deg;
    }
  }

  return num_added;
}
