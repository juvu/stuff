/* Coldump: Fill rectangles with pixel values from 0 to 255
 * 
 * Date  : 22/01/91
 *
 */

#include <X11/X.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <stdio.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL (char *) 0 
#endif

static void colMainLoop() ;
static void colRedrawWindow() ;
static void colDisplayHelp() ;
static void BadArgs() ;
static void colSetCmap() ;

Display       *display ;

static int my_xerr_handler (Display *dpy, XErrorEvent *myerr)
{
        char    msg[80] ;
        XGetErrorText (display, myerr->error_code, msg, 80) ;
        /* fprintf (stderr, "Error code %s\n", msg) ;  */
}

main ( argc,argv )
int    argc ;
char **argv ;
{
	char          *display_name  = NULL ;
	char          *cmapid = NULL ;
	int            screen ;
	Window         root,w ;
	unsigned int   dwidth,dheight,ddepth ;
	unsigned int   wwidth,wheight ;
	unsigned long  black_pv ;
	unsigned long  white_pv ;
	GC             gc ;
	XGCValues      values ;
	unsigned long  vmask ;
	short          use_root = FALSE ;
	int            arg ;

	arg = 1 ;

	while ( arg < argc )
	{
		if ( ! strcmp ( argv[arg],"-dpy" ) || 
			 ! strcmp ( argv[arg],"-display" ) )
		{
			arg++ ;

			if ( arg == argc )
			{
				BadArgs ( "-display expects a display to follow it." ) ;
			}

			display_name = argv[arg] ;
		}
		else if ( ! strcmp ( argv[arg],"-cmap" ) )
		{
			arg++ ;

			if ( arg == argc )
			{
				BadArgs ( "-cmap expects a (hex) colormap id to follow" ) ;
			}

			cmapid = argv[arg] ;
		}
		else if ( ! strcmp ( argv[arg],"-help" ) )
		{
			colDisplayHelp() ;
			exit ( 0 ) ;
		}
		else if ( ! strcmp ( argv[arg],"-r" ) ) 
		{
			use_root = TRUE ;
		}
		else
		{
			char bad_str[100] ;

			sprintf ( bad_str,"Invalid argument %s",argv[arg] ) ;

			BadArgs ( bad_str ) ;
		}

		arg++ ;
	}

	if ( ! (display = XOpenDisplay( display_name )) )
	{
		fprintf ( stderr,"Could not open display." ) ;
		exit ( -1 ) ;
	}

	screen = DefaultScreen(display) ;

	/* Some useful values */

	black_pv = BlackPixel(display,screen) ;
	white_pv = WhitePixel(display,screen) ;
	dwidth   = DisplayWidth(display,screen ) ;
	dheight  = DisplayHeight(display,screen) ;
	ddepth   = DefaultDepth(display,screen) ;
	root     = RootWindow(display,screen) ;

	values.background = black_pv ;

	vmask = GCBackground ;
	gc    = XCreateGC ( display,root,vmask,& values ) ;

	if ( ! use_root )
	{
		/* Preferred geometry */

		wwidth  = dwidth / 2 ;
		wheight = dheight / 2 ;

		/* Create our work window */

		w = XCreateSimpleWindow ( display,RootWindow(display,screen),
				dwidth/4,dheight/4,wwidth,wheight,2,black_pv,white_pv ) ;
								

		XStoreName ( display,w,"Coldump" ) ;
	}
	else  /* Drawing to root */
	{
		Pixmap pixmap = XCreatePixmap ( display,root,dwidth,dheight,ddepth ) ;

		colRedrawWindow ( display,screen,pixmap,gc,black_pv,white_pv ) ;
		colRedrawWindow ( display,screen,root,gc,black_pv,white_pv ) ;

		XSetWindowBackgroundPixmap ( display,root,pixmap ) ;

		XSync ( display,False ) ;

		exit ( 0 ) ;
	}

        XSetErrorHandler(my_xerr_handler) ;

	XSelectInput ( display,w,ButtonPressMask | ExposureMask ) ;

	colSetCmap ( display,w,cmapid ) ;

	XMapWindow ( display,w ) ;

	colMainLoop ( display,screen,w,gc,black_pv,white_pv ) ;
}

