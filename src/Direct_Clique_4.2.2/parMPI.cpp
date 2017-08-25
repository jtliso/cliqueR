#include "parMPI.h"

void par_init(int *argc, char ***argv)
{
  int numprocs;

  MPI_Init(argc, argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  if (numprocs < 2)
  {
    cerr << "At least two processes required for parallel clique." << endl;
    exit(EXIT_FAILURE);
  }
}

void par_exit() {MPI_Finalize();}
