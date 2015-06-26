#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NUMNUMS	6

#define	ADD	0
#define SUB 1
#define MULT 2
#define DIV 3

#define MAXSTRING	1024

int num[NUMNUMS] ;
int copy[NUMNUMS] ;
int sort[NUMNUMS] ;
int	target;
char desc[MAXSTRING+1] ;

shuffleSort(int level)
{
	int i,pos,tmp ;

	for (i=0;i<level; i++)
	{
		sort[i] = i ;
	}

	for (i=0;i<level; i++)
	{
		pos = random() % level ;
		tmp = sort[i] ;
		sort[i] = sort[pos] ;
		sort[pos] = tmp ;
	}
}

setupCopy (int aloop, int pos1, int pos2, int val)
{
	int curpos = 0 ;
	int	i ;

	for (i=0;i<aloop;i++)
	{
		if (i == pos1 || i == pos2)
			continue ;
		copy[curpos] = copy[i] ;
		curpos++ ;
	}
	copy[curpos] = val ;
}

int main (int argc, char **argv)
{
	int i, aloop, val ;
	int	total, totsav, oldTotal;
	int	diff ;
	int nearest;
	int numtries;
	struct timeval	tv ;
	struct timezone	tz ;
	char descsav[MAXSTRING+1] ;

	if (argc < 8)
	{
		printf ("Usage: numbers <num1> <num2> <num3> <num4> <num5> <num6> <target>\n") ;
		exit (0) ;
	}

	gettimeofday (&tv, &tz) ;
	srandom(tv.tv_usec) ;

	for (i=0;i<NUMNUMS;i++)
	{
		num[i] = atoi (argv[i+1]) ;
	}
	target = atoi (argv[NUMNUMS + 1]);

	nearest = 10000 ;
	numtries = 0 ;

	while (1)
	{
		for (i=0;i<NUMNUMS;i++)
		{
			copy[i] = num[i] ;
		}

		desc[0] = '\0' ;
		for (aloop=NUMNUMS; aloop > 1; aloop--)
		{
			shuffleSort (aloop) ;
			val = operation (sort[0], sort[1]) ;
			diff = abs (val - target) ;	
			if ((val > 0) && diff < nearest)
			{
				nearest = diff ;
				strcpy (descsav, desc) ;
				totsav = val ;
				if (!diff)
				{
					printf ("GOT IT!!(%d tries)		%s \n", numtries, desc) ;
					exit(0)  ;
				}

			}
			setupCopy (aloop, sort[0], sort[1], val) ;
		}

		numtries++ ;

		if (numtries > 1000000)
		{
			printf ("NEAREST %d - %d away		%s 	\n", totsav, abs(totsav - target), descsav) ;
			exit(0) ;
		}
	}
}

int operation (int pos1, int pos2)
{
	int	icalc ;
	int	op ;

	while (1)
	{
		op = random() % 31;
		if (op < 10)
		{
			icalc = copy[pos1] + copy[pos2] ;
			sprintf (desc, "%s\n %d + %d = %d", desc, copy[pos1], copy[pos2], icalc) ;
			return (icalc) ;
		}
		else if (op < 20)
		{
			if (copy[pos1] > copy[pos2])
			{
				icalc = copy[pos1] - copy[pos2] ;
				sprintf (desc, "%s\n %d - %d = %d", desc, copy[pos1], copy[pos2], icalc) ;
				return (icalc) ;
			}
			else
			{
				icalc = copy[pos2] - copy[pos1] ;
				sprintf (desc, "%s\n %d - %d = %d", desc, copy[pos2], copy[pos1], icalc) ;
				return (icalc) ;
			}
		}
		else if (op < 30)
		{
			// multiplying by 1 is pointless
			if (copy[pos1] == 1 || copy[pos2] == 1)
				continue ;
			icalc = copy[pos1] * copy[pos2] ;
			sprintf (desc, "%s\n %d * %d = %d", desc, copy[pos1], copy[pos2], icalc) ;
			return (icalc) ;
		}
		else 
		{
			double dtot ;
			double dnum ;
			double dcalc ;
					
			// dividing  by 1 is pointless
			if (copy[pos1] == 1 || copy[pos2] == 1)
				continue ;

			dtot = (double) copy[pos1] ;
			dnum = (double) copy[pos2] ;
	
			dcalc = dtot / dnum ;
			icalc = (int) dcalc ;
			if (dcalc == (double) icalc)
			{
				sprintf (desc, "%s\n %d / %d = %d", desc, copy[pos1], copy[pos2], icalc) ;
				return (icalc) ;
			}
		}
	}
}
	
	

	
