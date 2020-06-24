#include <stdio.h>
#include <math.h>
#include <Windows.h>

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

#define MAX_RANK 0.25
#define BAD_RANK 0.6

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

int doAction (int act, double CheckCall)
{
	if (act == FOLD && CheckCall == 0.0)
		act = CHECKCALL;
	if (act == FOLD)
	{
		printf ("FOLDING\n");
		SetAndClick (selectFoldX, selectFoldY) ;
		// and get rid of the annoying popup if it appears
		Sleep(1000);
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
	Sleep (1000) ;
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
			doAction (CHECKCALL, CheckCall) ;
		else if (Iaction == CHECKCALL)
			doAction (FOLD, CheckCall) ;
	}
}

int main(int argc, char **argv)
{
	int i=0;

	double AbsMax = atof (argv[1]) ;
	double BigBlind = atof (argv[2]) ;
	double CheckCall = 0.0 ;
	double Raise = 0.0 ;
	
	FILE *fp = fopen ("BaseXY.txt", "r") ;
	char buf[1024]="";
	fgets (buf, 1024, fp) ;
	sscanf (buf, "%d %d", &BaseX, &BaseY) ;
	fclose (fp) ;

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

			int IAction = CHECKCALL ;
			char Action[1024] = "" ;

			// wait for everything to catch up
			//
			Sleep (2500);

			// Get MaxBet and Equity
			//
			FILE *fp3 = fopen ("MaxBetEquity.txt", "r") ;
			if (!fp3)
				continue;

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
				IAction = ALLIN ;
			else if (CheckCall == 0.0 && (Raise > MaxBet || Raise > AbsMax))
				IAction = FOLD ;
			else if (CheckCall == 0.0 && (Raise > 10.0 * BigBlind) && Equity < 70.0)
				IAction = FOLD ;
			// Possible raise checks
			else if (Equity >= 75.0 && Equity < 95.0)
			{
				if (AllowRaise && Pot <= AbsMax && Pot <= MaxBet)
					IAction = RAISE100 ;
				else if (AllowRaise && (Pot * 0.5) <= AbsMax && (Pot * 0.5) <= MaxBet)
					IAction = RAISE50 ;
				else if (AllowRaise && Raise <= AbsMax && Raise <= MaxBet)
					IAction = RAISE ;
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
					IAction = CHECKCALL;
				else if (Rank <= MAX_RANK)
					IAction = CHECKCALL;
				else if (Rank >= BAD_RANK && Rank <=1.0)
					IAction = FOLD;
				else
					IAction = FOLD;
			}
			else if (Equity >= 60.0 && Equity < 75.0)
			{
				if (AllowRaise && (Pot * 0.5) <= AbsMax && (Pot * 0.5) <= MaxBet)
					IAction = RAISE50 ;
				else if (AllowRaise && Raise <= AbsMax && Raise <= MaxBet)
					IAction = RAISE ;
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
					IAction = CHECKCALL;
				else if (Rank <= MAX_RANK)
					IAction = CHECKCALL;
				else if (Rank >= BAD_RANK && Rank <=1.0)
					IAction = FOLD;
				else
					IAction = FOLD;
			}
			else if (Equity >= 50.0 && Equity < 60.0)
			{
				if (AllowRaise && Raise <= AbsMax && Raise <= MaxBet)
					IAction = RAISE ;
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
					IAction = CHECKCALL;
				else if (Rank <= MAX_RANK)
					IAction = CHECKCALL;
				else if (Rank >= BAD_RANK && Rank <=1.0)
					IAction = FOLD;
				else
					IAction = FOLD;
			}
			// Non-raise situations
			else 
			{
				// Note we always at least check the top 15% of hands
				if (Rank <= MAX_RANK)
					IAction = CHECKCALL;
				else if (Rank >= BAD_RANK && Rank <=1.0)
					IAction = FOLD;
				else if (CheckCall <= AbsMax && CheckCall <= MaxBet)
					IAction = CHECKCALL;
				else
					IAction = FOLD;
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

			doAction (IAction, CheckCall) ;
			CheckItWorked (IAction, CheckCall, Raise) ;

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


		Sleep (1000) ;
	}

	
	return 0;
}
