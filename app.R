library(shiny)
library(RgeoProfile)
library(ggmap)
register_google(key="<your key here>")
rm(list=ls(all=TRUE))


# Define UI for app that drives RgeoProfile ----
ui <- fluidPage(

  tags$head(
    tags$style(HTML("hr {border-top: 1px solid #000000;}"))
  ),

  # App title ----
  #titlePanel("RgeoProfile!"),

  # Sidebar layout with input and output definitions ----
  sidebarLayout(

    # Sidebar panel for inputs ----
    sidebarPanel(

	# Input: Slider for the zoom ----
	sliderInput(inputId = "zoom",
                  label = "Plot size:",
                  min = 200,
                  max = 1000,
                  value = 600),

	hr(),

	h4("Load Data"),
	fluidRow(
		column (6,
			fileInput("File1", "Events")
			),
		column (6,
			fileInput("File2", "Sources")
			)

	),

	hr(),

	h4("Generate Data"),

	fluidRow(
		column (4,
			numericInput("Latitude", "Latitude", "51.5235505")
			),
		column (4,
			numericInput("Longitude", "Longitude", "-0.04217491")
			),
		column (4,
			numericInput("numPoints", "numPoints", "50")
			),
		column (4,
			numericInput("Alpha", "Alpha", "1")
			),
		column (4,
			numericInput("Sigma", "Sigma", "1")
			),
		column (4,
			numericInput("Tau", "Tau", "3")
			),
		column (4,
			actionButton("generateButton", "Generate!")
			)
	),

	hr(),
	h4("Plot Data"),

	fluidRow(
		column (4,
  			radioButtons("map", "Map type:",
               			c("Terrain" = "terrain",
                 		"Satellite" = "satellite",
                 		"Roadmap" = "roadmap",
                 		"Hybrid" = "hybrid"))
			),
		column (4,
			numericInput("SigmaMean", "SigmaMean", "1.0")
			),
		column (4,
			numericInput("SigmaSquaredShape", "SigmaSquaredShape", "2")
			),
		column (4,
			numericInput("Lambda", "Lambda", "1.0")
			),
		column (4,
			numericInput("numPoints", "numPoints", "50")
			),
		column (4,
			actionButton("goButton", "Go!")
			)
	),


	hr(),
h4("Coordinate Data"),

	fluidRow(
		column (4,			
			downloadButton("downloadData", "Download Events")
			),
		column (4,
			downloadButton("downloadSource", "Download Sources")
			),
		column (6,
			textInput("downloadFile", "Download Filename", "data")
			)
	),

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
      # Output: RgeoProfile map ----
      imageOutput(outputId = "distPlot")

    )
  )
)

# Define server logic required to draw the RgeoPlot ----
server <- function(input, output, session) {

  x    <- faithful$waiting
  scale <- reactive ({input$zoom})

      observeEvent(input$goButton, {
		d <- attr (session, "d")
		s <- attr (session,"s")
		p <- geoParams(data = d, sigma_mean = 1.0, sigma_squared_shape = 2)
		m <- geoMCMC(data = d, params = p, lambda=0.05)
		type <- input$map

  		output$distPlot <- renderPlot({

			geoPlotMap(params = p, data = d, source = s, surface = m$geoProfile,
                		breakPercent = seq(0, 50, 5), mapType = type,
                		crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 2)},height = scale, width = scale
    )
      })

      observeEvent(input$generateButton, {
		lattude <- input$Latitude
		longtude <- input$Longitude
		numPoints <- input$numPoints
		Alpha <- input$Alpha
		Sigma <- input$Sigma
		Tau <- input$Tau
		sim <-rDPM(numPoints, priorMean_longitude = longtude, priorMean_latitude = lattude, alpha=Alpha, sigma=Sigma, tau=Tau)
		attr (session,"d") <- geoData(sim$longitude, sim$latitude)
		attr (session,"s") <- geoData(sim$source_lon, sim$source_lat)
		d <- attr (session, "d")
		s <- attr (session,"s")

  		output$events <- renderTable(d, digits = 7)
  		output$sources <- renderTable(s, digits = 7)
      })


       output$downloadData <- downloadHandler(

    		filename = function() {
     			 paste(input$downloadFile, "_events", ".txt", sep = "")
   		 },
   		 content = function(file) {
			d <- attr (session, "d")
			write.table (d, file, sep=" ")
    		})

       output$downloadSource <- downloadHandler(

    		filename = function() {
     			 paste(input$downloadFile, "_sources", ".txt", sep = "")
   		 },
   		 content = function(file) {
			s <- attr (session, "s")
			write.table (s, file, sep=" ")
    		})



      observeEvent(input$File1, {
  		output$events <- renderTable({
			inFile <- input$File1
			attr (session,"d") <- read.table(inFile$datapath)
		},digits = 7)
	})

      observeEvent(input$File2, {
  		output$sources <- renderTable({
			inFile <- input$File2
			attr (session,"s") <- read.table(inFile$datapath)
		},digits = 7)
	})
}

# Create Shiny app ----
shinyApp(ui = ui, server = server)
