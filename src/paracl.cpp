#include "paracl.h"

void build_paracl(Graph &g, Graph::Vertices &v, double glom)
{
  int i;
  vector<int> nodes;
  Graph::Vertices all_nodes(g);
  all_nodes.fill();
  all_nodes.node_array(nodes);
  Graph::Vertices new_members(g);

  //cerr << "glom is " << glom << endl;

//  do           //comment in this loop to do layered paraclique
//  {
    new_members.clear();
    for (i=0; i<g.size(); i++)
    {
      if (!v.in_set(nodes[i]))  //if vertex is not already in paraclique, check whether it belongs
      {
        if (glom >= 1)                   //glom is number of edges that can be missing
        {
          if ((v.size() - v.degree(nodes[i])) <= glom) new_members.insert(nodes[i]);
        }
        else                             //glom is percentage of edges that must be present
        {
          double percent = (double)v.degree(nodes[i]) / (double)v.size();
          if (percent >= glom)
          {    
            new_members.insert(nodes[i]);
            //cerr << "vertex is connected to " << percent << "% of vertices in clique, adding to paraclique\n";
          }        
          //else              
            //cerr << " vertex not added to paraclique\n";
        }
      }
    }
    v.insert(new_members);
    //cerr << "Added " << new_members.size() << " for a paraclique of size " << v.size() << endl;
//  } while (new_members.size() > 0);

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
    //cerr << "Added " << new_members.size() << " for a paraclique of size " << v.size() << endl;
  } while (agg_factor * glom <= v.size());
}
