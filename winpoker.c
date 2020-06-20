#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define WIDTH 30
#define HEIGHT 30
#define INPLAYWIDTH 10
#define INPLAYHEIGHT 10

double values[52] = { 897746968.0, 750926950.0, 1976990186.0, 2553233420.0, 2353596976.0, 613585518.0, 1331756022.0,
                        857226225.0, 844377893.0, 727538994.0, 772877715.0, 674470871.0, 1875345240.0, 2293524207.0,
                        973789104.0, 638939048.0, 956819214.0, 700234899.0, 843210913.0, 1590449751.0, 982966914.0,
                        1805218029.0, 607157800.0, 545311932.0, 1633531410.0, 735631533.0, 530488959.0, 984403670.0,
                        1707690606.0, 982208346.0, 982414370.0, 1358563916.0, 638323686.0, 591676449.0, 727078443.0,
                        634458482.0, 1887615836.0, 1926101014.0, 1337768365.0, 2119165880.0, 812055240.0, 2023111333.0,
                        632665488.0, 2001932559.0, 1247593702.0, 2142697414.0, 980973630.0, 2080419489.0, 788839444.0,
                        735599406.0, 1488472188.0, 716635732.0 };

char *card[52] = { "5H", "4C", "5D", "QD", "TD", "5S", "KS",
                        "3H", "6H", "AS", "AC", "5C", "4D", "KD",
                        "8H", "3C", "9H", "9S", "2H", "QH", "TC",
                        "KH", "2C", "7C", "QC", "4S", "7S", "4H",
                        "7D", "JH", "AH", "KC", "3S", "2S", "8S",
                        "6C", "3D", "2D", "QS", "JD", "JC", "9D",
                        "6S", "AD", "TH", "6D", "TS", "8D", "8C",
                        "7H", "JS", "9C" };


double inPlay = 1374193481.0;

int XPos[7] = {449, 516, 344, 413, 482, 551, 620} ;
int YPos[7] = {486, 486, 276, 276, 276, 276, 276} ;

int PotX = 473 ;
int PotY = 240 ;
int EquityX = 47 ;
int EquityY = 274 ;

int NumTurnFolds = 0 ;
int NumTurnFoldsSav = 0 ;
int MaxInPlay = 0 ;
int MaxInPlaySav = 0 ;
int NumPocketFolds = 0 ;
int NumPocketFoldsSav = 0 ;
int NumFlopFolds = 0 ;
int NumFlopFoldsSav = 0 ;
int numInPlay = 0 ;
int numInPlaySav = 0 ;


#define POT_WIDTH 100
#define POT_HEIGHT 25
#define EQUITY_WIDTH 50
#define EQUITY_HEIGHT 25

int playerXPos[5] = {267, 194, 565, 937, 864} ;
int playerYPos[5] = {492, 191, 112, 191, 492} ;

int XBase = -1 ;
int YBase = -1 ;

char cardBuffersav[100] = "AHAS" ;

static char cardBuffer[100] = "" ;


#define INTERVAL 30
#define LONGINTERVAL 30

#define HEARTS 0
#define CLUBS 1
#define DIAMONDS 2
#define SPADES 3

#define CARD_OFFSET 33

int cardsx[8] = {1,0,25,69,93,117,141,166} ;
int cardsy[8] = {1,204,204,204,204,204,204,204} ;

int playersaddx = 72 ;
int playersaddy = -87 ;
int playersdelx = 72 ;
int playersdely = -68;

int playersaddPocketx = 166 ;
int playersaddPockety = -87 ;
int playersdelPocketx = 166 ;
int playersdelPockety = -68;

int playersaddFlopx = 72 ;
int playersaddFlopy = -50 ;
int playersdelFlopx = 72 ;
int playersdelFlopy = -31;

int playersaddTurnx = 166 ;
int playersaddTurny = -50 ;
int playersdelTurnx = 166 ;
int playersdelTurny = -31;

