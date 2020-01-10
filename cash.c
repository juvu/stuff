#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#define MAX_BUFF 1024
#define NUM_ITERATIONS 10000

typedef struct
{
	int	year ;
	double Pru ;
	double ZRP ;
	double Zurich ;
	double cash ;
	double inflation ;
	double AnnuityIncrease ;
	double investmentReturn ;
	double cashReturn ;
	double PruIncome ;
	double ZRPIncome ;
	double ZurichIncome ;
	double cashIncome ;
	double rentIncome ;
	double stateIncome ;
	double FerrantiIncome ;
	double SimonIncome ;
	double F1 ;
	double F2 ;
	double F3 ;
	double F4 ;
	double F5 ;
	double F6 ;
	double S1 ;
	double S2 ;
	double Spend ;
	double inflationFactor ;
	double TaxAllowance ;
	double total ;
	int RTtotal ;
	double inheritance ;
	double income ;
	double tax ;
} Year ;

static Year	years[NUM_ITERATIONS][100] ;
static int InheritYear = 2024 ;
static int PruYear = 2029 ;
static int Pru25 = 0 ;
static int ZurichYear = 2024 ;
static int Zurich25 = 1 ;
static int FerrantiYear = 2029 ;
static int SimonYear = 2029 ;
static double InflationMean = 2.0 ;
static double InflationSD = 1.0 ;
static double InvestMean = 3.0 ;
static double InvestSD = 2.0 ;
static double CashMean = 3.0 ;
static double CashSD = 0.5 ;
static double SpendDecrease = 0.0 ;
static int realTerms = 1 ;

calculateAnnuityInflation(Year *year)
{
	year->AnnuityIncrease = year->inflation - 1.0 ;
	if (year->AnnuityIncrease < 2.5)
		year->AnnuityIncrease = 2.5 ;
	if (year->AnnuityIncrease > 5.0)
		year->AnnuityIncrease = 5.0 ;
}

calculateNewIncomes(Year *year, Year *lastyear)
{

	year->FerrantiIncome = (1.0 + (lastyear->AnnuityIncrease * 0.01)) * lastyear->FerrantiIncome ;
	year->SimonIncome = (1.0 + (lastyear->AnnuityIncrease * 0.01)) * lastyear->SimonIncome ;
	year->stateIncome = (1.0 + (lastyear->inflation * 0.01)) * lastyear->stateIncome ;
	year->rentIncome = (1.0 + (lastyear->inflation * 0.01)) * lastyear->rentIncome ;
	year->TaxAllowance = (1.0 + (lastyear->inflation * 0.01)) * lastyear->TaxAllowance ;
	year->Spend = lastyear->Spend * ((100.0 - SpendDecrease)/100.0) ;
	year->Spend = (1.0 + (lastyear->inflation * 0.01)) * year->Spend ;
	year->inheritance = (1.0 + (lastyear->inflation * 0.01)) * lastyear->inheritance ;
	year->inflationFactor = (1.0 + (lastyear->inflation * 0.01)) * lastyear->inflationFactor ;
	year->S1 = 1.07 * lastyear->S1 ;
	year->S2 = (1.0 + (lastyear->inflation * 0.01)) * lastyear->S2 ;
	year->F1 = 1.075 * lastyear->F1 ;
	year->F2 = 1.075 * lastyear->F2 ;
	year->F3 = 1.075 * lastyear->F3 ;
	year->F4 = (1.0 + (lastyear->inflation * 0.01)) * lastyear->F4 ;
	year->F5 = (1.0 + (lastyear->inflation * 0.01)) * lastyear->F5 ;
	year->F6 = 1.075 * lastyear->F6 ;

	if (lastyear->year != InheritYear)
		lastyear->inheritance = 0 ;
	if (lastyear->year < InheritYear)
		lastyear->rentIncome = 0 ;
	if (lastyear->year <2031)
		lastyear->stateIncome = 0 ;
}

