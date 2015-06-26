#include <stdio.h>

main (int argc, char **argv)
{
	printf ("Hello\n") ;
	printf ("Annabelle\n") ;
	printf ("I am also a Guinea pig\n") ;

	int i = 0 ;
	int j = 0 ;

	int total = 1 ;

	for (i=2; i<1000; i++)
	{
		printf (argv[1]) ;
		printf (" are C") ;
		for (j=0;j<i;j++)
		{
			printf ("O") ;
		}
		printf ("L!\n") ;

		total += i ;
	}

	int x = 6000537 + 1 ;
	printf ("ANSWER IS %d\n", x) ;
}

