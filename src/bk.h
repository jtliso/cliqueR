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
 * Function: free_cliques()                                      *
 * ------------------------------------------------------------- */
void free_cliques();


/* ------------------------------------------------------------- *
 * Function: clique_profile_out()                                *
 * ------------------------------------------------------------- */
SEXP clique_profile_out(u64 *nclique, Graph *G);


/* ------------------------------------------------------------- *
 * Function: clique_find_v2()                                    *
 *   Bron-Kerbosch version 2                                     *
 *   Recursive function to find cliques                          *
 * ------------------------------------------------------------- */
int clique_find_v2(u64 *, Graph *, vid_t *, vid_t *, int, int, int);


/* ------------------------------------------------------------- *
 * Function: extract_cliques()                                   *
 * ------------------------------------------------------------- */
void extract_cliques(SEXP R_cliques, Graph *G);
#endif
