#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#define NUMSLOTS 21 

#define INTERVAL 2000
#define SHORT_INTERVAL 500

#define OPPOSITE -1
#define SAME 1

int PositionCloseX = 945;
int RowOffset = 36;

int PositionX = 289;
int PositionY = 232; 

int SignalX = 1060;
int SignalY = 232;

int SignalBuyX = 1455;
int SignalBuyY = 377;

int SignalSellX = 1269;
int SignalSellY = 386;

int SignalSetValX = 1407;
int SignalSetValY = 445;

int SignalPlaceOrderX = 1373;
int SignalPlaceOrderY = 858;

int SignalCloseWinX = 1523;
int SignalCloseWinY = 266;


char *getStringAtLocation(char *file, int x, int y, int width, int height, int getText, char *buffer)
{
        char cmd[1024] = "" ;

        double number = 0.0 ;

        if (getText == 0)
                sprintf (cmd, "getText %s %d %d %d %d %d", file, x, y, width, height, getText) ;
        else
                sprintf (cmd, "nircmd savescreenshot %s.bmp %d %d %d %d", file, x, y, width, height) ;

	//printf ("Issuing cmd %s\n", cmd) ;

        FILE *ppa = popen (cmd, "r") ;

        // now read the results
        while (fgets (buffer, 1024, ppa))
        {
                //printf (buffer) ;
        }

        pclose (ppa) ;

        sprintf (cmd, "tesseract %s.bmp %s tess_config", file, file) ;
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

	int xx;
	for (xx=0;xx<=strlen(buffer)+1;xx++)
	{
		if (buffer[xx] == '\n')
			buffer[xx] = '\0' ;
	}

        return buffer ;
}

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

void SetAndClickShort (int x, int y)
{
        setCursorPos(x,y) ;
        DoMouseClick(x,y);
        Sleep (SHORT_INTERVAL);
}

void SetAndClick (int x, int y)
{
        setCursorPos(x,y) ;
        DoMouseClick(x,y);
        Sleep (INTERVAL);
}

int loadCoords (char *file)
{
        FILE *fp = fopen (file, "r");
        char buffer[1024] = "";
        char name[1024] = "";
        char coord[1024] = "";
        int iCoord = 0 ;

        while (fgets(buffer, 1024, fp))
        {
                char *ptr = strstr (buffer, "\n");
                if (ptr)
                        *ptr = '\0';

                sscanf(buffer,"%s %s", name, coord);
                if (!strcmp(name,"PositionCloseX"))
                        PositionCloseX = atoi (coord);
                else if (!strcmp(name,"RowOffset"))
                        RowOffset = atoi (coord);
                else if (!strcmp(name,"PositionX"))
                        PositionX = atoi (coord);
                else if (!strcmp(name,"PositionY"))
                        PositionY = atoi (coord);
                else if (!strcmp(name,"SignalX"))
                        SignalX = atoi (coord);
                else if (!strcmp(name,"SignalY"))
                        SignalY = atoi (coord);
                else if (!strcmp(name,"SignalBuyX"))
                        SignalBuyX = atoi (coord);
                else if (!strcmp(name,"SignalBuyY"))
                        SignalBuyY = atoi (coord);
                else if (!strcmp(name,"SignalSellX"))
                        SignalSellX = atoi (coord);
                else if (!strcmp(name,"SignalSellY"))
                        SignalSellY = atoi (coord);
                else if (!strcmp(name,"SignalSetValX"))
                        SignalSetValX = atoi (coord);
                else if (!strcmp(name,"SignalSetValY"))
                        SignalSetValY = atoi (coord);
                else if (!strcmp(name,"SignalPlaceOrderX"))
                        SignalPlaceOrderX = atoi (coord);
                else if (!strcmp(name,"SignalPlaceOrderY"))
                        SignalPlaceOrderY = atoi (coord);
                else if (!strcmp(name,"SignalCloseWinX"))
                        SignalCloseWinX = atoi (coord);
                else if (!strcmp(name,"SignalCloseWinY"))
                        SignalCloseWinY = atoi (coord);

        }
        fclose (fp) ;
        return (0);
}

void PlaceOrder(int direction)
{
	if (direction == 1)
		SetAndClick(SignalBuyX, SignalBuyY);
	else
		SetAndClick(SignalSellX, SignalSellY);
	SetAndClick(SignalSetValX, SignalSetValY);
	SetAndClick(SignalPlaceOrderX, SignalPlaceOrderY);
	SetAndClick(SignalCloseWinX, SignalCloseWinY);
}

int checkPositions()
{
	int x = PositionX;
	int y = PositionY;
	int count = 0 ;
	char posDirection[1024] = "" ;
	int iDir = 0;
	char posName[1024] = "" ;
	char latest[1024] = "" ;
	char profit[1024] = "" ;
	char position[1024] = "";

	for (count = 0; count < NUMSLOTS; count++,y+=RowOffset)
	{
		getStringAtLocation ("PositionCheck", x, y, 740, RowOffset, 1, position) ;
		//printf ("Position is *%s*\n", position);
		posName[0] = '\0';
		posDirection[0] = '\0';
		profit[0] = '\0';
		latest[0] = '\0';
		sscanf(position,"%s %s %s %s", posName, posDirection, latest, profit);
		//printf ("%s %s %s *%s*\n", posName, posDirection, latest, &profit[3]) ;
		if (profit[0] == '-')
		{
			double loss = atof (&profit[3]) ;
			if (loss > 50.0)
			{
				printf ("%s loss is %f\n", posName, loss) ;
				SetAndClick (PositionCloseX, y) ;
			}
		}
		else
		{
			double gain = atof (&profit[2]) ;
			if (gain > 100.0)
			{
				printf ("%s profit is %f\n", posName, gain) ;
				SetAndClick (PositionCloseX, y) ;
			}
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
        loadCoords("tradeCoords.txt");
	while(1)
	{
		checkPositions();
		Sleep (15000);
	}
	return 0;
}
