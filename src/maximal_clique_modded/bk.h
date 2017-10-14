/* Enumerate clique in graph using Bron-Kerbosch
 * Author: Yun Zhang
 * Date: October 2006
 */

#ifndef __CLIQUE_H
#define __CLIQUE_H
#include <R.h>
#include <Rinternals.h>
//#include "utility.h"


/* ------------------------------------------------------------- *
 * Function: clique_out()                                        *
 * ------------------------------------------------------------- */
void clique_out(FILE *fp, Graph *G, vid_t *clique, int len);

/* ------------------------------------------------------------- *
 * Function: clique_profile_out()                                *
 * ------------------------------------------------------------- */
void clique_profile_out(FILE *fp, u64 *nclique, Graph *G);

/* ------------------------------------------------------------- *
 * Function: clique_find_v1()                                    *
 *   Bron-Kerbosch version 1 : numerical order                   *
 *   Recursive function to find cliques                          *
 * ------------------------------------------------------------- */
void clique_find_v1(FILE *, u64 *, Graph *, vid_t *, vid_t *, int, int, int);

/* ------------------------------------------------------------- *
 * Function: clique_find_v2()                                    *
 *   Bron-Kerbosch version 2                                     *
 *   Recursive function to find cliques                          *
 * ------------------------------------------------------------- */
void clique_find_v2(FILE *, u64 *, Graph *, vid_t *, vid_t *, int, int, int);

/* ---------------------------------------- *
 * Clique Enumeration Functions             *
 * ---------------------------------------- */
void clique_enumerate(FILE *, u64 *, Graph *, vid_t *, int);

/* ------------------------------------------------------------- *
 * Function: maxclique_find()                                    *
 *   Modify Bron-Kerbosch to find one of maximum cliques         *
 * ------------------------------------------------------------- */
void maxclique_find(vid_t *, int *, Graph *, vid_t *, vid_t *, int, int, int);

#endif

