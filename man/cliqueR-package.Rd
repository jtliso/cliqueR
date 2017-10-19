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
  \itemize{
    \item{\code{\link{cliqueR.maximum}}}
    \item{\code{\link{cliqueR.maximal}}}
    \item{\code{\link{cliqueR.paraclique}}}
    }
    All functions require a tab separated edgelist graph file, with the number of vertices and edges prepended. \code{} 
}
\author{
J.T. Liso, Sean Whalen, Shan Lalani, Daniel Lowe, and Axel Hranov

Maintainer: J.T. Liso jtliso417@gmail.com
}
\references{
  The clique algorithms are written in C, C++ and wrapped in R at the request of Michael Langston and Charles Phillips at the University of Tennessee, Knoxville. The maximal clique C source was written by Yun Zhang, December, 2004. The maximum and paraclique C++ source is authored by John Eblen, August, 2007. All code used with permission. 
}
\keyword{ clique }
\seealso{
  Sister package to: \code{\link[biclique:biclique-package]{biclique}}
}