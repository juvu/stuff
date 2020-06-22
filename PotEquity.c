#include <stdio.h>
#include <math.h>
#include <Windows.h>

int PotX = 0 ;
int PotY = 0 ;
int HandRankX = 0 ;
int HandRankY = 0 ;
int EquityX = 0 ;
int EquityY = 0 ;

#define POT_WIDTH 100
#define POT_HEIGHT 25
#define HANDRANK_WIDTH 150
#define HANDRANK_HEIGHT 25
#define EQUITY_WIDTH 50
#define EQUITY_HEIGHT 25

double getNumberAtLocation(char *file, int x, int y, int width, int height, int getText)
{
	char cmd[1024] = "" ;
	char buffer[1024] = "" ;

	double number = 0.0 ;

	number = 0.0 ;
	if (getText)
		sprintf (cmd, "getText %s %d %d %d %d %d", file, x, y, width, height, getText) ;
	else
		sprintf (cmd, "nircmd savescreenshot %s.bmp %d %d %d %d", file, x, y, width, height, getText) ;

	FILE *ppa = popen (cmd, "r") ;

        // now read the results
        while (fgets (buffer, 1024, ppa))
        {
        	//printf (buffer) ;
	} 

        pclose (ppa) ;

	sprintf (cmd, "tesseract %s.bmp %s --psm 8 tess_config", file, file) ;
	FILE *ppb = popen (cmd, "r") ;

        // now read the results
        while (fgets (buffer, 1024, ppb))
        {
        }

        pclose (ppb) ;

	sprintf (cmd, "%s.txt", file) ;
      	FILE *fp = fopen (cmd, "r") ;
	buffer[0] = '\0' ;
       	fgets (buffer, 1024, fp);
	if (strlen(buffer))
	{
		number = atof (buffer) ;
	}

	fclose (fp) ;

	if (!strstr (buffer, "."))
		number = number / 10.0 ;

	return number ;
}

char *getStringAtLocation(char *file, int x, int y, int width, int height, int getText, char *buffer)
{
	char cmd[1024] = "" ;

	double number = 0.0 ;

	if (getText)
		sprintf (cmd, "getText %s %d %d %d %d %d", file, x, y, width, height, getText) ;
	else
		sprintf (cmd, "nircmd savescreenshot %s.bmp %d %d %d %d", file, x, y, width, height, getText) ;

	FILE *ppa = popen (cmd, "r") ;

        // now read the results
        while (fgets (buffer, 1024, ppa))
        {
        	//printf (buffer) ;
	} 

        pclose (ppa) ;

	sprintf (cmd, "tesseract %s.bmp %s --psm 8 tess_configPR", file, file) ;
	FILE *ppb = popen (cmd, "r") ;

        // now read the results
        while (fgets (buffer, 1024, ppb))
        {
        }

        pclose (ppb) ;

	sprintf (cmd, "%s.txt", file) ;
      	FILE *fp = fopen (cmd, "r") ;
	buffer[0] = '\0' ;
       	fgets (buffer, 1024, fp);
	fclose (fp) ;

	return buffer ;
}

int doCalibrationHH(void) 
{
	POINT p;
	BOOL b;
 
	printf ("CALIBRATING HH.....\n") ;

	/* Get the current cursor position */
	b = GetCursorPos(&p);
	if (!b)
		return 0;

	double Equity = getNumberAtLocation ("Equity", p.x, p.y, EQUITY_WIDTH, EQUITY_HEIGHT, 0) ;
	if (Equity > 0.0)
	{
		printf ("Calibration Equity is %f\n", Equity) ;
		EquityX = p.x ;
		EquityY = p.y ;
		HandRankX = EquityX - 57 ;
		HandRankY = EquityY - 47 ;
	}				

	return 0 ;
}

int doCalibrationPot(void) 
{
	POINT p;
	BOOL b;
 
	printf ("CALIBRATING POT.....\n") ;

	/* Get the current cursor position */
	b = GetCursorPos(&p);
	if (!b)
		return 0;

	double Pot = getNumberAtLocation ("Pot", p.x, p.y, POT_WIDTH, POT_HEIGHT, 1) ;
	if (Pot > 0.0)
	{
		printf ("Calibration Pot is %f\n", Pot) ;
		PotX = p.x ;
		PotY = p.y ;
	}				

	return 0 ;
}

int main(int argc, char **argv)
{
	int i=0;
	
	for (i=0;i<5;i++)
	{
		doCalibrationHH() ;
		Sleep (1000) ;
	}
	for (i=0;i<5;i++)
	{
		doCalibrationPot() ;
		Sleep (1000) ;
	}

	char handRank[1024] = "";

	while (1)
	{
		double Equity = getNumberAtLocation ("Equity", EquityX, EquityY, EQUITY_WIDTH, EQUITY_HEIGHT, 0) ;
		printf ("Equity is %f\n", Equity) ;

		if (Equity == 0.0)
		{
			system ("rm MaxBetEquity.txt");
			Sleep(500);
			continue;
		}

		double Pot = getNumberAtLocation ("Pot", PotX, PotY, POT_WIDTH, POT_HEIGHT, 1) ;
		printf ("Pot is %f\n", Pot) ;

		getStringAtLocation ("HandRank", HandRankX, HandRankY, HANDRANK_WIDTH, HANDRANK_HEIGHT, 0, handRank) ;
		printf ("HandRank is %s\n", handRank) ;

		double Rank = 2.0 ;
		if (strstr (handRank,"PocketRank:"))
		{
			char *ptr = handRank + strlen("PocketRank:") ;
			char *ptr2 = strstr (ptr, "/") ;
			if (ptr2)
			{
				*ptr2++ = '\0' ;
				double numerator = atof (ptr) ;
				double denominator = atof (ptr2) ;
				Rank = numerator / denominator ;
			}
		}

		//double MaxBet = Pot / ((1.0 / (Equity * 0.01)) - 1.0);
		//
		double Efrac = Equity * 0.01 ;
		double MaxBet = Pot * Efrac / (1.0 - Efrac);

		if (MaxBet > 0.0)
		{
			FILE *fp = fopen ("MaxBet.html", "w") ;
			char buf[1024]="";
			//sprintf (buf, "<meta http-equiv=\"refresh\" content=\"1\"> <h1>Max Bet £%.2f</h1><h1>Equity %.2f%%</h1><h1>Pot £%.2f</h1><h1>Hand Rank %s</h1>", MaxBet, Equity, Pot, handRank) ;
			sprintf (buf, "<meta http-equiv=\"refresh\" content=\"1\"> <h1>Max Bet £%.2f</h1><h1>Equity %.2f%%</h1><h1>Pot £%.2f</h1>", MaxBet, Equity, Pot) ;
			fputs (buf, fp) ;
        		fclose (fp) ;

			FILE *fp2 = fopen ("MaxBetEquity.txt", "w") ;
			char buf2[1024]="";
			sprintf (buf2, "%.2f\n%.2f\n%.2f\n%.2f\n", MaxBet, Equity, Pot, Rank) ;
			fputs (buf2, fp2) ;
        		fclose (fp2) ;

		}

		printf ("MAX BET is %f\n", MaxBet) ;

		Sleep (500) ;
	}

	
	return 0;
}
