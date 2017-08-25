#include "PBrancher.h"

int PBrancher::par_init(Graph &g, Graph::Vertices &max_clique) const
{
  int i;
  int rank;
  Master_Command command;
  MPI_Status status;
  Graph::Vertices tmp_graph(g);
  Graph::Vertices tmp_clique(g);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Initialize member variables
  delete my_largest_clique;
  my_largest_clique = new Graph::Vertices(g);
  my_largest_clique->clear();
  my_largest_clique->insert(max_clique);

  set_size = tmp_graph.pack_size();
  packet_size = 2 + 3*set_size;
  delete [] packet;
  packet = new unsigned int[packet_size];

  // Rank 0 is the master process and is the only process that uses
  // the following data structures.
  if (rank == 0)
  {
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    num_busy_procs = num_procs;
    busy_procs.resize(num_procs);
    for (i=0; i<num_procs; ++i) busy_procs[i] = 1;
  }

  // Master starts the ball rolling by calling find_mcl on the entire graph.
  if (rank == 0)
  {
    tmp_graph.fill();
    find_mcl(tmp_graph, *my_largest_clique);

    // Wait for all servants to finish
    while(num_busy_procs > 1)
    {
      MPI_Recv(packet, packet_size, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG,
                                                       MPI_COMM_WORLD, &status);
      switch(packet[0])
      {
        case SM_WAITING:
          cerr << "Error - unexpected waiting reply from process " << status.MPI_SOURCE << " - ignoring." << endl;
          break;
        case SM_READY:
          if (packet[1] > my_largest_clique->size())
          {
            my_largest_clique->unpack(&packet[2]);
            // Code needed to update other servants when clique size updated.
          }
          --num_busy_procs;
          busy_procs[status.MPI_SOURCE] = 0;
          break;
        case SM_INFO:
          // Code Needed
          break;
      }
    }

    // Once all are finished and waiting, send termination signal along
    // with the final maximum clique size and instance.
    packet[0] = MS_DONE;
    packet[1] = my_largest_clique->size();
    my_largest_clique->pack(&(packet[2]));
    
    for (i=1; i<num_procs; ++i)
      MPI_Send(packet, 2 + set_size, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD);

    // Don't forget to place the solution in the user's container
    max_clique.clear();
    max_clique.insert(*my_largest_clique);
    return NO_RUN_AFTER_INIT;
  }

  // Remaining processes are servants who receive a subproblem, solve it, and
  // repeat until told by master to halt.
  else
  {
    packet[0] = SM_READY;
    packet[1] = 0;
    MPI_Send(packet, 2, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
    MPI_Irecv(packet, packet_size, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
                                                                  &from_master);
    while(1)
    {
      MPI_Wait(&from_master, &status);

      switch(packet[0])
      {
        case MS_DONE:
          // Don't forget to place the solution in the user-provided container
          max_clique.clear();
          max_clique.unpack(&(packet[2]));
          return NO_RUN_AFTER_INIT;

        case MS_NEWJOB:
          // Unpack and get to work
          tmp_graph.unpack(&(packet[2]));
          tmp_clique.unpack(&(packet[2 + set_size]));
          if (packet[1] > my_largest_clique->size())
                          my_largest_clique->unpack(&packet[2 + 2*set_size]);

          // CANNOT issue this until done unpacking
          MPI_Irecv(packet, packet_size, MPI_UNSIGNED, 0, MPI_ANY_TAG,
                                                MPI_COMM_WORLD, &from_master);
          find_mcl(tmp_graph, *my_largest_clique, &tmp_clique);

          // Finished - tell the master we are ready for more
          packet[0] = SM_READY;
          packet[1] = my_largest_clique->size();
          my_largest_clique->pack(&(packet[2]));
          
          MPI_Send(packet, 2 + set_size, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
          break;

        case MS_INFO:
          MPI_Irecv(packet, packet_size, MPI_UNSIGNED, 0, MPI_ANY_TAG,
                                                MPI_COMM_WORLD, &from_master);
          // Code needed
          break;

        case MS_WAIT:
          MPI_Irecv(packet, packet_size, MPI_UNSIGNED, 0, MPI_ANY_TAG,
                                                MPI_COMM_WORLD, &from_master);
          // Ignore wait requests
          break;

        case MS_INST:
          MPI_Irecv(packet, packet_size, MPI_UNSIGNED, 0, MPI_ANY_TAG,
                                                MPI_COMM_WORLD, &from_master);
          cerr << "Internal error - Master sent process " << rank << " further instructions at odd time - ignoring them." << endl;
          break;
      }
    }
  }
}

int PBrancher::par_find_mcl(Graph::Vertices &mygraph,
                            Graph::Vertices &max_clique,
                            Graph::Vertices *current_clique, int depth) const
{
  int proc;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  //  Master ships job off to next available process
  if (rank == 0 && (depth > granularity))
  {
    // Find a free process
    if (num_busy_procs < num_procs)
    {
      for (proc=1; proc<num_procs; ++proc) if (!busy_procs[proc]) break;
      if (proc==num_procs)
      {
        cerr << "Internal error - unable to find available process in par_find_mcl" << endl;
        exit(EXIT_FAILURE);
      }
    }

    // If all are busy, wait for a process to become available
    else
    {
      while (num_busy_procs == num_procs)
      {
        MPI_Recv(packet, packet_size, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG,
                                                       MPI_COMM_WORLD, &status);
        switch(packet[0])
        {
          case SM_WAITING:
            cerr << "Error - unexpected waiting reply from process " << status.MPI_SOURCE << " - ignoring." << endl;
            break;
          case SM_READY:
            if (packet[1] > my_largest_clique->size())
            {
              my_largest_clique->unpack(&packet[2]);
              // Code needed to update other servants when clique size updated.
            }
            proc = status.MPI_SOURCE;
            --num_busy_procs;
            busy_procs[proc] = 0;
            break;
          case SM_INFO:
            // Code Needed
            break;
        }
      }
    }

    // Store job in packet and send to the process
    packet[0] = MS_NEWJOB;
    packet[1] = my_largest_clique->size();
    mygraph.pack(&(packet[2]));
    current_clique->pack(&(packet[2 + set_size]));
    my_largest_clique->pack(&(packet[2 + 2*set_size]));
    MPI_Send(packet, 2 + 3*set_size, MPI_UNSIGNED, proc, 0, MPI_COMM_WORLD);

    // Update records and return
    ++num_busy_procs;
    busy_procs[proc] = 1;
    return CONT_SEARCH;
  }

  // Servants simply continue working
  else
  {
    find_mcl(mygraph, max_clique, current_clique);
    return CONT_SEARCH;
  }
}

//  Bare bones for now because functionality not needed for this simplistic
//  version.
int PBrancher::par_checkin(Graph::Vertices &mygraph,
                           Graph::Vertices &maximum_clique,
                           Graph::Vertices *current_clique, int depth) const
{
  int rank;
  int flag;
  MPI_Status status;

  // Only servant need bother with checking in
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) return CONT_SEARCH;

  MPI_Test(&from_master, &flag, &status);
  if (flag)
  {
    switch(packet[0])
    {
      case MS_DONE:
        return HALT_SEARCH;  // This will not work - see notes!
        break;

      case MS_NEWJOB:
        cerr << "Internal error - master sent process " << rank << " a new job while still working - ignoring" << endl;
        break;

      case MS_INFO:
        // Code needed
        break;

      case MS_WAIT:
        // Code needed
        break;

      case MS_INST:
        // Code needed
        break;
    }

    // NOTE:  THIS IS TOO SIMPLE FOR NEXT VERSION - WILL NEED TO ISSUE IN
    // THE RIGHT LOCATION.
    MPI_Irecv(packet, packet_size, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
                                                                  &from_master);
  }

  return CONT_SEARCH;
}
