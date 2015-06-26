#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

static int numOutside[11] ;
static int numInside[11] ;

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

	int i = 0 ;
	int j = 0 ;
	int pos = 0 ;
	int numPoints = 0 ;

	double minTether = atof (argv[1]) ;
	double maxTether = atof (argv[2]) ;
	double tether = 0.0 ;
	double increment = (maxTether - minTether) / 10.0 ;

	// generate points

	for (i=0;i<20000;i++)
	{
		for (j=0;j<20000;j++)
		{
			// Each coordinate is somewhere from -1.0 to + 1.0

			double x = (double) i * 0.0001 - 1.0 ;
			double y = (double) j * 0.0001 - 1.0 ;

			// get distance from the centre of the field (position 0,0)

			double distanceCentre = getDistanceFromPosition(x, y, 0.0, 0.0);
			if (distanceCentre > 1.0)
			{
				// this blade of grass is not in the field
				continue;
			}

			// Now get the distance from the edge of the field. We can
			// choose any coordinate on the edge of the field (so long
			// as we alwyas choose the same point!)

			double distancePerimeter = getDistanceFromPosition(x, y, -1.0, 0.0) ;

			for (tether=minTether, pos=0; tether <= maxTether; tether += increment, pos++)
			{
				if (distancePerimeter > tether)
					numOutside[pos]++ ;
				else
					numInside[pos]++ ;
			}
		}
	}

	int first = 1;

	for (tether=minTether, pos=0; tether <= maxTether; tether += increment, pos++)
	{
		if (first && (numInside[pos] > numOutside[pos]))
		{
			first = 0 ;
			printf ("--------------------------\n") ;
		}
		printf ("%.10f %d  %d\n", tether, numInside[pos], numOutside[pos]) ;
	}
}