static void colMainLoop ( display,screen,w,gc,black_pv,white_pv )
Display      *display ;
int           screen ;
Window        w ;
GC            gc ;
unsigned long black_pv ;
unsigned long white_pv ;
{
	char finished = FALSE ;

	while ( ! finished )
	{
		XEvent event ;

		XNextEvent ( display,& event ) ;

		switch ( event.type )
		{
			case Expose :
				if ( ! event.xexpose.count )
					colRedrawWindow ( display,screen,w,gc,black_pv,white_pv ) ;
				break ;

			case ButtonPress : 
				colRedrawWindow ( display,screen,w,gc,black_pv,white_pv ) ;
				break ;
		}
	}
}

static void colRedrawWindow ( display,screen,w,gc,black_pv,white_pv )
Display      *display ;
int           screen  ;
Window        w ;
GC            gc ;
unsigned long black_pv ;
unsigned long white_pv ;
{
	Window       root ;
	int          wx,wy ;
	unsigned int wwidth,wheight ;
	unsigned int bwidth ;
	unsigned int depth ;
	unsigned int x,y ;
	unsigned int x_step,y_step ;
	unsigned long	pixel ;
	int		j ;
	int		used[256] ;

        for (j = 0; j < 256; j++)
                used[j] = 1;
        while (XAllocColorCells(display, DefaultColormap (display, 0), False, NULL, 0, &pixel, 1))
                used[pixel] = 0;

        for (j = 0; j < 256; j++)
        {
                if (!used[j])
                {
                        pixel = j ;
                        XFreeColors(display, DefaultColormap (display, 0), &pixel, 1, (unsigned long) NULL);
                }
        }


	XGetGeometry ( display,w,& root,& wx,& wy,& wwidth,& wheight,
					& bwidth,& depth ) ;

	x_step = wwidth  / 16 ;
	y_step = wheight / 16 ;

	for ( y = 0 ; y < 16 ; y++ )
	{
		for ( x = 0 ; x < 16 ; x++ )
		{
			unsigned long pixel_v = y*16 + x ;

			XSetForeground ( display,gc,pixel_v ) ;
			XFillRectangle ( display,w,gc,x*x_step,y*y_step,x_step,y_step ) ;

			/* Outline rectangle */

			XSetForeground ( display,gc,white_pv ) ;
			XDrawRectangle ( display,w,gc,x * x_step,y * y_step,
								x_step - 1,y_step - 1 );
			if (used[pixel_v])
			{
				XColor		xcol ;
				int		intensity ;

				xcol.pixel = pixel_v ;
				XQueryColor (display, DefaultColormap(display,0), &xcol) ;
				intensity = (0.3 * xcol.red) + (0.59 * xcol.green)  + (0.11 * xcol.blue) ;
				XSetForeground (display,gc,(intensity<32000) ? white_pv : black_pv) ;
				XDrawString (display,w,gc,x*x_step + 3, y*y_step + 10, "A", 1) ;
			}
		}
	}
}

static void colSetCmap ( display,w,cmapid )
Display *display ;
Window   w ;
char    *cmapid ;
{
	if ( cmapid )
	{
		XSetWindowAttributes w_atts ;
		unsigned long        w_mask ;
		Colormap             cmap ;

		/* Ignore 0x prefix */

		if ( cmapid[0] == '0' && cmapid[1] == 'x' )
			cmapid = cmapid + 2 ;
		
		sscanf ( cmapid,"%lx",& cmap ) ;

		w_atts.colormap = cmap ;
		w_mask = CWColormap ;

		XChangeWindowAttributes ( display,w,w_mask,& w_atts ) ;
	}
}

static void colDisplayHelp() 
{
	printf ( "\n" ) ;
	printf ( "Coldump:  Displays 256 pixel values from given colormap\n\n" ) ;

	printf ( "Usage: coldump [-display|-dpy display] [-r] [-cmap cmap_id]\n\n");

	printf ("The -cmap option can be used by applying the program xprop to\n");
	printf ("the root window of the required screen and use the id's of any\n");
	printf ("colormaps stored in its properties.\n\n" ) ;
	printf ( "The -r option will display the colormap in the root window.\n" ) ;
	printf ( "This option may not be used in conjunction with -cmap.\n\n" ) ;
}

static void BadArgs ( err_str )
char *err_str ;
{
	fprintf ( stderr,"\n%s\n\n",err_str ) ;

	exit ( -2 ) ;
}

