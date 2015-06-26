#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#define BINGO_SIZE 20000000

static long numbers[BINGO_SIZE] ;

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

	for (i=BINGO_SIZE;i>0;i--)
	{
		long pos = random() % i ;
		if (i < 50)
			printf ("%d, ", numbers[pos]) ;
		numbers[pos] = numbers[i-1] ;
	}
	printf ("etc \n") ;
}
