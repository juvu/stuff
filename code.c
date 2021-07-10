#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// asdf
main (int argc, char **argv)
{
	int 	i ;
	int 	n ;
	FILE	*fp ;
	char	*seed ;
	char buff[19] ;
	int	newchar, achar ;
/*
	if (argc != 2)
	{
		printf ("Usage: code <filename>\n") ;
		exit (0) ;
	}

	fp = fopen (argv[1], "r") ;
	if (!fp)
	{
		printf ("Could not open file\n") ;
		exit (-1) ;
	}
*/

	seed = getpass ("Enter seed value") ;
	srandom (atoi (seed)) ;

	while((n = read(STDIN_FILENO, buff, 1)) > 0)
	{
		newchar = buff[0] + (random()%20) ;
		printf ("%c", newchar) ;
	}
	//fclose (fp) ;
}
