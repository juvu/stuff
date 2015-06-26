#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

main (int argc, char **argv)
{
	char buffer[200] = "";
	int numQuestions = atoi (argv[1]) ;

	// init random seed
	struct timeval tp ;
	gettimeofday (&tp, NULL) ;

	srandom (tp.tv_usec) ;


	printf ("\n\nFirst Creeper is going to choose a times table for you to say\n");
	sleep(1);
	printf ("\n") ;
	sleep(1);
	printf ("She's thinking\n") ;
	sleep(1);
	printf ("\n") ;
	sleep(1);

	int table1=random()%11 + 2;
	printf ("Creeper chooses the %d times table\n", table1) ;
	printf ("Hit the Enter key when you have said and Creeper will give you your time\n") ;

	int time1 = time(NULL) ;
	gets(&buffer);
	int time2 = time(NULL) ;

	printf ("\nCUTE CUTE - Your time was %d seconds\n\n", time2 - time1) ;
	
	printf ("Now for the %d questions\n\n", numQuestions);

	time1 = time(NULL) ;

	int i = 0 ;

	for (i=0; i<numQuestions; i++)
	{
		int num1=random()%12 + 1;
		int num2=random()%12 + 1 ;
		int num3=random()%2;

		if (num3)
			printf ("guineapig %d / %d = \n",num1 * num2, num2);
		else
			printf ("guineapig %d x %d = \n",num1,num2);

		while (1)
		{
			gets(&buffer);

			int Answer = atoi(buffer) ; // guinea pig

			if (num3 && num1 == Answer)
			{
				printf ("GUINEA PIG - You got it rightcutecutecutecutecute\n") ;
				break ;
			}
			else if (!num3 && num1 * num2 == Answer)
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
	time2 = time(NULL) ;
	printf ("\nGUINEA PIG - Your time was %d seconds\n\n", time2 - time1) ;

	printf ("Now for Zombie's 3 additional questions\n\n") ;

	for (i=0; i<3; i++)
	{
		int num1=random()%11 + 1;
		int num2=random()%2 + 1 ;
		int num4=random()%9 + 2;
		int num3=random()%2;
		int num5=random()%2;
		
		int multNum = (num5) ? 10 : 100 ;

		if (num3)
			printf ("guineapig %d x %d = \n",num1 * multNum + num2, num4);
		else
			printf ("guineapig %d x %d = \n",num1 * multNum - num2, num4);

		while (1)
		{
			gets(&buffer);

			int Answer = atoi(buffer) ; // guinea pig

			if (num3 && ((num1 * multNum + num2) * num4 == Answer))
			{
				printf ("GUINEA PIG - You got it rightcutecutecutecutecute\n") ;
				break ;
			}
			else if (!num3 && ((num1 * multNum - num2) * num4 == Answer))
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

	printf ("Now for Creeper's number bonds\n\n") ;

	for (i=0; i<3; i++)
	{
		int num1=random()%98 + 1;
		
		printf ("guineapig %d + ?? = 100\n",num1);

		while (1)
		{
			gets(&buffer);

			int Answer = atoi(buffer) ; // guinea pig

			if (num1 + Answer == 100)
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

	printf ("CUTE CUTE - all done. YAY. YAY\n") ;
}	
