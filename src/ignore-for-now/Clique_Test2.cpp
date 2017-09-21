#include "Clique_Test2.h"

bool Edge_Test::operator() (const Graph::Vertices &v, int min_size, int node)
                                                                        const
{
  if (node != -1)
  {
    cerr << "Error - edge test called with specific node." << endl;
    exit(EXIT_FAILURE);
  }

  int degree_sum = 0;
  const int degree_sum_needed = min_size * (min_size - 1);
  Graph::Vertices::Vex_ptr vp(v);

  for (; !vp.end(); ++vp)
  {
    degree_sum += v.degree(*vp);
    if (degree_sum >= degree_sum_needed) return 0;
  }

  return 1;
}
