#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <stdint.h>

#define WIDTH 113
#define HEIGHT 25
#define CHECKCALLX 780
#define CHECKCALLY 697
#define RAISEX 893
#define RAISEY 697

#define FOLD 0
#define CHECKCALL 1
#define RAISE 2
#define RAISE50 3
#define RAISE100 4
#define ALLIN 5

double MAX_RANK = 0.25 ;
double BAD_RANK = 0.6 ;

int select50X = 704 ;
int select50Y = 622 ;
int select100X = 784 ;
int select100Y = 622 ;
int allInX = 881 ;
int allInY = 622 ;

int selectFoldX = 717 ;
int selectFoldY =  697;
int selectCheckCallX = 837 ;
int selectCheckCallY = 697 ;
int selectRaiseX = 952 ;
int selectRaiseY = 697 ;

int confirmCheckX = 717 ;
int confirmCheckY = 447 ;

int BaseX = 0 ;
int BaseY = 0 ;

#define INTERVAL 200


void setCursorPos(int x, int y)
{
    POINT p;
    COLORREF color;
    HDC hDC;
    BOOL b;

    /* Get the device context for the screen */
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return;

    p.x = x;
    p.y = y;
    b = SetCursorPos(x, y);
    if (!b)
        return;

    //printf ("Pos is %d %d\n", p.x, p.y) ;

    /* Release the device context again */
    ReleaseDC(GetDesktopWindow(), hDC);
}

void DoMouseClick(int x, int y)
{
      mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

void SetAndClick (int x, int y)
{
        setCursorPos(BaseX + x, BaseY + y) ;
        DoMouseClick(BaseX + x, BaseY + y);
        Sleep (INTERVAL);
}


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

int doAction (int act, double CheckCall, int Rule)
{
	printf ("Act is %d Rule is %d\n", act, Rule) ;

	if (act == FOLD && CheckCall == 0.0)
		act = CHECKCALL;
	if (act == FOLD)
	{
		printf ("FOLDING\n");
		SetAndClick (selectFoldX, selectFoldY) ;
		// and get rid of the annoying popup if it appears
		Sleep(500);
		SetAndClick (confirmCheckX, confirmCheckY) ;
	}
	else if (act == CHECKCALL)
	{
		printf ("CALLING\n");
		SetAndClick (selectCheckCallX, selectCheckCallY) ;
	}
	else if (act == RAISE)
	{
		printf ("RAISING\n");
		SetAndClick (selectRaiseX, selectRaiseY) ;
	}
	else if (act == RAISE50)
	{
		printf ("RAISING 50\n");
		SetAndClick (select50X, select50Y) ;
		SetAndClick (selectRaiseX, selectRaiseY) ;
	}
	else if (act == RAISE100)
	{
		printf ("RAISING 100\n");
		SetAndClick (select100X, select100Y) ;
		SetAndClick (selectRaiseX, selectRaiseY) ;
	}
	else if (act == ALLIN)
	{
		printf ("ALLIN\n");
		SetAndClick (allInX, allInY) ;
		SetAndClick (selectRaiseX, selectRaiseY) ;
	}
}

int CheckItWorked (int Iaction, double CheckCall, double Raise) 
{
	Sleep (100) ;
	double newRaise = getNumberAtLocation ("Raise", BaseX + RAISEX, BaseY + RAISEY, WIDTH, HEIGHT, 1) ;
	printf ("New Raise is %f\n", newRaise) ;
	double newCheckCall = getNumberAtLocation ("CheckCall", BaseX + CHECKCALLX, BaseY + CHECKCALLY, WIDTH, HEIGHT, 1) ;
	printf ("newCheckCall is %f\n", newCheckCall) ;

	// Buttons are sometimes reversed in AllIn
	//
	if (newCheckCall > newRaise)
	{
		double tmp = newRaise ;
		newCheckCall = newRaise;
		newRaise = tmp;
	}

	if (newRaise == Raise && newCheckCall == CheckCall)
	{
		if (Iaction == ALLIN || Iaction == RAISE100 || Iaction == RAISE50 || Iaction == RAISE)
			doAction (CHECKCALL, CheckCall, 1000) ;
		else if (Iaction == CHECKCALL)
			doAction (FOLD, CheckCall, 1001) ;
	}
}

double getminLargeBetEquity(int numCards)
{
	double minEquity[8] = {70.0, 70.0, 65.0, 65.0, 65.0, 80.0, 85.0, 90.0} ;

	if (numCards < 2 || numCards > 7)
		return (70.0) ;

	return (minEquity[numCards]) ;
}

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}


