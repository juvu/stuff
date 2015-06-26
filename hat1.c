#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define BINGO_SIZE 20000000

long numbers[BINGO_SIZE] ;

main (int argc, char **argv)
{
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

	// now call them

	long remaining = BINGO_SIZE ;

	while (remaining)
	{
		long pos = random() % BINGO_SIZE ;
		if (numbers[pos] != 0)
		{
			if (remaining < 50)
				printf ("%d, ", numbers[pos]) ;
			numbers[pos] = 0 ;
			remaining-- ;
		}
	}
	printf ("etc \n") ;
}
