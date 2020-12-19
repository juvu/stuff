#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <ctype.h>

#define INTERVAL 2000
#define SHORT_INTERVAL 500

int PositionCloseX = 0;
int RowOffset = 0;

int PositionX = 244;
int PositionY = 811;

int SignalX = 1100;
int SignalY = 188;

int SignalOpenX = 1255;
int SignalOpenY = 610;

int SignalBuyX = 1212;
int SignalBuyY = 569;

int SignalSellX = 1212;
int SignalSellY = 569;

int SignalSetValX = 1438;
int SignalSetValY = 360;

int SignalPlaceOrderX = 1422;
int SignalPlaceOrderY = 698;

int SignalCloseWinX = 1522;
int SignalCloseWinY = 244;

void strToUpper(char *str)
{
        // Convert to upper case
        char *s = str;
        while (*s)
        {
                *s = toupper((unsigned char) *s);
                s++;
        }
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

char *getStringAtLocation(char *file, int x, int y, int width, int height, int getText, char *buffer)
{
        char cmd[1024] = "" ;

        double number = 0.0 ;

        if (getText == 0)
                sprintf (cmd, "getText %s %d %d %d %d %d", file, x, y, width, height, getText) ;
        else
                sprintf (cmd, "nircmd savescreenshot %s.bmp %d %d %d %d", file, x, y, width, height) ;

	printf ("Issuing cmd %s\n", cmd) ;

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


void readPositions(int num)
{
	int x = PositionX;
	int y = PositionY;
	int count = 0;
	int positions = 0 ;
	char buffer[1024] = "";
	char posName[1024] = "";
	char *ptr = NULL;

	for (count = 0;count < num;count++,y+=RowOffset)
	{
		getStringAtLocation ("PositionName", x, y, 740, 30, 1, buffer) ;
                strToUpper(buffer);
                printf ("Position is *%s*\n", buffer);

	        ptr = strstr (buffer, "+");
		if (!ptr)
	        	ptr = strstr (buffer, "-");
        	if (ptr)
        	{
                	*ptr = '\0';
                	for (ptr--;ptr>buffer;ptr--)
                	{
                        	if (*ptr == ' ')
                                	*ptr = '\0';
                        	else
                                	break;
                	}
                	strcpy (posName,buffer) ;
			printf ("Position is *%s*\n", posName);
        	}
	}		
}

int main(int argc, char **argv)
{
        loadCoords("tradeCoords.txt");
	readPositions(20);
	return 0;
}
