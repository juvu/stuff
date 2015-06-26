#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

#define NUMVOWELS 		5

#define NUMLETTERS 		9
#define NUMNUMS			6

#define NUMLARGE		4
#define NUMSMALL		9

#define LETTERS			1
#define NUMBERS			2

#define COUNTDOWN_TIME	30

#define MAXSTRING		1024

static char letter[NUMLETTERS+1] ;
static int 	num[NUMNUMS+1] ;

int large[] = {25, 50, 75, 100} ;
int small[] = {1, 2, 3, 4, 5, 6, 7, 8, 9} ;

char consonants[] ={'b', 'b', 
					'c', 'c', 'c', 
					'd', 'd', 'd', 'd', 'd',
					'f', 'f', 'f', 
					'g', 'g', 'g', 
					'h', 'h', 'h', 'h', 'h', 'h', 'h', 'h',
					'j', 
					'k', 
					'l', 'l', 'l', 'l', 
					'm', 'm', 'm', 
					'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n',   
					'p', 'p', 
					'q', 
					'r', 'r', 'r', 'r', 'r', 'r', 'r', 
					's', 's', 's', 's', 's', 's', 's',  
					't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 
					'v', 
					'w', 'w', 'w', 
					'x', 
					'y', 'y', 
					'z' } ;

char vowels[] ={'a', 'a', 'a', 'a', 'a', 
				'e', 'e', 'e', 'e', 'e', 'e', 'e', 
				'i', 'i', 'i', 'i',
				'o', 'o', 'o', 'o',
				'u', 'u',
				} ;

int main (int argc, char **argv)
{
	struct timeval	tv ;
	struct timezone	tz ;

	int	type ;
	int	retval ;

	int 	i, j ;
	int		target ;
	int		tleft ;

	char	buffer[MAXSTRING] ;

    gettimeofday (&tv, &tz) ;
    srandom(tv.tv_usec) ;

	while (1)
	{
		printf ("Letters (L), or Numbers (N)\n") ;
		gets(buffer) ;
		if (buffer[0] == 'l' || buffer[0] == 'L')
		{
			type = LETTERS ;
			for (i=0;i<NUMLETTERS;i++)
			{
				printf ("Consonant (C) or Vowel (V)\n") ;
				gets(buffer) ;
				if (buffer[0] == 'C' || buffer[0] == 'c')
					letter[i] = consonants [random() % sizeof(consonants)] ;
				else
					letter[i] = vowels [random() % sizeof(vowels)] ;

				for (j=0 ; j<=i ;j++) 
				{
					printf ("%c", letter[j]) ;
				}
				printf ("\n") ;
			}
		}
		else
		{
			type = NUMBERS ;
			for (i=0;i<NUMNUMS;i++)
			{
				printf ("Large (L) or Small (S)\n") ;
				gets(buffer) ;
				if (buffer[0] == 'L' || buffer[0] == 'l')
					num[i] = large [random() % NUMLARGE] ;
				else
					num[i] = small [random() % NUMSMALL] ;

				for (j=0 ; j<=i ;j++) 
				{
					printf ("%d ", num[j]) ;
				}
				printf ("\n") ;
			}

			target = random() % 898 + 101;
			printf ("And the target is %d\n", target) ;
		}
		printf ("Clock's running\n") ;
		tleft = COUNTDOWN_TIME ;
		printf ("Hit return for solution\n") ;
		while (1)
		{
			fd_set 	rfds;
			struct timeval tv;
			int retval;
	
			/* Watch stdin (fd 0) to see when it has input. */
			FD_ZERO(&rfds);
			FD_SET(0, &rfds);
			/* Wait up to one second. */
			tv.tv_sec = 1;
			tv.tv_usec = 0;

			retval = select(1, &rfds, NULL, NULL, &tv);
			if (retval)
			{
				gets(buffer) ;
				tleft = 1 ;
				if (type == LETTERS)
				{
					sprintf (buffer, "letters %s", letter) ;
					system (buffer) ;
				}
				else
				{
					strcpy (buffer, "numbers ") ;
					for (i=0;i<NUMNUMS;i++)
					{
						sprintf  (buffer, "%s %d ", buffer, num[i]) ;
					}
					sprintf  (buffer, "%s %d ", buffer, target) ;
					printf ("Command %s\n", buffer) ;
					system (buffer) ;
				}
				break ;
			}
			tleft-- ;
			if (tleft >= 0)
				printf ("%d seconds\n", tleft) ;
			else if (tleft == -1)
				printf ("Out of time \n") ;
		}
	}
}

