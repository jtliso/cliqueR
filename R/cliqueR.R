# cliqueR library functions

library(Rcpp)

#verifies that the given filename is formatted correctly
#returns 0 if successful, -1 on error
formatter <- function(filename){
	sourceCpp("../src/format_checker/format_checker.cpp")
	result <- format_checker(filename)

	return(result)
}

# Finds maximal cliques in the file specified by filename
maximal <- function(filename){
	#checking the format of the file
	res <- formatter(filename)
	
	if(res == -1){
		return(res)
	}
	
	sourceCpp("../src/Direct_Clique_4.2.2/clique.cpp")
	vertices <- maximal_clique(filename)
	return(vertices)
}

maximal("../graphs/bio-yeast.el")
