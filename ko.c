#include <stdio.h>
#include <strings.h>
#include <math.h>

#define		TOP   1
#define		LEFT  2
#define		RIGHT 3

typedef struct
{
	int 	ranking ;
	char	name[200] ;
	double	prob_level[10] ;
} Player ;

typedef struct Node
{
	struct Node		*left ;
	struct Node		*right ;
	int				 player ;
} NODE ;

NODE *node[8][128] ;
int	  left[128] ;
int   right[128] ;
int	  nlefts, nrights ;
Player	player[256] ;

int	method = 1 ;
double	total = 0.0 ;
char    result[256] = "" ;
char	buffer[256] = "" ;

main(argc, argv)
int 	argc ;
char	**argv ;
{
	FILE	*stream = NULL;
	int     rd = 0 , loop = 0 , loop2 = 0;
	int		no_of_players = 0;
	int		level = 0 ;
	char	pr[30] = "";
	char	*ptr = NULL;
	int		lval=0;
	int		html=0;
	int		lastyear=0 ;

	/* open the file containing the draw */

	if (argc < 2 || argc > 4)
	{
		printf ("Usage: ko <filename>\n");
		printf ("or: 	ko <filename> <method>\n");
		printf ("or: 	ko -html <filename>\n");
		printf ("or: 	ko -html <filename> <method>\n");
		exit (0) ;
	}

	if (!strcmp (argv[1], "-html"))
	{
		html = 1 ;
		stream = fopen (argv[2], "r") ;
		if (argc == 4)
			method = atoi (argv[3]) ;
	}
	else
	{
		if (argc == 3)
			method = atoi (argv[2]) ;
		stream = fopen (argv[1], "r") ;
	}


	/* read int the data from this file */

	loop=0;

	while (fgets (buffer, 256, stream))
	{
		char *rest = NULL ;

		ptr = (char *) strtok_r (buffer, ":", &rest) ;
		sprintf (player[loop].name, "%-30s", ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		player[loop].ranking = atoi (ptr) ;
		ptr = (char *) strtok_r (NULL, ":", &rest) ;
		lastyear = atoi (ptr) ;

		/* if last years results are available, treat this years as
			twice as significant */

		if (lastyear)
		{
			player[loop].ranking *= 2 ;
			player[loop].ranking += lastyear ;
		}
		loop++ ;
	}

	/* set up some info */

	no_of_players = loop;

	for (level=1, lval=no_of_players ;;level++)
	{
		lval = lval / 2 ;
		if (lval <= 1)
			break ;
	}

	/* ok, fairly obviously everyone has a 1.0 chance of getting
       to this bottom level */

	for (loop=0;loop<no_of_players;loop++)
		player[loop].prob_level[level] = 1.0 ;

	/* create the tree */

	create_tree(level) ;

	/* calculate the probabilties */

	for (loop = level-1; loop >=0; loop--)
		calculate_probs (loop) ;

	/* print out the results */

	if (html)
	{
		printf ("<center><TABLE COLS=%d\n border=3>", level+2) ;
		printf ("<TR><TD><center>Player<center></TD><TD><center>Ranking</center></TD>\n") ;
		for (loop2=level-1;loop2>=0;loop2--)
		{
			if (loop2 > 2)
				printf ("<TD><center>Round %d</center></TD>\n", level - loop2) ;
			else if (loop2 == 2)
				printf ("<TD><center>Quarter Final</center></TD>\n") ;
			else if (loop2 == 1)
				printf ("<TD><center>Semi Final</center></TD>\n") ;
			else if (loop2 == 0)
				printf ("<TD><center>Final</center></TD>\n") ;
		}
		printf ("</TR>\n") ;
		for (loop=0;loop<no_of_players;loop++)
		{
			printf ("<TR><TD><center>%s</center></TD><TD><center>%d</center></TD>\n", player[loop].name, player[loop].ranking) ;
			for (loop2=level-1;loop2>=0;loop2--)
			{
				double odds ;
				odds = player[loop].prob_level[loop2] * 100.0 ;
				printf ("<TD><center>%2.2f</center></TD>\n", odds) ;
			}
			printf ("</TR>\n") ;
		}
		printf ("</TABLE>\n") ;
	}
	else
	{
		for (loop=0;loop<no_of_players;loop++)
		{
			sprintf (result, "%s", player[loop].name) ;
	
			for (loop2=level-1;loop2>=0;loop2--)
			{
				sprintf (pr, "%2.2f\t", player[loop].prob_level[loop2] * 100.0) ;	
				strcat (result, pr) ;
			}

			total += player[loop].prob_level[0] ;	
			strcat (result, "\n") ;
			printf (result) ;
		}
		printf ("total %f\n", total) ;
	}

	/* close the input stream */

	fclose (stream) ;
}

create_tree(levels)
int levels ;
{
	int loop, loop2, i ;
	int	no_of_nodes ;

	/* create the binary tree which represents the draw */

	for (loop=levels;loop>=0;loop--)
	{
		for (i=0, no_of_nodes=1;i<levels;i++)
		{
			no_of_nodes = no_of_nodes * 2 ;
		}

		/* no_of_nodes = (int) pow (2.0, (double) loop) ; */

		for (loop2=0;loop2<no_of_nodes;loop2++)
		{
			node[loop][loop2] = (NODE *) malloc (sizeof(NODE)) ;

			if (loop == levels)
			{
				node[loop][loop2]->left = NULL ;
				node[loop][loop2]->right = NULL ;
				node[loop][loop2]->player = loop2 ;
			}
			else
			{
				node[loop][loop2]->left = node[loop+1][loop2*2] ;
				node[loop][loop2]->right = node[loop+1][loop2*2 + 1] ;
				node[loop][loop2]->player = -1 ;
			}
		}
	}
}

walk_tree(node, branch)
NODE *node ;
int  branch ;
{
	int nbranch ;

	/* collect the left and right children of a node in arrays */
	
	if (node->left)
	{
		nbranch = (branch == TOP) ? LEFT : branch ;	

		if (node->left->player != -1)
		{
			if (nbranch == RIGHT)
				right[nrights++] = node->left->player ;
			else
				left[nlefts++] = node->left->player ;
		}

		walk_tree(node->left, nbranch) ;
	}

	if (node->right)
	{
		nbranch = (branch == TOP) ? RIGHT : branch ;	

		if (node->right->player != -1)
		{
			if (nbranch == RIGHT)
				right[nrights++] = node->right->player ;
			else
				left[nlefts++] = node->right->player ;
		}

		walk_tree(node->right, nbranch) ;
	}
}

calculate_probs(level)
int level ;
{
	int		no_of_nodes ;
	int		loop, loop1, loop2, i ;
	double	prob, oprob ;
	double  probability() ;

	for (i=0, no_of_nodes=1;i<level;i++)
	{
		no_of_nodes = no_of_nodes * 2 ;
	}

	/* no_of_nodes = (int) pow (2.0, (double)level) ; */

	for (loop=0;loop<no_of_nodes;loop++)
	{
		nlefts = nrights = 0 ;
		walk_tree (node[level][loop], TOP) ;

		/* for each on the left - probability of winning all
           possibilities on the right */

		for (loop1=0;loop1<nlefts;loop1++)
		{
			oprob = 0.0 ;
			for (loop2=0;loop2<nrights;loop2++)
			{
				prob = probability (left[loop1], right[loop2]) ;
				oprob += player[left[loop1]].prob_level[level+1]
				       * player[right[loop2]].prob_level[level+1] * prob ;
			}

			if ((oprob == 0.0) && (prob != 0.0))
				oprob = player[left[loop1]].prob_level[level+1] ;

			player[left[loop1]].prob_level[level] = oprob ;
		}

		/* for each on the right - probability of winning all
           possibilities on the left */

		for (loop1=0;loop1<nrights;loop1++)
		{
			oprob = 0.0 ;
			for (loop2=0;loop2<nlefts;loop2++)
			{
				prob = probability (right[loop1], left[loop2]) ;
				oprob += player[right[loop1]].prob_level[level+1]
				       * player[left[loop2]].prob_level[level+1] * prob ;
			}

			if ((oprob == 0.0) && (prob != 0.0))
				oprob = player[right[loop1]].prob_level[level+1] ;

			player[right[loop1]].prob_level[level] = oprob ;
		}
	}
}

double probability(p1, p2)
int p1, p2 ;
{
	/* probability that p1 beats p2 */

	double 	prob ;
	int		denom ;

	if (strstr (player[p1].name, "Lost"))
		return (0.0) ;

	if (strstr (player[p2].name, "Lost"))
		return (1.0) ;

	denom = player[p1].ranking + player[p2].ranking ;
	if (denom == 0)
		return (1.0) ;
	
	if (method == 1)	
	{
		prob = (double) player[p1].ranking / ((double) player[p1].ranking + (double) player[p2].ranking) ;
	}
	else if (method == 2)	
	{
		double p1sq, p2sq ;
		p1sq = (double) player[p1].ranking ;
		p1sq = p1sq * p1sq ;
		p2sq = (double) player[p2].ranking ;
		p2sq = p2sq * p2sq ;
		prob = p1sq / (p1sq + p2sq) ;
	}
	return (prob) ;
}
