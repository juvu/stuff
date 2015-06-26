#include <stdio.h>
#include <stdlib.h>

// absolute base month is Jan 1996

#define BASE			20
#define BASEDATE		89
#define ALLOCDATE		96

typedef struct stock
{
	int		num ;
	float   price ;
	int		firstVestDate;
	int		firstVestNum;
	int		vestNum ;
	int		vestInterval ;
	int		newNum ;
	struct stock	*next ;
} Stock ;

Stock	*stocks ;
double 	price = BASE ;
double 	allocPrice ;
int		curDate = BASEDATE;

double oldPrice (Stock *stock)
{
	int 		numVested = 0 ;
	int			elapsedMonths ;
	double		val ;

	if (stock->price > price)
		return (0.0) ;

	if (curDate > stock->firstVestDate)
		numVested = stock->firstVestNum ;
	else
		return (0.0) ;

	elapsedMonths = curDate - stock->firstVestDate ;
	numVested += (stock->vestInterval % elapsedMonths) * stock->vestNum ;
	if (numVested > stock->num)
		numVested = stock->num ;
	val = (price - stock->price) * numVested ;
	
	return (val) ;
}

double newPrice (Stock *stock)
{
	int 		numVested = 0 ;
	int			elapsedMonths ;
	double		val ;

	if (curDate < ALLOCDATE)
		return (0.0) ;

	if (allocPrice > price)
		return (0.0) ;

	elapsedMonths = curDate - ALLOCDATE ;
	if (elapsedMonths < 12)
		return (0.0) ;

	if (elapsedMonths < 18)
		numVested = 0.4 * stock->newNum ;
	else if (elapsedMonths < 24)
		numVested = 0.6 * stock->newNum ;
	else if (elapsedMonths < 30)
		numVested = 0.8 * stock->newNum ;
	else
		numVested = stock->newNum ;

	val = (price - allocPrice) * numVested ;
	
	return (val) ;
}

double getPrice (char option)
{
	double total = 0.0 ;
	Stock	*ptr ;

	switch (option)
	{
		case 'A':
				
			for (ptr=stocks;ptr;ptr=ptr->next)
			{
				total += newPrice(ptr) ;
			}
			break ;

		case 'B':
				
			for (ptr=stocks;ptr;ptr=ptr->next)
			{
				if (ptr->price > 12.15)
					total += newPrice(ptr) ;
				else
					total += oldPrice(ptr) ;
			}
			break ;

		case 'C':
				
			for (ptr=stocks;ptr;ptr=ptr->next)
			{
				if (ptr->price > 16.0313)
					total += newPrice(ptr) ;
				else
					total += oldPrice(ptr) ;
			}
			break ;

		case 'D':
				
			for (ptr=stocks;ptr;ptr=ptr->next)
			{
				if (ptr->price > 30.0)
					total += newPrice(ptr) ;
				else
					total += oldPrice(ptr) ;
			}
			break ;

		case 'E':

			for (ptr=stocks;ptr;ptr=ptr->next)
			{
				total += oldPrice(ptr) ;
			}
			break ;
	}

	return (total) ;
}

int addStock (char *buffer)
{
	Stock	*stock ;

	stock = (Stock *) calloc (1, sizeof (struct stock)) ;

	sscanf (buffer, "%d:%f:%d:%d:%d:%d", &stock->num, &stock->price, &stock->firstVestDate,
										&stock->firstVestNum, &stock->vestInterval, &stock->vestNum) ;

	if (stock->price < 24.99)
		stock->newNum = stock->num * (1.0/1.25) ;
	else if (stock->price < 39.99)
		stock->newNum = stock->num * (1.0/2.0) ;
	else if (stock->price < 49.99)
		stock->newNum = stock->num * (1.0/4.0) ;
	else 
		stock->newNum = stock->num * (1.0/6.00) ;

	stock->next = stocks ;
	stocks = stock ;
}


main (int argc, char **argv)
{
	FILE	*fp ;
	double	rate ;
	int		i ;
	double	valA, valB, valC, valD, valE ;
	char	buffer[1024] ;

	rate = atof (argv[2]) ;
	rate *= 0.01 ; 
	rate /= 12.0 ;
	rate += 1.0 ;          // a monthly rate

	fp = fopen (argv[1], "r") ;
	while (fgets(buffer, 1024, fp))
	{
		addStock (buffer) ;
	}

	for (i=0;curDate<ALLOCDATE;i++)
	{
		price *= rate ;
		curDate++ ;

		valA = getPrice ('A') ;
		valB = getPrice ('B') ;
		valC = getPrice ('C') ;
		valD = getPrice ('D') ;
		valE = getPrice ('E') ;

		printf ("%f\t%f\t%f\t%f\t%f\n", valA, valB, valC, valD, valE) ;
	}
	allocPrice = price ;
	printf ("ALLOC DATE Share Price is %f **********************************\n", allocPrice) ;

	for (i=0;i<60;i++)
	{
		price *= rate ;
		curDate++ ;

		valA = getPrice ('A') ;
		valB = getPrice ('B') ;
		valC = getPrice ('C') ;
		valD = getPrice ('D') ;
		valE = getPrice ('E') ;

		printf ("%f\t%f\t%f\t%f\t%f\n", valA, valB, valC, valD, valE) ;
	}
}
		
