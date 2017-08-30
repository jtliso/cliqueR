library(Rcpp)
sourceCpp("../src/format_checker/format_checker.cpp")
format_checker("../graphs/bio-yeast.el")
