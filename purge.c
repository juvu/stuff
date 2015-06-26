#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

main(argc, argv)
int             argc;
char          **argv;
{
	int             days, hours;
	int             time, ptime;
	struct timeval  tv;
	struct timezone tz;
	struct stat     status;
	struct dirent	*dp;
	DIR				*dfd;

	if ((argc != 2) && (argc != 3))
	{
		printf("Usage: purge <days> <hours> \n");
		exit(0);
	}

	/* get the number of days to purge */

	days = atoi(argv[1]);

	/* get the number of hours to purge */

	hours = (argc == 3) ? atoi(argv[2]) : 0;

	/* calculate the time offset in seconds */

	time = (days * 86400) + (hours * 3600);

	/* get the current time */

	gettimeofday(&tv, &tz);

	/* and so calculate the purge time */

	ptime = tv.tv_sec - time;

	/* Right. Open up the current directory to see whats inside */

	if ((dfd = opendir(".")) == NULL)
	{
		fprintf (stderr, "purge: Cant open directory") ;
		exit(0) ;
	}

	while ((dp = readdir(dfd)) != NULL)
	{
		/* get status information on this file */

		if (stat(dp->d_name, &status) == -1)
		{
			fprintf (stderr, "purge: Cant get status info on file %s", dp->d_name) ;
			exit(0) ;
		}

		/* not interested in directories */

		if ((status.st_mode & S_IFMT) == S_IFDIR)
			continue;

		/* also not interested in symbolic links */

		if ((status.st_mode & S_IFMT) == S_IFLNK)
			continue;

		/* is the last modification time before the purge time */

		if (status.st_mtime < ptime)
			remove (dp->d_name) ;
	}
}

