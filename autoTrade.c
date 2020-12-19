#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
#include <ctype.h>

#define NUMSLOTS 21 

#define INTERVAL 2000
#define SHORT_INTERVAL 500

#define OPPOSITE -1
#define SAME 1
/*
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
*/
int PositionCloseX = 0;
int RowOffset = 0;

int PositionX = 0;
int PositionY = 0; 

int SignalX = 0;
int SignalY = 0;

int SignalBuyX = 0;
int SignalBuyY = 0;

int SignalSellX = 0;
int SignalSellY = 0;

int SignalSetValX = 0;
int SignalSetValY = 0;

int SignalPlaceOrderX = 0;
int SignalPlaceOrderY = 0;

int SignalCloseWinX = 0;
int SignalCloseWinY = 0;

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

int findPosition(char *name)
{
	int x = PositionX;
	int y = PositionY;
	int count = 0 ;
	char posDirection[1024] = "" ;
	int iDir = 0;
	char posName[1024] = "" ;
	char rest[1024] = "" ;

	char position[1024] = "";

	for (count = 0; count < NUMSLOTS; count++,y+=RowOffset)
	{
		getStringAtLocation ("Position", x, y, 740, RowOffset, 1, position) ;
		strToUpper(position);
		printf ("Position is *%s*\n", position);
		posName[0] = '\0';
		posDirection[0] = '\0';
		rest[0] = '\0';
		sscanf(position,"%s %s %s", posName, posDirection, rest);
		if (!strcmp(name, posName))
			return y;
	}
	return 0;
}

int checkForPosition(char *name, int direction, int checkDirection)
{
	int x = PositionX;
	int y = PositionY;
	int count = 0 ;
	char posDirection[1024] = "" ;
	int iDir = 0;
	char posName[1024] = "" ;
	char rest[1024] = "" ;

	char position[1024] = "";

	for (count = 0; count < NUMSLOTS; count++,y+=RowOffset)
	{
		getStringAtLocation ("Position", x, y, 440, RowOffset, 1, position) ;
		printf ("Position is *%s*\n", position);
		posName[0] = '\0';
		posDirection[0] = '\0';
		rest[0] = '\0';
		sscanf(position,"%s %s %s", posName, posDirection, rest);
		iDir = atoi (posDirection);
		if (iDir > 1)
			iDir = 1;
		if (iDir < 1)
			iDir = -1;
		printf ("Position Name is *%s* name is *%s* direction is %d iDir is %d\n", posName, name, direction, iDir);
		if (checkDirection == OPPOSITE)
		{
			if (!strcmp(name, posName) && iDir != direction)
				return y;
		}
		else
		{
			if (!strcmp(name, posName) && iDir == direction)
				return y;
		}
	}
	return 0;
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

int addRecord (char *file, char *record)
{
	FILE *fp = fopen (file, "a+");
	if (fp)
	{
		fputs (record,fp) ;
		fputs ("\n", fp);
	}
	fclose (fp) ;
}

int numRecords (char *file)
{
	char buffer[1024] = "";
	FILE *fp = fopen (file, "r");
	int num = 0;

	while (fgets(buffer, 1024, fp))
	{
		num++;
	}
	fclose (fp) ;
	return (num);
}

int checkRecord (char *file, char *record)
{
	FILE *fp = fopen (file, "r");
	char buffer[1024] = "";

	while (fgets(buffer, 1024, fp))
	{
		strToUpper(buffer);
		char *ptr = strstr (buffer, "\n");
		if (ptr)
			*ptr = '\0';

		if (!strcmp (record, buffer)) 
		{
			fclose (fp) ;
			return (1);
		}
	}
	fclose (fp) ;
	return (0);
}

int removeRecord (char *file, char *record)
{
	FILE *fp = fopen (file, "r");
	FILE *fp2 = fopen ("tmpFile.txt", "w");
	char buffer[1024] = "";

	while (fgets(buffer, 1024, fp))
	{
		char *ptr = strstr (buffer, "\n");
		if (ptr)
			*ptr = '\0';

		if (strcmp (record, buffer))
		{
			fputs (buffer,fp2) ;
			fputs ("\n", fp2);
		}
	}
	fclose (fp) ;
	fclose (fp2) ;

	char cmd[1024] = "" ;
        sprintf (cmd, "cp tmpFile.txt %s", file) ;
        FILE *ppb = popen (cmd, "r") ;

        // now read the results
        while (fgets (buffer, 1024, ppb))
        {
        }

        pclose (ppb) ;
}

void getNameAndDirection (char *buffer, char *name, char *direction)
{
	char *ptr = NULL ;

	strcpy (name, "Unknown");
	strcpy (direction, "Unknown");

	if (strstr(buffer,"Sell"))
		strcpy (direction,"Sell") ;
	if (strstr(buffer,"Buy"))
		strcpy (direction,"Buy") ;

	ptr = strstr (buffer, "Autochartist");
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
		strcpy (name,buffer) ;
	}
	ptr = strstr (buffer, "PIA");
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
		strcpy (name,buffer) ;
	}
}