int heartsx[14] = {1,240,0,20,40,60,80,100,120,140,160,180,200,220} ;
int heartsy[14] = {1,72,72,72,72,72,72,72,72,72,72,72,72,72} ;
int clubsx[14] = {1,240,0,20,40,60,80,100,120,140,160,180,200,220} ;
int clubsy[14] = {1,37,37,37,37,37,37,37,37,37,37,37,37,37} ;
int diamondsx[14] = {1,240,0,20,40,60,80,100,120,140,160,180,200,220} ;
int diamondsy[14] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0} ;
int spadesx[14] = {1,240,0,20,40,60,80,100,120,140,160,180,200,220} ;
int spadesy[14] = {1,112,112,112,112,112,112,112,112,112,112,112,112,112} ;


int BaseX = -1 ;
int BaseY = -1 ;

typedef struct
{
	unsigned long value ;
	unsigned long suit ;
} Card ;

Card myCards[8] ;


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
	setCursorPos(x,y) ;
	DoMouseClick(x,y);
	Sleep (INTERVAL);
}

void SetAndClickLong (int x, int y)
{
	setCursorPos(x,y) ;
	DoMouseClick(x,y);
	Sleep (LONGINTERVAL);
}

addCard (Card *cards, char *str, int *nextCard)
{
    cards[*nextCard].value = str[0] - 48;
    if (str[0] == 't' || str[0] == 'T')
        cards[*nextCard].value = 10 ;
    else if (str[0] == 'j' || str[0] == 'J')
        cards[*nextCard].value = 11 ;
    else if (str[0] == 'q' || str[0] == 'Q')
        cards[*nextCard].value = 12 ;
    else if (str[0] == 'k' || str[0] == 'K')
        cards[*nextCard].value = 13 ;
    else if (str[0] == 'a' || str[0] == 'A')
        cards[*nextCard].value = 1 ;

    if (str[1] == 'h' || str[1] == 'H')
        cards[*nextCard].suit = HEARTS ;
    else if (str[1] == 'd' || str[1] == 'D')
        cards[*nextCard].suit = DIAMONDS ;
    else if (str[1] == 'c' || str[1] == 'C')
        cards[*nextCard].suit = CLUBS ;
    else if (str[1] == 's' || str[1] == 'S')
        cards[*nextCard].suit = SPADES ;

    (*nextCard)++;
}

int setupPlayers()
{
	int i = 0 ;

	for (i=1;i<5;i++)
	{
		SetAndClick (BaseX + playersdelTurnx, BaseY + playersdelTurny) ;
	}

	for (i=1;i<5;i++)
	{
		SetAndClick (BaseX + playersdelFlopx, BaseY + playersdelFlopy) ;
	}

	for (i=1;i<5;i++)
	{
		SetAndClick (BaseX + playersdelPocketx, BaseY + playersdelPockety) ;
	}

	for (i=1;i<5;i++)
	{
		SetAndClick (BaseX + playersdelx, BaseY + playersdely) ;
	}


	for (i=2;i<=MaxInPlay;i++)
	{
		SetAndClick (BaseX + playersaddx, BaseY + playersaddy) ;
	}

	for (i=1;i<=NumPocketFolds;i++)
	{
		SetAndClick (BaseX + playersaddPocketx, BaseY + playersaddPockety) ;
	}

	for (i=1;i<=NumFlopFolds;i++)
	{
		SetAndClick (BaseX + playersaddFlopx, BaseY + playersaddFlopy) ;
	}

	for (i=1;i<=NumTurnFolds;i++)
	{
		SetAndClick (BaseX + playersaddTurnx, BaseY + playersaddTurny) ;
	}


	NumTurnFoldsSav = NumTurnFolds ;
	NumFlopFoldsSav = NumFlopFolds ;
	NumPocketFoldsSav = NumPocketFolds ;
	MaxInPlaySav = MaxInPlay ;

}

int clearCards(int first, int last)
{
	int i = 0 ;

	for (i=first;i>=last;i--)
	{
		SetAndClick (BaseX + cardsx[i], BaseY + cardsy[i]) ;
	}
}

