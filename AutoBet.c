#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define WIDTH 113
#define HEIGHT 25
#define CHECKCALLX 780
#define CHECKCALLY 696
#define RAISEX 893
#define RAISEY 696

int BaseX = 0 ;
int BaseY = 0 ;

double getNumberAtLocation(char *file, int x, int y, int width, int height, int getText)
{
	char cmd[1024] = "" ;
	char buffer[1024] = "" ;

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

int main(int argc, char **argv)
{
	int i=0;

	
	FILE *fp = fopen ("BaseXY.txt", "r") ;
	char buf[1024]="";
	fgets (buf, 1024, fp) ;
	sscanf (buf, "%d %d", &BaseX, &BaseY) ;
	fclose (fp) ;

	while (1)
	{
		double Raise = getNumberAtLocation ("Raise", BaseX + RAISEX, BaseY + RAISEY, WIDTH, HEIGHT, 1) ;
		printf ("Raise is %f\n", Raise) ;

		double CheckCall = getNumberAtLocation ("CheckCall", BaseX + CHECKCALLX, BaseY + CHECKCALLY, WIDTH, HEIGHT, 1) ;
		printf ("CheckCall is %f\n", CheckCall) ;

                if (Raise > 0.0)
                {
			double MaxBet = 0.0 ;
			double Equity = 0.0 ;
			char Action[1024] = "" ;

			// wait for everything to catch up
			//
			Sleep (2500);

			// Get MaxBet and Equity
			//
			FILE *fp3 = fopen ("MaxBetEquity.txt", "r") ;
			char buff3[1024]="";
			fgets (buff3, 1024, fp3) ;
			MaxBet = atof (buff3) ;
			fgets (buff3, 1024, fp3) ;
			Equity = atof (buff3) ;
			fclose (fp3) ;
			printf ("MaxBet is %f Equity is %f\n", MaxBet, Equity) ;

			if (Equity >= 50.0)
			{
				if (Raise <= MaxBet)
					sprintf (Action, "Raise (%.2f)", Raise) ;
				else if (CheckCall <= MaxBet)
					sprintf (Action, "Check/Call (%.2f)", CheckCall) ;
				else
					sprintf (Action, "Fold") ;
			}
			else 
			{
				if (CheckCall <= MaxBet)
					sprintf (Action, "Check/Call (%.2f)", CheckCall) ;
				else
					sprintf (Action, "Fold") ;
			}


                        FILE *fp2 = fopen ("CheckRaise.html", "w") ;
                        char buff2[1024]="";
                        sprintf (buff2, "<meta http-equiv=\"refresh\" content=\"1\"> <h1>Call £%.2f</h1><h1>Raise £%.2f</h1><h1>Action: %s</h1>", CheckCall, Raise, Action) ;
                        fputs (buff2, fp2) ;
                        fclose (fp2) ;
                }
		else
                {
                        FILE *fp = fopen ("CheckRaise.html", "w") ;
                        char buffer[1024]="";
                        sprintf (buffer, "<meta http-equiv=\"refresh\" content=\"1\"> <h1>Call £0.00</h1><h1>Raise £0.00</h1><h1>Action: Waiting") ;
                        fputs (buffer, fp) ;
                        fclose (fp) ;
                }


		Sleep (500) ;
	}

	
	return 0;
}
