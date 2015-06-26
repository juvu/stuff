#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 1024

int sortItOut(char *a, char *b)
{
	if (*a < *b)
		return -1 ;
	if (*a > *b)
		return 1 ;

	return 0 ;
}

// returns 0 if they dont match, or the length if they do
compare (char *a, char *b, int len)
{
	int i, j, pos ;
	char	*ptr ;

	int (*sortfn)() = sortItOut ;

	ptr = (char *) strchr (a, (int)'\n') ;
	if (ptr)
		*ptr = '\0' ;

	if (strlen(a) > len)
		return (0) ;

	qsort (a, strlen(a), sizeof(char), sortfn) ;


	pos = 0 ;
	for (i=0;i<strlen(a);i++)
	{
		for (j=pos;j<len;j++)
		{
			pos++ ;
			if (a[i] == b[j])
				break ;
		}
		if (j == len)
			return (0) ;
	}

	return (strlen(a)) ;
}

main (int argc, char **argv)
{
	int		i ;
	FILE	*fp ;
	int		len ;
	int		all = 0 ;
	int		maxlen = -1 ;
	int		minlen = 3 ;
	char letters[MAXSTRING] ;
	char dict[MAXSTRING] ;
	char dictcpy[MAXSTRING] ;
	char reqletters[MAXSTRING] ;
	int (*sortfn)() = sortItOut ;

	if (argc < 2)
	{
		printf ("Usage: letters <list of letters> <printAll>\n");
		exit(0) ;
	}

	strcpy (letters, argv[1]) ;

	if (argc > 2)
	{
		all = 1;
		minlen = atoi (argv[2]) ;
	}

	reqletters[0] = '\0' ;
	if (argc > 3)
	{
		strcpy (reqletters, argv[3]) ;
	}

	qsort (letters, strlen(letters), sizeof(char), sortfn) ;

	fp = fopen ("/usr/share/dict/words", "r") ;
	//fp = fopen ("./words", "r") ;
	if (!fp)
		exit (-1) ;

	while (fgets (dict, MAXSTRING, fp))
	{
		strcpy (dictcpy, dict) ;
		len = compare (dictcpy, letters, strlen(letters)) ;
		if (len && (len >= minlen || len >= 7))
		{
			if (all)
				printf ("%s", dict) ;

			if (len >= maxlen)
			{
				maxlen = len ;	
				if (!all)
					printf ("%s", dict) ;
			}
		}
	}	

	fclose (fp) ;
}
	
