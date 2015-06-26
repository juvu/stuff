#include <stdio.h>

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
compare (char *a, char *b, int len, char compulsary, int minlen)
{
	int i, j, pos ;
	char	*ptr ;

	int (*sortfn)() = sortItOut ;

	ptr = (char *) strchr (a, compulsary) ;
	if (!ptr)
		return (0) ;

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

	if (strlen(a) < minlen)
		return (0) ;

	return (strlen(a)) ;
}

main (int argc, char **argv)
{
	int 	total = 0 ;
	FILE	*fp ;
	int		len ;
	int		minlen = 4 ;
	char letters[MAXSTRING] ;
	char dict[MAXSTRING] ;
	char dictcpy[MAXSTRING] ;
	char reqletters[MAXSTRING] ;
	int (*sortfn)() = sortItOut ;

	if (argc < 3)
	{
		printf ("Usage: target <list of letters> <compulsory letter> <minLength>\n");
		exit(0) ;
	}

	strcpy (letters, argv[1]) ;
	char mandatory = *argv[2] ;

	if (argc == 4)
		minlen = atoi (argv[3]) ;

	qsort (letters, strlen(letters), sizeof(char), sortfn) ;

	//fp = fopen ("/usr/share/dict/words", "r") ;
	fp = fopen ("./words", "r") ;
	if (!fp)
		exit (-1) ;

	while (fgets (dict, MAXSTRING, fp))
	{
		strcpy (dictcpy, dict) ;
		len = compare (dictcpy, letters, strlen(letters), mandatory, minlen) ;
		if (len)
		{
			total++;
			printf ("%s", dict) ;
		}
	}	

	printf ("Total words: %d\n", total) ;

	fclose (fp) ;
}
	
