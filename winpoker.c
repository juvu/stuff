#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define WIDTH 54
#define HEIGHT 73

double values[52] = { 18177952911.0, 7682500513.0, 10325406097.0, 9203852322.0, 9276333579.0, 13672230557.0, 7758037321.0, 17347243922.0,
            16141506534.0, 17271333463.0, 8963267466.0, 7365181911.0, 7357748331.0, 18697157637.0, 8695077024.0, 8229043733.0,
            11040881984.0, 11987325471.0, 8233211157.0, 17213388308.0, 18060426607.0, 8244844988.0, 8141452273.0,
            15013588852.0, 7564702930.0, 7968657029.0, 7615171758.0, 7916204114.0, 8334624420.0, 13037692225.0, 10245089350.0,
            7737046004.0, 7853678602.0, 7616439579.0, 7773297054.0, 14295559056.0, 10460547667.0, 7652278402.0, 20078406445.0,
            10064001225.0, 18955054218.0, 9594074001.0, 14858617746.0, 11712393432.0, 9298060231.0, 8246249071.0, 14101256129.0,
		7597957718.0, 9566392462.0, 8581101308.0, 7123659395.0, 10036282747.0 };

char *card[52] = { "KH", "6S", "AS", "9C", "TS", "9D", "7S", "QC", "JC", "KS", "8H", "5S", "2S", "QH", "8C", "8S", "5D", "7D", "7C", "KC",
            "QD", "3H", "6C", "TD", "4C", "3C", "4H", "5H", "7H", "8D", "3D", "3S", "5C", "2C", "AH", "JS", "4D", "2H", "KD", "TH", "JD",
            "AD", "JH", "6D", "9H", "6H", "QS", "AC", "2D", "9S", "4S", "TC"};


int XOffsets[7] = {0, 65, -160, -89, -18, 53, 124} ;
int YOffsets[7] = {0, 0, -198, -198, -198, -198, -198} ;

int XBase = 0 ;
int YBase = 0 ;

static char cardBuffer[100] = "" ;

int getCards() 
{
	POINT p;
	COLORREF color;
	HDC hDC;
	
 
	/* Get the device context for the screen */
	hDC = GetDC(NULL);
	if (hDC == NULL)
		return 0;
 

	LPVOID lpvBits[WIDTH*(HEIGHT+1)];
	HBITMAP bitmap = CreateBitmap (WIDTH, HEIGHT, 1, 32, lpvBits) ;

	//HGDIOBJ bitobj = SelectObject (hDC, bitmap) ;

	HDC hDC2 = CreateCompatibleDC(hDC);
	SelectObject (hDC2, bitmap) ;

	int cards = 0;
	int cardFound = 0 ;

	for (cards=0; cards<7; cards++)
	{
		BitBlt (hDC2, 0, 0, WIDTH, HEIGHT, hDC, XBase + XOffsets[cards], 
							YBase + YOffsets[cards], SRCCOPY) ;

		int i = 0 ;
		int j = 0;
		double total = 0.0;

		total = 0.0;
		for (i=0;i<HEIGHT;i++)
		{
			for (j=0;j<WIDTH;j++)
			{
 				color = GetPixel(hDC2, j, i);
				if (color < 15000000)
					total += color;
			}
		}
		
		cardFound = 0 ;	
		for (i=0;i<52;i++)
		{
			if (values[i] == total)
			{
				cardFound = 1 ;
				strcat (cardBuffer, card[i]) ;
				//printf ("Card%d FOUND: %s\n", cards+1, card[i]);
			}
		}

		if (!cardFound)
		{
			//printf ("Card%d UNKNOWN: %f\n", cards+1, total);
		}
	}

	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return 0;
}

int doCalibration(void) 
{
	POINT p;
	COLORREF color;
	HDC hDC;
	BOOL b;
 
	printf ("CALIBRATING.....\n") ;

	/* Get the device context for the screen */
	hDC = GetDC(NULL);
	if (hDC == NULL)
		return 0;
 
	/* Get the current cursor position */
	b = GetCursorPos(&p);
	if (!b)
		return 0;

	LPVOID lpvBits[WIDTH*(HEIGHT+1)];
	HBITMAP bitmap = CreateBitmap (WIDTH, HEIGHT, 1, 32, lpvBits) ;

	//HGDIOBJ bitobj = SelectObject (hDC, bitmap) ;

	HDC hDC2 = CreateCompatibleDC(hDC);
	SelectObject (hDC2, bitmap) ;

	int x = 0 ;
	int y = 0 ;
	int nearestX = 0 ;
	int nearestY = 0 ;
	int nearestPX = 0 ;
	int nearestPY = 0 ;
	int cardIndex = -1 ;

	double minTotal = 99999999999.0 ;
	for (x=p.x-5; x<=p.x+5; x++)
	{
		for (y=p.y-5; y<=p.y+5; y++)
		{
			BitBlt (hDC2, 0, 0, WIDTH, HEIGHT, hDC, x, y, SRCCOPY) ;

			int i = 0 ;
			int j = 0;
			double total = 0.0;

			total = 0.0;
			for (i=0;i<HEIGHT;i++)
			{
				for (j=0;j<WIDTH;j++)
				{
 					color = GetPixel(hDC2, j, i);
					if (color < 15000000)
						total += color;
				}
			}
			
			for (i=0;i<52;i++)
			{
				if (fabs(values[i] - total) < minTotal)
				{
					minTotal = fabs(values[i] - total) ;
					cardIndex = i ;
					nearestX = x ;
					nearestY = y ;
					nearestPX = p.x ;
					nearestPY = p.y ;
				}
			}
		}
	}

	
	if (minTotal == 0.0)
	{
		XBase = nearestX ;
		YBase = nearestY ;
	}

 
	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return 0;
}




int main(int argc, char **argv)
{
	char cmd[1000] = "" ;
	char buffer[1025] = "" ;

	if (argc !=7)
	{
		printf ("Usage: winpoker <user> <pass> <host> <2|6> <BigBlind> <SmallBlind>\n") ;
		exit(0) ;
	}

	XBase = 0 ;
	YBase = 0 ;
	int mode = atoi (argv[4]) ;
	int BigBlind = atoi (argv[5]) ;
	int SmallBlind = atoi (argv[6]) ;

	while (!XBase)
	{
		doCalibration() ;
		Sleep (1000) ;
	}

	while (1)
	{
		int i = 0 ;
	
		for (i=0;i<100;i++)
		{
			cardBuffer[i] = '\0' ;
		}

		getCards() ;

		//strcpy (cardBuffer, "AHJC") ;

		if (strlen (cardBuffer) >= 4)
		{
			if (mode !=6)
			{
				sprintf (cmd, "plink -pw %s %s@%s github/stuff/poker %d %d %s nohtml", 
							argv[2], argv[1], argv[3], BigBlind, SmallBlind, cardBuffer) ;
			}
			else
			{
				sprintf (cmd, "plink -pw %s %s@%s github/stuff/6poker %d %d %s nohtml", 
							argv[2], argv[1], argv[3], BigBlind, SmallBlind, cardBuffer) ;
			}
			printf ("%s\n", cmd) ;


			FILE *pp = popen (cmd, "r") ;

			// now read the results
			while (fgets (buffer, 1024, pp))
			{
				printf (buffer) ;
			}
	
			pclose (pp) ;
		}
		Sleep (1000) ;
	}

	return 0;
}
