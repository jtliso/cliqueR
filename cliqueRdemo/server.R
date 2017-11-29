
# This is the server logic for a Shiny web application.
# You can find out more about building applications with Shiny here:
#
# http://shiny.rstudio.com
#
library(shiny)
library(cliqueR)
library(igraph)

function(input, output) {
  
  genGraph <- reactive({
    data = read.table(paste0("graphs/", input$graph_file))
    g = graph_from_data_frame(data[-c(1),], directed = FALSE)
    l = layout_with_lgl(g)
    return(list("graph" = g, "layout" = l))
  })
  
  getCliques <- reactive({
    if (input$function_type == "maximal") {
      cl = cliqueR.maximal(paste0("graphs/", input$graph_file), input$sizeRange[[1]], input$sizeRange[[2]])
      return(cl)
    }
    
    if (input$function_type == "paraclique") {
      cl = cliqueR.paraclique(paste0("graphs/", input$graph_file), input$glom, input$minMC, input$minPC)
      return(cl)
    }
    return()
  })
  
  
  output$ui_one <- renderUI({
    if (is.null(input$function_type))
      return()
    
    # Depending on input$input_type, we'll generate a different
    # UI component and send it to the client.
    switch(input$function_type,
           "maximal" = sliderInput("sizeRange", "Clique size range",
                                  min = 3, max = 20, value = c(3,20)),
           
          "paraclique" = sliderInput("glom", "Glom factor",
                                  min = 0.0, max = 1.0, value = c(0.5)),
          
           "maximum" = checkboxInput("sizeOnly", "Return size only?",
                                      value = FALSE)
    )
  })
  
  output$ui_two <- renderUI({
    if (is.null(input$function_type))
      return()
    
    switch(input$function_type,
           "maximal" = checkboxInput("profile", "Return size profile only?", value=FALSE),
           "paraclique" = sliderInput("minMC", "Minimum clique size", min=3, max=20, value=5),
           "maximum" = return()
    )
  })
  
  output$ui_three <- renderUI({
    if (is.null(input$function_type))
      return()
    
    if (input$function_type == "paraclique") {
      sliderInput("minPC", "Minimum paraclique size", min=3, max=20, value=5)
    }
  })
  
  output$ui_cl <- renderUI({
    sliderInput("clique_index", "clique", min=1, max=as.integer(length(getCliques())), value=1, step=1)
  })
  
  output$graph <- renderPlot({
    vars = genGraph()
    
    if (input$function_type == "maximum") {
      mc = cliqueR.maximum(paste0("graphs/", input$graph_file))
      V(vars$graph)[unlist(mc)]$color = "red"
      clique = V(vars$graph)[unlist(mc)]
      E(vars$graph)$color = "grey"
      E(vars$graph)[clique %--% clique]$color = "red"
      plot(vars$graph, frame=TRUE, vertix.size=5, vertex.label=NA, layout=vars$layout, rescale=FALSE, xlim=range(vars$layout[,1]), ylim=range(vars$layout[,2]))
    }
    else {
      cliques = getCliques()
      if (length(cliques) == 0) {
        return(
          plot(vars$graph, frame=TRUE, vertix.size=5, vertex.label=NA, layout=vars$layout, rescale=FALSE, xlim=range(vars$layout[,1]), ylim=range(vars$layout[,2]))
        )
      }
      i=input$clique_index
      
      mc = cliques[[i]]
      V(vars$graph)[unlist(mc)]$color = "red"
      clique = V(vars$graph)[unlist(mc)]
      E(vars$graph)$color = "grey"
      E(vars$graph)[clique %--% clique]$color = "red"
      plot(vars$graph, frame=TRUE, vertix.size=5, vertex.label=NA, layout=vars$layout, rescale=FALSE, xlim=range(vars$layout[,1]), ylim=range(vars$layout[,2]))
    }
    
  })
  
  
  
}