int playCards(Card *cards, int first, int last)
{
	int i = 0 ;

	for (i=first-1;i<last;i++)
	{
		printf ("Playing Card %d %d\n", cards[i].value, cards[i].suit) ;

		if (i<4)
		{
			if (cards[i].suit == HEARTS)
				SetAndClick (BaseX + heartsx[cards[i].value], BaseY + heartsy[cards[i].value]) ;
			if (cards[i].suit == SPADES)
				SetAndClick (BaseX + spadesx[cards[i].value], BaseY + spadesy[cards[i].value]) ;
			if (cards[i].suit == CLUBS)
				SetAndClick (BaseX + clubsx[cards[i].value], BaseY + clubsy[cards[i].value]) ;
			if (cards[i].suit == DIAMONDS)
				SetAndClick (BaseX + diamondsx[cards[i].value], BaseY + diamondsy[cards[i].value]) ;
		}
		else
		{
			if (cards[i].suit == HEARTS)
				SetAndClickLong (BaseX + heartsx[cards[i].value], BaseY + heartsy[cards[i].value]) ;
			if (cards[i].suit == SPADES)
				SetAndClickLong (BaseX + spadesx[cards[i].value], BaseY + spadesy[cards[i].value]) ;
			if (cards[i].suit == CLUBS)
				SetAndClickLong (BaseX + clubsx[cards[i].value], BaseY + clubsy[cards[i].value]) ;
			if (cards[i].suit == DIAMONDS)
				SetAndClickLong (BaseX + diamondsx[cards[i].value], BaseY + diamondsy[cards[i].value]) ;
		}
	}
}

int doOdds(int numOppos, char *inputCards)
{
	int inputNumCards = strlen(inputCards) / 2 ;

	int i = 0 ;
	int nextCard = 0 ;

	printf ("In doOdds numInPlay is %d numInPlaySav is %d cards are %s\n", numInPlay, numInPlaySav, inputCards) ;
	printf ("In doOdds maxInPlay is %d NumPocketFOlds is %d NumFlopFolds is %d NumTurnFolds is %d\n", 
					MaxInPlay, NumPocketFolds, NumFlopFolds, NumTurnFolds) ;
	for (i=0;i<inputNumCards*2;i+=2)
	{
		char theCard[3];

		theCard[0] = inputCards[i];
		theCard[1] = inputCards[i+1];
		theCard[2] = '\0';

		addCard (myCards, theCard, &nextCard) ;
	}

	if (numInPlay != numInPlaySav)
		setupPlayers() ;

	if (strcmp (cardBuffer, cardBuffersav))
	{
		clearCards(7, 1) ;
		playCards(myCards, 1, inputNumCards) ;
	}

#if 0
	if (strlen(cardBuffersav) > strlen (cardBuffer))
	{
		int first = strlen(cardBuffersav) / 2 ;
		clearCards(first, 1) ;
	}

	if (inputNumCards == 2)
	{
		setupPlayers(numOppos) ;
		SetAndClick (BaseX + cardsx[1], BaseY + cardsy[1]) ;
		playCards(myCards, 1, 1) ;
		SetAndClick (BaseX + cardsx[2], BaseY + cardsy[2]) ;
		playCards(myCards, 2, 2) ;
	}
	if (inputNumCards == 5)
	{
		setupPlayers(numOppos) ;
		if (strlen(cardBuffersav) != strlen (cardBuffer))
		{
			SetAndClick (BaseX + cardsx[3], BaseY + cardsy[3]) ;
			playCards(myCards, 3, inputNumCards) ;
		}
	}
	else if (inputNumCards == 6)
	{
		setupPlayers(numOppos) ;
		if (strlen(cardBuffersav) != strlen (cardBuffer))
		{
			SetAndClick (BaseX + cardsx[6], BaseY + cardsy[6]) ;
			playCards(myCards, 6, inputNumCards) ;
		}
	}
	else if (inputNumCards == 7)
	{
		setupPlayers(numOppos) ;
		if (strlen(cardBuffersav) != strlen (cardBuffer))
		{
			SetAndClick (BaseX + cardsx[7], BaseY + cardsy[7]) ;
			playCards(myCards, 7, inputNumCards) ;
		}
	}

	if (inputNumCards >=5)
	{
		SetAndClickLong (BaseX + cardsx[3], BaseY + cardsy[3]) ;
		playCards(myCards, 3, inputNumCards) ;
	}
#endif


   return 0;
}


