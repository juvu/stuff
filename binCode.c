#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

char letters[26] = {'A', 'B', 'C', 'D', 'E',
					'F', 'G', 'H', 'I', 'J',
					'K', 'L', 'M', 'N', 'O',
					'P', 'Q', 'R', 'S', 'T',
					'U', 'V', 'W', 'X', 'Y', 'Z'} ;

static char result1[1024] = "" ;
static char result2[1024] = "" ;

main (int argc, char **argv)
{
	FILE *stream = fopen ("CodeData", "r") ;
	int i = 0;
	int done = 0 ;

	char    buffer[1024] = "" ;
	char    tmpbuf[1024] = "" ;
	char    numBuffer[1024] = "" ;
	int mode = atoi (argv[1]) ;

	for (i=0;!done;i++)
	{
		if (fgets (buffer, 1024, stream))
		{
			char *rest = NULL ;
			char *ptr = NULL;
			long val = 0 ;
			int j = 0 ;
  
			for (j=0;j<2;j++)
			{
				if (j == 0)
					ptr = (char *) strtok_r (buffer, " ", &rest) ;
				else
					ptr = (char *) strtok_r (NULL, " ", &rest) ;

				if (ptr)
					val = strtol (ptr, NULL, 2) ;
				//printf ("val is %d\n", val) ;
	
				if (ptr && val < 16)
				{
					if (val >= 12)
						val -= 12 ;

					ptr = (char *) strtok_r (NULL, " ", &rest) ;
					if (val)
					{
						strncpy (numBuffer, &ptr[12 - val], val) ;
						strncpy (&numBuffer[val], ptr, 12-val) ;
					}
					else
						strncpy (numBuffer, ptr, 12) ;
				
					//printf ("numBuffer is %s\n", numBuffer) ;	
	
					val = strtol (numBuffer, NULL, 2) ;
					//printf ("val is %d\n", val) ;	
	
					double dnum = val / 26.0 ;
					int num1 = (int) dnum ;
					double dnum2 = ((double) (dnum - num1) * 26.0) + 0.1;
					int num2 = (int) dnum2 ;
					sprintf (tmpbuf, "%c%c", letters[num2-1], letters[num1-1]) ;
					if (j == 0)
						strcat (result1, tmpbuf) ;
					else
						strcat (result2, tmpbuf) ;

					if (mode == 1) 
						printf ("%c%c ", letters[num1-1], letters[num2-1]) ;
					else if (mode == 2) 
						printf ("%c%c ", letters[num2-1], letters[num1-1]) ;
					else if (mode == 3) 
						printf ("%c%c", letters[num1-1], letters[num2-1]) ;
					else if (mode == 4) 
						printf ("%c%c", letters[num2-1], letters[num1-1]) ;
				}
			}
			if (mode < 3)
				printf ("\n") ;
		}
		else
			done = 1 ;
	}
	
	printf ("\n\n %s%s \n\n", result1, result2) ;
					
	fclose (stream) ;
}

	
