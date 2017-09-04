# cliqueR library functions

library(Rcpp)

formatter <- function(filename){
	sourceCpp("../src/format_checker/format_checker.cpp")
	result <- format_checker(filename)

	return(result)
}