calculateNewTotals (Year *year, Year *lastyear)
{
	if ((lastyear->FerrantiIncome < 1.0) && (lastyear->year == FerrantiYear))
	{
		int yearsEarly = 2029 - lastyear->year ;
		double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
		lastyear->FerrantiIncome = reductionFactor * (lastyear->F1 + lastyear->F2 + lastyear->F3 + lastyear->F4 + lastyear->F5 + lastyear->F6) ;
	}
	if ((lastyear->SimonIncome < 1.0) && (lastyear->year == SimonYear))
	{
		int yearsEarly = 2029 - lastyear->year ;
		double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
		lastyear->SimonIncome = reductionFactor * (lastyear->S1 + lastyear->S2) ;
	}

	lastyear->income = lastyear->FerrantiIncome + lastyear->SimonIncome + lastyear->cashIncome 
						+ lastyear->ZurichIncome + +lastyear->ZRPIncome + lastyear->PruIncome;
	if (year->year > InheritYear)
		lastyear->income += lastyear->rentIncome ;

	if (year->year > 2031)
		lastyear->income += lastyear->stateIncome ;

	double taxable = lastyear->income - lastyear->TaxAllowance ;

	// note - assume only half the cashIncome is taxable (ISAs, allowances etc)
	taxable -= (lastyear->cashIncome * 0.5) ;

	// and only 75% of Zurich and Pru taxable if we haven't taken 25% lump sum
	if (!Pru25)
		taxable -= (lastyear->PruIncome * 0.25) ;
	if (!Zurich25)
		taxable -= (lastyear->ZurichIncome * 0.25) ;

	lastyear->tax = 0 ;
	if (taxable > 0)
		lastyear->tax = taxable * 0.2 ;

	// note - this may well be negative
	double cashSpend = lastyear->Spend + lastyear->tax - lastyear->income ;

	year->cash = lastyear->cash - cashSpend ;
	if (year->year == InheritYear + 1)
		year->cash += lastyear->inheritance ;
	year->cashIncome = (year->cashReturn * 0.01) * year->cash;
	year->ZRP = ((1.0 + (lastyear->investmentReturn * 0.01)) * lastyear->ZRP) - lastyear->ZRPIncome;
	year->Zurich = ((1.0 + (lastyear->investmentReturn * 0.01)) * lastyear->Zurich) - lastyear->ZurichIncome;
	year->Pru = ((1.0 + (lastyear->investmentReturn * 0.01)) * lastyear->Pru) - lastyear->PruIncome;
	year->total = year->ZRP + year->Zurich + year->Pru + year->cash ;

	if (year->year == ZurichYear)
	{
		if (Zurich25)
		{
			double cashTake = year->Zurich * 0.25 ;
			year->Zurich -= cashTake ;
			year->cash += cashTake ;
		}
	}
	if (year->year == PruYear)
	{
		if (Pru25)
		{
			double cashTake = year->Pru * 0.25 ;
			year->Pru -= cashTake ;
			year->cash += cashTake ;
		}
	}
}

int calculateIncome (Year *year)
{
	// note - assume only half the cashIncome is taxable (ISAs, allowances etc)
	double income = year->FerrantiIncome + year->SimonIncome + (year->cashIncome * 0.5) ;
	if (year->year >= InheritYear)
		income += year->rentIncome ;
	if (year->year > 2031)
		income += year->stateIncome ;

	double taxable = income - year->TaxAllowance ;

	double tax = 0 ;
	if (taxable > 0)
		tax = taxable * 0.2 ;

	// how much extra do we still need?
	double required = year->Spend + tax - income ;

	// how much can we take tax free?
	double taxFree = year->TaxAllowance - income ;
	if (taxFree < 0.0)
		taxFree = 0.0;

	// take the tax free amount from Zurich and pru (or from ZRP before ZurichYear)
	year->ZRPIncome = 0.0 ;
	if (year->year < ZurichYear)
	{
		year->ZRPIncome = year->TaxAllowance ;
	}
	if (year->year >= ZurichYear && year->year < PruYear)
	{
		year->ZurichIncome = taxFree ;
	}
	else if (year->year >= PruYear)
	{
		year->ZurichIncome = taxFree / 2.0 ;
		year->PruIncome = taxFree / 2.0 ;
	}

	// how much do we still need?
	required -= taxFree ;
	if (required < 0)
		required = 0 ;

	// take the extra required proportionate to
	// what we have left in each pot

	if (required > 0)
	{
		if (year->year >= ZurichYear && year->year < PruYear)
		{
			year->ZurichIncome += (year->Zurich / (year->Zurich + year->ZRP + year->cash)) * required ;
			year->ZRPIncome += (year->ZRP / (year->Zurich + year->ZRP + year->cash)) * required ;
		}
		else if (year->year >= PruYear && year->year >= ZurichYear)
		{
			year->ZurichIncome += (year->Zurich / year->total) * required ;
			year->PruIncome += (year->Pru / year->total) * required ;
			year->ZRPIncome += (year->ZRP / year->total) * required ;
		}
	}

	return 0 ;
}

