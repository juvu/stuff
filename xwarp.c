#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include <Xm/Label.h>
#include <string.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/time.h>

/* External Declarations */

extern char    *getlogin();
extern char    *getenv();
extern char    *malloc();

/* Forward Declarations */

static void     Usage();
static int      ValidateArgs();
static void     SetupWarp();
static char    *GetRecipientDisplay();
static void     ReadDatabaseEntry();

/* Macro Definitions */

#define TRUE       1
#define FALSE      0
#define MAX_LINE 256

#define XMSG_CLASS "Xmsg"
#define XMSG_NAME  "xmsg"

#define XMSG_DISPLAY_DB "XMSG_DB"

main(argc, argv)
int             argc;
char          **argv;
{
	char           *recipient;

	if (!ValidateArgs(argc, argv, &recipient))
	{
		Usage();
		exit(-1);
	}

	SetupWarp(recipient, argc, argv);
}

static void     SetupWarp(recipient, argc, argv)
char           *recipient;
int             argc;
char          **argv;
{
	Display        *display1, *display2;
	char           own_name[100];
	char           *dpy_name = GetRecipientDisplay(recipient);
	Window			children[100] ;
	Widget			widgets[100] ;
	int				next = 0 ;
	int				loop ;
	struct timeval	tp, tp1 ;
	struct timezone	tzp ;
	XEvent			event ;
	char			command[200] ;
	char			fhost[50] ;
	int 			go ;

	strcpy (fhost, dpy_name) ;

	/* note the current time */

	gettimeofday (&tp, &tzp) ;

	if (!dpy_name)
	{
		fprintf(stderr, "Could not determine recipient's display.\n");
		exit(-2);
	}

	gethostname(own_name, 100);
	strcat (own_name, ":0") ;

	XtToolkitInitialize() ;

	display1 = XOpenDisplay(dpy_name);
	if (!display1)
	{
		fprintf(stderr, "Failed to open recipient's display.\n");
		exit(-3);
	}

	display2 = XOpenDisplay(own_name); 
	if (!display2)
	{
		fprintf(stderr, "Failed to open own display.\n");
		exit(-3);
	}

	go = 0 ;
	while (1)
	{
		Window	root, child ;
		int		x, y, xw, yw, keys ;

		/* what time is it */

		gettimeofday (&tp1, &tzp) ;


		/* query the pointer position on our display */

		XQueryPointer (display2, RootWindow (display2, 0), &root, &child,
						&x, &y, &xw, &yw, &keys) ;

		/* move the cursor on the target display */

		if (tp1.tv_sec - tp.tv_sec < 7)
		{
			XWarpPointer (display1, None, RootWindow (display1, 0), 
						x, y,  DisplayWidth(display1, 0),
						DisplayHeight(display1, 0), x, y) ;
		}

		/* query the pointer on the child display */

		XQueryPointer (display1, RootWindow (display1, 0), &root, &child,
						&x, &y, &xw, &yw, &keys) ;


		/* are we positioned over a child window */

		if (child)
		{
			/* wicked ! */

			/* XUnmapWindow (display1, child) ; */
			/* XIconifyWindow (display1, child, 0) ;*/
			XMoveWindow (display1, child, x,y) ;
		}
	}
}

static char    *GetRecipientDisplay(recipient)
char           *recipient;
{
	char           *recip;
	char           *dpy;
	FILE           *fp;
	char           *xmsg_db = getenv(XMSG_DISPLAY_DB);
	short           found = FALSE;

	if (!xmsg_db)
	{
		fprintf(stderr, "Environment variable %s not set.\n", XMSG_DISPLAY_DB);

		exit(-4);
	}

	fp = fopen(xmsg_db, "r");
	if (!fp)
	{
		fprintf(stderr, "Could not open display database %s\n", xmsg_db);
		exit(-5);
	}

	while ((!feof(fp)) && !found)
	{
		ReadDatabaseEntry(fp, &recip, &dpy);
		if (!strcmp(recip, recipient))
		{
			found = TRUE;
		}
	}

	fclose(fp);
	return (found) ? dpy : NULL;
}

static int      ValidateArgs(argc, argv, recipient)
int             argc;
char          **argv;
char          **recipient;
{
	if (argc != 2)
	{
		return FALSE;
	}

	*recipient = malloc(strlen(argv[1]) + 1);
	strcpy(*recipient, argv[1]);

	return TRUE;
}

static void     Usage()
{
	fprintf(stderr, "Usage:  xmsg <recipient> \"message text\"\n");
}

static void     ReadDatabaseEntry(fp, recip, dpy)
FILE           *fp;
char          **recip;
char          **dpy;
{
	static char     buffer[MAX_LINE];
	char           *seps = " \t\n";

	fgets(buffer, sizeof(buffer), fp);

	*recip = strtok(buffer, seps);
	*dpy = strtok((char *) NULL, seps);

	if (!*recip)
	{
		*recip = "";
	}

	if (!*dpy)
	{
		*dpy = "";
	}
}
