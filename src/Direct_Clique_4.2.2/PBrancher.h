// John Eblen
// November 6, 2006
// Parallel brancher - a derived class of Brancher to add parallel
// capabilities

#ifndef _PBRANCHER_H
#define _PBRANCHER_H 1
#include "Brancher.h"
#include "mpi.h"

class PBrancher : public Brancher
{
  // June 5, 2007 - the granularity of parallelization.  How deeply
  // should the master process traverse the search tree before calling
  // servant processes?  Default is 0 - do not traverse the tree at all.
  int granularity;

  // Each thread keeps track of its largest clique found so far.
  mutable Graph::Vertices *my_largest_clique;

  // Buffer for sending/receiving data and some size information
  // Packet structure:  [Command/Reply Type][clique size][subgraph1][subgraph2]
  mutable int set_size;  // Size of a subgraph instance inside a packet
  mutable int packet_size;
  mutable unsigned int *packet;

  // Record-keeping variables only used by the master
  mutable int num_procs;
  mutable int num_busy_procs;
  mutable vector<bool> busy_procs;

  // Handle used by servant to detect when master sends a message 
  mutable MPI_Request from_master;

  // Annoying - why is this not part of standard C++?
  #define max(a,b) (((a)>(b))?(a):(b));

  // Master commands
  enum Master_Command
  {
    MS_DONE,    // We are done - exit.
    MS_NEWJOB,  // Here is a new job to work on
    MS_INFO,    // Here is some updated information

    MS_WAIT,    // Send current status and wait for further instructions
    MS_INST     // Here are the new instructions (follows above command)
  };

  // Servant replies
  enum Servant_Reply
  {
    SM_READY,    // Ready to receive a new job
    SM_INFO,     // Here is some updated information

    SM_WAITING  // Here is current status - waiting for instructions
                // (reply to MS_WAIT command)
  };

  int par_init(Graph &g, Graph::Vertices &maximum_clique) const;
  int par_find_mcl(Graph::Vertices &mygraph, Graph::Vertices &maximum_clique,
                   Graph::Vertices *current_clique, int depth) const;
  int par_checkin(Graph::Vertices &mygraph, Graph::Vertices &maximum_clique,
                  Graph::Vertices *current_clique, int depth) const;

  public:
  PBrancher() {init(0);}
  PBrancher(int gran) {init(gran);}

  private:
  void init(int gran)
  {
    granularity = gran;
    par_imp = 1;
    my_largest_clique = 0;
    set_size = 0;
    packet_size = 0;
    packet = 0;
    num_procs = 0;
    num_busy_procs = 0;
  }

  public: 
  ~PBrancher() {delete [] packet; delete my_largest_clique;}
};

#endif
