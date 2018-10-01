#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#define MAX_BUFF 1024

typedef struct
{
	int	year ;
	double Pru ;
	double ZRP ;
	double cash ;
	double inflation ;
	double AnnuityIncrease ;
	double investmentReturn ;
	double cashReturn ;
	double PruIncome ;
	double ZRPIncome ;
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
	double TaxAllowance ;
	double total ;
	double inheritance ;
	double income ;
	double tax ;
} Year ;

static FILE	*stream = NULL;
static Year	years[100] ;
static int InheritYear = 2024 ;
static int PruYear = 2024 ;
static int Pru25 = 0 ;
static int ZRPYear = 2019 ;
static int ZRP25 = 1 ;
static int FerrantiYear = 2019 ;
static int SimonYear = 2019 ;
static double InflationMin = 0.0 ;
static double InflationMax = 0.0 ;
static double InvestMin = 0.0 ;
static double InvestMax = 0.0 ;
static double CashMin = 0.0 ;
static double CashMax = 0.0 ;
static double SpendDecrease = 0.0 ;

calculateAnnuityInflation(Year *year)
{
	year->AnnuityIncrease = year->inflation - 1.0 ;
	if (year->AnnuityIncrease < 2.5)
		year->AnnuityIncrease = 2.5 ;
	if (year->AnnuityIncrease > 5.0)
		year->AnnuityIncrease = 5.0 ;
}

