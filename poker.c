#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

// scores for different hands
#define STRAIGHT_FLUSH 80000
#define FOUR_OF_KIND 70000
#define FULL_HOUSE 60000
#define FLUSH 50000
#define STRAIGHT 40000
#define THREE_OF_KIND 30000
#define TWO_PAIRS 20000
#define PAIR 10000

#define HEARTS 1
#define DIAMONDS 2
#define CLUBS 3
#define SPADES 4

#define NUM_CARDS 7
#define NUM_DECK_CARDS 52 

#define ITERATIONS 50000

typedef struct
{
	unsigned long value ;
	unsigned long suit ;
} Card ;

Card deckCards[NUM_DECK_CARDS] ;
Card myCards[NUM_CARDS] ;
Card oppoCards[NUM_CARDS] ;
Card FlushCards[NUM_CARDS] ;

int nextCard = 0 ;
int nextDeckCard = 0 ;
int nextFlushCard = 0 ;
int nextOppoCard = 0 ;

int highestOfSuit (Card *cards, int numCards, int suit)
{
	int i = 0 ;

	int highest = 0 ;

	for (i = 0; i < numCards; i++)
	{
		if (cards[i].suit == suit && cards[i].value > highest)
			highest = cards[i].value ;
	}
	
	return (highest) ;
}

int isFlush (Card *cards, int numCards, int *flushSuit, char *desc)
{
	int i = 0 ;
	int totals[5];
	int highest = 0 ;

	for (i = 0; i < 5; i++)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].suit] ++ ;
	}

	for (i = 1; i < 5; i++)
	{
		if (totals[i] >= 5) 
		{
			highest = highestOfSuit (cards, numCards, i) ;
			*flushSuit = i ;
			sprintf (desc, "Flush %d\n", FLUSH + highest) ;
			return (FLUSH + highest) ;
		}
	}

	return (0) ;
}

int isFourOfKind (Card *cards, int numCards, char *desc)
{
	int i = 0 ;
	int totals[15];

	for (i = 14; i >= 2; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 2; i--)
	{
		if (totals[i] == 4)
		{
			sprintf (desc, "Four of a Kind %d\n", FOUR_OF_KIND + i) ;
			return (FOUR_OF_KIND + i) ;
		}
	}
	return (0) ;
}

int isFullHouse (Card *cards, int numCards, char *desc)
{
	int i = 0 ;
	int j = 0 ;
	int totals[15];

	for (i = 14; i >= 2; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 2; i--)
	{
		if (totals[i] == 3)
		{
			for (j = 14; j >= 2; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] >= 2)
				{
					sprintf (desc, "Full House %d\n", FULL_HOUSE + (i*500) + j) ;
					return (FULL_HOUSE + (i*500) + j) ;
				}
			}
		}
	}
	return (0) ;
}

int isTwoPairs (Card *cards, int numCards, char *desc)
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int totals[15];

	for (i = 14; i >= 2; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 2; i--)
	{
		if (totals[i] == 2)
		{
			for (j = 14; j >= 2; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 2)
				{
					for (k = 14; k >=2; k--)
					{
						if (k == i || k == j)
							continue ;

						if (totals[k] == 1)
						{
							sprintf (desc, "Two Pairs %d\n", TWO_PAIRS + (i*500) + (j*50) + k) ;
							return (TWO_PAIRS + (i*500) + (j*50) + k) ;
						}
					}
				}
			}
		}
	}
	return (0) ;
}

int isPair (Card *cards, int numCards, char *desc)
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int totals[15];

	for (i = 14; i >= 2; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 2; i--)
	{
		if (totals[i] == 2)
		{
			if (numCards == 2)
				return (PAIR + (i*500)) ;
			
			for (j = 14; j >= 2; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 1)
				{
					for (k = 14; k >=2; k--)
					{
						if (k == i || k == j)
							continue ;
	
						if (totals[k] == 1)
						{
							sprintf (desc, "Pair %d\n", PAIR + (i*500) + (j*50) + k) ;
							return (PAIR + (i*500) + (j*50) + k) ;
						}
					}
				}
			}
		}
	}
	return (0) ;
}

