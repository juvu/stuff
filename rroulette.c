#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

main (int argc, char **argv)
{
	int firstShot = 0 ;
	int secondShot = 0 ;

        // init random seed
        struct timeval tp ;
        gettimeofday (&tp, NULL) ;

        srandom (tp.tv_usec) ;

	int maxNum = atoi (argv[1]) ;

	printf ("With no spinning of magazine\n") ;

	int loop = 0 ;
	for (loop=1; loop<=maxNum;loop++)
	{
		int pos = random()%6 + 1 ;
		if (pos%2)
			firstShot++ ;
		else 
			secondShot++;

	}
	printf ("First person shot %d times\n", firstShot) ;
	printf ("Second person shot %d times\n", secondShot) ;


	printf ("\nIf spinning the magazine\n") ;

	firstShot = 0 ;
	secondShot = 0 ;
	int loop2 = 0 ;
	for (loop=1; loop<=maxNum;loop++)
	{
		for (loop2=1; ;loop2++)
		{
			int pos = random()%6 + 1 ;
			if (pos == 1)
			{
				if (loop2%2)
					firstShot++ ;
				else 
					secondShot++;
				break ;
			}
		}
	}

	printf ("First person shot %d times\n", firstShot) ;
	printf ("Second person shot %d times\n", secondShot) ;
}
