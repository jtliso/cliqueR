/* Enumerate clique in graph using Bron-Kerbosch
 * Author: Yun Zhang
 * Date: October 2006
 */

#ifndef __BK_H
#define __BK_H

//#include "utility.h"
#include <R.h>
#include <Rinternals.h>

/* ------------------------------------------------------------- *
 * Function: clique_profile_out()                                *
 * ------------------------------------------------------------- */
SEXP clique_profile_out(u64 *nclique, Graph *G);


/* ------------------------------------------------------------- *
 * Function: clique_find_v2()                                    *
 *   Bron-Kerbosch version 2                                     *
 *   Recursive function to find cliques                          *
 * ------------------------------------------------------------- */
void clique_find_v2(u64 *, Graph *, vid_t *, vid_t *, SEXP, int, int, int);

#endif
