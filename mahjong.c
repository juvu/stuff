#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>

#define EAST	0
#define	SOUTH	1
#define	WEST	2
#define	NORTH	3

#define CHARACTERS 	1
#define BAMBOO 		2
#define CIRCLES 	4
#define DRAGONS 	8
#define WINDS	 	16

static int	score[4] ;
static int	adjScore[4] ;
static int	doubles[4] = {1,1,1,1} ;
static int  suits[4] ;
static int  chows[4] ;
static int  numOneNine[4] ;
static int  numTwoEight[4] ;
static char	*winds[] = {"East","South","West","North"} ;


#define MAX_FIELDS	100

/* break down the individual fields of the hpi command line */
char **getFields(char *str, char separator)
{
    char    **Fields ;
    char    *strfield ;
    char    *field, *ptr ;
    int     breakout ;
    int     i ;

	if (!str)
		return (NULL);

	if (str[0] == '#')
		return (NULL) ;

    /* remove the trailing carriage return */

    ptr = strrchr (str, '\n') ;
    if (ptr)
        *ptr = '\0' ;

    Fields = (char **) calloc (1, MAX_FIELDS * sizeof (char *)) ;
    if (!Fields)
        return NULL;

    for (strfield=str, breakout=0, i=0; i<MAX_FIELDS; i++)
    {
        ptr = strchr (strfield, separator) ;
        if (!ptr)
            breakout = 1 ;
        else
            *ptr = '\0' ;

        field = (char *) calloc (1, (strlen (strfield) + 1) * sizeof (char)) ;
        if (!field)
            return NULL;
        
        strncpy (field, strfield, strlen (strfield)) ;
        Fields[i] = field ;

        if (breakout)
            break ;

        ptr++ ;
		strfield = ptr ;
    }

    return (Fields) ;
}

void freeFields (char **fields)
{
    int i ;

    for (i=0; fields[i]; i++)
    {
        free (fields[i]) ;
    }
    free (fields) ;
}

char    *getField (char **fields, char *fieldsel)
{
    int i, len ;

    if (!fields || !fieldsel)
        return (NULL) ;

    len = strlen(fieldsel) ;
    for (i=0; fields[i]; i++)
    {
        if (!strncasecmp (fields[i], fieldsel, len))
            return (&fields[i][len+1]) ;
    }
    return (NULL) ;
}

int getWind (char *wind)
{
	if (!wind)
		return (EAST);
    if (!strncasecmp (wind, "w", 1))
        return (WEST) ;
    if (!strncasecmp (wind, "e", 1))
        return (EAST) ;
    if (!strncasecmp (wind, "n", 1))
        return (NORTH) ;
    if (!strncasecmp (wind, "s", 1))
        return (SOUTH) ;

	return (EAST) ;
}

int getType (char *type)
{
	if (!type)
		return (0) ;

	if (!strncasecmp (type, "char", 4))
		return (CHARACTERS) ;
	else if (!strncasecmp (type, "bamboo", 6))
		return (BAMBOO) ;
	else if (!strncasecmp (type, "circle", 6))
		return (CIRCLES) ;
	else if (!strncasecmp (type, "dragon", 6))
		return (DRAGONS) ;
	else if (!strncasecmp (type, "wind", 4))
		return (WINDS) ;

	return (0) ;
}

