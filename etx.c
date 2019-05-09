#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define WIDTH 54
#define HEIGHT 73

POINT p;
int cursorpos = 0 ;

COLORREF getColourAtCursor(void) {

    COLORREF color;
    HDC hDC;
    BOOL b;
 
    /* Get the device context for the screen */
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return CLR_INVALID;
 

    /* Get the current cursor position */
if (!cursorpos)
{
    b = GetCursorPos(&p);
    if (!b)
        return CLR_INVALID;
} 



  color = GetPixel(hDC, p.x, p.y);

 
    /* Release the device context again */
    ReleaseDC(GetDesktopWindow(), hDC);


    return color;
}

static int trend10000[9999] = {0} ;
static int trend1000[999] = {0} ;
static int trend100[99] = {0} ;

int main()
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;

	POINT asdf;
	asdf.x=1273;
	asdf.y=373;

	SetCursorPos (asdf.x, asdf.y) ;

  INPUT    Input={0};
  // left down 
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  SendInput(1,&Input,sizeof(INPUT));

  // left up
  ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  SendInput(1,&Input,sizeof(INPUT));

	//ClickOnPoint(asdf);

	while (1)
	{
  		//printf("Hello World!");
		COLORREF colour = getColourAtCursor() ;

		//printf ("Colour is %x\n",colour);

		//printf ("Colour is %d\n",colour);

		if (colour == 2171169 || colour == 0)
		{
			trend10000[i++] = 0 ;
			trend1000[j++] = 0 ;
			trend100[k++] = 0 ;
		}
		else if (colour < 2171169)
		{
			trend10000[i++] = -1 ;
			trend1000[j++] = -1 ;
			trend100[k++] = -1 ;
		}
		else
		{
			trend10000[i++] = 1 ;
			trend1000[j++] = 1 ;
			trend100[k++] = 1 ;

		}
		if (i >= 10000)
			i=0;
		if (j >= 1000)
			j=0;
		if (k >= 100)
		{
			k=0;
			int itotal = 0 ;
			int jtotal = 0 ;
			int ktotal = 0 ;
			int loop = 0 ;
			cursorpos = 1 ;
			for (loop=0;loop<10000;loop++)
			{
				itotal += trend10000[loop] ;
			}
			for (loop=0;loop<1000;loop++)
			{
				jtotal += trend1000[loop] ;
			}
			for (loop=0;loop<100;loop++)
			{
				ktotal += trend100[loop] ;
			}
			printf ("Trend 10000 (500 secs) %d, Trend 1000 (50 secs) %d, Trend 100 (5 secs) %d\n", itotal, jtotal, ktotal) ;
		}
		Sleep (50);
	}
   return 0;
}
