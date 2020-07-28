#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#define BET_TAX     0.02					   /* 2% */

static char buffer [100] = { (char) 0 } ;

double getOdds (char *str)
{
	double value = 0.0 ;

	strcpy (buffer, str) ;

	char *rest = NULL ;

	char *ptr = NULL ;
	char *ptr2 = NULL ;

	ptr = (char *) strtok_r (buffer, ":/", &rest) ;
	if (rest && strlen (rest))
	{
		ptr2 = (char *) strtok_r (NULL, ":/", &rest) ;
		double val1 = (double) atoi(ptr) ;
		double val2 = (double) atoi(ptr2) ;
		value = val1 / val2 ;
	}
	else
	{
		value = (double) atof (str) ;
	}

	return (value) ;
}


main(argc, argv)
int             argc;
char          **argv;
{
	int             loop;
	double          total, totsum, profit;
	double          odds[100];
	double          bet[100];
	double 			totalBet = 10.0 ;

	printf("\n\n");

	/* read in the odds, and work out the value of all the odds + 1 multiplied
	   together */

	// Note the +1 is for odds like 2 to 1, 3 to 1 etc. Now use decimal format
	// so don't need the + 1
	
	if (argc < 3)
	{
		printf ("Usage: win <totalToBet> <odds1> <odds2> <odds3> ... \n") ;
		exit (0) ;
	}

	totalBet = (double) atoi(argv[1]);
	for (loop = 2, total = 1.0; loop < argc; loop++)
	{
		odds[loop] = getOdds(argv[loop]);
		//total *= (odds[loop] + 1.0);
		total *= (odds[loop]);
	}

	/* calculate each bet and add up the total bet */

	for (loop = 2, totsum = 0; loop < argc; loop++)
	{
		//bet[loop] = total / (odds[loop] + 1.0);
		bet[loop] = total / (odds[loop] );
		totsum += bet[loop];
	}

	/* reduce total bet to totalBet */

	totsum /= totalBet;

	for (loop = 2; loop < argc; loop++)
	{
		bet[loop] = bet[loop] / totsum;
		printf("Bet on %d, amount %f \n", loop-1, bet[loop]);
	}

	/* calculate profit by taking horse 1 wins as an example */

	//profit = bet[2] * odds[2] - totalBet + bet[2] ;
	profit = bet[2] * odds[2] - totalBet ;

	printf("\n\nProfit       %f\n", profit);
	printf("Profit Post  %f\n", profit * (1.0 - BET_TAX));
	printf("Profit Pre   %f\n", profit - (BET_TAX * totalBet));
}
