\name{cliqueR-package}
\alias{cliqueR-package}
\alias{cliqueR}
\docType{package}
\title{
  Clique Enumeration Suite 
}
\description{
  Fast maximum clique, maximal clique, and paraclique enumeration for edgelist graph files
}
\details{
  Three clique finding functions are provided:
  maximum clique - cliqueR.maximum
  maximal clique - cliqueR.maximal
  paraclique - cliqueR.paraclique
  
  All functions require a tab separated edgelist graph file, with the number of vertices and edges prepended. 
}
\author{
J.T. Liso, Sean Whalen, Shan Lalani, Daniel Lowe, and Axel Hranov

Maintainer: J.T. Liso <your@email.com>
}
\references{
  This package is an R wrapper for clique algorithms originally written in C C++, produced at the request of Michael Langston and Charles Phillips at the University of Tennessee, Knoxville. The maximal clique C source was written by Yun Zhang, yzhang@cs.utk.edu, December 12, 2004. The maximum and paraclique C++ source is authored by John Eblen, August 15, 2007. All code used with permission. 
}
\keyword{ clique }
\seealso{
  Sister package to: \link{https://cran.r-project.org/package=biclique}
}
\examples{
  \dontrun{
     ## Optional simple examples of the most important functions
     ## These can be in \dontrun{} and \donttest{} blocks.   
  }
}
