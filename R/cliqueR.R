library(Rcpp)

# Internal Graph format checker
# 
# verifies that a graph file is correctly formatted for use with cliqueR. 
# 
# The edgelist must be tab seperated, with the two vertices comprising an edge per line.
# the first line of the file must be the number of vertices, tab, 
# and the number of edges. If an edge is repeated, there is an edge 
# between a vertex and itself, or inconsistencies with the provided
# and derived number of vertices or edges, an error will be thrown. 
# 
# param - filename Character path to an edgelist (.el) file
# return - If the graph is formatted correctly 0, otherwise -1 and an error
#   message is printed.
formatter <- function(filename) {
  t = read.table(filename, colClasses = "character")
  if (length(t) != 2) {
    warning(paste0("file ", filename, "should have 2 columns"))
    return(invisible(-1))
  }
  if (nrow(t) < 2) {
    warning(paste0("file", filename, " should have at least 2 rows"))
    return(invisible(-1))
  }
  
  # separate heading from edges
  heading = t[1,]
  t = t[2:nrow(t),]
  
  if (nrow(t) != as.integer(heading[2])) {
    warning(paste0("file", filename, " heading doesn't match number of edges"))
    return(invisible(-1))
  }
  
  if (anyDuplicated(t)) {
    warning(paste0("file", filename, " has duplicated edges"))
    return(invisible(-1))
  }
  
  for (i in 1:nrow(t)) {
    edge = t[i,]
    if (toString(edge[1]) == toString(edge[2])) {
      print (edge)
      print(edge[1])
      print(edge[2])
      warning(paste0("file", filename, " has a edge between the same vertex: ", toString(edge[1]), " ", toString(edge[2])))
      return(invisible(-1))
    }
  }
  
  numVerts = length(unique(c(t[,1],t[,2])))
  
  if (numVerts != as.integer(heading[1])) {
    warning(paste0("file", filename, " heading doesn't match number of vertices: ", heading[1], " ", numVerts))
    return(invisible(-1))
  }
  return(invisible(1))
}

#' Maximum clique finder
#' 
#' \code{cliqueR.maximum} returns the maximum clique of a graph file.
#' 
#' A clique is a set of vertices such that every vertex is connected to every other
#' vertex in the set. The maximum clique of a graph is the clique that has the largest
#' number of vertices. 
#' 
#' @param filename Character path to an edglist graph file.
#' @param sizeOnly Logical option to return the size of the maximum clique.
#'   instead of the maximum clique itself. 
#' @return A list of character vertex labels in the maximum clique. If sizeOnly is specified, the size of the maximum
#'   clique is returned.
#' @examples 
#' cliqueR.maximum(system.file("extdata","bio-yeast.el",package = "cliqueR"))
#' cliqueR.maximum(system.file("extdata","bio-yeast.el",package="cliqueR"), TRUE)
#' @export 
cliqueR.maximum <- function(filename, sizeOnly=FALSE) {
	res <- formatter(filename)
	
	if(res == -1){
	  return(invisible(res))
	}
	
	vertices <- maximum_clique(filename)
	
	if(sizeOnly){
	  return(length(vertices))
	}
	
	return(vertices)
}


#' Paraclique enumeration 
#' 
#' \code{cliqueR.paraclique} returns a list of paracliques, ordered by size.
#' 
#' A clique is a set of vertices such that every vertex is connected to every other
#' vertex in the set. A paraclique is a clique that includes vertices which are not 
#' connected to every other vertex in the clique, but are connected to some. The proportion of edges
#' a vertex has to the size of the core clique is called the glom factor. The glom factor
#' determines membership of a given vertex to a paraclique. Note that a glom factor approaching
#' 0 will produce numerous large cliques, and significantly increase the risk of the function
#' never returning for large graphs. 
#' 
#' @param filename Character path to a edgelist graph file.
#' @param igf Numeric glom factor, 1.0 being the strictest, 0.0 the loosest.
#' @param min_mc_size Integer the minimum clique size to include.
#' @param min_pc_size Integer the minimum paraclique size to include.
#' @param max_num_pcs Integer max number of paracliques to return, default is number of vertices. 
#' @return A List of paracliques. Each paraclique itself is a list of Character vertex labels. 
#'   Ordered by size.
#' @examples 
#' cliqueR.paraclique(system.file("extdata","bio-yeast.el",package="cliqueR"))
#' \donttest{cliqueR.paraclique(system.file("extdata","bio-yeast.el",package="cliqueR"), 0.7)
#' cliqueR.paraclique(system.file("extdata","bio-yeast.el",package="cliqueR"), 0.7, 6)
#' cliqueR.paraclique(system.file("extdata","bio-yeast.el",package="cliqueR"), 0.7, 6, 7)
#' cliqueR.paraclique(system.file("extdata","bio-yeast.el",package="cliqueR"), 0.7, 6, 7, 20)}
#' @export

cliqueR.paraclique <- function(filename, igf=1.0, min_mc_size=5, min_pc_size=5, max_num_pcs=-1){ 
  res <- formatter(filename)
  
  if(res == -1){
    return(invisible(res))
  }
  vertices <- find_paracliques(filename, igf, min_mc_size, min_pc_size, max_num_pcs)
  return(vertices[order(sapply(vertices,length),decreasing=F)])
}


#' Maximal clique enumeration
#' 
#' \code{cliqueR.maximal} returns a list of cliques, ordered by size.
#' 
#' A clique is a set of vertices such that every vertex is connected to every other
#' vertex in the set. Maximal clique returns a list of all the cliques in a graph, from 
#' 3 to the number of vertices in the graph, unless the user specifies otherwise. 
#' 
#' @param filename Character path to an edgelist graph file.
#' @param least Integer. Lower bound of clique size to include. (a value less than 3 will simply
#'   return every edge in the graph).
#' @param most Integer. Upper bound of clique size to include.
#' @param profile Integer. Option to return a clique profile instead of all cliques.
#' @return A list of cliques ordered by size. Each clique is a list of Character vertex labels.
#'   If the profile option is specified a list of cliques sizes and number of occurrences
#'    will be returned instead.
#' @examples 
#' cliqueR.maximal(system.file("extdata","bio-yeast.el",package="cliqueR"))
#' cliqueR.maximal(system.file("extdata","bio-yeast.el",package="cliqueR"), profile=1)
#' \donttest{cliqueR.maximal(system.file("extdata","bio-yeast.el",package="cliqueR"), 5, 10)}
#' @export   
cliqueR.maximal <- function(filename, least=3, most=-1, profile=0) {
  res <- formatter(filename)
  
  if(res == -1){
    return(invisible(res))
  }
  a = .Call("R_maximal_clique", filename, least, most, profile)
  if (is.null(a)) {
    print("NULL")
    return(list())

  }
  if (profile) {
    blank = sapply(a, is.null)
    if (length(blank))
      return(a[-which(blank)])
    else
      return(a)
  } 
  return(a[order(sapply(a,length),decreasing=F)])
}



