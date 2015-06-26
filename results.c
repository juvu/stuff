#include <stdio.h>

main (int argc , char **argv)
{
	FILE	*pp;
	char 	*id, *date, *opponent, *handicap, *newindex, *game, *result, *score, *tournament ;
	char	cmd[1024] ;
	char	buffer[1024] ;

	printf ("<HTML><TABLE CELLPADDING=2 COLS=8 BORDER=1> <TR BGCOLOR=#a8b878> <TD>ID</TD> <TD>Date</TD><TD>Opponent</TD><TD>Handicap</TD><TD>Game(L/H)</TD><TD>Result(W/L)</TD><TD>Score</TD><TD>NewIndex</TD><TD>Tournament</TD></TR>\n") ;

	if (argc == 1)
		sprintf (cmd, "/usr/local/bin/results") ;
	else
		sprintf (cmd, "/usr/local/bin/results %s", argv[1]) ;
	pp = popen (cmd, "r") ;

	// read the first line and throw away 
	fgets (buffer, 1024, pp) ;

	// now read the results
	while (fgets (buffer, 1024, pp)) 
	{
		id = strtok (buffer, "\t") ;
		date = strtok (NULL, "\t") ;
		opponent = strtok (NULL, "\t") ;
		handicap = strtok (NULL, "\t") ;
		game = strtok (NULL, "\t") ;
		result = strtok (NULL, "\t") ;
		score = strtok (NULL, "\t") ;
		newindex = strtok (NULL, "\t") ;
		tournament = strtok (NULL, "\t") ;
	
		printf ("<TR BGCOLOR=#a878b8>") ;
		printf ("<TD> %s</TD><TD> %s</TD><TD> %s</TD><TD> %s</TD><TD> %s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>\n", id, date, opponent, handicap, game, result, score, newindex, tournament) ;
	}
	pclose (pp) ;

	printf ("</TABLE></HTML\n") ;
}
