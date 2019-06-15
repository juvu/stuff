brary(shiny)
library(RgeoProfile)
library(ggmap)
rm(list=ls(all=TRUE))
d <- Cholera
s <- WaterPumps
p <- geoParams(data = d, sigma_mean = 1.0, sigma_squared_shape = 2)
m <- geoMCMC(data = d, params = p, lambda=0.05)


# Define UI for app that draws a histogram ----
ui <- fluidPage(

  # App title ----
  titlePanel("RgeoPlot!"),

  # Sidebar layout with input and output definitions ----
  sidebarLayout(

    # Sidebar panel for inputs ----
    sidebarPanel(

      # Input: Slider for the number of bins ----
      sliderInput(inputId = "zoom",
                  label = "size:",
                  min = 200,
                  max = 1000,
                  value = 600)

    ),

    # Main panel for displaying outputs ----
    mainPanel(

      # Output: Histogram ----
      imageOutput(outputId = "distPlot")

    )
  )
)

# Define server logic required to draw a histogram ----
server <- function(input, output) {

  # Histogram of the Old Faithful Geyser Data ----
  # with requested number of bins
  # This expression that generates a histogram is wrapped in a call
  # to renderPlot to indicate that:
  #
  # 1. It is "reactive" and therefore should be automatically
  #    re-executed when inputs (input$bins) change
  # 2. Its output type is a plot

  x    <- faithful$waiting
 
  scale <- reactive ({input$zoom})

  output$distPlot <- renderPlot({

	geoPlotMap(params = p, data = d, source = s, surface = m$geoProfile,
                breakPercent = seq(0, 50, 5), mapType = "roadmap",
                crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 2)},height = scale, width = scale
    )

}

# Create Shiny app ----
shinyApp(ui = ui, server = server)
