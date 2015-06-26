
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include <Xm/Label.h>
#include <string.h>
#include <pwd.h>
#include <stdio.h>

XtAppContext    app_context ;
Display         *display ;
Display         *display2 ;

void EventProc (Widget w, XtPointer client_data, XEvent *ev, Boolean *ctd) ;

main ( argc,argv )
int    argc ;
char **argv ;
{
	XConfigureRequestEvent	ev ;
	Arg		args[5] ;
	Cardinal	n = 0 ;
	extern Widget	mainShell ;
	extern Widget	DrArW ;

	XtToolkitInitialize() ;
	app_context = XtCreateApplicationContext() ;
        display = XtOpenDisplay (app_context, NULL, argv[0], "Form",
                                 NULL, 0, &argc, argv);


	display2 = XOpenDisplay (argv[1]) ;	

	create_mainShell (display, argv[0], argc, argv) ;
	XtAddEventHandler (DrArW, ButtonPressMask | ButtonReleaseMask
                                | KeyPressMask | PointerMotionMask | LeaveWindowMask | EnterWindowMask,
                                False, EventProc, 0) ;

	XtRealizeWidget(mainShell) ;
	XtAppMainLoop (app_context) ;
#if 0
	while (1)
	{
		int	rx, ry, wx, wy ;
		unsigned int 	kb ;
		Window		root, child ;

       		XQueryPointer (display, RootWindow(display,0), &root, &child, &rx, &ry, &wx, &wy, &kb) ; 
		XIconifyWindow (display, child, 0) ;
	}
#endif
}

void EventProc (Widget w, XtPointer client_data, XEvent *ev, Boolean *ctd)
{
	int	rx, ry, wx, wy ;
	unsigned int 	kb ;
	Window		root, child ;

	/* tee hee */

       	/* XQueryPointer (display2, RootWindow(display2,0), &root, &child, &rx, &ry, &wx, &wy, &kb) ; */
	XSendEvent (display2, PointerWindow, True, ButtonPressMask | ButtonReleaseMask | KeyPressMask
				| PointerMotionMask | LeaveWindowMask | EnterWindowMask, ev) ;
}

