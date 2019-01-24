#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>

// scores for different hands
#define STRAIGHT_FLUSH 80000
#define FOUR_OF_KIND 70000
#define FLUSH 60000
#define FULL_HOUSE 50000
#define THREE_OF_KIND 40000
#define STRAIGHT 30000
#define TWO_PAIRS 20000
#define PAIR 10000

#define HEARTS 1
#define DIAMONDS 2
#define CLUBS 3
#define SPADES 4

#define NUM_CARDS 7

typedef struct
{
	unsigned long value ;
	unsigned long suit ;
} Card ;

Card myCards[NUM_CARDS] ;
Card myFlushCards[NUM_CARDS] ;

int nextCard = 0 ;
int nextFlushCard = 0 ;

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

int isFlush (Card *cards, int numCards, int *flushSuit)
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
			printf ("Flush %d\n", FLUSH + highest) ;
			return (FLUSH + highest) ;
		}
	}

	return (0) ;
}

int isFourOfKind (Card *cards, int numCards)
{
	int i = 0 ;
	int totals[15];

	for (i = 14; i >= 6; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 6; i--)
	{
		if (totals[i] == 4)
		{
			printf ("Four of a Kind %d\n", FOUR_OF_KIND + i) ;
			return (FOUR_OF_KIND + i) ;
		}
	}
	return (0) ;
}

int isFullHouse (Card *cards, int numCards)
{
	int i = 0 ;
	int j = 0 ;
	int totals[15];

	for (i = 14; i >= 6; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 6; i--)
	{
		if (totals[i] == 3)
		{
			for (j = 14; j >= 6; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 2)
				{
					printf ("Full House %d\n", FULL_HOUSE + (i*500) + j) ;
					return (FULL_HOUSE + (i*500) + j) ;
				}
			}
		}
	}
	return (0) ;
}

int isTwoPairs (Card *cards, int numCards)
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int totals[15];

	for (i = 14; i >= 6; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 6; i--)
	{
		if (totals[i] == 2)
		{
			for (j = 14; j >= 6; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 2)
				{
					for (k = 14; k >=6; k--)
					{
						if (k == i || k == j)
							continue ;

						if (totals[k] == 1)
						{
							printf ("Two Pairs %d\n", TWO_PAIRS + (i*500) + (j*50) + k) ;
							return (TWO_PAIRS + (i*500) + (j*50) + k) ;
						}
					}
				}
			}
		}
	}
	return (0) ;
}

int isPair (Card *cards, int numCards)
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int totals[15];

	for (i = 14; i >= 6; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 6; i--)
	{
		if (totals[i] == 2)
		{
			for (j = 14; j >= 6; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 1)
				{
					for (k = 14; k >=6; k--)
					{
						if (k == i || k == j)
							continue ;

						if (totals[k] == 1)
						{
							printf ("Pair %d\n", PAIR + (i*500) + (j*50) + k) ;
							return (PAIR + (i*500) + (j*50) + k) ;
						}
					}
				}
			}
		}
	}
	return (0) ;
}

int highCard (Card *cards, int numCards)
{
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int totals[15];

	for (i = 14; i >= 6; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 6; i--)
	{
		if (totals[i] == 1)
		{
			for (j = 14; j >= 6; j--)
			{
				if (j == i)
					continue ;
			
				if (totals[j] == 1)
				{
					for (k = 14; k >=6; k--)
					{
						if (k == i || k == j)
							continue ;

						if (totals[k] == 1)
						{
							printf ("High Card %d\n", (i*500) + (j*50) + k) ;
							return ((i*500) + (j*50) + k) ;
						}
					}
				}
			}
		}
	}
	return (0) ;
}

int isThreeOfKind (Card *cards, int numCards)
{
	int i = 0 ;
	int totals[15];

	for (i = 14; i >= 6; i--)
	{
		totals[i] = 0 ;
	}
	
	for (i = 0; i < numCards; i++)
	{
		totals[cards[i].value] ++ ;
	}

	for (i = 14; i >= 6; i--)
	{
		if (totals[i] == 3)
		{
			printf ("Three of a Kind %d\n", THREE_OF_KIND + i) ;
			return (THREE_OF_KIND + i) ;
		}
	}
	return (0) ;
}

