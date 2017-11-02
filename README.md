# cliqueR
R Library used to calculate Maximal Cliques, Maximum Cliques, and Paracliques for graphs

Examples
--------
Load the package.

    library(cliqueR)
   
There are three different function calls, each with their own set of parameters:
*  Maximal
*  Maximum
*  Paraclique

### Maximal

To find the maximal clique, you need the file name at a bare minimum.  The other parameters are the following:
*  least: Lower bound of clique size to include. (a value less than 3 will simply  return every edge in the graph).
*  most:  Upper bound of clique size to include.
*  profile:  Option to return a clique profile instead of all cliques.
By default, these values are 3, -1, and 0 respectively.

Example calls:

    cliqueR.maximal("inst/extdata/bio-yeast.el")
    cliqueR.maximal("inst/extdata/bio-yeast.el", 5)
    
### Maximum

To find the maximum clique, you need the file name at a bare minimum.  It also has an optional second parameter called `sizeOnly` which defaults to `FALSE`.  This option allows the user to return the size of the maximum clique.

Example calls:
    
    cliqueR.maximum("inst/extdata/bio-yeast.el")
    cliqueR.maximum("inst/extdata/bio-yeast.el", TRUE)
    
### Paraclique

Paraclique returns a list of paracliques ordered by size.  At a minimum, the file name is required as the first parameter.  It also has the following additional parameters:
*  igf:  Glom Factor, initialized to 1.0.
*  min_mc_size:  The minimum clique size to include.  Initialized to 5.
*  min_pc_size:  The minimum paraclique size to include.  Initialzed to 5.
*  max_num_pcs:  Max number of paracliques to include.  Initialized to -1.  
