# cliqueR library functions

library(Rcpp)

#verifies that the given filename is formatted correctly
#returns 0 if successful, -1 on error
formatter <- function(filename){
	sourceCpp("../src/format_checker/format_checker.cpp")
	result <- format_checker(filename)

	return(result)
}