int isStraight (Card *cards, int numCards)
{
	unsigned long handMap = 0 ;

	int i = 0 ;
	
	for (i = 0; i < numCards; i++)
	{
		handMap += 1 << cards[i].value ;
	}

	unsigned long handMask = 0 ;

	handMask = 31 << 10 ;
	if (handMask == (handMask & handMap))
	{
		printf ("Ace high straight %d\n", STRAIGHT + 14) ;
		return (STRAIGHT + 14) ;
	}

	handMask = 31 << 9 ;
	if (handMask == (handMask & handMap))
	{
		printf ("King high straight %d\n", STRAIGHT + 13) ;
		return (STRAIGHT + 13) ;
	}

	handMask = 31 << 8 ;
	if (handMask == (handMask & handMap))
	{
		printf ("Queen high straight %d\n", STRAIGHT + 12) ;
		return (STRAIGHT + 12) ;
	}

	handMask = 31 << 7 ;
	if (handMask == (handMask & handMap))
	{
		printf ("Jack high straight %d\n", STRAIGHT + 11) ;
		return (STRAIGHT + 11) ;
	}

	handMask = 31 << 6 ;
	if (handMask == (handMask & handMap))
	{
		printf ("Jack high straight %d\n", STRAIGHT + 10) ;
		return (STRAIGHT + 10) ;
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

int isStraightFlush (Card *cards, int numCards)
{
	int score = 0 ;
	int FlushSuit = 0 ;

	score = isFlush (cards, numCards, &FlushSuit) ;
	if (!score)
		return (0) ;

	collectFlushCards (cards, myFlushCards, numCards, FlushSuit);

	score = isStraight (myFlushCards, nextFlushCard) ;
	if (score)
	{
		// wow, a straight flush
		printf ("Straight Flush %d\n", STRAIGHT_FLUSH + score) ;
		return (STRAIGHT_FLUSH + score) ;
	}

	return (0); 
}


addCard (Card *cards, char *str)
{
	cards[nextCard].value = str[0] - 48;
	if (str[0] == 't' || str[0] == 'T')
		cards[nextCard].value = 10 ;
	else if (str[0] == 'j' || str[0] == 'J')
		cards[nextCard].value = 11 ;
	else if (str[0] == 'q' || str[0] == 'Q')
		cards[nextCard].value = 12 ;
	else if (str[0] == 'k' || str[0] == 'K')
		cards[nextCard].value = 13 ;
	else if (str[0] == 'a' || str[0] == 'A')
		cards[nextCard].value = 14 ;

	if (str[1] == 'h' || str[1] == 'H')
		cards[nextCard++].suit = HEARTS ;	
	else if (str[1] == 'd' || str[1] == 'D')
		cards[nextCard++].suit = DIAMONDS ;	
	else if (str[1] == 'c' || str[1] == 'C')
		cards[nextCard++].suit = CLUBS ;	
	else if (str[1] == 's' || str[1] == 'S')
		cards[nextCard++].suit = SPADES ;	
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
		addCard (cards, str) ;
	}
	va_end (ap) ;
}


int evaluateHand (Card *cards, int numCards)
{
	int FlushSuit = 0 ;

	int score = 0 ;
	score = isStraightFlush (myCards, numCards) ;
	if (score) return (score) ;
	score = isFourOfKind (myCards, numCards) ;
	if (score) return (score) ;
	score = isFlush (myCards, numCards, &FlushSuit) ;
	if (score) return (score) ;
	score = isFullHouse (myCards, numCards) ;
	if (score) return (score) ;
	score = isThreeOfKind (myCards, numCards) ;
	if (score) return (score) ;
	score = isStraight (myCards, numCards) ;
	if (score) return (score) ;
	score = isTwoPairs (myCards, numCards) ;
	if (score) return (score) ;
	score = isPair (myCards, numCards) ;
	if (score) return (score) ;
	score = highCard (myCards, numCards) ;
	return (score) ;
}

main (int argc, char **argv)
{
	addCard (myCards, "TH") ;
	addCard (myCards, "QC") ;
	addCard (myCards, "7H") ;
	addCard (myCards, "AD") ;
	addCard (myCards, "JH") ;
	addCard (myCards, "8H") ;
	addCard (myCards, "9H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "TH", "8S", "TD", "9H", "TC", "JS", "TS") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 5, "TH", "8S", "9H", "JS", "QS") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "TH", "8S", "6H", "9H", "TC", "JH", "TH") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "TH", "8S", "6H", "9H", "TC", "7H", "6H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "AH", "8S", "6S", "6D", "TC", "7H", "6H") ;
	evaluateHand (myCards, nextCard) ;
	printf ("Full Houses\n") ;
	setCards (myCards, 7, "6H", "8S", "AS", "AD", "AC", "7H", "6H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "AH", "8S", "KS", "KD", "KC", "7H", "AH") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "AH", "8S", "QS", "QD", "QC", "7H", "AH") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "8S", "7S", "7D", "6C", "7H", "9H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "AH", "8S", "6S", "6D", "AC", "7H", "6H") ;
	evaluateHand (myCards, nextCard) ;
	printf ("Two Pairs\n") ;
	setCards (myCards, 7, "6H", "8S", "9S", "AD", "AC", "7H", "6H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "QH", "8S", "9S", "KD", "KC", "7H", "QH") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "TH", "8S", "9S", "QD", "QC", "7H", "TH") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "8S", "9S", "7D", "6C", "7H", "9H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "8S", "9S", "8D", "AC", "7H", "6H") ;
	evaluateHand (myCards, nextCard) ;
	printf ("Pairs\n") ;
	setCards (myCards, 7, "6H", "8S", "9S", "TD", "TC", "AH", "KS") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "8S", "9S", "TD", "TC", "QH", "KS") ;
	evaluateHand (myCards, nextCard) ;
	printf ("HighCard\n") ;
	setCards (myCards, 7, "6H", "8S", "9S", "TD", "QC", "AH", "KS") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "8S", "9S", "TD", "QC", "AH", "3S") ;
	evaluateHand (myCards, nextCard) ;
	printf ("Straight Flush\n") ;
	setCards (myCards, 7, "JH", "TH", "9H", "TD", "QC", "8H", "7H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "TH", "9H", "TD", "QC", "8H", "7H") ;
	evaluateHand (myCards, nextCard) ;
	setCards (myCards, 7, "6H", "TH", "9H", "TD", "AH", "8H", "7H") ;
	evaluateHand (myCards, nextCard) ;
}