int highCard (Card *cards, int numCards, char *desc)
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int totals[15];

	for (i = 14; i >= 2; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 2; i--)
	{
		if (totals[i] == 1)
		{
			for (j = 14; j >= 2; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 1)
				{
					if (numCards > 2)
					{
						for (k = 14; k >=2; k--)
						{
							if (k == i || k == j)
								continue ;
	
							if (totals[k] == 1)
							{
								sprintf (desc, "High Card %d\n", (i*500) + (j*50) + k) ;
								return ((i*500) + (j*50) + k) ;
							}
						}
					}
					else
					{
						return ((i*500) + (j*50)) ;
					}
				}
			}
		}
	}
	return (0) ;
}

int isThreeOfKind (Card *cards, int numCards, char *desc)
{
	int i = 0 ;
	int totals[15];

	for (i = 14; i >= 2; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 2; i--)
	{
		if (totals[i] == 3)
		{
			sprintf (desc, "Three of a Kind %d\n", THREE_OF_KIND + i) ;
			return (THREE_OF_KIND + i) ;
		}
	}
	return (0) ;
}

int isStraight (Card *cards, int numCards, char *desc)
{
	unsigned long handMap = 0 ;

	int i = 0 ;
	
	for (i = 0; i < numCards; i++)
	{
		handMap |= 1 << cards[i].value ;
	}

	unsigned long handMask = 0 ;

	handMask = 31 << 10 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Ace high straight %d\n", STRAIGHT + 14) ;
		return (STRAIGHT + 14) ;
	}

	handMask = 31 << 9 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "King high straight %d\n", STRAIGHT + 13) ;
		return (STRAIGHT + 13) ;
	}

	handMask = 31 << 8 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Queen high straight %d\n", STRAIGHT + 12) ;
		return (STRAIGHT + 12) ;
	}

	handMask = 31 << 7 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Jack high straight %d\n", STRAIGHT + 11) ;
		return (STRAIGHT + 11) ;
	}

	handMask = 31 << 6 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Ten high straight %d\n", STRAIGHT + 10) ;
		return (STRAIGHT + 10) ;
	}

	handMask = 31 << 5 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Nine high straight %d\n", STRAIGHT + 9) ;
		return (STRAIGHT + 9) ;
	}

	handMask = 31 << 4 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Eight high straight %d\n", STRAIGHT + 8) ;
		return (STRAIGHT + 8) ;
	}

	handMask = 31 << 3 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Seven high straight %d\n", STRAIGHT + 7) ;
		return (STRAIGHT + 7) ;
	}

	handMask = 31 << 2 ;
	if (handMask == (handMask & handMap))
	{
		sprintf (desc, "Six high straight %d\n", STRAIGHT + 6) ;
		return (STRAIGHT + 6) ;
	}


	return (0) ;
}

collectFlushCards (Card *cards, Card *flushCards, int numCards, int flushSuit)
{
	int 	i=0;

	nextFlushCard = 0 ;
	for (i=0; i<numCards; i++)
	{
		if (cards[i].suit == flushSuit)
		{
			flushCards[nextFlushCard].value = cards[i].value ;
			flushCards[nextFlushCard].suit = cards[i].suit ;
			nextFlushCard++ ;
		}
	}
}

int isStraightFlush (Card *cards, int numCards, char *desc)
{
	int score = 0 ;
	int FlushSuit = 0 ;

	score = isFlush (cards, numCards, &FlushSuit, desc) ;
	if (!score)
		return (0) ;

	collectFlushCards (cards, FlushCards, numCards, FlushSuit);

	score = isStraight (FlushCards, nextFlushCard, desc) ;
	if (score)
	{
		// wow, a straight flush
		strcpy (desc, "Straight Flush") ;
		return (STRAIGHT_FLUSH + score) ;
	}

	return (0); 
}