int main(int argc, char **argv)
{
	int i=0;
	int FailedEquity = 0 ;
	int ForceAllin = 0 ;

	int numCards = 0 ;
	int startPlayers = 0 ;
	int pocketFolds = 0 ;
	int flopFolds = 0 ;
	int turnFolds = 0 ;
	char myCards[1024] = "" ;
	char communityCards[1024] = "" ;

	double AbsMax = atof (argv[1]) ;
	double CheckCall = 0.0 ;
	double Raise = 0.0 ;
	
	FILE *fp = fopen ("BaseXY.txt", "r") ;
	char buf[1024]="";
	fgets (buf, 1024, fp) ;
	sscanf (buf, "%d %d", &BaseX, &BaseY) ;
	fclose (fp) ;

        struct timeval tv ;
        struct tm *tinfo ;
        time_t curtime ;

        gettimeofday(&tv, NULL);
        curtime=tv.tv_sec;

        tinfo = localtime(&curtime) ;

	// open the log file

	char logFile[1024]="";

	sprintf (logFile, "AutoBetLog_%d_%d%d", tinfo->tm_yday, tinfo->tm_hour, tinfo->tm_min) ;

	FILE *lf = fopen (logFile, "w");

	char logbuf[1024] = "";
	sprintf (logbuf, "MyCards,Community Cards,Balance,Action,Call Amount,Raise Amount,Pot Size,Equity,MaxBet,Start Opp,Pocket Folds, Flop Folds, Turn Folds\n") ;
        fputs (logbuf, lf) ;
	fflush (lf) ;

	while (1)
	{
		Raise = 0.0 ;
		CheckCall = 0.0 ;
		Raise = getNumberAtLocation ("Raise", BaseX + RAISEX, BaseY + RAISEY, WIDTH, HEIGHT, 1) ;
		printf ("Raise is %f\n", Raise) ;

		CheckCall = getNumberAtLocation ("CheckCall", BaseX + CHECKCALLX, BaseY + CHECKCALLY, WIDTH, HEIGHT, 1) ;
		printf ("CheckCall is %f\n", CheckCall) ;

                if (Raise > 0.0 || CheckCall > 0.0)
                {
			double MaxBet = 0.0 ;
			double Equity = 0.0 ;
			double Pot = 0.0 ;
			double Rank = 1.0 ;
			int Rule = 0 ;

			// for timing reason we need to reread both
			//

			if (Raise == 0.0)
			{
				Raise = getNumberAtLocation ("Raise", BaseX + RAISEX, BaseY + RAISEY, WIDTH, HEIGHT, 1) ;
				printf ("Confirmed Raise is %f\n", Raise) ;
			}

			if (CheckCall == 0.0)
			{
				CheckCall = getNumberAtLocation ("CheckCall", BaseX + CHECKCALLX, BaseY + CHECKCALLY, WIDTH, HEIGHT, 1) ;
				printf ("Confirmed CheckCall is %f\n", CheckCall) ;
			}

			int IAction = CHECKCALL ;
			char Action[1024] = "" ;

			// wait for everything to catch up
			//
			Sleep (1500);

			ForceAllin = 0 ;

			// Get MaxBet and Equity
			//
			FILE *fp3 = fopen ("MaxBetEquity.txt", "r") ;
			if (!fp3)
			{
				FailedEquity++;
				if (FailedEquity == 4)
				{
					// TEMPORARY - this is failure to read 100% equity
					ForceAllin = 1 ;
				}
				else
					continue;
			}


			FailedEquity = 0 ;

			if (!ForceAllin)
			{
				char buff3[1024]="";
				fgets (buff3, 1024, fp3) ;
				MaxBet = atof (buff3) ;
				fgets (buff3, 1024, fp3) ;
				Equity = atof (buff3) ;
				fgets (buff3, 1024, fp3) ;
				Pot = atof (buff3) ;
				fgets (buff3, 1024, fp3) ;
				Rank = atof (buff3) ;
				fclose (fp3) ;
				printf ("MaxBet is %f Equity is %f CheckCall is %f Pot is %f Rank is %f\n", 
								MaxBet, Equity, CheckCall, Pot, Rank) ;
			}

			FILE *fpCPI = fopen ("cardPlayerInfo.txt", "r") ;
			if (fpCPI)
			{
				char buff[1024]="";
				fgets (buff, 1024, fpCPI) ;
				numCards = atoi (buff) ;
				fgets (buff, 1024, fpCPI) ;
				strcpy (myCards, buff) ;
				char *ptr = strstr (myCards, "\n") ; 
				if (ptr)
					*ptr = '\0';
				fgets (buff, 1024, fpCPI) ;
				strcpy (communityCards, buff) ;
				ptr = strstr (communityCards, "\n") ; 
				if (ptr)
					*ptr = '\0';
				fgets (buff, 1024, fpCPI) ;
				startPlayers = atoi (buff) ;
				fgets (buff, 1024, fpCPI) ;
				pocketFolds = atoi (buff) ;
				fgets (buff, 1024, fpCPI) ;
				flopFolds = atoi (buff) ;
				fgets (buff, 1024, fpCPI) ;
				turnFolds = atoi (buff) ;
				fclose (fpCPI) ;
			}

			// Buttons are sometimes reversed in AllIn
			//
			if (CheckCall > Raise)
			{
				double tmp = Raise ;
				CheckCall = Raise;
				Raise = tmp;
			}

			// do not reraise if the bet is too big
			int AllowRaise =1 ;
			if (CheckCall > 0.25 * Pot)
				AllowRaise = 0 ;

			// allin checks
			if (Equity >= 95.0)
			{
				IAction = ALLIN ;
				Rule = 1;
			}

			else if (CheckCall == 0.0 && (Raise > MaxBet || Raise > AbsMax))
			{
				if (Equity < 0.98)
				{
					IAction = FOLD ;
					Rule = 2;
				}
			}

			else if (CheckCall == 0.0 && Raise >= AbsMax * 0.25 )
			{
				double minLargeBetEquity = getminLargeBetEquity(numCards) ;
				if (Equity < minLargeBetEquity)
				{
					IAction = FOLD ;
					Rule = 3;
				}
			}
			// Large call amount situation
			else if (CheckCall >= AbsMax * 0.25 )
			{
				double minLargeBetEquity = getminLargeBetEquity(numCards) ;
				if (Equity < minLargeBetEquity)
				{
					IAction = FOLD ;
					Rule = 103;
				}
			}
			// Possible raise checks
			else if (Equity >= 75.0 && Equity < 95.0)
			{
				if (Rank <= MAX_RANK)
				{
					IAction = CHECKCALL;
					Rule = 8;
				}
				else if (Rank >= BAD_RANK && Rank <=1.0)
				{
					IAction = FOLD;
					Rule = 9;
				}
				else if (AllowRaise && Pot <= AbsMax && Pot <= MaxBet)
				{
					IAction = RAISE100 ;
					Rule = 4;
				}
				else if (AllowRaise && (Pot * 0.5) <= AbsMax && (Pot * 0.5) <= MaxBet)
				{
					IAction = RAISE50 ;
					Rule = 5;
				}
				else if (AllowRaise && Raise <= AbsMax && Raise <= MaxBet)
				{
					IAction = RAISE ;
					Rule = 6;
				}
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
				{
					IAction = CHECKCALL;
					Rule = 7;
				}
				else
				{
					IAction = FOLD;
					Rule = 9;
				}
			}
			else if (Equity >= 60.0 && Equity < 75.0)
			{
				if (Rank <= MAX_RANK)
				{
					IAction = CHECKCALL;
					Rule = 13;
				}
				else if (Rank >= BAD_RANK && Rank <=1.0)
				{
					IAction = FOLD;
					Rule = 14;
				}
				else if (AllowRaise && (Pot * 0.5) <= AbsMax && (Pot * 0.5) <= MaxBet)
				{
					IAction = RAISE50 ;
					Rule = 10;
				}
				else if (AllowRaise && Raise <= AbsMax && Raise <= MaxBet)
				{
					IAction = RAISE ;
					Rule = 11;
				}
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
				{
					IAction = CHECKCALL;
					Rule = 12;
				}
				else
				{
					IAction = FOLD;
					Rule = 15;
				}
			}
			else if (Equity >= 50.0 && Equity < 60.0)
			{
				if (AllowRaise && Raise <= AbsMax && Raise <= MaxBet)
				{
					IAction = RAISE ;
					Rule = 16;
				}
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
				{
					IAction = CHECKCALL;
					Rule = 17;
				}
				else if (Rank <= MAX_RANK)
				{
					IAction = CHECKCALL;
					Rule = 18;
				}
				else if (Rank >= BAD_RANK && Rank <=1.0)
				{
					IAction = FOLD;
					Rule = 19;
				}
				else
				{
					IAction = FOLD;
					Rule = 20;
				}
			}
			// Non-raise situations
			else 
			{
				// Note we always at least check the top hands
				if (Rank <= MAX_RANK)
				{
					IAction = CHECKCALL;
					Rule = 21;
				}
				// and throw away the worst ones
				else if (Rank >= BAD_RANK && Rank <=1.0)
				{
					IAction = FOLD;
					Rule = 22;
				}
				// don't chase a really bad hand because of theoretical positive EV
				else if (Equity < 20.0)
				{
					IAction = FOLD ;
					Rule = 23;
				}
				// main EV check
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
				{
					IAction = CHECKCALL;
					Rule = 24;
				}
				else
				{
					IAction = FOLD;
					Rule = 25;
				}
			}

			if (IAction == FOLD)
				sprintf (Action, "Fold") ;
			else if (IAction == CHECKCALL)
				sprintf (Action, "Check/Call (%.2f)", CheckCall) ;
			else if (IAction == RAISE)
				sprintf (Action, "Raise (%.2f)", Raise) ;
			else if (IAction == RAISE50)
				sprintf (Action, "Raise 50%% Pot (%.2f)", Pot * 0.5) ;
			else if (IAction == RAISE100)
				sprintf (Action, "Raise 100%% Pot (%.2f)", Pot) ;
			else if (IAction == ALLIN)
				sprintf (Action, "All-in") ;

			doAction (IAction, CheckCall, Rule) ;
			CheckItWorked (IAction, CheckCall, Raise) ;

			double Balance = 0.0; // for now
			sprintf (logbuf, "%s,%s,%f,%s,%f,%f,%f,%f,%f,%d,%d,%d,%d\n", myCards, communityCards,Balance,Action,CheckCall,Raise,Pot,Equity,MaxBet,startPlayers, pocketFolds, flopFolds, turnFolds); 
			fputs (logbuf, lf) ;
			fflush (lf) ;

			system ("rm  MaxBetEquity.txt") ;

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
