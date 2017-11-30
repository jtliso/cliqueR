
# This is the user-interface definition of a Shiny web application.
# You can find out more about building applications with Shiny here:
#
# http://shiny.rstudio.com
#

library(shiny)

shinyUI(fluidPage(

  fluidRow(
    
    column(3, wellPanel(
      radioButtons("function_type", "Function",
                   choices = c("Maximum" = "maximum",
                               "Maximal" = "maximal",
                               "Paraclique" = "paraclique"),
                   selected = "maximum"
      ),
      selectInput("graph_file", "Graph file",
                  list.files("graphs")
      )
    )),
    
    column(3, wellPanel(
      # This outputs the dynamic UI component
      uiOutput("ui_one"),
      uiOutput("ui_two"),
      uiOutput("ui_three")
    ))),
  
  fluidRow(
    column(12, plotOutput("graph", height=700))
  )
    
  )
  
)
