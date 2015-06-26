#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define NUMPOINTS 100000

// now for a big array
static double distancePerimeter[4000000] ;

double getDistanceFromPosition(double x, double y, double px, double py)
{
	// all we need is a bit of pythagoras
	// px, py is our position and x,y is the position of the blade of grass

	double xSquared = (x - px) * (x - px) ;
	double ySquared = (y - py) * (y - py) ;
	double distance = sqrt (xSquared + ySquared) ;
	return (distance) ;
}

int distanceSort (double *first, double *second)
{
    // we don't really care how they are sorted so just use strcmp

	if (*first < *second)
		return (-1) ;
	else
		return (1) ;

	return (0) ;
}


main (int argc, char **argv)
{
	int i = 0 ;
	int j = 0 ;
	int numPoints = 0 ;

	// generate points

	double percent = atof (argv[1]) ;

	double goatX = -1.0 ;
	double goatY = 0.0 ;

	if (argc == 4)
	{
		goatX = atof (argv[2]) ;
		goatY = atof (argv[3]) ;
	}

	for (i=0;i<2000;i++)
	{
		for (j=0;j<2000;j++)
		{
			// Each coordinate is somewhere from -1.0 to + 1.0

			double x = (double) i * 0.001 - 1.0 ;
			double y = (double) j * 0.001 - 1.0 ;

			// get distance from the centre of the field (position 0,0)

			double distanceCentre = getDistanceFromPosition(x, y, 0.0, 0.0);
			if (distanceCentre > 1.0)
			{
				// this blade of grass is not in the field
				continue;
			}

			// Now get the distance from the edge of the field. We can
			// choose any coordinate on the edge of the field (so long
			// as we always choose the same point!)

			
			double distPerimeter = getDistanceFromPosition(x, y, -1.0, 0.0) ;

			distancePerimeter[numPoints++] = getDistanceFromPosition(x, y, goatX, goatY) ;
		}
	}

	// sort the blades of grass

	int (*sortfn)() = distanceSort ;
	qsort (distancePerimeter, numPoints, sizeof(double), sortfn) ;

	int percentPos = (double) numPoints / (100.0 / percent) - 1;
	printf ("Length of Tether = %f \n", distancePerimeter[percentPos]) ;
}
