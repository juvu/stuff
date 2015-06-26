#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define NUMPOINTS 10000000

double getDistanceFromPosition(double x, double y, double px, double py)
{
	// all we need is a bit of pythagoras
	// px, py is our position and x,y is the position of the blade of grass

	double xSquared = (x - px) * (x - px) ;
	double ySquared = (y - py) * (y - py) ;
	double distance = sqrt (xSquared + ySquared) ;
	return (distance) ;
}

main (int argc, char **argv)
{
	if (argc != 3)
	{
		printf ("Usage: goat <minTether> <maxTether>\n") ;
		exit (0) ;
	}

	double minTether = atof (argv[1]) ;
	double maxTether = atof (argv[2]) ;
	double tether = 0.0 ;

	double increment = (maxTether - minTether) / 10.0 ;

	
    // init random seed
    struct timeval tp ;
    gettimeofday (&tp, NULL) ;

    srandom (tp.tv_usec) ;

	for (tether = minTether; tether <= maxTether; tether += increment)
	{
		// generate points
	
		int numPoints = 0 ;
		int numOutside = 0 ;
		int numInside = 0 ;

		while (numPoints < NUMPOINTS)
		{
			// Each coordinate is somewhere from -1.0 to + 1.0

			double x = 0.000002 * (double) (random()%1000000) - 1.0 ;
			double y = 0.000002 * (double) (random()%1000000) - 1.0 ;

			// get distance from the centre of the field (position 0,0)
			double distanceCentre = getDistanceFromPosition(x, y, 0.0, 0.0);
			if (distanceCentre > 1.0)
			{
				// this blade of grass is not in the field
				continue;
			}
	
			numPoints++ ;
	
			// Now get the distance from the edge of the field. We can
			// choose any coordinate on the edge of the field (so long
			// as we alwyas choose the same point!)
	
			double distancePerimeter = getDistanceFromPosition(x, y, -1.0, 0.0) ;
			
			if (distancePerimeter > tether)
				numOutside++ ;
			else
				numInside++ ;
		}

		printf ("%f,%d,%d\n", tether, numInside, numOutside) ;
	}
}