void processSignals()
{
	int x = SignalX;
	int y = SignalY;
	int count = 0;
	char buffer[1024] = "" ;
	char name[1024] = "" ;
	char direction[1024] = "" ;
	int dir = 1;
	int numSignals = 0 ;
	char signals[NUMSLOTS][1024] = {""};

	numSignals = 0 ;

	for (count = 0;count < NUMSLOTS;count++,y+=RowOffset)
	{
        	//setCursorPos(x-20,y) ;
		getStringAtLocation ("Signal", x, y, 440, RowOffset, 1, buffer) ;
		getNameAndDirection(buffer, name, direction);
		printf ("Name is %s direction is %s\n", name, direction);
		strToUpper(name);

		if (!strcmp (name,"Unknown"))
			continue;
		if (!strcmp (direction,"Unknown"))
			continue;

		// first check our list of allowed trades
		int allowed = checkRecord("tradesAllowed.txt",name);
		if (!allowed)
			continue;

		// check we haven't already processed it
		int loop = 0;
		int found = 0;
		for (loop=0;loop<numSignals;loop++)
		{
			if(!strcmp(signals[loop],name))
				found = 1 ;
		}
		if (found)
		{
			printf ("REPEAT - Name is %s direction is %s\n", name, direction);
			continue;
		}

		// add to list of processed signals
		strcpy(signals[numSignals++], name);

		// convert direction to integer
		if (!strcmp (direction,"Buy"))
			dir = 1;
		if (!strcmp (direction,"Sell"))
			dir = -1;


		// now check for an opposite trade in force
		// if so remove it
		char buff[1024] = "";
		if (dir == 1)
			sprintf (buff, "%s -1", name) ;
		else
			sprintf (buff, "%s 1", name) ;

		if (checkRecord("tradesRecord.txt",buff))
		{
			int posY = findPosition(name) ;
			if (posY)
			{
				// close the position
				printf ("Closing position %s\n", name);
				SetAndClick(PositionCloseX, posY);
			}
			// note - we may not find the position because
			// autostop has removed it. But now we need to
			// remove it from the trading record
			removeRecord ("tradesRecord.txt", buff) ;
		}

		// and now place the recommended trade if we do not
		// already have it
		sprintf (buff, "%s %d", name, dir) ;
		if (checkRecord("tradesRecord.txt",buff))
			continue;
		else
		{
			SetAndClick(x+RowOffset, y);
			PlaceOrder(dir);
			sprintf (buff, "%s %d", name, dir) ;
			addRecord ("tradesRecord.txt", buff) ;
		}
	}		
}

void keepalive()
{
	SetAndClick(SignalX+RowOffset, SignalY);
	SetAndClick(SignalCloseWinX, SignalCloseWinY);
}


int main(int argc, char **argv)
{
	loadCoords("tradeCoords.txt");
	while(1)
	{
		processSignals();
		Sleep (15000);
		keepalive();
	}
	return 0;
}
