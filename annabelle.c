#include <stdio.h>
#include <strings.h>
#include <string.h>


main (int argc, char **argv)
{
	char buffer[100000] ;
	int i = 0 ;

	printf ("Code (c) or Decode (d)\n");
    gets(buffer) ;

	if (buffer[0] == 'c')
	{
		printf ("Enter secret message\n");
    	gets(buffer) ;

		for (i=0;i<strlen(buffer);i++)
		{
			buffer[i] += i%5;
		}
	}
	else
	{
		printf ("Enter code\n");
    	gets(buffer) ;
		for (i=0;i<strlen(buffer);i++)
		{
			buffer[i] -= i%5;
		}
	}

	printf ("%s\n", buffer) ;

#if 0
	for (i=0;i<1000;i++)
	{
		printf ("And now. 1 to a million GUINEA PIGS ARE SO COOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOLLLLLLLLLLLLLLLLL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
#endif
}
	

