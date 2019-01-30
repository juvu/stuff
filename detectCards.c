#include <stdio.h>
#include <Windows.h>
 
COLORREF getColourAtCursor(void) {
    POINT p;
    COLORREF color;
    HDC hDC;
    BOOL b;
 
    /* Get the device context for the screen */
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return CLR_INVALID;
 
    /* Get the current cursor position */
    b = GetCursorPos(&p);
    if (!b)
        return CLR_INVALID;
 
    printf ("Getting colours\n");

	LPVOID lpvBits[1000];
	HBITMAP bitmap = CreateBitmap (20, 40, 1, 32, lpvBits) ;

	//HGDIOBJ bitobj = SelectObject (hDC, bitmap) ;

	HDC hDC2 = CreateCompatibleDC(hDC);
	SelectObject (hDC2, bitmap) ;

	BitBlt (hDC2, 0, 0, 20, 40, hDC, p.x, p.y, SRCCOPY) ;

/*
	LPVOID lpvBits[1000];
	LONG numBytes = GetObject (bitobj, 10, lpvBits) ;

	//LONG numBytes = GetBitmapBits (bitmap, 10, lpvBits) ;

	int i=0;
	for (i=0;i<10;i++)
	{
		
		printf ("Bit %d is %x\n", i, lpvBits[i]) ;
	}
*/
	int i = 0 ;
	int j = 0;
	for (i=0;i<40;i++)
	{
		for (j=0;j<20;j++)
		{
 			color = GetPixel(hDC2, j, i);
			printf ("Colour is %x\n",color);
		}
	}
			
  color = GetPixel(hDC, p.x, p.y);
	printf ("Got colours\n") ;
    /* Retrieve the color at that position */
	printf ("Coordinate is %d, %d\n", p.x, p.y) ;
    color = GetPixel(hDC, p.x, p.y);
 
    /* Release the device context again */
    ReleaseDC(GetDesktopWindow(), hDC);
  ReleaseDC(GetDesktopWindow(), hDC2);
 
    return color;
}

int main()
{
	while (1)
	{
  		 printf("Hello World!");
		COLORREF colour = getColourAtCursor() ;

		printf ("Colour is %x\n",colour);
		Sleep (1000);
	}
   return 0;
}
