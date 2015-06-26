#include <stdio.h>
#include <sys/time.h>

static int bet[500000] ;
static int series = 0 ;
static int seriesCount = 0 ;
static int lowbet = 0;
static int highbet = 0;
static int thisbet = 0;
static int numBets = 0 ;
static int total = 0 ;
static int target = 0 ;
static int maxbet = 0 ;
static int runningTotal = 0 ;
static int totalSav = 0 ;
static int totalBets = 0 ;
static int numSuccess = 0 ;
static int numFails = 0 ;
static int runningLoss = 0 ;

#define NUM_SERIES 50000

nextSeries(int argc, char **argv)
{
	runningTotal += (total - totalSav);
	totalBets += numBets ;

	//printf ("Running total %d\n", runningTotal) ;

	total = atoi (argv[1]) ;
	target = atoi (argv[2]) ;
	series++ ;
	maxbet = 0 ;
	numBets = 0 ;
}

resetbets(int argc, char **argv)
{
	int i ;
	for (i=0;i<10000;i++)
	{
		bet[i] = 0 ;
	}


	for (i=3; i < argc; i++)
	{
		bet[i-3] = atoi (argv[i]) ;
	}

	lowbet = 0;
	highbet = argc - 4 ;
}

main (int argc, char **argv)
{
	int		winfactor ;
	int		maxhighbet = 0 ;

	if (argc < 4)
	{
		printf ("Usage: dalamb <wallet> <target> <first> <second> <third> ...\n") ;
		printf ("eg dalamb 1000 1100 2 2 2 2\n") ;
		exit (0) ;
	}

	// init totals
	totalSav = atoi (argv[1]) ;
	total = totalSav ;
	runningTotal = totalSav ;

	// init random seed
	struct timeval tp ;
	gettimeofday (&tp, NULL) ;

	srandom (tp.tv_usec) ;

	// init series and bets
	nextSeries(argc, argv) ;
	resetbets(argc, argv) ;

	while (1)
	{
		if (series > NUM_SERIES)
			break ;

		numBets++ ;

		if (lowbet == highbet)
			thisbet = bet[lowbet] ;
		else
			thisbet = bet[lowbet] + bet[highbet] ;

		if (thisbet > total)
		{
			/*
			printf ("Failed: Series %d, Total %d, maxBet %d, numBets %d ThisBet %d ",
						series, total, maxbet, numBets, thisbet) ;
			*/

			runningLoss += (totalSav - total) ;
			numFails++ ;
			nextSeries(argc, argv) ;
			resetbets(argc, argv) ;
			continue ;
		}

		if (thisbet > maxbet)
			maxbet = thisbet ;

		winfactor = random() % 100000 ;
		//if (winfactor <= 48649)
		if (winfactor <= 51351)
		{
			total += thisbet ;
			lowbet++ ;
			highbet--;
			if (lowbet > highbet)
				resetbets(argc, argv) ;
		}
		else
		{
			total -= thisbet ;
			highbet++ ;
			bet[highbet] = thisbet ;
		}

		if (total >= target)
		{
			/*
			printf ("Success: Series %d, Total %d, maxBet %d, numBets %d ",
						series, total, maxbet, numBets) ;
			*/

			numSuccess++ ;
			nextSeries(argc, argv) ;
			resetbets(argc, argv) ;
		}


	}

	double seriesSuccess = numSuccess / (double) NUM_SERIES * 100.0 ;
	double seriesFail = numFails / (double) NUM_SERIES * 100.0 ;
	double avBets = totalBets / (double) NUM_SERIES ;
	double avTimeSeries = avBets * 45.0 / 60 ;
	double avGain = (runningTotal - totalSav) / (double) NUM_SERIES ;
	double avGainBet = (runningTotal - totalSav) / (double) totalBets ;
	double avLoss = runningLoss / (double) numFails ;

	printf ("\n") ;
	printf ("Current Balance				%d\n", runningTotal) ;
	//printf ("Num Target Success 			%d\n", numSuccess) ;
	//printf ("Num Target Fail 			%d\n", numFails) ;

	printf ("Target success percentage 		%f\n",seriesSuccess);
	printf ("Target fail percentage 			%f\n",seriesFail);
	printf ("Average loss per Target failure 	%f\n",avLoss);
	printf ("Average bets per Target attempt		%f\n",avBets);
	printf ("Average gain per Target attempt		%f\n",avGain);
	printf ("Average gain per bet 			%f\n",avGainBet);

	if (avTimeSeries < 60.0)
		printf ("Average time per Target attempt		%f minutes\n",avTimeSeries);
	else
		printf ("Average time per Target attempt		%f hours\n",avTimeSeries / 60.0);
}
