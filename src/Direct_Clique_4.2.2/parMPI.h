// John Eblen
// October 3, 2006
// Functions for handling the implementation of parallel operations.
// The underlying implementation uses MPI for now.

#include <iostream>
#include "mpi.h"
using namespace std;

// Initialize parallelism
void par_init(int *argc, char ***argv);

// Parallel replacement for "exit" system call, to properly shut down the
// entire system
void par_exit();
