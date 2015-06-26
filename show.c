#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define SIZE 3
#define NUM_ITERATIONS 1000000

static int boxes[SIZE] ;

main (int argc, char **argv)
{
    // init random seed
    struct timeval tp ;
    gettimeofday (&tp, NULL) ;

    srandom (tp.tv_usec) ;

	int usingPriorKnowledge = atoi (argv[1]) ;

	int count = 0 ;
	int wins = 0 ;
	int losses = 0 ;
	int markContinue = 0 ;

	while (count < NUM_ITERATIONS)
	{	
		markContinue = 0 ;

		// value of 1 indicates presence of car. Place in randowm slot
		// value of 0 indicates empty box
		// value of -1 indicates opened box

		// empty boxes first
		int i=0;	
		for (i=0;i<SIZE;i++)
		{
			boxes[i] = 0 ;
		}

		// hide the car at random
		int carSlot = random()%SIZE;	
		boxes[carSlot] = 1 ;

		// contestant chooses a box	
		int contestantSlot = random()%SIZE ;

		// presenter opens a box

		for (i=0;i<SIZE;i++)
		{

			if (usingPriorKnowledge)
			{
				// presenter uses knowledge of where the prize is hidden to open a box
				if (i != carSlot && i != contestantSlot)
				{
					boxes [i] = -1 ;
					break ;
				}
			}
			else
			{
				// presenter opens at random - sufficient to choose
				// first array slot that is not the contestants here since
				// the car has been positioned at random

				if (i != contestantSlot)
				{
					if (i == carSlot)
					{
						// Presenter has revealed the prize. This has
						// ruined the show - we are not interested in
						// these scenarios

						markContinue = 1 ;
						break ;

					}
					boxes [i] = -1 ;
					break ;
				}
			}
		}

		if (markContinue)
			continue ; // to next iteration of loop
	
		// contestant swaps his box
	
		for (i=0;i<SIZE;i++)
		{
			// knowledge of where the prize is hidden to open a box
			if (i != contestantSlot && boxes[i] != -1)
			{
				contestantSlot = i;
				break ;
			}
		}
	
		// have we won ?
	
		if (contestantSlot == carSlot)
			wins++ ;
		else
			losses++;

		// keep count of number of iterations
		count++ ;
	}

	printf ("Wins: %d\n", wins) ;
	printf ("Losses: %d\n", losses) ;
}

