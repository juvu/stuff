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
	double Spend ;
	double TaxAllowance ;
	double total ;
} Year ;

static FILE	*stream = NULL;
static Year	years[100] ;

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
	year->Spend = (1.0 + (lastyear->inflation * 0.01)) * lastyear->Spend ;
}

calculateNewTotals (Year *year, Year *lastyear)
{
	double income = lastyear->FerrantiIncome + lastyear->SimonIncome + lastyear->cashIncome 
						+ lastyear->ZRPIncome + lastyear->PruIncome;
	if (year->year > 2025)
		income += lastyear->rentIncome ;
	if (year->year > 2031)
		income += lastyear->stateIncome ;

	double taxable = income - lastyear->TaxAllowance ;

	double tax = 0 ;
	if (taxable > 0)
		tax = taxable * 0.2 ;

	// note - this may well be negative
	double cashSpend = lastyear->Spend + tax - income ;

	year->cash = lastyear->cash - cashSpend ;
	year->cashIncome = (year->cashReturn * 0.01) * year->cash;
	year->ZRP = ((1.0 + (lastyear->investmentReturn * 0.01)) * lastyear->ZRP) - lastyear->ZRPIncome;
	year->Pru = ((1.0 + (lastyear->investmentReturn * 0.01)) * lastyear->Pru) - lastyear->PruIncome;
	year->total = year->ZRP + year->Pru + year->cash ;
}

int calculateIncome (Year *year)
{
	double income = year->FerrantiIncome + year->SimonIncome + year->cashIncome ;
	if (year->year > 2025)
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
	if (year->year <= 2024)
	{
		year->ZRPIncome = taxFree ;
	}
	else
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
		if (year->year <= 2024)
		{
			year->ZRPIncome += (year->ZRP / (year->ZRP + year->cash)) * required ;
		}
		else
		{
			year->ZRPIncome += (year->ZRP / year->total) * required ;
			year->PruIncome += (year->Pru / year->total) * required ;
		}
	}

	return 0 ;
}

int processYear (Year *year, Year *lastyear)
{
	int finished = 0 ;
	
	finished = readYear(year) ;
	if (finished)
		return 1;

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
	int html = 0 ;
	int loop = 0 ;

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

	// read in starting amounts

	readStart(&years[0]) ;
	calculateIncome(&years[0]);

	// now process every year

	while (finished != 1)
	{
		nextYear++;
		finished = processYear(&years[nextYear], &years[nextYear-1]);
	}

	/* print out the results */

	for (loop=0;loop<nextYear;loop++)
	{
		printf ("%d, ZRP %f, Pru %f, Cash %f Total %f\n", years[loop].year, years[loop].ZRP,
														years[loop].Pru, years[loop].cash, years[loop].total) ;
	}

#if 0
	if (html)
	{
		printf ("<center><TABLE COLS=%d\n border=3>", level+2) ;
		printf ("<TR><TD><center>Player<center></TD><TD><center>Ranking</center></TD>\n") ;
		for (loop2=level-1;loop2>=0;loop2--)
		{
			if (loop2 > 2)
				printf ("<TD><center>Round %d</center></TD>\n", level - loop2) ;
			else if (loop2 == 2)
				printf ("<TD><center>Quarter Final</center></TD>\n") ;
			else if (loop2 == 1)
				printf ("<TD><center>Semi Final</center></TD>\n") ;
			else if (loop2 == 0)
				printf ("<TD><center>Final</center></TD>\n") ;
		}
		printf ("</TR>\n") ;
		for (loop=0;loop<no_of_players;loop++)
		{
			printf ("<TR><TD><center>%s</center></TD><TD><center>%d</center></TD>\n", player[loop].name, player[loop].ranking) ;
			for (loop2=level-1;loop2>=0;loop2--)
			{
				double odds ;
				odds = player[loop].prob_level[loop2] * 100.0 ;
				printf ("<TD><center>%2.2f</center></TD>\n", odds) ;
			}
			printf ("</TR>\n") ;
		}
		printf ("</TABLE>\n") ;
	}
	else
	{
		for (loop=0;loop<no_of_players;loop++)
		{
			sprintf (result, "%s", player[loop].name) ;
	
			for (loop2=level-1;loop2>=0;loop2--)
			{
				sprintf (pr, "%2.2f\t", player[loop].prob_level[loop2] * 100.0) ;	
				strcat (result, pr) ;
			}

			total += player[loop].prob_level[0] ;	
			strcat (result, "\n") ;
			printf (result) ;
		}
		printf ("total %f\n", total) ;
	}
#endif

	/* close the input stream */

	fclose (stream) ;
}

