#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
int double_me3(int x) {
	  // takes a numeric input and doubles it
	  return 2 * x;
}
