#include <stdio.h>

main()
{
	int i ;
	FILE	*fp ;
	char	buf[20] ;


	fp = fopen ("asdf", "w") ;
	for (i=1;i<256;i++)
	{
		sprintf (buf, "%d %c\n", i, (char)i) ;
		fputs (buf, fp) ;
	}
	fclose (fp) ;
}