int readStart(Year *year)
{
	char	buffer[MAX_BUFF+1] = "" ;
	if (fgets (buffer, MAX_BUFF, stream) != EOF)
	{
		char *rest = NULL ;
		char	*ptr = NULL;

		ptr = (char *) strtok_r (buffer, ":", &rest) ;
		year->year = atoi(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->inflation = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->investmentReturn = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->cashReturn = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->ZRP = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->Pru = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->cash = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->FerrantiIncome = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->SimonIncome = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->stateIncome = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->rentIncome = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->inheritance = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->TaxAllowance = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->Spend = atof(ptr) ;

		year->total = year->ZRP + year->Pru + year->cash ;
		year->cashIncome = (year->cashReturn * 0.01) * year->cash;

		calculateAnnuityInflation(year);
	
		return 0 ;
	}
	return 1 ;
}

int readYear(Year *year)
{
	char	buffer[MAX_BUFF+1] = "" ;
	if (fgets (buffer, MAX_BUFF, stream) != EOF)
	{
		char *rest = NULL ;
		char	*ptr = NULL;

		ptr = (char *) strtok_r (buffer, ":", &rest) ;
		if (!ptr)
			return 1 ;
		year->year = atoi(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->inflation = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->investmentReturn = atof(ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		year->cashReturn = atof(ptr) ;

		calculateAnnuityInflation(year);

		return 0 ;
	}
	return 1 ;
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
	if (lastyear->year == FerrantiYear)
	{
		int yearsEarly = 2029 - lastyear->year ;
		double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
		lastyear->FerrantiIncome = reductionFactor * (lastyear->F1 + lastyear->F2 + lastyear->F3 + lastyear->F4 + lastyear->F5 + lastyear->F6) ;
	}
	if (lastyear->year == SimonYear)
	{
		int yearsEarly = 2029 - lastyear->year ;
		double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
		lastyear->SimonIncome = reductionFactor * (lastyear->S1 + lastyear->S2) ;
	}

	lastyear->income = lastyear->FerrantiIncome + lastyear->SimonIncome + lastyear->cashIncome 
						+ lastyear->ZRPIncome + lastyear->PruIncome;
	if (year->year > InheritYear)
		lastyear->income += lastyear->rentIncome ;

	if (year->year > 2031)
		lastyear->income += lastyear->stateIncome ;

	double taxable = lastyear->income - lastyear->TaxAllowance ;

	// note - assume only half the cashIncome is taxable (ISAs, allowances etc)
	taxable -= (lastyear->cashIncome * 0.5) ;

	// and only 75% of ZRP and Pru taxable if we haven't taken 25% lump sum
	if (!Pru25)
		taxable -= (lastyear->PruIncome * 0.25) ;
	if (!ZRP25)
		taxable -= (lastyear->ZRPIncome * 0.25) ;

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
	year->Pru = ((1.0 + (lastyear->investmentReturn * 0.01)) * lastyear->Pru) - lastyear->PruIncome;
	year->total = year->ZRP + year->Pru + year->cash ;

	if (year->year == ZRPYear)
	{
		if (ZRP25)
		{
			double cashTake = year->ZRP * 0.25 ;
			year->ZRP -= cashTake ;
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

	// take the tax free amount from ZRP and pru
	if (year->year >= ZRPYear && year->year < PruYear)
	{
		year->ZRPIncome = taxFree ;
	}
	else if (year->year >= PruYear && year->year < ZRPYear)
	{
		year->PruIncome = taxFree ;
	}
	else if (year->year >= PruYear && year->year >= ZRPYear)
	{
		year->ZRPIncome = taxFree / 2.0 ;
		year->PruIncome = taxFree / 2.0 ;
	}

	// how much do we still need?
	required -= taxFree ;
	if (required < 0)
		required = 0 ;

	// take the extra required (plus 25% for tax) proportionate to
	// what we have left in each pot

	if (required > 0)
	{
		required *= 1.25;
		if (year->year >= ZRPYear && year->year < PruYear)
		{
			year->ZRPIncome += (year->ZRP / (year->ZRP + year->cash)) * required ;
		}
		else if (year->year >= PruYear && year->year < ZRPYear)
		{
			year->PruIncome += (year->Pru / (year->Pru + year->cash)) * required ;
		}
		else if (year->year >= PruYear && year->year >= ZRPYear)
		{
			year->ZRPIncome += (year->ZRP / year->total) * required ;
			year->PruIncome += (year->Pru / year->total) * required ;
		}
	}

	return 0 ;
}

int setupReturns(Year *year) 
{
	int InflationDiff = (InflationMax - InflationMin) * 10 ;
	int inflation = (InflationDiff > 0) ? random()%InflationDiff : 0;
	year->inflation = (inflation / 10.0) + InflationMin ;

	int InvestDiff = (InvestMax - InvestMin) * 10 ;
	int invest = (InvestDiff > 0) ? random()%InvestDiff : 0;
	year->investmentReturn = (invest / 10.0) + InvestMin ;

	int CashDiff = (CashMax - CashMin) * 10 ;
	int cash = (CashDiff > 0) ? random()%CashDiff : 0;
	year->cashReturn = (cash / 10.0) + CashMin ;
}

int processYear (Year *year, Year *lastyear)
{
	int finished = 0 ;
	
	//readYear(year) ;

	setupReturns(year);
	calculateAnnuityInflation(year);
	calculateNewTotals(year, lastyear);
	calculateNewIncomes(year, lastyear);
	calculateIncome(year);

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

	if (argc != 19)
	{
		printf ("Usage: cash <random seed> <rent> <inherit> <inheritYear> <spend> <ZRPYear> <ZRP25> <PruYear> <Pru25> <FerrantiYear> <SimonYear>\n");
		printf ("                                   <InflationMin> <InflationMax> <InvestMin> <InvestMax> <CashMin> <CashMax> <SpendDecrease>\n");
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

	//readStart(&years[0]) ;
	firstyear = &years[0];
	firstyear->year = 2019;
	firstyear->ZRP = 350620;
	firstyear->Pru = 238277;
	firstyear->cash = 139517;
	firstyear->FerrantiIncome = 0 ;
	firstyear->SimonIncome = 0 ;
	firstyear->stateIncome = 7700 ;
	firstyear->rentIncome = atof(argv[2]) ;
	firstyear->inheritance = atof(argv[3]) ;
	firstyear->TaxAllowance = 12000 ;
	firstyear->Spend = atof(argv[5]) ;
	ZRPYear = atoi (argv[6]) ;
	ZRP25 = atoi (argv[7]) ;
	PruYear = atoi (argv[8]) ;
	Pru25 = atoi (argv[9]) ;
	FerrantiYear = atoi (argv[10]) ;
	SimonYear = atoi (argv[11]) ;
	InflationMin = atof (argv[12]) ;
	InflationMax = atof (argv[13]) ;
	InvestMin = atof (argv[14]) ;
	InvestMax = atof (argv[15]) ;
	CashMin = atof (argv[16]) ;
	CashMax = atof (argv[17]) ;
	SpendDecrease = atof (argv[18]) ;
	firstyear->F1 = 1171;
	firstyear->F2 = 870;
	firstyear->F3 = 313;
	firstyear->F4 = 1639;
	firstyear->F5 = 257;
	firstyear->F6 = 35;
	firstyear->S1 = 975;
	firstyear->S2 = 1044;

	setupReturns (firstyear) ;

	if (firstyear->year == ZRPYear)
	{
		if (ZRP25)
		{
			double cashTake = firstyear->ZRP * 0.25 ;
			firstyear->ZRP -= cashTake ;
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
	firstyear->total = firstyear->ZRP + firstyear->Pru + firstyear->cash ;
	firstyear->cashIncome = (firstyear->cashReturn * 0.01) * firstyear->cash;

	if (firstyear->year == FerrantiYear)
	{
		int yearsEarly = 2029 - firstyear->year ;
		double reductionFactor = (100 - (yearsEarly * 1.5)) * 0.01 ;
		firstyear->FerrantiIncome = reductionFactor * (firstyear->F1 + firstyear->F2 + firstyear->F3 + firstyear->F4 + firstyear->F5 + firstyear->F6) ;
	}
	if (firstyear->year == SimonYear)
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
		years[nextYear].year = nextYear + 2019;
		processYear(&years[nextYear], &years[nextYear-1]);
	}

	/* print out the results */

	if (!html)
	{
		for (loop=0;loop<nextYear-1;loop++)
		{
			printf ("%d, ZRP %d, Pru %d, Cash %d Total %d Tax %d Spend %d\n", years[loop].year, (int) years[loop].ZRP,
														(int) years[loop].Pru, (int) years[loop].cash, (int) years[loop].total,
															(int) years[loop].tax, (int) years[loop].Spend) ;
		}
	}
	else
	{
		printf ("<center><TABLE COLS=10\n border=3>") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>year<center>\n") ;
		printf ("<TD><center>ZRPTotal<center>\n") ;
		printf ("<TD><center>PruTotal<center>\n") ;
		printf ("<TD><center>investReturn<center>\n") ;
		printf ("<TD><center>Cash<center>\n") ;
		printf ("<TD><center>CashReturn<center>\n") ;
		printf ("<TD><center>inflation<center>\n") ;
		printf ("<TD><center>ZRPIncome<center>\n") ;
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
		printf ("<TR>\n") ;
		for (loop=0;loop<nextYear-1;loop++)
		{
			printf ("<TR>\n") ;
			printf ("<TD><center>%d<center>\n", years[loop].year) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].ZRP) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].Pru) ;
			printf ("<TD><center>%.2f<center>\n", years[loop].investmentReturn) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].cash) ;
			printf ("<TD><center>%.2f<center>\n", years[loop].cashReturn) ;
			printf ("<TD><center>%.2f<center>\n", years[loop].inflation) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].ZRPIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].PruIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].cashIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].rentIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].stateIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].FerrantiIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].SimonIncome) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].Spend) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].TaxAllowance) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].tax) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].total) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].inheritance) ;
			printf ("<TD><center>%d<center>\n", (int)years[loop].income) ;
			printf ("<TD><center>%.1f<center>\n", (years[loop].total / years[loop].Spend)) ;
			printf ("</TR>\n") ;
		}
		printf ("</TABLE>\n") ;
	}

	/* close the input stream */

	//fclose (stream) ;
}

