#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

#define MAXSTRING 32000

void getCoords(char **kmlData, double *minLat, double *maxLat, double *minLong, double *maxLong) 
{
	char buffer[MAXSTRING+1] = "" ;
	char buffer2[MAXSTRING+1] = "" ;
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
		char *point = strstr (buffer, "<Point>") ;
		if (point)
		{
       		while (fgets (buffer, MAXSTRING, fp1))
       		{
				char *pointBreak = strstr (buffer, "</Point>") ;

				if (pointBreak)
					break ;

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

            				char *rest = NULL ;
            				char *ptr4= NULL;

							strcpy (buffer2, buffer) ;

            				ptr4 = (char *) strtok_r (buffer2, " ", &rest) ;
							double longitude = atof(ptr4) ;
							if (longitude < *minLong || longitude > *maxLong)
								continue ;
            				ptr4 = (char *) strtok_r (NULL, " ", &rest) ;
							double latitude = atof(ptr4) ;
							if (latitude < *minLat || latitude > *maxLat)
								continue ;

							fprintf (fp, "\"%d\" %s\n", count++, buffer) ;
						}
					}
				}
			}
		}
	}

	fclose (fp) ;

	fclose(fp1);
}

main (int argc, char **argv)
{
	double minLat = 51.49;
	double maxLat = 51.54;
	double minLong = -0.16;
	double maxLong = -0.07 ;
	getCoords (&argv[1], &minLat, &maxLat, &minLong, &maxLong) ;
}
