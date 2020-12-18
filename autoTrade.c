#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define NUMSLOTS 21 

#define OFFSET 36
#define INTERVAL 2000
#define SHORT_INTERVAL 500

#define OPPOSITE -1
#define SAME 1

int PositionCloseX = 945;

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

int DeleteX = 1000 ;
int DeleteY = 200 ;

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

	for (count = 0; count < NUMSLOTS; count++,y+=OFFSET)
	{
		getStringAtLocation ("Position", x, y, 440, OFFSET, 1, position) ;
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

	for (count = 0; count < NUMSLOTS; count++,y+=OFFSET)
	{
		getStringAtLocation ("Position", x, y, 440, OFFSET, 1, position) ;
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

void processSignals()
{
	int x = SignalX;
	int y = SignalY;
	int count = 0;
	char buffer[1024] = "" ;
	char name[1024] = "" ;
	char tool[1024] = "" ;
	char direction[1024] = "" ;
	char dir2[1024] = "" ;
	int dir = 1;
	int numSignals = 0 ;
	//char signals[NUMSLOTS][1024] = {"","","","","","","","","","","","","","","","","","","",""};
	char signals[NUMSLOTS][1024] = {""};

	for (count = 0;count < NUMSLOTS;count++,y+=OFFSET)
	{
        	setCursorPos(x-20,y) ;
		getStringAtLocation ("Signal", x, y, 440, OFFSET, 1, buffer) ;
		//printf ("Signal is *%s*\n", buffer);
		if (!strstr(buffer,"/"))
			continue;
		name[0] = '\0';
		tool[0] = '\0';
		direction[0] = '\0';
		dir2[0] = '\0';
		sscanf(buffer,"%s %s %s %s", name, tool, direction, dir2);
		int loop = 0;
		int found = 0;
		for (loop=0;loop<numSignals;loop++)
		{
			if(!strcmp(signals[loop],name))
				found = 1 ;
		}
		if (found)
		{
			printf ("REPEAT - Name is %s tool is %s direction is %d\n", name, tool, dir);
			continue;
		}
		strcpy(signals[numSignals++], name);

		dir = 1;
		if (!strcmp(tool,"PIA"))
		{
			if (!strcmp(dir2,"Sell"))
				dir = -1;
		}
		else if (!strcmp(direction,"Sell"))
		{
			dir = -1;
		}
		printf ("Name is %s tool is %s direction is %d\n", name, tool, dir);

		char buff[1024] = "";
		if (dir == 1)
			sprintf (buff, "%s -1", name) ;
		else
			sprintf (buff, "%s 1", name) ;

		if (checkRecord("testRecord.txt",buff))
		{
			int posY = findPosition(name) ;
			if (posY)
			{
				// close the position
				printf ("Closing position %s\n", name);
				removeRecord ("testRecord.txt", buff) ;
				SetAndClick(PositionCloseX, posY);
			}
		}

		sprintf (buff, "%s %d", name, dir) ;
		if (checkRecord("testRecord.txt",buff))
			continue;
		else
		{
			int num = numRecords("testRecord.txt");
			if (num < 20)
			{
				SetAndClick(x+OFFSET, y);
				PlaceOrder(dir);
				sprintf (buff, "%s %d", name, dir) ;
				addRecord ("testRecord.txt", buff) ;
			}
		}
	}		
}

int main(int argc, char **argv)
{
	while(1)
	{
		processSignals();
		Sleep (15000);
	}
	return 0;
}
