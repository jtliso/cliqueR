# cliqueR
R Library used to calculate Maximal Cliques, Maximum Cliques, and Paracliques for graphs

# Dependencies
Libarary depends on Rcpp.
```
install.packages('Rcpp')
```

# Workflow:

1. open cliqueR.Rproj in Rstudio
2. should auto detect DESCRIPTION and NAMEPACE files, which tell R Studio this is a package - look at build tab
3. (first time) on the R console in R studio load the package (intall(cliqueR), require(cliqueR) - may not have to do this every time)
   or from your OS terminal go to the directory above /cliquR and do R CMD INSTALL cliqueR
4. make whatever changes
5. rebuild from the build tab
6. you should see whatever Rcpp exported functions are in /src and R functions in /R loaded in the environment panel as well as cliqueR in the packages tab
7. test said functions and rebuild, then test again


for addition guidance see "interface with R" in this Rcpp quick guide:
http://dirk.eddelbuettel.com/code/rcpp/Rcpp-quickref.pdf

and this stackover flow post: 
(only follow steps about making package, not cutting and pasting and sourcing one massive file)
https://stackoverflow.com/questions/13995266/using-3rd-party-header-files-with-rcpp
