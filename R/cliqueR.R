# cliqueR library functions

library(Rcpp)

#verifies that the given filename is formatted correctly
#returns 0 if successful, -1 on error
formatter <- function(filename){
	result <- format_checker(filename)

	return(result)
}

# Finds maximum cliques in the file specified by filename
cliqueR.maximum <- function(filename, sizeOnly=FALSE) {
	#checking the format of the file
	res <- formatter(filename)
	
	#exiting on error
	if(res == -1){
		return(res)
	}
	
	vertices <- maximum_clique(filename)
	
	#return only the clique size if specified by the user
	if(sizeOnly){
	  return(length(vertices))
	}
	
	return(vertices)
}

#finds paracliques
cliqueR.paraclique <- function(filename, igf=1.0, min_mc_size=5, min_pc_size=5, max_num_pcs=-1){ #return all paracliques, max clique size=5 and paraclique
  #checking the format of the file
  res <- formatter(filename)
  
  #exiting on error
  if(res == -1){
    return(res)
  }
  
  vertices <- find_paracliques(filename, igf, min_mc_size, min_pc_size, max_num_pcs)
  
  return(vertices)
}

cliqueR.maximal <- function(filename, least=3, most=-1) {
  #checking the format of the file
  res <- formatter(filename)
  
  #exiting on error
  if(res == -1){
    return(res)
  }
  
  vertices <- .Call("R_maximal_clique", filename, least, most)

  return(vertices)
}

