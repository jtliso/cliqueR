// John Eblen
// July 19, 2006
// Function for preprocessing by degree.  These preprocessing tests do not
// fit the mold for a general clique test used by the Preprocessor.  They
// work by finding nodes that ARE in the clique.  Thus, they are implemented
// separately.  At present, there are not enough of these tests to justify
// a general preprocessing algorithm based on them.  There is also no
// general theory developed to implement it.  Perhaps someday...

#ifndef _DEGREE_PREPROCESSOR_H
#define _DEGREE_PREPROCESSOR_H 1

#include "Graph2.h"
using namespace std;

int Degree_Preprocessor(Graph::Vertices &mygraph, Graph::Vertices &clique,
                                                           int *ldnode = 0);

#endif