double getTotal(int x, int y) 
{
	POINT p;
	COLORREF color;
	HDC hDC;
	
 
	/* Get the device context for the screen */
	hDC = GetDC(NULL);
	if (hDC == NULL)
		return 0;
 

	LPVOID lpvBits[INPLAYWIDTH*(INPLAYHEIGHT+1)];
	HBITMAP bitmap = CreateBitmap (INPLAYWIDTH, INPLAYHEIGHT, 1, 32, lpvBits) ;

	//HGDIOBJ bitobj = SelectObject (hDC, bitmap) ;

	HDC hDC2 = CreateCompatibleDC(hDC);
	SelectObject (hDC2, bitmap) ;

	BitBlt (hDC2, 0, 0, INPLAYWIDTH, INPLAYHEIGHT, hDC, XBase + x, YBase + y, SRCCOPY) ;

	int i = 0 ;
	int j = 0;
	double total = 0.0;

	total = 0.0;
	for (i=0;i<INPLAYHEIGHT;i++)
	{
		for (j=0;j<INPLAYWIDTH;j++)
		{
 			color = GetPixel(hDC2, j, i);
			total += color;
		}
	}
		

	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return total;
}

int getNumInPlay() 
{
	POINT p;
	COLORREF color;
	HDC hDC;
	
 
	/* Get the device context for the screen */
	hDC = GetDC(NULL);
	if (hDC == NULL)
		return 0;
 

	LPVOID lpvBits[INPLAYWIDTH*(INPLAYHEIGHT+1)];
	HBITMAP bitmap = CreateBitmap (INPLAYWIDTH, INPLAYHEIGHT, 1, 32, lpvBits) ;

	//HGDIOBJ bitobj = SelectObject (hDC, bitmap) ;

	HDC hDC2 = CreateCompatibleDC(hDC);
	SelectObject (hDC2, bitmap) ;

	int numPlayers = 0 ;
	int players = 0 ;

	for (players=0; players<5; players++)
	{
		BitBlt (hDC2, 0, 0, INPLAYWIDTH, INPLAYHEIGHT, hDC, XBase + playerXPos[players], 
							YBase + playerYPos[players], SRCCOPY) ;

		int i = 0 ;
		int j = 0;
		double total = 0.0;

		total = 0.0;
		for (i=0;i<INPLAYHEIGHT;i++)
		{
			for (j=0;j<INPLAYWIDTH;j++)
			{
 				color = GetPixel(hDC2, j, i);
				total += color;
			}
		}
		

		if (inPlay == total)
		{
			numPlayers++ ;
		}

	}

	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return numPlayers;
}

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
		BitBlt (hDC2, 0, 0, WIDTH, HEIGHT, hDC, XBase + XPos[cards], 
							YBase + YPos[cards], SRCCOPY) ;

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
			if (abs(total - 1488472188) < 1000000)
			{
				strcat (cardBuffer, "JS") ;
				cardFound = 1;
			}

			//printf ("Card%d UNKNOWN: %f\n", cards+1, total);
		}

		// forget it if we don't have any cards
		if (cards == 0 && !cardFound)
			break ;
	}

	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return 0;
}

int doCalibrationHH(void) 
{
	POINT p;
	COLORREF color;
	HDC hDC;
	BOOL b;
 
	printf ("CALIBRATING HH.....\n") ;

	/* Get the current cursor position */
	b = GetCursorPos(&p);
	if (!b)
		return 0;

	BaseX = p.x ;
	BaseY = p.y ;

	return 0 ;
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
	        printf ("Found card %s at %d %d\n", card[cardIndex], nearestX, nearestY) ;
		XBase = nearestX - 449 ;
		YBase = nearestY - 486 ;
		printf ("Setting base to %d %d\n", XBase, YBase) ;

		FILE *fp = fopen ("BaseXY.txt", "w") ;
		char buf[1024]="";
		sprintf (buf, "%d %d\n", XBase, YBase) ;
		fputs (buf, fp) ;
		fclose (fp) ;
	}

 
	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return 0;
}

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
	char cmd[1000] = "" ;
	char buffer[1025] = "" ;

	XBase = -1 ;
	YBase = -1 ;

	int BigBlind = atoi (argv[1]) ;

	strcpy (cardBuffersav, "Start") ;

	int i=0;
	for (i=0;i<10;i++)
	{
		doCalibrationHH() ;
		Sleep (1000) ;
	}


