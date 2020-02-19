#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

int a[32]={2,15,23,19,3,2,3,27,20,11,27,10,19,10,13,10};
int b[32]={22,9,5,10,5,1,24,2,10,9,7,3,12,24,10,9};
int c[32]={10,0,15,0,6,0,9,0,16,0,17,0,2,0,2,0};
int d[32]={8,3,6,15,22,6,1,1,11,27,14,5,5,7,8,24};
int e[32]={15,0,14,0,5,0,10,0,2,0,22,0,2,0,17,0};
int f[32]={5,5,4,8,6,3,1,6,10,6,10,2,6,10,4,1};
int g[32]={6,0,13,0,3,0,3,0,6,0,10,0,10,0,10,0};

int calculateTotals (int cpos, int epos, int gpos)
{
	int total[16] ;
	int i = 0 ;

	for (i=0;i<16;i++)
	{
		total[i] = a[i] + c[cpos+i] + e[epos+i] + g[gpos+i];
		if (c[cpos+i] == 0)
			total[i] += b[i] ;
		if (e[epos+i] == 0)
			total[i] += d[cpos+i];
		if (g[gpos+i] == 0)
			total[i] += f[epos+i];
	}

	for (i=0;i<16;i++)
	{
		if (total[i] != total[0])
			return 0 ;
	}
	printf ("Total is %d, cpos is %d, epos is %d, gpos is %d\n", total[0], cpos, epos, gpos) ;
	return 1;
}


main (int argc, char **argv)
{
    // init random seed
    struct timeval tp ;
    gettimeofday (&tp, NULL) ;

    srand (tp.tv_usec) ;

    // duplkicate cells 0 to 15 in 16 to 31 to represent circular nature of puzzle
    //

    int i = 0 ;

    for (i=0;i<16;i++)
    {
	    a[i+16] = a[i] ;
	    b[i+16] = b[i] ;
	    c[i+16] = c[i] ;
	    d[i+16] = d[i] ;
	    e[i+16] = e[i] ;
	    f[i+16] = f[i] ;
	    g[i+16] = g[i] ;
    }

    int found = 0 ;
    int cpos, epos, gpos;

    while (found == 0)
    {
	    cpos = rand()%16 ;
	    epos = rand()%16;
	    gpos = rand()%16;

	    found = calculateTotals(cpos, epos, gpos) ;
    }

    for (cpos=0;cpos<16;cpos++)
    {
	    for (epos=0;epos<16;epos++)
	    {
		    for (gpos=0;gpos<16;gpos+=2)
		    {
	    		found = calculateTotals(cpos, epos, gpos) ;
		    }
	    }
    }




}
