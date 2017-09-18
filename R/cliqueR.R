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