#if 0
	clearCards(7,1) ;
	setupPlayers(5) ;
	doOdds (3, "2D3CJCQCKDADQD") ;
	Sleep(2000);
	double Equity = getNumberAtLocation ("Equity", BaseX + EquityX, BaseY + EquityY, EQUITY_WIDTH, EQUITY_HEIGHT, 0) ;
	printf ("Equity is %f\n", Equity) ;
#endif

	// test
	//
#if 0	
	numInPlay = 4 ;
	numInPlaySav = 0 ;
	MaxInPlay = 4 ;
	NumPocketFolds = 0 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	sprintf (cardBuffer, "TS9D") ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);

	numInPlay = 3 ;
	numInPlaySav = 4 ;
	MaxInPlay = 4 ;
	NumPocketFolds = 1 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);

	numInPlay = 2 ;
	numInPlaySav = 3 ;
	MaxInPlay = 4 ;
	NumPocketFolds = 2 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);

	numInPlay = 1 ;
	numInPlaySav = 2 ;
	MaxInPlay = 4 ;
	NumPocketFolds = 3 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);
	
	numInPlay = 5 ;
	numInPlaySav = 1 ;
	sprintf (cardBuffer, "3HQC") ;
	MaxInPlay = 5 ;
	NumPocketFolds = 0 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);
	
	numInPlay = 4 ;
	numInPlaySav = 5 ;
	sprintf (cardBuffer, "3HQC") ;
	MaxInPlay = 5 ;
	NumPocketFolds = 1 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);
	
	numInPlay = 3 ;
	numInPlaySav = 4 ;
	MaxInPlay = 5 ;
	sprintf (cardBuffer, "3HQCADTC2H") ;
	NumPocketFolds = 1 ;
	NumFlopFolds = 1 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);

	numInPlay = 2 ;
	numInPlaySav = 3 ;
	MaxInPlay = 5 ;
	NumPocketFolds = 1 ;
	NumFlopFolds = 2 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);
	
	numInPlay = 1 ;
	numInPlaySav = 2 ;
	MaxInPlay = 5 ;
	sprintf (cardBuffer, "3HQCADTC2H8s") ;
	NumPocketFolds = 1 ;
	NumFlopFolds = 2 ;
	NumTurnFolds = 1 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);
	
	numInPlay = 4 ;
	numInPlaySav = 0 ;
	MaxInPlay = 4 ;
	NumPocketFolds = 0 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	sprintf (cardBuffer, "TS9D") ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);

	numInPlay = 3 ;
	numInPlaySav = 4 ;
	MaxInPlay = 4 ;
	NumPocketFolds = 1 ;
	NumFlopFolds = 0 ;
	NumTurnFolds = 0 ;
	doOdds(1,cardBuffer) ;
	Sleep(3000);
#endif
	
	while (XBase < 0)
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
		int numCards = strlen (cardBuffer) / 2 ;

		if (strlen (cardBuffer) > 1)
			printf ("cardbuffer is %s\n", cardBuffer) ;

		//strcpy (cardBuffer, "AHJC") ;

		if (strlen (cardBuffer) >= 4)
		{
			POINT p ;
			
        		/* Get the current cursor position */
        		GetCursorPos(&p);

			numInPlay = getNumInPlay() ;
			if (numInPlay > numInPlaySav)
			{
				MaxInPlay = numInPlay ;
				NumPocketFolds = 0 ;
				NumFlopFolds = 0 ;
				NumTurnFolds = 0 ;
			}

			if (numCards == 2 && numInPlay < MaxInPlay)
				NumPocketFolds = MaxInPlay - numInPlay ;

			if (numCards == 5 && numInPlay < (MaxInPlay - NumPocketFolds))
				NumFlopFolds = MaxInPlay - NumPocketFolds - numInPlay ;

			if (numCards >= 6 && numInPlay < (MaxInPlay - NumPocketFolds - NumFlopFolds))
				NumTurnFolds = MaxInPlay - NumPocketFolds - NumFlopFolds - numInPlay ;


                        if (strcmp (cardBuffer, cardBuffersav) || (numInPlay != numInPlaySav))
                        {

				doOdds (numInPlay, cardBuffer) ;
				
				strcpy (cardBuffersav, cardBuffer) ;
				numInPlaySav = numInPlay ;
			}

			setCursorPos (p.x, p.y) ;
		}
		Sleep (1000) ;
	}

	return 0;
}