double StandardDeviationsFromMean(double SD)
{
	int sdkey = random()%1000 ;
	int interval = random()%50 ;
	double adjust = interval / 100.0 ;
	double amount = 0.0 ;

	if (sdkey <= 6)
		amount = -2.5 ;
	else if (sdkey <= 23)
		amount = -2.0 ;
	else if (sdkey <= 67)
		amount = -1.5 ;
	else if (sdkey <= 159)
		amount = -1.0 ;
	else if (sdkey <= 309)
		amount = -0.5 ;
	else if (sdkey <= 500)
		amount = 0.0 ;
	else if (sdkey <= 691)
		amount = 0.5 ;
	else if (sdkey <= 841)
		amount = 1.0 ;
	else if (sdkey <= 933)
		amount = 1.5 ;
	else if (sdkey <= 977)
		amount = 2.0 ;
	else if (sdkey <= 994)
		amount = 2.5 ;
	else
		amount = 3.0 ;

	return ((amount - adjust) * SD) ;
}

int setupReturns(Year *year) 
{
	//int InflationDiff = (InflationMax - InflationMin) * 10 ;
	//int inflation = (InflationDiff > 0) ? random()%InflationDiff : 0;
	//year->inflation = (inflation / 10.0) + InflationMin ;
	year->inflation = InflationMean + StandardDeviationsFromMean(InflationSD) ;

	//int InvestDiff = (InvestMax - InvestMin) * 10 ;
	//int invest = (InvestDiff > 0) ? random()%InvestDiff : 0;
	//year->investmentReturn = (invest / 10.0) + InvestMin ;
	year->investmentReturn = InvestMean + StandardDeviationsFromMean(InvestSD) ;

	//int CashDiff = (CashMax - CashMin) * 10 ;
	//int cash = (CashDiff > 0) ? random()%CashDiff : 0;
	//year->cashReturn = (cash / 10.0) + CashMin ;
	year->cashReturn = CashMean + StandardDeviationsFromMean(CashSD) ;
}

int processYear (Year *year, Year *lastyear)
{
	int finished = 0 ;
	
	setupReturns(year);
	calculateAnnuityInflation(year);
	calculateNewTotals(year, lastyear);
	calculateNewIncomes(year, lastyear);
	calculateIncome(year);

	return 0 ;
}

setupFerranti (Year *year)
{	
	FILE *stream = fopen ("FerrantiData", "r") ;
	int i=0;

	char	buffer[MAX_BUFF+1] = "" ;
	for (i=0;i<10;i++)
	{
		if (fgets (buffer, MAX_BUFF, stream))
		{
			char *rest = NULL ;
			char *ptr = NULL;
	
			ptr = (char *) strtok_r (buffer, ":", &rest) ;
			int theyear = atoi(ptr) ;
	
			if (theyear == year->year)
			{
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->F1 = atof (ptr);
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->F2 = atof (ptr);
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->F3 = atof (ptr);
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->F4 = atof (ptr);
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->F5 = atof (ptr);
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->F6 = atof (ptr);
			}
		}
	}
	fclose (stream) ;
}

setupSimon (Year *year)
{	
	FILE *stream = fopen ("SimonData", "r") ;
	int	i=0;

	char	buffer[MAX_BUFF+1] = "" ;
	for (i=0;i<10;i++)
	{
		if (fgets (buffer, MAX_BUFF, stream))
		{
			char *rest = NULL ;
			char	*ptr = NULL;
	
			ptr = (char *) strtok_r (buffer, ":", &rest) ;
			int theyear = atoi(ptr) ;
	
			if (theyear == year->year)
			{
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->S1 = atof (ptr);
				ptr = (char *) strtok_r (NULL, ":", &rest) ;
				year->S2 = atof (ptr);
			}
		}
	}
	fclose (stream) ;
}

