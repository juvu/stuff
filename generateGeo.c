#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#define MAX_BUFF 1024

typedef struct
{
	double latitude ;
	double longitude ;
} Point ;

Point sources[MAX_BUFF] ;


double StandardDeviationsAdjust(double SD)
{
	int sdkey = random()%1000 ;
	int interval = random()%50 ;
	double adjust = interval / 100.0 ;
	double amount = 0.0 ;

	if (sdkey <= 6)
		amount = -2.5 ;
	else if (sdkey <= 23)
		amount = -2.0 ;
	else if (sdkey <= 67)
		amount = -1.5 ;
	else if (sdkey <= 159)
		amount = -1.0 ;
	else if (sdkey <= 309)
		amount = -0.5 ;
	else if (sdkey <= 500)
		amount = 0.0 ;
	else if (sdkey <= 691)
		amount = 0.5 ;
	else if (sdkey <= 841)
		amount = 1.0 ;
	else if (sdkey <= 933)
		amount = 1.5 ;
	else if (sdkey <= 977)
		amount = 2.0 ;
	else if (sdkey <= 994)
		amount = 2.5 ;
	else
		amount = 3.0 ;

	return ((amount - adjust) * SD) ;
}

main(argc, argv)
int 	argc ;
char	**argv ;
{
	if (argc != 10)
	{
		printf ("Usage: generateGeo <numSources> <minLatitude> <maxLatitude> <latitideSD>\n"); 
		printf ("								<minLongitude> <maxLongitude> <longitudeSD>\n") ;
		printf ("								<minPointsPerSource> <maxPointsPerSource>\n") ;
		exit (0) ;
	}

    // init random seed
    struct timeval tp ;
    gettimeofday (&tp, NULL) ;

    srandom (tp.tv_usec) ;

	int numSources = atoi (argv[1]) ;
	double minLat = atof (argv[2]) ;
	double maxLat = atof (argv[3]) ;
	double latSD = atof (argv[4]) ;
	double minLong = atof (argv[5]) ;
	double maxLong = atof (argv[6]) ;
	double longSD = atof (argv[7]) ;
	int minPoints = atoi (argv[8]) ;
	int maxPoints = atoi (argv[9]) ;
	int pointsDiff = maxPoints - minPoints ;
	int latDiff = (maxLat * 1000.0) - (minLat * 1000.0) ;
	int longDiff = (maxLong * 1000.0) - (minLong * 1000.0) ;

	int i=0;
	int j=0;
	int count=0;
	printf ("\"longitude\" \"latitude\"\n") ;
	for (i=0; i<numSources; i++)
	{
		// how many points do we want for this source

		int numPoints = random()%pointsDiff + minPoints ;
		//printf ("Creating %d points\n", numPoints) ;

		// select out latitude and longitude for this souce

		sources[i].latitude = (random()%latDiff) / 1000.0 + minLat ;
		sources[i].longitude = (random()%longDiff) / 1000.0 + minLong ;

		//printf ("Creating source at %f %f\n", sources[i].latitude, sources[i].longitude) ;

		// now create points for this source

		for (j=0;j<numPoints;j++)
		{
			count++;
			double latitude = sources[i].latitude + StandardDeviationsAdjust (latSD) ;
			double longitude = sources[i].longitude + StandardDeviationsAdjust (longSD) ;
			printf ("\"%d\" %f %f\n", count, longitude, latitude) ;
		}
		
	}
	printf ("\n\n\n\n\"longitude\" \"latitude\"\n") ;
	count = 0 ;
	for (i=0; i<numSources; i++)
	{
			count++ ;
			printf ("\"%d\" %f %f\n", count, sources[i].longitude, sources[i].latitude) ;
	}
}
		
	
