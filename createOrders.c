#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define OFFSET 24
#define INTERVAL 2000
#define SHORT_INTERVAL 500

int PositionX = 192;
int PositionY = 765;

int SignalX = 1100;
int SignalY = 188;

int SignalOpenX = 1255;
int SignalOpenY = 610;

int SignalCopyX = 1212;
int SignalCopyY = 569;

int SignalSetValX = 1438;
int SignalSetValY = 360;

int SignalPlaceOrderX = 1422;
int SignalPlaceOrderY = 698;

int SignalCloseWinX = 1522;
int SignalCloseWinY = 244;

int DeleteX = 1000 ;
int DeleteY = 200 ;

char *getStringAtLocation(char *file, int x, int y, int width, int height, int getText, char *buffer)
{
        char cmd[1024] = "" ;

        double number = 0.0 ;

        if (0)
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

        sprintf (cmd, "tesseract %s.bmp %s", file, file) ;
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
			buffer[xx] = '\0';
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

void PlaceOrder()
{
	SetAndClick(SignalOpenX, SignalOpenY);
	SetAndClick(SignalCopyX, SignalCopyY);
	SetAndClick(SignalSetValX, SignalSetValY);
	SetAndClick(SignalPlaceOrderX, SignalPlaceOrderY);
	SetAndClick(SignalCloseWinX, SignalCloseWinY);
}

int checkForPosition(char *buffer)
{
	int x = PositionX;
	int y = PositionY;
	int count = 0 ;

	char position[1024] = "";

	for (count = 0; count < 15; count++,y+=OFFSET)
	{
		getStringAtLocation ("Position", x, y, 140, 15, 1, position) ;
		printf ("Position is *%s*\n", position);
		printf ("Buffer is *%s*\n", buffer);
		if (strstr (position, buffer) || strstr (buffer, position)) 
			return 1 ;
	}
	return 0;
}

void openNew(int num)
{
	int x = SignalX;
	int y = SignalY;
	int count = 0;
	char buffer[1024] = "";


	for (count = 0;count < num;count++,y+=OFFSET)
	{
		getStringAtLocation ("OrderName", x, y, 140, 15, 1, buffer) ;
		printf ("Order is %s\n", buffer);
		if (!checkForPosition(buffer))
		{
			SetAndClick (x,y);
			PlaceOrder ();
		}
	}		
}

void closeExisting(int num)
{
	int x = DeleteX;
	int y = DeleteY + (num * OFFSET);
	int count = 0;

	for (count = 0;count <= num;count++,y-=OFFSET)
	{
		SetAndClickShort (x,y);
	}		
}

int main(int argc, char **argv)
{

	Sleep (5000);

	while (1)
	{
		closeExisting(10);
		openNew(10);
		Sleep (600000);
	}

	return 0;
}