addDeckCard (Card *cards, int deckPos, int *nextCard)
{
	cards[*nextCard].value = deckCards[deckPos].value ;
	cards[*nextCard].suit = deckCards[deckPos].suit ;
	(*nextCard)++;
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
		cards[*nextCard].value = 14 ;

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

setCards (Card *cards, int numCards, ...)
{
	int 	i=0;
	char	*str = "";
	va_list	ap;

	va_start (ap, numCards) ;

	nextCard = 0 ;
	for (i=0; i<numCards; i++)
	{
		str = va_arg (ap, char *) ;
		addCard (cards, str, &nextCard) ;
	}
	va_end (ap) ;
}


int evaluateHand (Card *cards, int numCards, char *desc)
{
	int FlushSuit = 0 ;

	int score = 0 ;
	score = isStraightFlush (cards, numCards, desc) ;
	if (score) return (score) ;
	score = isFourOfKind (cards, numCards, desc) ;
	if (score) return (score) ;
	score = isFullHouse (cards, numCards, desc) ;
	if (score) return (score) ;
	score = isFlush (cards, numCards, &FlushSuit, desc) ;
	if (score) return (score) ;
	score = isStraight (cards, numCards, desc) ;
	if (score) return (score) ;
	score = isThreeOfKind (cards, numCards, desc) ;
	if (score) return (score) ;
	score = isTwoPairs (cards, numCards, desc) ;
	if (score) return (score) ;
	score = isPair (cards, numCards, desc) ;
	if (score) return (score) ;
	score = highCard (cards, numCards, desc) ;
	return (score) ;
}

populateDeckCards()
{
	int i=0;
	int j=0;

	nextDeckCard = 0 ;
	for (i=1;i<=4;i++)
	{
		for (j=2;j<15;j++)
		{
			deckCards[nextDeckCard].value = j;
			deckCards[nextDeckCard++].suit = i;
		}
	}

	for (i=0;i<2;i++)
	{
		for (j=0;j<nextDeckCard;j++)
		{
			if (deckCards[j].value == myCards[i].value
			&&  deckCards[j].suit == myCards[i].suit)
			{
				deckCards[j].value = deckCards[nextDeckCard-1].value ;
				deckCards[j].suit = deckCards[nextDeckCard-1].suit ;
				nextDeckCard-- ;
			}
		}
	}

	for (i=2;i<nextCard;i++)
	{
		int tmpSuit = 0 ;
		int tmpVal = 0 ;

		for (j=0;j<nextDeckCard;j++)
		{
			if (deckCards[j].value == myCards[i].value
			&&  deckCards[j].suit == myCards[i].suit)
			{
				tmpVal = deckCards[i-2].value ;
				tmpSuit = deckCards[i-2].suit ;
				deckCards[i-2].value = myCards[i].value ;
				deckCards[i-2].suit = myCards[i].suit ;
				deckCards[j].value = tmpVal ;
				deckCards[j].suit = tmpSuit ;
			}
		}
	}
}

shuffleDeckCards(int inputNumCards)
{
	int i = 0;
	int numShuffleCards = nextDeckCard - inputNumCards + 2;
	int startPos = inputNumCards - 2;

	for (i=startPos; i < nextDeckCard; i++)
	{
		int pos = random()%numShuffleCards + startPos;
		int tmpVal = deckCards[pos].value;
		int tmpSuit = deckCards[pos].suit;
		deckCards[pos].value = deckCards[i].value;
		deckCards[pos].suit = deckCards[i].suit;
		deckCards[i].value = tmpVal;
		deckCards[i].suit = tmpSuit;
	}
}

int checkOppoPair (int oppoPos)
{
	if (deckCards[oppoPos].value == 9 && deckCards[oppoPos+1].value == 9)
		return 1 ;
	if (deckCards[oppoPos].value == 8 && deckCards[oppoPos+1].value == 8)
		return 1 ;
	if (deckCards[oppoPos].value < 10 && deckCards[oppoPos+1].value < 10)
		return 0 ;
	if (deckCards[oppoPos].value >= 11)
		return 1 ;
	if (deckCards[oppoPos+1].value >= 11)
		return 1 ;
	if (deckCards[oppoPos].suit == deckCards[oppoPos+1].suit) 
		return 1 ;
	if (deckCards[oppoPos].value - deckCards[oppoPos+1].value == 1)
		return 1 ;
	if (deckCards[oppoPos].value - deckCards[oppoPos+1].value == 0)
		return 1 ;
	if (deckCards[oppoPos].value - deckCards[oppoPos+1].value == -1)
		return 1 ;

	return 0 ;
}
		
createOppoHand(int *oppoPair, int checkPos)
{
	int i=0;

	nextOppoCard = 0;
	for (i=0;i<checkPos;i++)
	{
		addDeckCard(oppoCards, i, &nextOppoCard) ;
	}

	while (1)
	{
		if (checkOppoPair (*oppoPair))
			break ;

		(*oppoPair) += 2 ;
	}

	addDeckCard (oppoCards, *oppoPair, &nextOppoCard) ;
	addDeckCard (oppoCards, (*oppoPair)+1, &nextOppoCard) ;

	(*oppoPair) += 2 ;
}

double calculateOdds (int inputNumCards, int checkPos, int NumOppo, char *desc)
{
	int myScore = 0 ;
	char oppodesc[200] = "" ;
	int i = 0;

	int myWins = 0 ;
	int goes = 0;
	int oppoScore = 0;
	int nextOppoPair = 0 ;

	for (goes=0;goes<ITERATIONS;goes++)
	{
		shuffleDeckCards (inputNumCards);

		nextCard = inputNumCards;
		for (i=inputNumCards-2;i<checkPos;i++)
		{
			addDeckCard(myCards, i, &nextCard) ;
		}
		myScore = evaluateHand (myCards, nextCard, desc) ;

		int maxOppoScore = 0 ;

		int j = 0 ;

		nextOppoPair = 5;
		for (j=0;j<NumOppo;j++)
		{
			createOppoHand(&nextOppoPair, checkPos);
			oppoScore = evaluateHand (oppoCards, nextOppoCard, oppodesc) ;
			if (oppoScore > maxOppoScore)
				maxOppoScore = oppoScore ;
		}

		if (myScore >= maxOppoScore)
			myWins++;
	}

	double winPercent = ((double) myWins / (double) ITERATIONS) * 100.0 ;

	return (winPercent) ;
}
	
main (int argc, char **argv)
{
	int i = 0 ;
	int		nohtml = 0 ;
	char	desc[200] = "" ;
	char	calcdesc[200] = "" ;
	int inputNumCards = strlen (argv[3]) / 2;

	int Pounds = atoi (argv[1]) ;
	int Pence = atoi (argv[2]) ;

	if (!strcmp (argv[4], "nohtml"))
		nohtml = 1 ;

    // init random seed
    struct timeval tp ;
    gettimeofday (&tp, NULL) ;

    srandom (tp.tv_usec) ;

	nextCard = 0 ;

	for (i=0;i<inputNumCards*2;i+=2)
	{
		char theCard[3];

		theCard[0] = argv[3][i];
		theCard[1] = argv[3][i+1];
		theCard[2] = '\0';
		addCard (myCards, theCard, &nextCard) ;
	}

	evaluateHand (myCards, nextCard, desc) ;

	populateDeckCards () ;

	double winPercent21 = 0.0 ;
	double winPercent22 = 0.0 ;
	double winPercent23 = 0.0 ;
	double winPercent51 = 0.0 ;
	double winPercent52 = 0.0 ;
	double winPercent53 = 0.0 ;
	double winPercent61 = 0.0 ;
	double winPercent62 = 0.0 ;
	double winPercent63 = 0.0 ;
	double winPercent71 = 0.0 ;
	double winPercent72 = 0.0 ;
	double winPercent73 = 0.0 ;

	if (inputNumCards == 2)
	{
		if (!nohtml)
		{
			winPercent21 = calculateOdds (inputNumCards, 0, 1, calcdesc) ;
			winPercent22 = calculateOdds (inputNumCards, 0, 2, calcdesc) ;
			winPercent23 = calculateOdds (inputNumCards, 0, 3, calcdesc) ;
			winPercent51 = calculateOdds (inputNumCards, 3, 1, calcdesc) ;
			winPercent52 = calculateOdds (inputNumCards, 3, 2, calcdesc) ;
			winPercent53 = calculateOdds (inputNumCards, 3, 3, calcdesc) ;
			winPercent61 = calculateOdds (inputNumCards, 4, 1, calcdesc) ;
			winPercent62 = calculateOdds (inputNumCards, 4, 2, calcdesc) ;
			winPercent63 = calculateOdds (inputNumCards, 4, 3, calcdesc) ;
		}
		winPercent71 = calculateOdds (inputNumCards, 5, 1, calcdesc) ;
		winPercent72 = calculateOdds (inputNumCards, 5, 2, calcdesc) ;
		winPercent73 = calculateOdds (inputNumCards, 5, 3, calcdesc) ;
	}
	else if (inputNumCards == 5)
	{
		if (!nohtml)
		{
			winPercent51 = calculateOdds (inputNumCards, 3, 1, calcdesc) ;
			winPercent52 = calculateOdds (inputNumCards, 3, 2, calcdesc) ;
			winPercent53 = calculateOdds (inputNumCards, 3, 3, calcdesc) ;
			winPercent61 = calculateOdds (inputNumCards, 4, 1, calcdesc) ;
			winPercent62 = calculateOdds (inputNumCards, 4, 2, calcdesc) ;
			winPercent63 = calculateOdds (inputNumCards, 4, 3, calcdesc) ;
		}
		winPercent71 = calculateOdds (inputNumCards, 5, 1, calcdesc) ;
		winPercent72 = calculateOdds (inputNumCards, 5, 2, calcdesc) ;
		winPercent73 = calculateOdds (inputNumCards, 5, 3, calcdesc) ;
	}	
	else if (inputNumCards == 6)
	{
		if (!nohtml)
		{
			winPercent61 = calculateOdds (inputNumCards, 4, 1, calcdesc) ;
			winPercent62 = calculateOdds (inputNumCards, 4, 2, calcdesc) ;
			winPercent63 = calculateOdds (inputNumCards, 4, 3, calcdesc) ;
		}
		winPercent71 = calculateOdds (inputNumCards, 5, 1, calcdesc) ;
		winPercent72 = calculateOdds (inputNumCards, 5, 2, calcdesc) ;
		winPercent73 = calculateOdds (inputNumCards, 5, 3, calcdesc) ;
	}
	else if (inputNumCards == 7)
	{
		winPercent71 = calculateOdds (inputNumCards, 5, 1, calcdesc) ;
		winPercent72 = calculateOdds (inputNumCards, 5, 2, calcdesc) ;
		winPercent73 = calculateOdds (inputNumCards, 5, 3, calcdesc) ;
	}

	double pot = (double) Pounds + ((double) Pence * 0.01);

	if (nohtml)
	{
		printf ("1 OPPO: Bet %.2f of pot\n", winPercent71) ;
		printf ("2 OPPO: Bet %.2f of pot\n", winPercent72) ;
		printf ("3 OPPO: Bet %.2f of pot\n", winPercent73) ;
	}
	else
	{
		printf ("<center><TABLE COLS=10\n border=3>") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>Stage<center>\n") ;
		printf ("<TD><center>1 Oppo<center>\n") ;
		printf ("<TD><center>2 Oppo<center>\n") ;
		printf ("<TD><center>3 Oppo<center>\n") ;
		printf ("</TR>\n") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>Deal<center>\n") ;
		printf ("<TD><center>%.2f<center>\n", winPercent21) ;
		printf ("<TD><center>%.2f<center>\n", winPercent22) ;
		printf ("<TD><center>%.2f<center>\n", winPercent23) ;
		printf ("</TR>\n") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>Flop<center>\n") ;
		printf ("<TD><center>%.2f<center>\n", winPercent51) ;
		printf ("<TD><center>%.2f<center>\n", winPercent52) ;
		printf ("<TD><center>%.2f<center>\n", winPercent53) ;
		printf ("</TR>\n") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>Fourth Card<center>\n") ;
		printf ("<TD><center>%.2f<center>\n", winPercent61) ;
		printf ("<TD><center>%.2f<center>\n", winPercent62) ;
		printf ("<TD><center>%.2f<center>\n", winPercent63) ;
		printf ("</TR>\n") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>River<center>\n") ;
		printf ("<TD><center>%.2f<center>\n", winPercent71) ;
		printf ("<TD><center>%.2f<center>\n", winPercent72) ;
		printf ("<TD><center>%.2f<center>\n", winPercent73) ;
		printf ("</TR>\n") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>Bet<center>\n") ;
		printf ("<TD><center>%.2f<center>\n", winPercent71 * 0.01 * pot) ;
		printf ("<TD><center>%.2f<center>\n", winPercent72 * 0.01 * pot) ;
		printf ("<TD><center>%.2f<center>\n", winPercent73 * 0.01 * pot) ;
		printf ("</TR>\n") ;
		printf ("<TR>\n") ;
		printf ("<TD><center>%s<center>\n", desc) ;
		printf ("</TR>\n") ;
		printf ("</TABLE>\n") ;
	}
}
