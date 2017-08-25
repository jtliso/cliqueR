#include "paracl.h"

void build_paracl(Graph &g, Graph::Vertices &v, int glom)
{
  int i;
  vector<int> nodes;
  Graph::Vertices all_nodes(g);
  all_nodes.fill();
  all_nodes.node_array(nodes);
  Graph::Vertices new_members(g);

  do
  {
    new_members.clear();
    for (i=0; i<g.size(); i++)
    {
      if (!v.in_set(nodes[i]))
      {
        if ((v.size() - v.degree(nodes[i])) <= glom) new_members.insert(nodes[i]);
      }
    }
    v.insert(new_members);
    cerr << "Added " << new_members.size() << " for a paraclique of size " << v.size() << endl;
  } while (new_members.size() > 0);
}

void build_acc_paracl(Graph &g, Graph::Vertices &v, int agg_factor)
{
  int i;
  int glom = 1;
  vector<int> nodes;
  Graph::Vertices all_nodes(g);
  all_nodes.fill();
  all_nodes.node_array(nodes);
  Graph::Vertices new_members(g);

  do
  {
    new_members.clear();
    for (i=0; i<g.size(); i++)
    {
      if (!v.in_set(nodes[i]))
      {
        if ((v.size() - v.degree(nodes[i])) <= glom) new_members.insert(nodes[i]);
      }
    }
    v.insert(new_members);
    ++glom;
    cerr << "Added " << new_members.size() << " for a paraclique of size " << v.size() << endl;
  } while (agg_factor * glom <= v.size());
}
