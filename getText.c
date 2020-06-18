#include <stdio.h>
#include <math.h>
#include <Windows.h>

#define HEIGHT 27 
#define WIDTH 80 


#define BITS_PER_PIXEL 32
#define BYTES_PER_PIXEL (BITS_PER_PIXEL / 8)


PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
BITMAP bmp;
PBITMAPINFO pbmi;
WORD cClrBits;

// Retrieve the bitmap's color format, width, and height.
if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
	return NULL ;

// Convert the color format to a count of bits.
cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
if (cClrBits == 1)
cClrBits = 1;
else if (cClrBits <= 4)
cClrBits = 4;
else if (cClrBits <= 8)
cClrBits = 8;
else if (cClrBits <= 16)
cClrBits = 16;
else if (cClrBits <= 24)
cClrBits = 24;
else cClrBits = 32;

// Allocate memory for the BITMAPINFO structure. (This structure
// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
// data structures.)

if (cClrBits != 24)
pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
sizeof(BITMAPINFOHEADER) +
sizeof(RGBQUAD) * (1<< cClrBits));

// There is no RGBQUAD array for the 24-bit-per-pixel format.

else
pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
sizeof(BITMAPINFOHEADER));

// Initialize the fields in the BITMAPINFO structure.

pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
pbmi->bmiHeader.biWidth = bmp.bmWidth;
pbmi->bmiHeader.biHeight = bmp.bmHeight;
pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
if (cClrBits < 24)
pbmi->bmiHeader.biClrUsed = (1<<cClrBits);

// If the bitmap is not compressed, set the BI_RGB flag.
pbmi->bmiHeader.biCompression = BI_RGB;

// Compute the number of bytes in the array of color
// indices and store the result in biSizeImage.
// For Windows NT/2000, the width must be DWORD aligned unless
// the bitmap is RLE compressed. This example shows this.
// For Windows 95/98, the width must be WORD aligned unless the
// bitmap is RLE compressed.
pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits
+31) & ~31) /8
* pbmi->bmiHeader.biHeight;
// Set biClrImportant to 0, indicating that all of the
// device colors are important.
pbmi->bmiHeader.biClrImportant = 0;
return pbmi;
}

void
WriteBmpTofile(LPSTR pszFile, PBITMAPINFO pbi, HBITMAP hBmp, HDC hDC)
{
    HANDLE hFile;
    BITMAPFILEHEADER hdr;
    PBITMAPINFOHEADER pbih;
    LPBYTE lpBits;
    DWORD dwTemp;

    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if(!lpBits)
    {
        return; // could not allocate bitmap
    }

    GetDIBits(hDC, hBmp, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS);

    hFile = CreateFile(pszFile,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

    if(hFile == INVALID_HANDLE_VALUE)
    {
        return; // Could not open screenshot file
    }

    // type == BM
    hdr.bfType = 0x4d42;

    hdr.bfSize = (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

    // write the bitmap file header to file
    WriteFile(hFile, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);

    // write the bitmap header to file
    WriteFile(hFile, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);

    // copy the bitmap colour data into the file
    WriteFile(hFile, (LPSTR)lpBits, pbih->biSizeImage, &dwTemp, NULL);

    CloseHandle(hFile);

    GlobalFree((HGLOBAL)lpBits);
}

int getText(void) 
{
	POINT p;
	COLORREF color;
	HDC hDC;
	BOOL b;
 
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

	HDC hDC2 = CreateCompatibleDC(hDC);
	SelectObject (hDC2, bitmap) ;
	BitBlt (hDC2, 0, 0, WIDTH, HEIGHT, hDC, p.x, p.y, SRCCOPY) ;

	HWND hwnd = GetDesktopWindow() ;
	PBITMAPINFO pbi = CreateBitmapInfoStruct(hwnd, bitmap);
	WriteBmpTofile("shot.bmp", pbi, bitmap, hDC);

	/* Release the device context again */
	ReleaseDC(GetDesktopWindow(), hDC);
	ReleaseDC(GetDesktopWindow(), hDC2);

	return 0;
}


int main()
{
	int x = 0 ;
	for (x=0;x<10;x++)
	{
		getText() ;
		Sleep (2000);
	}
   return 0;
}
