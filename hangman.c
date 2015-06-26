#include <stdio.h>
#include <string.h>

int main (int argc, char ** argv)
{
	int		i ;
	char	*ptr ;
	FILE	*pp ;
	int		forgetit ;
	char	elim[100] ;
	char	search[100] ;
	char	buffer[100] ;
	char	buffer2[100] ;

	printf ("Enter list of characters eliminated\n") ;
	fgets (elim, 100, stdin) ;

	/* get rid of the carriage return*/
	ptr = strchr (elim, '\n') ;
	if (ptr)
		*ptr = '\0' ;

	printf ("Enter search string\n") ;
	fgets (search, 100, stdin) ;

	/* get rid of the carriage return*/
	ptr = strchr (search, '\n') ;
	if (ptr)
		*ptr = '\0' ;

	sprintf (buffer, "grep -i \"^%s$\" /usr/share/dict/words", search) ;
	pp = popen (buffer, "r") ;
	while (fgets (buffer2, 100, pp))
	{
		forgetit = 0 ;
		for (i=0;i<strlen(elim);i++)
		{
			if (strchr (buffer2, elim[i]))
			{
				forgetit = 1 ;
				break ;
			}
		}
		/* check for duplicates */
		for (i=0;i<strlen(search);i++)
		{
			if ((search[i] == '.') && strchr (search, buffer2[i]))
			{
				forgetit = 1 ;
				break ;
			}
		}
		if (!forgetit)
			printf (buffer2) ;
	}
	pclose (pp) ;
}
		

