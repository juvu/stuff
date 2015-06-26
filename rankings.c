#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main (int argc , char **argv)
{
	FILE	*pp;
	char 	*rank, *first, *second, *grade, *index, *country, *wins, *games, *percent ;
	char	cmd[1024] ;
	char	buffer[1024] ;

	printf ("<HTML><TABLE CELLPADDING=2 COLS=8 BORDER=1> <TR BGCOLOR=#a8b878> <TD>&nbsp;</TD> <TD>Rank</TD><TD>&nbsp;</TD><TD>Grade</TD><TD>Games</TD><TD>Wins</TD><TD>Index</TD><TD>%%age</TD></TR>\n") ;

	sprintf (cmd, "curl -s http://www.croquetrecords.com/rank/currentworld.htm | grep -i %s", argv[1]) ;
	pp = popen (cmd, "r") ;
	while (fgets (buffer, 1024, pp)) 
	{
		if (strstr (buffer, "<!--"))
			continue ;
		rank = strtok (buffer, " ") ;
		first = strtok (NULL, " ") ;
		second = strtok (NULL, " ") ;
		country = strtok (NULL, " ") ;
		grade = strtok (NULL, " ") ;
		games = strtok (NULL, " ") ;
		wins = strtok (NULL, " ") ;
		index = strtok (NULL, " ") ;
		percent = strtok (NULL, " ") ;
	
		printf ("<TR BGCOLOR=#a878b8>") ;
		printf ("<TD>World</TD><TD>%s</TD><TD>%s %s %s </TD><TD> %s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>\n", rank, first, second, country, grade, games, wins, index, percent) ;
	}
	pclose (pp) ;


	sprintf (cmd, "curl -s http://www.croquetrecords.com/rank/currentuk.htm | grep -i %s", argv[1]) ;
	pp = popen (cmd, "r") ;
	while (fgets (buffer, 1024, pp))
	{
		if (strstr (buffer, "<!--"))
			continue ;
		rank = strtok (buffer, " ") ;
		first = strtok (NULL, " ") ;
		second = strtok (NULL, " ") ;
		country = strtok (NULL, " ") ;
		grade = strtok (NULL, " ") ;
		games = strtok (NULL, " ") ;
		wins = strtok (NULL, " ") ;
		index = strtok (NULL, " ") ;
		percent = strtok (NULL, " ") ;

		printf ("<TR BGCOLOR=#18b8b8>") ;
		printf ("<TD>UK</TD><TD>%s</TD><TD>%s %s %s </TD><TD> %s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>\n", rank, first, second, country, grade, games, wins, index, percent) ;
	}
	pclose (pp) ;
	printf ("</TABLE></HTML\n") ;
}
