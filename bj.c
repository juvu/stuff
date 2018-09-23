
#include <stdio.h>

static int	cards[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		   	   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		   	   3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
			   4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		   	   5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		   	   6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		   	   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		   	   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		   	   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		   	   10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
		   	   10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
		   	   10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
		   	   10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10} ;

static int	numCards[11] ;

// asdf

int	bet ;
int	TotalNumCards ;
int	CurrentCardPos = 200 ;
int	cardsLeft ;
double	MaxDealerProb = 0.0 ;

Shuffle()
{
	int	i, tmp, pos ;

	printf ("Shuffling now\n") ;

	TotalNumCards = sizeof(cards) / sizeof(int) ;
	for (i=0;i<TotalNumCards;i++)
	{
		pos = random() % TotalNumCards ;
		tmp = cards[pos] ;
		cards[pos] = cards[i] ;
		cards[i] = tmp ;
	}
	for (i=0;i<11;i++)
		numCards[i] = 0 ;
	for (i=0;i<TotalNumCards;i++)
		numCards[cards[i]]++ ;

	CurrentCardPos = 0 ;
	cardsLeft = TotalNumCards - CurrentCardPos ;
}

double Prob (x)
int x ;
{
	double pb ;

	/* the probability of scoring x with your next card */

	pb = (double) numCards[x] / (double) cardsLeft ;
	return (pb) ;
}


double ImmediateBustProb (total, variance)
int total, variance ;
{
	int	i ;
	int	max ;
	double 	pb = 0.0 ;

	/* we will assume that it is not a good idea to
	   twist with >=19 soft or >=17 hard */
#if 0
	if (total >= 17)
		return (1.0) ;
	if (variance && (total >= 9))
		return (1.0) ;
#endif
	if (total <= 11)
		return (0.0) ;

	max = 21 - total ;

	for (i=10;i>=max;i--)
	{
		if (numCards[i])
			pb += Prob(i) ;
	}

	return (pb) ;
}

double DealerBustProb (total, variance)
int total, variance ;
{
	int	i ;
	int	TheTotal ;
	double 	pb = 0.0 ;
	double 	pb2 = 0.0 ;

	TheTotal = GetTotal(total, variance) ;
	if (TheTotal == 0)
		return (1.0) ;
	if (TheTotal >= 17)
		return (0.0) ;
	for (i=1;i<11;i++)
	{
		if (numCards[i])
		{
			pb = Prob(i) ;
			numCards[i]-- ;
			cardsLeft-- ;
			pb2 += pb * DealerBustProb (total+i, (i==1)) ;
			numCards[i]++ ;
			cardsLeft++ ;
		}
	}

	return (pb2) ;
}

int GetCard(variance)
int	*variance ;
{
	int	card ;

	card = cards[CurrentCardPos++] ;
	numCards[card]-- ;
	cardsLeft-- ;
	if (card == 1)
		*variance = 1 ;
	return (card) ;
}

int	PlayDealersHand(dealer, dealerVariance) 
int	dealer, dealerVariance ;
{
	int total ;

	while(1)
	{
		total = GetTotal (dealer, dealerVariance) ;
		if ((total >= 17) || (total == 0))
			return (total) ;
		dealer += GetCard(&dealerVariance) ;
	}
}


int	PlayMyHand(mine, myVariance, dealer, dealerVariance) 
int	mine, myVariance, dealer, dealerVariance ;
{
	int	GoOn = 1 ;
	int	total ;
	double  DealerProb ;
	double  MyProb ;

#if 0
	DealerProb = DealerBustProb (dealer, dealerVariance) ;
	MyProb = ImmediateBustProb (mine, myVariance) ;

	if ((mine >= 9) || (mine == 1)) 
	{
		bet *= 2 ;
		mine += GetCard(&myVariance) ;
	}
	else
	{
#endif
		while (GoOn)
		{
			/* DealerProb = DealerBustProb (dealer, dealerVariance) ; */
			DealerProb = ImmediateBustProb (dealer, dealerVariance) ; 
			if (DealerProb > MaxDealerProb)
				MaxDealerProb = DealerProb ;
			MyProb = ImmediateBustProb (mine, myVariance) ;
			if (MyProb == 1.0)
				GoOn = 0 ;
			else if (MyProb == 0.0)	
				mine += GetCard(&myVariance) ;
			else if (MyProb < DealerProb)
				mine += GetCard(&myVariance) ;
			else
				GoOn = 0 ;
		}
#if 0 
	}

#endif
	total = GetTotal (mine, myVariance) ;
	return (total) ;
}

GetTotal(score, variance)
int score, variance ;
{
	if (variance && (score<=11))
		score += 10 ;

	if (score > 21)
		score = 0 ;	/* bust */

	return (score) ;
}

main(argc, argv)
int argc ;
char **argv;
{
	int	dealer, dealerVariance ;
	int	mine, myVariance ;
	int	MyTotal, DealerTotal ;
	double	hands = 0.0 ;
	double dbt = 0.0 ;
	double	DealerProb ;
	static int	BetStatus = 0 ;

	srandom(atoi(argv[1])) ;

	while (1)
	{
		hands += 1.0 ;
		dealerVariance = 0 ;
		myVariance = 0 ;

		if (CurrentCardPos > 150)
			Shuffle() ;

		bet = CurrentCardPos + 1;
		dealer = GetCard(&dealerVariance) ;
		mine = GetCard(&myVariance) ;

		MyTotal = PlayMyHand(mine, myVariance, dealer, dealerVariance) ;
		if (MyTotal == 0)
			BetStatus -= bet ;
		else
		{
			DealerTotal = PlayDealersHand (dealer, dealerVariance) ;
			if (MyTotal > DealerTotal)
				BetStatus += bet ;
			else if (MyTotal < DealerTotal)
				BetStatus -= bet ;
		}
		printf ("My Total is %d, dealer total is %d, BetStatus is %d, MaxDealerProb is %f\n", 
						MyTotal, DealerTotal, BetStatus, MaxDealerProb) ;
	}
}



