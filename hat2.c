#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define BINGO_SIZE 20000000

static 	long numbers[BINGO_SIZE] ;

main (int argc, char **argv)
{
	long tmp = 0 ;

    // init random seed
    struct timeval tp ;
    gettimeofday (&tp, NULL) ;

    srandom (tp.tv_usec) ;

	// create the bingo numbers

	long i = 0 ;
	for (i=0;i<BINGO_SIZE;i++)
	{
		numbers[i] = i + 1 ;
	}

	// now randomize

	for (i=0;i<BINGO_SIZE;i++)
	{
		long newpos = random() % BINGO_SIZE ;

		tmp = numbers[newpos] ;
		numbers[newpos] = numbers[i] ;
		numbers[i] = tmp ;
		
	}
	// now call them

	for (i=0;i<BINGO_SIZE;i++)
	{
		if (i < 50)
			printf ("%d, ", numbers[i]) ;
	}
	printf ("etc \n") ;
}
