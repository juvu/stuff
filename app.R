library(shiny)
library(RgeoProfile)
library(ggmap)
register_google(key="Your key here")
rm(list=ls(all=TRUE))

dyn.load("../getCoords.dll")


# Define UI for app that drives RgeoProfile ----
ui <- fluidPage(
  	tags$head(
    		tags$style(HTML("hr {border-top: 1px solid #000000;}"))
  	),

	# App title ----
	#titlePanel("RgeoProfile!"),

	# Sidebar layout with input and output definitions ---
	sidebarLayout(
		# Sidebar panel for inputs ----
		sidebarPanel(
			tabsetPanel(
				tabPanel("Plot",

					hr(),

					# Input: Slider for the zoom ----
					sliderInput(inputId = "zoom",
                  				label = "Plot size:",
                  				min = 300,
                  				max = 1500,
                  				value = 800),

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
  							radioButtons("plot", "Plot type:",
               						c("Normal" = "normal",
                 					"Perspective raw" = "raw",
                 					"Perspective geoProfile" = "geoprofile",
                 					"Lorentz" = "lorentz",
                 					"Map Ring" = "mapring",
							"Sigma" = "sigma"))
						),
						column (4,
							numericInput("SigmaMean", "SigmaMean", "1.0")
						),
						column (4,
							numericInput("SigmaSquaredShape", "SigmaSquaredShape", "2", min = 2)
						),
						column (4,
							numericInput("Lambda", "Lambda", "0.05")
						),
						column (4,
							numericInput("Iterations", "Iterations", "10000")
						)

					),
					hr(),
					h4("Area restriction"),
					h5("Click and drag on map to setup view area"),

					fluidRow(
   						verbatimTextOutput("info")

					),
					hr(),
					actionButton("goButton", "Run simulation and plot"),
					actionButton("plotButton", "Plot Only"),
					actionButton("filterButton", "Filter data to area"),
					hr()

    				),
        			tabPanel("Coordinate Data", 

					hr(),

					h4("Load Data"),
					fluidRow(
						column (6,
							fileInput("File1", "Events (txt)")
						),
						column (6,
							fileInput("File2", "Sources (txt)")
						),
						column (6,
							fileInput("File3", "Events (kml)")
						),
						column (6,
							fileInput("File4", "Sources (kml)")
						),
						column (6,
							textInput("minLat", "Min Latitude", "50.0")
						),
						column (6,
							textInput("maxLat", "Max Latitude", "53.0")
						),
						column (6,
							textInput("minLong", "Min Longitude", "-2.0")
						),
						column (6,
							textInput("maxLong", "Max Longitude", "2.0")
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
							actionButton("generateButton", "Generate")
						)
					),

					hr(),

					h4("Download Data"),

					fluidRow(
						column (4,			
							downloadButton("downloadData", "Download Events")
						),
						column (4,
							downloadButton("downloadSource", "Download Sources")
						),
						column (4,
							actionButton("rmSource", "Remove Sources")
						),
						column (4,
							textInput("downloadFile", "Download Filename", "data")
						)

					),

					hr(),

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
				)
			)	
      		),
	

		# Main panel for displaying outputs ----
		mainPanel
		(
			imageOutput(outputId = "distPlot",
               click = "plot_click",

    					brush = "plot_brush"
  				)

 	
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
		l <- input$Lambda
		session$resetBrush("plot_brush")


		# min value for Sigma Squared shape is apparently 2
		sss <- input$SigmaSquaredShape
		if (sss < 2)
		{
			sss <- 2
		}


		if (!is.null(d))
		{


			# Create a Progress object
			progress <- shiny::Progress$new(style = "notification")

		
			p <- geoParams(data = d, sigma_mean = input$SigmaMean, sigma_squared_shape = sss, samples = input$Iterations)
			attr (session,"p") <- p



				minLat <- attr (session, "minLat")
				if (!is.null(minLat))
				{
					maxLat <- attr (session, "maxLat")
					minLong <- attr (session, "minLong")
					maxLong <- attr (session, "maxLong")
					zoomLon <- c(as.numeric(minLong), as.numeric(maxLong))
					zoomLat <- c(as.numeric(minLat), as.numeric(maxLat))
					p$output$longitude_minMax <- zoomLon
					p$output$latitude_minMax <- zoomLat
				}
		

    			progress$set(message = "Running geoMCMC- Please wait", value = 1)
			m <- geoMCMC(data = d, params = p, lambda=l)

			attr (session,"m") <- m
			attr (session,"p") <- p


			type <- input$map
    			plottype <- switch(input$plot,
                   		normal = 1,
                   		raw = 2,
                   		geoprofile = 3,
		   		lorentz = 4,
		   		mapring = 5,
  		   		sigma = 6,
                   		1)

    		
			if (plottype == 1)
			{

					output$distPlot <- renderPlot({

						withProgress(message = 'Making plot - plot may take a few seconds to appear after this message disappears', value = 2, {

							geoPlotMap(params = p, data = d, source = s, surface = m$geoProfile,
                						breakPercent = seq(0, 50, 5), mapType = type, 
                						crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 4)
						})
					},height = scale, width = scale)

			}
			else if (plottype == 2)
			{
  				output$distPlot <- renderPlot({

				geoPersp(m$posteriorSurface, surface_type = "prob")},height = scale, width = scale)
			}
			else if (plottype == 3)
			{
  				output$distPlot <- renderPlot({

				geoPersp(surface = m$geoProfile, aggregate_size = 3, surface_type = "gp")},height = scale, width = scale)
			}
			else if (plottype == 4)
			{
  				output$distPlot <- renderPlot({

				hs <- geoReportHitscores(params = p, source = s, surface =m$geoProfile)
				geoPlotLorenz(hit_scores = hs, crimeNumbers = NULL)},height = scale, width = scale)
			}
			else if (plottype == 5)
			{
  				output$distPlot <- renderPlot({

				surface_ring <- geoRing(params = p, data = d, source = s, mcmc = m)
				gp_ring <- geoProfile(surface = surface_ring)
				geoPlotMap(params = p, data = d, source = s, surface = gp_ring, surfaceCols <- c("red", "white"))},height = scale, width = scale)
			}
			else if (plottype == 6)
			{
  				output$distPlot <- renderPlot({

				geoPlotSigma(params = p, mcmc = m)},height = scale, width = scale)
			}


			progress$close()
		}
	})


      observeEvent(input$plotButton, {
		m <- attr (session, "m")
		p <- attr (session, "p")
		d <- attr (session, "d")
		s <- attr (session,"s")
		session$resetBrush("plot_brush")


		if (!is.null(d))
		{
			type <- input$map
    			plottype <- switch(input$plot,
                   		normal = 1,
                   		raw = 2,
                   		geoprofile = 3,
		   		lorentz = 4,
		   		mapring = 5,
  		   		sigma = 6,
                   		1)

    		

			zoomLon <- p$output$longitude_minMax 
			zoomLat <- p$output$latitude_minMax 


				minLat <- attr (session, "minLat")
				if (!is.null(minLat))
				{
					maxLat <- attr (session, "maxLat")
					minLong <- attr (session, "minLong")
					maxLong <- attr (session, "maxLong")
					zoomLon <- c(as.numeric(minLong), as.numeric(maxLong))
					zoomLat <- c(as.numeric(minLat), as.numeric(maxLat))
				}
	

			if (plottype == 1)
			{

					output$distPlot <- renderPlot({

						withProgress(message = 'Making plot - plot may take a few seconds to appear after this message disappears', value = 2, {

							geoPlotMap(lonLimits = zoomLon, latLimits = zoomLat, params = p, data = d, source = s, surface = m$geoProfile,
                						breakPercent = seq(0, 50, 5), mapType = type, 
                						crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 4)
						})
					},height = scale, width = scale)

			}
			else if (plottype == 2)
			{
  				output$distPlot <- renderPlot({

				geoPersp(m$posteriorSurface, surface_type = "prob")},height = scale, width = scale)
			}
			else if (plottype == 3)
			{
  				output$distPlot <- renderPlot({

				geoPersp(surface = m$geoProfile, aggregate_size = 3, surface_type = "gp")},height = scale, width = scale)
			}
			else if (plottype == 4)
			{
  				output$distPlot <- renderPlot({

				hs <- geoReportHitscores(params = p, source = s, surface =m$geoProfile)
				geoPlotLorenz(hit_scores = hs, crimeNumbers = NULL)},height = scale, width = scale)
			}
			else if (plottype == 5)
			{
  				output$distPlot <- renderPlot({

				surface_ring <- geoRing(params = p, data = d, source = s, mcmc = m)
				gp_ring <- geoProfile(surface = surface_ring)
				geoPlotMap(params = p, data = d, source = s, surface = gp_ring, surfaceCols <- c("red", "white"))},height = scale, width = scale)
			}
			else if (plottype == 6)
			{
  				output$distPlot <- renderPlot({

				geoPlotSigma(params = p, mcmc = m)},height = scale, width = scale)
			}
		}
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

		# min value for Sigma Squared shape is apparently 2
		sss <- input$SigmaSquaredShape
		if (sss < 2)
		{
			sss <- 2
		}

			p <- geoParams(data = d, sigma_mean = input$SigmaMean, sigma_squared_shape = sss, samples = input$Iterations)


					output$distPlot <- renderPlot({

						withProgress(message = 'Making plot - plot may take a few seconds to appear after this message disappears', value = 2, {

							geoPlotMap(params = p, data = d, source = s,
                						breakPercent = seq(0, 50, 5), mapType = "roadmap", 
                						crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 4)
						})
					},height = scale, width = scale)

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

      observeEvent(input$filterButton, {
		d <- attr (session, "d")
		minLat <- attr (session, "minLat")
		maxLat <- attr (session, "maxLat")
		minLong <- attr (session, "minLong")
		maxLong <- attr (session, "maxLong")
		d <- d[(d$latitude > minLat),]
		d <- d[(d$latitude < maxLat),]
		d <- d[(d$longitude > minLong),]
		d <- d[(d$longitude < maxLong),]
		#d <- d[latitude < maxLat]
		#d <- d[longitude > minLong]
		#d <- d[longitude < maxLong]
		#d$latitude <- d$latitude[-(minLat:maxLat)]
		#d$longitude <- d$longitude[-(minLong:maxLong)]
		attr (session,"d") <- d
  		output$events <- renderTable({
			attr (session,"d")
		},digits = 7)
	})

      observeEvent(input$rmSource, {
		attr (session,"s") <- NULL
  		output$sources <- renderTable({
			attr (session,"s")
		},digits = 7)
	})

      observeEvent(input$File1, {
		inFile <- input$File1
		attr (session,"d") <- read.table(inFile$datapath)
		s <- attr (session,"s")
		d <- attr (session,"d")
  		output$events <- renderTable({
			read.table(inFile$datapath)
		},digits = 7)

		# min value for Sigma Squared shape is apparently 2
		sss <- input$SigmaSquaredShape
		if (sss < 2)
		{
			sss <- 2
		}

		p <- geoParams(data = d, sigma_mean = input$SigmaMean, sigma_squared_shape = sss, samples = input$Iterations)


					output$distPlot <- renderPlot({

						withProgress(message = 'Making plot - plot may take a few seconds to appear after this message disappears', value = 2, {

							geoPlotMap(params = p, data = d, source = s,
                						breakPercent = seq(0, 50, 5), mapType = "roadmap", 
                						crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 4)
						})
					},height = scale, width = scale)

	})

      observeEvent(input$File2, {
		inFile <- input$File2
		attr (session,"s") <- read.table(inFile$datapath)
		s <- attr (session,"s")
		d <- attr (session,"d")
  		output$sources <- renderTable({
			read.table(inFile$datapath)
		},digits = 7)

		# min value for Sigma Squared shape is apparently 2
		sss <- input$SigmaSquaredShape
		if (sss < 2)
		{
			sss <- 2
		}

		if (!is.null(d))
		{
			p <- geoParams(data = d, sigma_mean = input$SigmaMean, sigma_squared_shape = sss, samples = input$Iterations)


			output$distPlot <- renderPlot({

				withProgress(message = 'Making plot - plot may take a few seconds to appear after this message disappears', value = 2, {

					geoPlotMap(params = p, data = d, source = s,
                						breakPercent = seq(0, 50, 5), mapType = "roadmap", 
                						crimeCol = "black", crimeCex = 2, sourceCol = "red", sourceCex = 4)
				})
	
			},height = scale, width = scale)
		}
	
	})

	getCoords <- function(kmlData, minLat, maxLat, minLong, maxLong) {
		result <- .C("getCoords", kmlData=kmlData, minLat=minLat, maxLat=maxLat, minLong=minLong, maxLong=maxLong)
		return (result$kmlData)
	}

      observeEvent(input$File3, {
		inFile <- input$File3
		newFile <- getCoords (inFile$datapath, input$minLat, input$maxLat, input$minLong, input$maxLong)
		attr (session,"d") <- read.table(newFile)
  		output$events <- renderTable({
			read.table(newFile)
		},digits = 7)
	})

      observeEvent(input$File4, {
		inFile <- input$File4
		newFile <- getCoords (inFile$datapath, input$minLat, input$maxLat, input$minLong, input$maxLong)
		attr (session,"s") <- read.table(newFile)
  		output$sources <- renderTable({
			read.table(newFile)
		},digits = 7)
	})


  output$info <- renderText({

        xy_str <- function(e) {
            if(is.null(e)) return("NULL\n")
            paste0("x=", round(e$x, 7), " y=", round(e$y, 7), "\n")
        }


	attr (session, "minLat") <- NULL
	attr (session, "maxLat") <- NULL
	attr (session, "minLong") <- NULL
	attr (session, "maxLong") <- NULL

    xy_range_str <- function(e) {
      if(is.null(e)) return("Entire Area\n")


	attr (session, "minLat") <- round(e$ymin, 7)
	attr (session, "maxLat") <- round(e$ymax, 7)
	attr (session, "minLong") <- round(e$xmin, 7)
	attr (session, "maxLong") <- round(e$xmax, 7)

      paste0("Long=", round(e$xmin, 7), ",", round(e$xmax, 7), 
             " Lat=", round(e$ymin, 7), ",", round(e$ymax, 7))

	

    }

    paste0(
      "Area: ", xy_range_str(input$plot_brush)
    )
  })
}

# Create Shiny app ----
shinyApp(ui = ui, server = server)
