#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#define MAXSTRING 32000

void getCoords(char **kmlData) 
{
	char buffer[MAXSTRING+1] = "" ;
	int count = 1 ;

	FILE *fp1 = fopen (*kmlData, "r") ;

	char *ptr = strstr (*kmlData, ".kml") ;

	if (ptr)
		*ptr = '\0' ;

	strcat (*kmlData, ".xxx") ; 

	FILE *fp = fopen (*kmlData, "w") ;
	fputs ("\"longitude\" \"latitude\"", fp) ;
	fputs ("\n", fp) ;

    while (fgets (buffer, MAXSTRING, fp1))
    {
		char *ptr1 = strstr (buffer, "<coordinates>") ;

		if (ptr1)
		{
    		while (fgets (buffer, MAXSTRING, fp1))
    		{
				char *ptr2 = strstr (buffer, "</coordinates>") ;

				if (ptr2)
					break ;

				ptr2 = strstr (buffer, ",") ;
			
				if (ptr2)
				{
					*ptr2 = ' ' ;
					char *ptr3 = strstr (ptr2, ",") ;
					if (ptr3)
						*ptr3 = '\0' ;
				}
				fprintf (fp, "\"%d\" %s\n", count++, buffer) ;
			}
		}
	}

	fclose (fp) ;

	fclose(fp1);
}