int main (int argc, char ** argv)
{
	int		winner ;
	int		windOfRound ;
	int		wind, thewind ;
	int		num ;
	int		number, i, j ;
	int		numSeasonFlowers ;
	char	**fields ;
	FILE	*fp ;
	char	buffer[200] ;


	windOfRound = getWind (argv[1]) ;

	fp = fopen (argv[2], "r") ;
	
	while (fgets (buffer, 200, fp))
	{
		fields = getFields (buffer, ':') ;
		if (!fields)
			continue ;

		wind = getWind(fields[0]) ;

		if (!strcasecmp (fields[1], "exposed"))
		{
			suits[wind] |= getType (fields[3]) ;

			if (!strcasecmp (fields[2], "pung"))
			{
				if (!strcasecmp (fields[3], "dragon"))
				{
					score[wind]+=4 ;
					doubles[wind]*=2 ;
				}
				else if (!strcasecmp (fields[3], "wind"))
				{
					score[wind]+=4 ;
					thewind = getWind (fields[3]) ; 
					if ((thewind == wind) || (thewind == windOfRound))
						doubles[wind] *=2 ;
				}
				else
				{
					num=atoi(fields[4]) ;
					if ((num == 1) || (num == 9))
					{
						score[wind]+=4 ;
						numOneNine[wind]++ ;
					}
					else
					{
						score[wind]+=2 ;
						numTwoEight[wind]++ ;
					}
				}
			}
			else if (!strcasecmp (fields[2], "kong"))
			{
				if (!strcasecmp (fields[3], "dragon"))
				{
					score[wind]+=16 ;
					doubles[wind]*=2 ;
				}
				else if (!strcasecmp (fields[3], "wind"))
				{
					score[wind]+=16 ;
					thewind = getWind (fields[3]) ; 
					if ((thewind == wind) || (thewind == windOfRound))
						doubles[wind] *=2 ;
				}
				else
				{
					num=atoi(fields[4]) ;
					if ((num == 1) || (num == 9))
					{
						score[wind]+=16 ;
						numOneNine[wind]++ ;
					}
					else
					{
						score[wind]+=8 ;
						numTwoEight[wind]++ ;
					}
				}
			}
		}
		else if (!strcasecmp (fields[1], "concealed"))
		{
			suits[wind] |= getType (fields[3]) ;

			if (!strcasecmp (fields[2], "pung"))
			{
				if (!strcasecmp (fields[3], "dragon"))
				{
					score[wind]+=8 ;
					doubles[wind]*=2 ;
				}
				else if (!strcasecmp (fields[3], "wind"))
				{
					score[wind]+=8 ;
					thewind = getWind (fields[3]) ; 
					if ((thewind == wind) || (thewind == windOfRound))
						doubles[wind] *=2 ;
				}
				else
				{
					num=atoi(fields[4]) ;
					if ((num == 1) || (num == 9))
					{
						score[wind]+=8 ;
						numOneNine[wind]++ ;
					}
					else
					{
						score[wind]+=4 ;
						numTwoEight[wind]++ ;
					}
				}
			}
			else if (!strcasecmp (fields[2], "kong"))
			{
				if (!strcasecmp (fields[3], "dragon"))
				{
					score[wind]+=32 ;
					doubles[wind]*=2 ;
				}
				else if (!strcasecmp (fields[3], "wind"))
				{
					score[wind]+=32 ;
					thewind = getWind (fields[3]) ; 
					if ((thewind == wind) || (thewind == windOfRound))
						doubles[wind] *=2 ;
				}
				else
				{
					num=atoi(fields[4]) ;
					if ((num == 1) || (num == 9))
					{
						score[wind]+=32 ;
						numOneNine[wind]++ ;
					}
					else
					{
						score[wind]+=16 ;
						numTwoEight[wind]++ ;
					}
				}
			}
		}
		else if (!strcasecmp (fields[1], "pair"))
		{
			suits[wind] |= getType (fields[2]) ;

			if (!strcasecmp (fields[2], "dragon"))
				score[wind]+=2 ;
			if (!strcasecmp (fields[2], "wind"))
			{
				thewind = getWind (fields[3]) ; 
				if ((thewind == wind) || (thewind == windOfRound))
					 score[wind]+=2 ;
			}
		}
		else if (!strcasecmp (fields[1], "chow"))
		{
			chows[wind]++ ;
			suits[wind] |= getType (fields[2]) ;

			for (i=3;fields[i];i++)
			{
				num=atoi(fields[i]) ;
				if ((num == 1) || (num == 9))
					numOneNine[wind]++ ;
				else
					numTwoEight[wind]++ ;
			}
		}
		else if (!strcasecmp (fields[1], "other"))
		{
			suits[wind] |= getType (fields[2]) ;

			for (i=3;fields[i];i++)
			{
				num=atoi(fields[i]) ;
				if ((num == 1) || (num == 9))
					numOneNine[wind]++ ;
				else
					numTwoEight[wind]++ ;
			}
		}
		else if (!strncasecmp (fields[1], "season", 6))
		{
			for (i=2;i<6 && fields[i];i++)
			{
				score[wind]+=4;
				num=atoi(fields[i]) ;
				if ((num-1) == wind)
					doubles[wind]*=2;
			}
			if (i == 6)
				doubles[wind]*=8;
		}
		else if (!strncasecmp (fields[1], "flower", 6))
		{
			for (i=2;i<6 && fields[i];i++)
			{
				score[wind]+=4;
				num=atoi(fields[i]) ;
				if ((num-1) == wind)
					doubles[wind]*=2;
			}
			if (i == 6)
				doubles[wind]*=8;
		}
		else if (!strncasecmp (fields[1], "mahjong", 7))
		{
			winner = wind ;

			if (!score[wind])
				score[wind] += 10 ; /* ten points for no scoring value in hand */
	
			score[wind] += 20 ;  /* for mahjong */

			if (suits[wind]&DRAGONS || suits[wind]&WINDS)
			{
				if ((!(suits[wind]&CHARACTERS)) && (!(suits[wind]&BAMBOO))
						&& (!(suits[wind]&CIRCLES)))
				{
					doubles[wind] *= 8; /* just winds and dragons */
				}
				else if ((!(suits[wind]&CHARACTERS)) && (!(suits[wind]&BAMBOO)))
					doubles[wind] *= 2;
				else if ((!(suits[wind]&CHARACTERS)) && (!(suits[wind]&CIRCLES)))
					doubles[wind] *= 2;
				else if ((!(suits[wind]&BAMBOO)) && (!(suits[wind]&CIRCLES)))
					doubles[wind] *= 2;
			}
			else if ((suits[wind] == CHARACTERS) || (suits[wind] == BAMBOO)
						|| (suits[wind] == CIRCLES))
			{
				doubles[wind] *= 8 ; /* just one type */
			}

			if (!numTwoEight[wind])
				doubles[wind] *= 2 ;
			
			if (!chows[wind])
				score[wind] += 10 ;	

			for (i=2;fields[i];i++)
			{
				if (!strncasecmp (fields[i], "snatch", 6))
					doubles[wind] *=2 ;
				if (!strncasecmp (fields[i], "origin", 6))
					doubles[wind] *=8 ;
				if (!strncasecmp (fields[i], "winwall", 7))
					score[wind] += 2 ;
				if (!strncasecmp (fields[i], "winposs", 7))
					score[wind] += 2 ;
				if (!strncasecmp (fields[i], "winlast", 7))
					score[wind] += 10 ;
				if (!strncasecmp (fields[i], "winloose", 8))
					score[wind] += 10 ;
				if (!strncasecmp (fields[i], "standing", 8))
					score[wind] += 100 ;
			}
		}
	}
	fclose (fp) ;

	for (i=0;i<4;i++)
	{
		score[i] *= doubles[i] ;
	}

	printf ("Raw scores are as follows - North %d, South %d, East %d, West %d\n",
						score[NORTH], score[SOUTH], score[EAST], score [WEST]) ;

	/* now work out this complicated score settlement business */

	for (i=0;i<4;i++)
	{
		if (i == winner)
		{
			adjScore[i] = 3 * score[i] ;
			if (i == EAST)
				adjScore[i] *= 2 ;
		}
		else
		{
			adjScore[i] = score[i] ;
			for (j=0;j<4;j++)
			{
				if (j == i)
					continue ;
				if (j == winner)
					continue ;
				if (score[i] > score[j])
					adjScore[i] += (score[i] - score[j]) ;
			}
		}
	}

	printf ("Settled scores are as follows - North %d, South %d, East %d, West %d\n",
						adjScore[NORTH], adjScore[SOUTH], adjScore[EAST], adjScore [WEST]) ;

}
	
