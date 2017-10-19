/* Driver file for Clique Enumerator by BK
 *
 * Author: Yun Zhang, yzhang@cs.utk.edu
 * Version: 2.1.a
 * Last modified: 08-18-2008
 *
 * Changes: Add the upper bound of clique size to the BK 
 * 
 * Copyright 2005-2006
 * Department of Computer Science, University of Tennessee, Knoxville
 *
 * 
 * R wrapper of maximal clique C code (clique-driver.c)
 *
 * Author: Sean Whalen, wck942@vols.utk.edu
 * Created: October 2016
 *
 * Copyright 2017-2018
 * Department of Computer Science, University of Tennessee, Knoxville
 *
 * Last modified: October 2017
 *
 */
 
#include <R.h>
#include <Rinternals.h>

#include <string.h>

#include "bit.h"
#include "graph_maximal.h"
#include "utility.h"
#include "bk.h"

#define CHARPT(x,i)  ((char*)CHAR(STRING_ELT(x,i)))

/* Global variables */
extern int LB,UB;  // lower bound and upper bound of clique size
extern int PROFILE;
extern int NUM_PROTECTED;
extern int NUM_CLIQUES;
extern int CLIQUES_SZ;
extern vid_t **CLIQUES;
char infn[100];

SEXP run_maximal_clique(Graph *G)
{
  double utime;
  unsigned int n = num_vertices(G);
  vid_t clique[n];
  vid_t vertices[n];
  SEXP R_cliques = R_NilValue;
  u64 nclique[n+1];
  int i;

  utime = get_cur_time();
  memset(nclique, 0, (n+1)*sizeof(u64));
  memset(clique, -1, n*sizeof(vid_t));
  for (i = 0; i < n; i++) vertices[i] = i;
  
  CLIQUES_SZ = n;
  if (!PROFILE) {
    CLIQUES = malloc(CLIQUES_SZ * sizeof(vid_t*));
	  if (CLIQUES == NULL) {
		  error("malloc: Memory exhausted, quitting");
		  return R_NilValue;
	  }
  }
  
  if (clique_find_v2(nclique, G, clique, vertices, 0, 0, n)) {
	  free_cliques();
	  return R_NilValue;
  }
  
  utime = get_cur_time() - utime;

  if (PROFILE) {
    R_cliques = clique_profile_out(nclique, G);
    //Rprintf("Time (seconds)  : %.6f\n", utime);
  }
  else {
    R_cliques = PROTECT(allocVector(VECSXP, NUM_CLIQUES));
	  extract_cliques(R_cliques, G);
  }	
  
  return R_cliques;
}

SEXP R_maximal_clique(SEXP R_file, SEXP R_lowerbound, SEXP R_upperbound, SEXP R_profile)
{
  Graph *G;
  FILE *fp;

  UB = asInteger(R_upperbound); 
  LB = asInteger(R_lowerbound);
  PROFILE = asInteger(R_profile);
  NUM_CLIQUES = 0;
  
  const char *filepath = CHARPT(R_file, 0);
  if (strlen(filepath) > 99) {
	  REprintf("Filepath: %s is too long. Max length is 99 characters\n", infn);
	  return R_NilValue;
  }
  strcpy(infn, filepath);
  
  if ((fp = fopen(infn, "r")) == NULL) {
    REprintf("Cannot open file %s\n", infn);
    return R_NilValue;
  }
  G = graph_edgelist_in(fp);
  fclose(fp);
  
  if (UB <= 0) UB = num_vertices(G);
  
  SEXP ret = run_maximal_clique(G);
  graph_free(G);
  
  UNPROTECT(1);
  return ret;
}




