library(shiny)
library(RgeoProfile)
library(ggmap)
rm(list=ls(all=TRUE))
#d <- Cholera
#s <- WaterPumps
#p <- geoParams(data = d, sigma_mean = 1.0, sigma_squared_shape = 2)
#m <- geoMCMC(data = d, params = p, lambda=0.05)


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
                  value = 600),
fileInput("File1", "Events"),
fileInput("File2", "Sources"),
 actionButton("goButton", "Go!"),
 actionButton("generateButton", "Generate!"),

	fluidRow(
		column (6,
			h4("Events"),
			tableOutput (outputId = "events")
			),
		column (6,
			h4("Sources"),
			tableOutput (outputId = "sources")
			)
	)
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
  # 2. Its output type is a plo


   x    <- faithful$waiting
  scale <- reactive ({input$zoom})

      observeEvent(input$goButton, {

		inFile <- input$File1
		d <- read.table(inFile$datapath)
		inFile <- input$File2
		s <- read.table(inFile$datapath)
		p <- geoParams(data = d, sigma_mean = 1.0, sigma_squared_shape = 2)
		m <- geoMCMC(data = d, params = p, lambda=0.05)

  output$distPlot <- renderPlot({

	geoPlotMap(params = p, data = d, source = s, surface = m$geoProfile,
                breakPercent = seq(0, 50, 5), mapType = "roadmap",
                crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 2)},height = scale, width = scale
    )
      })

      observeEvent(input$generateButton, {

sim <-rDPM(50, priorMean_longitude = -0.04217491, priorMean_latitude = 51.5235505, alpha=1, sigma=1, tau=3)
d <- geoData(sim$longitude, sim$latitude)
s <- geoData(sim$source_lon, sim$source_lat)
p <- geoParams(data = d, sigma_mean = 1.0, sigma_squared_shape = 2)
m <- geoMCMC(data = d, params = p)


  output$events <- renderTable(d)

  output$sources <- renderTable(s)



  output$distPlot <- renderPlot({

	#geoPlotSigma(params = p, mcmc = m)},height = scale, width = scale
	geoPlotMap(params = p, data = d, source = s, surface = m$geoProfile,
                breakPercent = seq(0, 50, 5), mapType = "roadmap",
                crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 2)},height = scale, width = scale
  
    )
      })




  output$events <- renderTable({

inFile <- input$File1
read.table(inFile$datapath)
})

  output$sources <- renderTable({

inFile <- input$File2
read.table(inFile$datapath)
})




}

# Create Shiny app ----
shinyApp(ui = ui, server = server)
