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
extern int PRINT;
char *outfn, infn[100];

void run_maximal_clique(Graph *G)
{
  FILE *fp1=stdout, *fp2;
  char fname[100];
  double utime;
  unsigned int n = num_vertices(G);
  vid_t clique[n];
  vid_t vertices[n];
  u64 nclique[n+1];
  int i;

  if (outfn != NULL) {
    sprintf(fname, "%s.clique", outfn);
    fp1 = fopen(fname, "w");
  }
  sprintf(fname, "%s.profile", infn);
  fp2 = fopen(fname, "w");

  utime = get_cur_time();
  memset(nclique, 0, (n+1)*sizeof(u64));
  memset(clique, -1, n*sizeof(vid_t));
  for (i = 0; i < n; i++) vertices[i] = i;
  clique_find_v2(fp1, nclique, G, clique, vertices, 0, 0, n);
  utime = get_cur_time() - utime;

  clique_profile_out(fp2, nclique, G);
  fprintf(fp2, "Time (seconds)  : %.6f\n", utime);

  if (outfn != NULL) free(outfn);
  fclose(fp1);
  fclose(fp2);

}

SEXP R_maximal_clique(SEXP R_file, SEXP R_lowerbound, SEXP R_upperbound)
{
  Graph *G;
  FILE *fp;

  /* set globals */
  UB = asInteger(R_upperbound); 
  LB = asInteger(R_lowerbound);
  PRINT = 1;
  
  const char *filepath = CHARPT(R_file, 0);
  if (strlen(filepath) > 99) {
	  REprintf("Filepath: %s is too long. Max length: 99\n", infn);
	  return R_NilValue;
  }
  strcpy(infn, filepath);
  
  if ((fp = fopen(infn, "r")) == NULL) {
    REprintf("Can't open file %s\n", infn);
    return R_NilValue;
  }
  G = graph_edgelist_in(fp);
  fclose(fp);
  
  if (UB <= 0) UB = num_vertices(G);
  
  run_maximal_clique(G);
  graph_free(G);

  return 0;
}




