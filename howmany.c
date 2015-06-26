#include <stdio.h>

int main (int argc, char **argv)
{
	int loop = 0 ;
	int loop2 = 0 ;
	int twoFound = 0 ;
	int twoNums = 0 ;
	char buffer[256] = "" ;

	int maxNum = atoi (argv[1]) ;

	for (loop=1; loop<=maxNum;loop++)
	{
		sprintf (buffer, "%d", loop) ;
		if (strstr (buffer, "2"))
		{
			twoNums++ ;
			//printf ("%d\n", loop) ;
		}

		int len = strlen(buffer) ;
		for (loop2=0;loop2<len;loop2++)
		{
			if (buffer[loop2] == '2')
				twoFound++ ;
		}
	}
	printf ("Number of 2's is %d\n", twoFound) ;
	printf ("Number of numbers including a 2 is %d\n", twoNums) ;
}