int sortItOut(int *a, int *b)
{
    if (*a < *b)
        return -1 ;
    if (*a > *b)
        return 1 ;

    return 0 ;
}


main(argc, argv)
int 	argc ;
char	**argv ;
{
	int nextYear = 0 ;
	int finished = 0 ;
	int html = 1 ;
	int loop = 0 ;
	Year *firstyear = NULL ;

	if (argc != 30)
	{
		printf ("Usage: %d cash <random seed> <rent> <inherit> <inheritYear> <spend> <ZurichYear> <Zurich25> <PruYear> <Pru25> <FerrantiYear> <SimonYear>\n", argc);
		printf ("                                   <InflationMean> <InflationSD> <InvestMean> <InvestSD> <CashMean> <CashSD> <SpendDecrease>\n");
		printf ("                                   <FirstYear> <ZRP> <Zurich> <Pru> <cash> <FerrantiAmount> <SimonAmount> <stateAmount> <taxAllowance> <realTerms>\n");
		exit (0) ;
	}
    // init random seed

	int seed = atoi (argv[1]) ;
    srandom (seed) ;

	// store the inheritance year

	InheritYear = atoi (argv[4]) ;

#if 0
	/* open the file containing the rates */

	if (argc !=2 && argc !=3)
	{
		printf ("Usage: cash <filename>\n");
		printf ("   or: cash -html <filename>\n");
		exit (0) ;
	}

	if (!strcmp (argv[1], "-html"))
	{
		stream = fopen (argv[2], "r") ;
		html = 1 ;
	}
	else	
		stream = fopen (argv[1], "r") ;
#endif

	// read in starting amounts

	int iteration = 0 ;
	char resultsShow [100] ;
	double RTSpend = 0.0 ;

	for (iteration=0;iteration<NUM_ITERATIONS;iteration++)
	{
		firstyear = &years[iteration][0];
		firstyear->year = atoi (argv[19]);
		firstyear->ZRP = atof (argv[20]) * 1000;
		firstyear->Zurich = atof (argv[21]) * 1000;
		firstyear->Pru = atof (argv[22]) * 1000;
		firstyear->cash = atof (argv[23]) * 1000;
		firstyear->rentIncome = atof(argv[2]) ;
		firstyear->inheritance = atof(argv[3]) * 1000;
		firstyear->Spend = atof(argv[5]) ;
		firstyear->inflationFactor = 1.0 ;
		ZurichYear = atoi (argv[6]) ;
		Zurich25 = atoi (argv[7]) ;
		PruYear = atoi (argv[8]) ;
		Pru25 = atoi (argv[9]) ;
		FerrantiYear = atoi (argv[10]) ;
		SimonYear = atoi (argv[11]) ;
		InflationMean = atof (argv[12]) ;
		InflationSD = atof (argv[13]) ;
		InvestMean = atof (argv[14]) ;
		InvestSD = atof (argv[15]) ;
		CashMean = atof (argv[16]) ;
		CashSD = atof (argv[17]) ;
		SpendDecrease = atof (argv[18]) ;
		firstyear->FerrantiIncome = atof (argv[24]) ;
		setupFerranti (firstyear) ;
		firstyear->SimonIncome = atof (argv[25]) ;
		setupSimon (firstyear) ;
		firstyear->stateIncome = atof (argv[26]) ;
		firstyear->TaxAllowance = atof (argv[27]) ;
		realTerms = atoi (argv[28]) ;
		RTSpend = firstyear->Spend ;
		strcpy (resultsShow, argv[29]) ;
/*
		int rentOrSell = random()%2 ;
		if (rentOrSell)
			firstyear->inheritance = 0 ;
		else
			firstyear->rentIncome = 0 ;
*/	
		setupReturns (firstyear) ;
	
		if (firstyear->year == ZurichYear)
		{
			if (Zurich25)
			{
				double cashTake = firstyear->Zurich * 0.25 ;
				firstyear->Zurich -= cashTake ;
				firstyear->cash += cashTake ;
			}
		}
		if (firstyear->year == PruYear)
		{
			if (Pru25)
			{
				double cashTake = firstyear->Pru * 0.25 ;
				firstyear->Pru -= cashTake ;
				firstyear->cash += cashTake ;
			}
		}
		firstyear->total = firstyear->ZRP + firstyear->Zurich + firstyear->Pru + firstyear->cash ;
		firstyear->cashIncome = (firstyear->cashReturn * 0.01) * firstyear->cash;
	
		if ((firstyear->FerrantiIncome < 1.0) && (firstyear->year == FerrantiYear))
		{
			int yearsEarly = 2029 - firstyear->year ;
			double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
			firstyear->FerrantiIncome = reductionFactor * (firstyear->F1 + firstyear->F2 + firstyear->F3 + firstyear->F4 + firstyear->F5 + firstyear->F6) ;
		}
		if ((firstyear->SimonIncome < 1.0) && (firstyear->year == SimonYear))
		{
			int yearsEarly = 2029 - firstyear->year ;
			double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
			firstyear->SimonIncome = reductionFactor * (firstyear->S1 + firstyear->S2) ;
		}
	
		calculateAnnuityInflation(firstyear);
		calculateIncome(firstyear);
	
		// now process every year
	
		for (nextYear = 1; nextYear<=45; nextYear++)
		{
			years[iteration][nextYear].year = nextYear + firstyear->year;
			processYear(&years[iteration][nextYear], &years[iteration][nextYear-1]);
		}
	}

	/* collect best, worst and average year info */

	int bestYear = -1 ;
	int worstYear = -1 ;
	int averageYear = -1 ;
	int Year10 = -1 ;
	int Year25 = -1 ;
	int Year75 = -1 ;
	int Year90 = -1 ;

	int year80 = 2044 - firstyear->year ;
	int results80[NUM_ITERATIONS];

	for (iteration=0;iteration<NUM_ITERATIONS;iteration++)
	{
		int asdf = 0;
		double inflationFactor = 1.0 ;

		asdf = (int) years[iteration][year80].total;
		//inflationFactor = years[iteration][0].Spend / years[iteration][year80].Spend ;
		inflationFactor = 1.0 / years[iteration][year80].inflationFactor ;
		results80[iteration] = (int) (asdf * inflationFactor);
		years[iteration][year80].RTtotal = results80[iteration];
	}

    int (*sortfn)() = sortItOut ;
    qsort (results80, NUM_ITERATIONS, sizeof(int), sortfn) ;

	long long itTotal = 0 ;
	for (iteration=0;iteration<NUM_ITERATIONS;iteration++)
	{
		itTotal += (long long) years[iteration][year80].RTtotal ;
	}

	int avTotal = itTotal / NUM_ITERATIONS ;
	for (iteration=0;iteration<NUM_ITERATIONS;iteration++)
	{
		if (results80[iteration] > avTotal)
		{
			avTotal = results80[iteration];
			break ;
		}
	}

	for (iteration=0;iteration<NUM_ITERATIONS;iteration++)
	{
		if (results80[0] == years[iteration][year80].RTtotal)
			worstYear = iteration ;
		if (results80[NUM_ITERATIONS/4] == years[iteration][year80].RTtotal)
			Year25 = iteration ;
		if (results80[NUM_ITERATIONS/10] == years[iteration][year80].RTtotal)
			Year10 = iteration ;
		if (avTotal == years[iteration][year80].RTtotal)
			averageYear = iteration ;
		if (results80[3*NUM_ITERATIONS/4] == years[iteration][year80].RTtotal)
			Year75 = iteration ;
		if (results80[9*NUM_ITERATIONS/10] == years[iteration][year80].RTtotal)
			Year90 = iteration ;
		if (results80[NUM_ITERATIONS-1] == years[iteration][year80].RTtotal)
			bestYear = iteration ;
	}

	int rs = 0 ;

	if (!strcmp (resultsShow,"best"))
		rs = bestYear ;
	else if (!strcmp (resultsShow,"worst"))
		rs = worstYear ;
	else if (!strcmp (resultsShow,"average"))
		rs = averageYear ;
	else if (!strcmp (resultsShow,"year25"))
		rs = Year25 ;
	else if (!strcmp (resultsShow,"year75"))
		rs = Year75 ;
	else if (!strcmp (resultsShow,"year10"))
		rs = Year10 ;
	else if (!strcmp (resultsShow,"year90"))
		rs = Year90 ;

	/* print out the results */

	if (!html)
	{
		for (loop=0;loop<nextYear-1;loop++)
		{
			printf ("%d, ZRP %d, Zurich %d, Pru %d, Cash %d Total %d Tax %d Spend %d\n", years[rs][loop].year, (int) years[rs][loop].ZRP, (int) years[rs][loop].Zurich,
														(int) years[rs][loop].Pru, (int) years[rs][loop].cash, (int) years[rs][loop].total,
															(int) years[rs][loop].tax, (int) years[rs][loop].Spend) ;
		}
	}
	else
	{
		printf ("<center><TABLE COLS=10\n border=3>") ;
		//printf ("<TR bgcolor=green> \n") ;
		printf ("<TR> \n") ;
		printf ("<TD><center>year<center>\n") ;
		printf ("<TD><center>ZRPTotal<center>\n") ;
		printf ("<TD><center>ZurichTotal<center>\n") ;
		printf ("<TD><center>PruTotal<center>\n") ;
		printf ("<TD><center>investReturn<center>\n") ;
		printf ("<TD><center>Cash<center>\n") ;
		printf ("<TD><center>CashReturn<center>\n") ;
		printf ("<TD><center>inflation<center>\n") ;
		printf ("<TD><center>ZRPIncome<center>\n") ;
		printf ("<TD><center>ZurichIncome<center>\n") ;
		printf ("<TD><center>PruIncome<center>\n") ;
		printf ("<TD><center>cashIncome<center>\n") ;
		printf ("<TD><center>rentIncome<center>\n") ;
		printf ("<TD><center>stateIncome<center>\n") ;
		printf ("<TD><center>FerrantiIncome<center>\n") ;
		printf ("<TD><center>SimonIncome<center>\n") ;
		printf ("<TD><center>Spend<center>\n") ;
		printf ("<TD><center>TaxAllowance<center>\n") ;
		printf ("<TD><center>Tax<center>\n") ;
		printf ("<TD><center>total<center>\n") ;
		printf ("<TD><center>inheritance<center>\n") ;
		printf ("<TD><center>income<center>\n") ;
		printf ("<TD><center>Ratio<center>\n") ;
		printf ("<TD><center>RTSpend<center>\n") ;
		printf ("<TR>\n") ;

		for (loop=0;loop<nextYear-1;loop++)
		{
			double inflationFactor = 1.0 ;

			if (!(loop%5))
			{
				printf ("<TR bgcolor=yellow>\n") ;
				//printf ("<TR>\n") ;
			}
			else
				printf ("<TR>\n") ;

			//inflationFactor = years[rs][0].Spend / years[rs][loop].Spend ;
			inflationFactor = 1.0 / years[rs][loop].inflationFactor ;

			if (!realTerms)
				inflationFactor = 1.0 ;

			printf ("<TD><center>%d<center>\n", years[rs][loop].year) ;
			printf ("<TD><center>%d<center>\n", (int) (years[rs][loop].ZRP * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].Zurich * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].Pru * inflationFactor)) ;
			printf ("<TD><center>%.2f<center>\n", (years[rs][loop].investmentReturn * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].cash * inflationFactor)) ;
			printf ("<TD><center>%.2f<center>\n", (years[rs][loop].cashReturn * inflationFactor)) ;
			printf ("<TD><center>%.2f<center>\n", years[rs][loop].inflation) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].ZRPIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].ZurichIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].PruIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].cashIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].rentIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].stateIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].FerrantiIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].SimonIncome * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].Spend * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].TaxAllowance * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].tax * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].total * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].inheritance * inflationFactor)) ;
			printf ("<TD><center>%d<center>\n", (int)(years[rs][loop].income * inflationFactor)) ;
			printf ("<TD><center>%.1f<center>\n", (years[rs][loop].total * years[rs][loop].Spend)) ;
			if (loop)
				RTSpend *= (1.0 - (SpendDecrease * 0.01)) ;
			printf ("<TD><center>%d<center>\n", (int)(RTSpend)) ;
			printf ("</TR>\n") ;
		}
		printf ("</TABLE>\n") ;
	}

	/* close the input stream */

	//fclose (stream) ;
}

