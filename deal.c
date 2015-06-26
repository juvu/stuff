#include <stdio.h>
#include <math.h>

#define TOTAL_BOXES 22

double Boxes[TOTAL_BOXES+1] = {0.0, 0.01, 0.1, 0.5, 1.0, 5.0, 
							10.0, 50.0, 100.0, 250.0, 500.0, 750.0,
							1000.0, 3000.0, 5000.0, 10000.0, 15000.0, 20000.0, 
							35000.0, 50000.0, 75000.0, 100000.0, 250000.0} ;

static int selectionArray [TOTAL_BOXES+1] ; 
static double AllOffers[1000000] ;

static int selRange = 1000 ;

static int actualNumBoxes = TOTAL_BOXES ;
static int numBoxes = TOTAL_BOXES ;

void printBoard()
{
	printf ("<pre>\n") ;

	if (Boxes[1] == 0.01 && Boxes[12] == 1000.0)
		printf ("1p			£1000\n") ;
	else if (Boxes[1] == 0.0 && Boxes[12] == 1000.0)
		printf ("  			£1000\n") ;
	else if (Boxes[1] == 0.01 && Boxes[12] == 0.0)
		printf ("1p  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[2] == 0.1 && Boxes[13] == 3000.0)
		printf ("10p			£3000\n") ;
	else if (Boxes[2] == 0.0 && Boxes[13] == 3000.0)
		printf ("  			£3000\n") ;
	else if (Boxes[2] == 0.1 && Boxes[13] == 0.0)
		printf ("10p  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[3] == 0.5 && Boxes[14] == 5000.0)
		printf ("50p			£35000\n") ;
	else if (Boxes[3] == 0.0 && Boxes[14] == 5000.0)
		printf ("  			£5000\n") ;
	else if (Boxes[3] == 0.5 && Boxes[14] == 0.0)
		printf ("50p  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[4] == 1.0 && Boxes[15] == 10000.0)
		printf ("£1			£10000\n") ;
	else if (Boxes[4] == 0.0 && Boxes[15] == 10000.0)
		printf ("  			£10000\n") ;
	else if (Boxes[4] == 1.0 && Boxes[15] == 0.0)
		printf ("£1  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[5] == 5.0 && Boxes[16] == 15000.0)
		printf ("£5			£15000\n") ;
	else if (Boxes[5] == 0.0 && Boxes[16] == 15000.0)
		printf ("  			£15000\n") ;
	else if (Boxes[5] == 5.0 && Boxes[16] == 0.0)
		printf ("£5  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[6] == 10.0 && Boxes[17] == 20000.0)
		printf ("£10			£20000\n") ;
	else if (Boxes[6] == 0.0 && Boxes[17] == 20000.0)
		printf ("  			£20000\n") ;
	else if (Boxes[6] == 10.0 && Boxes[17] == 0.0)
		printf ("£10  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[7] == 50.0 && Boxes[18] == 35000.0)
		printf ("£50			£5000\n") ;
	else if (Boxes[7] == 0.0 && Boxes[18] == 35000.0)
		printf ("  			£5000\n") ;
	else if (Boxes[7] == 50.0 && Boxes[18] == 0.0)
		printf ("£50  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[8] == 100.0 && Boxes[19] == 50000.0)
		printf ("£100			£50000\n") ;
	else if (Boxes[8] == 0.0 && Boxes[19] == 50000.0)
		printf ("  			£50000\n") ;
	else if (Boxes[8] == 100.0 && Boxes[19] == 0.0)
		printf ("£100  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[9] == 250.0 && Boxes[20] == 75000.0)
		printf ("£250			£75000\n") ;
	else if (Boxes[9] == 0.0 && Boxes[20] == 75000.0)
		printf ("  			£75000\n") ;
	else if (Boxes[9] == 250.0 && Boxes[20] == 0.0)
		printf ("£250  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[10] == 500.0 && Boxes[21] == 100000.0)
		printf ("£500			£100000\n") ;
	else if (Boxes[10] == 0.0 && Boxes[21] == 100000.0)
		printf ("  			£100000\n") ;
	else if (Boxes[10] == 500.0 && Boxes[21] == 0.0)
		printf ("£500  			\n") ;
	else
		printf ("\n") ;

	if (Boxes[11] == 750.0 && Boxes[22] == 250000.0)
		printf ("£750			£250000\n") ;
	else if (Boxes[11] == 0.0 && Boxes[22] == 250000.0)
		printf ("  			£250000\n") ;
	else if (Boxes[11] == 750.0 && Boxes[22] == 0.0)
		printf ("£750  			\n") ;
	else
		printf ("\n") ;
	printf ("</pre>\n\n") ;

}

int offerSort (double *first, double *second)
{
	if (*first < *second)
		return (-1) ;
	else
		return (1) ;

	return (0) ;
}

void initSelectionArray()
{
	int i=0 ;
	for (i = 1; i <= TOTAL_BOXES; i++)
	{
		selectionArray[i] = 0 ;
	}

	for (i = 1; i <= numBoxes; i++)
	{
		selectionArray[i] = i ;
	}
}

void initAllOffersArray()
{
	int i=0 ;
	for (i = 0; i <= 1000000; i++)
	{
		AllOffers[i] = 0.0 ;
	}
}

int nextSelection() 
{
	int box = 0;

	// start by increasing the last box in the selection array
	selectionArray[numBoxes]++ ;

	// if the last item in the selection array has gone above TOTAL_BOXES
	// then need to increase the selection position before that
	// and possibly earlier positions too

	for (box=numBoxes; selectionArray[box] > TOTAL_BOXES - numBoxes + box;)
	{
		selectionArray[--box]++ ;
	}

	// have we exhausted all combinations?
	if (selectionArray[1] > TOTAL_BOXES - numBoxes + 1) 
		return 0; 

	// need to setup all positions in the selection array 
	// according to the earliest one we manipulated

	for (box = box + 1; box <= numBoxes; box++)
	{
		selectionArray[box] = selectionArray[box - 1] + 1 ;
	}

	return 1;
}

int checkValidSelection()
{
	int i=0;

	// if the box contains 0.0 this means already opened
	// so not a valid choice

	for (i=1;i<=numBoxes;i++)
	{
		if (Boxes[selectionArray[i]] == 0.0)
			return 0 ;
	}
	return 1 ;
}

double getOffer()
{
	double total = 0.0 ;

	double min = 250000.0;

	int i = 0 ;
	for (i=1; i<=numBoxes; i++)
	{
		total += Boxes[selectionArray[i]] ;
		if (Boxes[selectionArray[i]] < min)
			min = Boxes[selectionArray[i]] ;
	}

	double Avg = total / numBoxes ;

	double variance = 0.0 ;
	double totVariance = 0.0 ;

	for (i=1; i<=numBoxes; i++)
	{
		variance = (Boxes[selectionArray[i]] - Avg) * (Boxes[selectionArray[i]] - Avg) ;
		totVariance += variance ;
	}

	double sd = sqrt (totVariance / numBoxes) ;

	double bankerPercent = Avg / (Avg + sd) ;

	// banker offers the minimum amount plus his percentage (usually about 50%)
	// of the difference between the average and the minimum

	double offer = min + ((Avg - min) * bankerPercent) ;

	return (offer) ;
}

getResult (int detail)
{
	// init the selection Array
	initSelectionArray () ;

	// init the AllOffers Array
	initAllOffersArray () ;

	double totalOffer = 0.0 ;
	double thisOffer = 0.0 ;
	int numOffers = 0 ;

	double lowestOffer = 250000.0 ;
	double highestOffer = 0.0 ;

	int result = 1 ;
	while (result)
	{
		int valid = checkValidSelection() ;

		// Print the selection
		if (valid)
		{
			thisOffer = getOffer() ;

			AllOffers[numOffers++] = thisOffer ;

			if (thisOffer < lowestOffer)
				lowestOffer = thisOffer ;

			if (thisOffer > highestOffer)
				highestOffer = thisOffer ;

			totalOffer += thisOffer ;
		}

		// get the next selection
		result = nextSelection () ;
	}

	if (!detail)
	{
		printf ("%d: ", numBoxes) ;
		if (numOffers > 10)
		{
			printf ("Low: %f ", lowestOffer) ;
			printf ("High: %f ", highestOffer) ;
		}
	}

	// sort the array of offers

    int (*sortfn)() = offerSort ;
    qsort (AllOffers, numOffers, sizeof(double), sortfn) ;

	if (!detail)
	{
		if (numOffers <=10)
		{
			int k=0;
			for (k=0;k<numOffers;k++)
			{
				printf ("%f  ",  AllOffers[k]) ;
			}
			printf ("\n") ;
		}
		else
			printf ("Median %f\n", numOffers, AllOffers[numOffers/2]) ;
	}

	if (detail == 1)
	{
		int offerRange[250000] ;

		int minRange = AllOffers[0] / selRange ;
		int maxRange = AllOffers[numOffers-1] / selRange ;

		int i = 0 ;

		// clear the graph data first
		for (i=0; i<250000; i++)
		{
			offerRange[i] = 0 ;
		}

		// now collect the graph data
		for (i=0; i<numOffers; i++)
		{
			int range = AllOffers[i] / selRange ;
			offerRange[range]++ ;
		}

		// and print it out
		for (i=minRange; i<=maxRange; i++)
		{
			printf ("%d - %d\n", i * selRange, (i+1) * selRange) ;
		}
		for (i=minRange; i<=maxRange; i++)
		{
			printf ("%d\n", offerRange[i]) ;
		}
	}

	if (detail == 2)
	{
		int offerRange[250000] ;

		int minRange = AllOffers[0] / selRange ;
		int maxRange = AllOffers[numOffers-1] / selRange ;

		int i = 0 ;

		// clear the graph data first
		for (i=0; i<250000; i++)
		{
			offerRange[i] = 0 ;
		}

		// now collect the graph data
		for (i=0; i<numOffers; i++)
		{
			int range = AllOffers[i] / selRange ;
	
			int j = 0 ;	
			for (j=range; j>= minRange; j--)
			{
				offerRange[j]++ ;
			}
		}

		// and print it out
		for (i=minRange; i<=maxRange; i++)
		{
			printf ("%d - %d\n", i * selRange, (i+1) * selRange) ;
		}
		for (i=minRange; i<=maxRange; i++)
		{
			printf ("%.2f\n", offerRange[i] * 100.0 / (double) numOffers) ;
		}
	}
}

void chooseBox(char *box)
{
	if (!strcmp (box, "1p"))
	{
		Boxes[1] = 0.0 ;
		actualNumBoxes--;
	}
	else if (!strcmp (box, "10p"))
	{
		Boxes[2] = 0.0 ;
		actualNumBoxes--;
	}
	else if (!strcmp (box, "50p"))
	{
		Boxes[3] = 0.0 ;
		actualNumBoxes--;
	}
	else 
	{
		double amount = (double) atoi (box) ;

		int i = 0 ;
		for (i=4;i<=TOTAL_BOXES;i++)
		{
			if (Boxes[i] == amount)
			{
				Boxes[i] = 0.0 ;
				actualNumBoxes--;
				return ;
			}
		}

		printf ("Unrecognised or already selected: %f\n", amount) ;
	}
}

double getBankerRate (double offer)
{
    double total = 0.0 ;

    double min = 250000.0;

    int i = 0 ;
    for (i=1; i<=TOTAL_BOXES; i++)
    {
        total += Boxes[i] ;
        if (Boxes[i] < min)
            min = Boxes[i] ;
    }

    double Avg = total / numBoxes ;

    // banker offers the minimum amount plus his percentage (usually about 50%)
    // of the difference between the average and the minimum

	double rate = (offer - min) / (Avg - min) ;

	return (rate) ;
}
		
int main(int argc, char **argv) 
{
	char buffer[200] = "" ;
	while (1)
	{
		printf ("Action?\n");
		gets(buffer) ;

		if (!strncmp (buffer, "box", 3))
		{
			chooseBox (&buffer[4]) ;
		}
		else if (!strncmp (buffer, "banker", 6))
		{
			double bankerOffer = (double) atoi (&buffer[7]) ;
			double rate = getBankerRate(bankerOffer) ;
			printf ("Banker Rate is %f percent of the average\n", rate*100) ;
		}
		else if (!strncmp (buffer, "detail", 6))
		{
			numBoxes = atoi (&buffer[7]) ;
			getResult (1);
		}
		else if (!strncmp (buffer, "range", 5))
		{
			selRange = atoi (&buffer[6]) ;
		}
		else if (!strncmp (buffer, "board", 5))
		{
			printBoard() ;
		}
		else if (!strncmp (buffer, "percent", 7))
		{
			numBoxes = atoi (&buffer[8]) ;
			getResult (2);
		}
		else if (!strncmp (buffer, "summary", 7))
		{
			numBoxes = actualNumBoxes - 1 ;
		
			while (numBoxes >= 2)
			{
				if (numBoxes == 14 || numBoxes == 11 
						|| numBoxes == 8 || numBoxes <=5)
				{
					getResult(0) ;
				}
				numBoxes--;
			}
		}
	}
	return 0;
}

