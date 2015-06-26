#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

main (int argc, char **argv)
{
	int maxTable = atoi (argv[1]) ;

	// init random seed
	struct timeval tp ;
	gettimeofday (&tp, NULL) ;

	srandom (tp.tv_usec) ;

	int time1 = time(NULL) ;

	int i = 0 ;

	for (i=0; i<10; i++)
	{
		char buffer[200] = "";

		int num1=random()%12 + 1;
		int num2=random()%maxTable + 1 ;

		printf ("guineapig %d x %d = \n",num1,num2);

		while (1)
		{
			gets(&buffer);

			int Answer = atoi(buffer) ; // guinea pig

			if (num1 * num2 == Answer)
			{
				printf ("GUINEA PIG - You got it rightcutecutecutecutecute\n") ;
				break ;
			}
			else
			{
				printf ("GUINEA PIG - Try again\n") ;
			}
		}
	}
	int time2 = time(NULL) ;
	printf ("\nGUINEA PIG - Your time was %d seconds\n\n", time2 - time1) ;
}	
