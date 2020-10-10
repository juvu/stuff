#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      antdi
#
# Created:     10/10/2020
# Copyright:   (c) antdi 2020
# Licence:     <your licence>
#-------------------------------------------------------------------------------

GREEN = 0
MUSTARD = 1
PEACOCK = 2
PLUM = 3
SCARLET = 4
WHITE = 5

WRENCH = 6
CANDLESTICK = 7
DAGGER = 8
PISTOL = 9
LEAD_PIPE = 10
ROPE = 11

BATHROOM = 12
STUDY = 13
DINING_ROOM = 14
GAMES_ROOM = 15
GARAGE = 16
BEDROOM = 17
LIVING_ROOM = 18
KITCHEN = 19
COURTYARD = 20

numPlayers = 3
numPlayerCards = 6
numCards = 21
numWho = 6
numWhat = 12

nameCards = ["GREEN", "MUSTARD", "PEACOCK", "PLUM", "SCARLET", "WHITE",
                    "WRENCH", "CANDLESTICK", "DAGGER", "PISTOL", "LEAD_PIPE", "ROPE",
                    "BATHROOM", "STUDY", "DINING_ROOM", "GAMES_ROOM", "GARAGE", "BEDROOM", "LIVING_ROOM", "KITCHEN", "COURTYARD"]


class Player:
    def __init__(self):
        # A set of cards that the player is known to have
        self.Cards = 0
        # A set of cards that the player is known not to have
        self.notCards = 0
        # A list of clauses.  Each clause is a set of cards, one of which
        # the player is known to have.
        self.possibleCards = []
        # a copy of the possible cards
        self.newPossible = []

    def addCard (self, card):
        self.Cards = self.Cards | (1 << card)

    def addNotCards (self, card1, card2, card3):
        self.notCards = self.notCards | (1 << card1)
        self.notCards = self.notCards | (1 << card2)
        self.notCards = self.notCards | (1 << card3)
        # if we have (numCards - numPlayerCards) of notCards
        # then the remaining cards must all be ours
        numFound = 0
        for y in range (numCards):
            if (self.notCards & (1 << y)):
                numFound = numFound + 1
        if (numFound == (numCards - numPlayerCards)):
            for y in range (numCards):
                if ((self.notCards & (1 << y)) == 0):
                    self.Cards = self.Cards | (1 << y)
        # having added notCards we should rationalize the possibilities
        self.rationalizePossibilities()


    def addPossibility (self, card1, card2, card3):
        poss = 0
        poss = poss | (1 << card1)
        poss = poss | (1 << card2)
        poss = poss | (1 << card3)
        self.possibleCards.append(poss)
        self.rationalizePossibilities()

    def rationalizePossibilities(self):
        # first remove any notCards from each possibility set
        self.newPossible = []
        for x in self.possibleCards:
            for y in range (numCards):
                if (self.notCards & (1 << y)):
                    if (x & (1 << y)):
                        x = x ^ (1 << y)
            self.newPossible.append(x)
        self.possibleCards = []
        self.possibleCards = self.newPossible.copy()

        # now check to see if any possibility set has only one card in it
        for x in self.possibleCards:
            numFound = 0
            for y in range (numCards):
                if (x & (1 << y)):
                    numFound = numFound + 1
            # found a possibility set with only one card, so
            # add this card to the set of definites
            if (numFound == 1):
                self.Cards = self.Cards | x


    def printPossibilities(self):
        print ("Possibilities")
        for x in self.possibleCards:
            for y in range (numCards):
                if (x & (1 << y)):
                    print (nameCards[y])


def printCards (cards):
    print ("Cards")
    for x in range (numCards):
        if (cards & (1 << x)):
            print (nameCards[x])


def solution(players):
    solutionCards = 0
    for x in players:
        solutionCards = solutionCards | x.Cards
    # Cards that may be in the solution are ones that we haven't found in any player hand
    solutionCards = ~solutionCards
    print ("Solution")
    print ("WHO -", end=" ")
    for x in range (numWho):
        if (solutionCards & (1 << x)):
            print (nameCards[x], end=" ")
    print ("\nWHAT -", end=" ")
    for x in range (numWho, numWhat):
        if (solutionCards & (1 << x)):
            print (nameCards[x], end= " ")
    print ("\nWHERE -", end=" ")
    for x in range (numWhat, numCards):
        if (solutionCards & (1 << x)):
            print (nameCards[x], end=" ")
    print("\n")



def main():
    players = []

    for x in range(numPlayers):
        players.append(Player())

    solution(players)

    for x in range(numPlayers):
        players[x].addCard (x)
        players[x].addCard (x+6)
        players[x].addCard (x+12)
        #print (players[x].Cards)
        #printCards(players[x].Cards)

    players[0].addPossibility(PEACOCK, PISTOL, GARAGE)
    players[0].addPossibility(WHITE, ROPE, DINING_ROOM)
    players[1].addPossibility(KITCHEN, ROPE, STUDY)

    #players[0].printPossibilities()
    players[0].addNotCards(ROPE, DINING_ROOM, STUDY)
    players[1].addNotCards(KITCHEN, COURTYARD, STUDY)
    #printCards(players[0].Cards)

    solution(players)

if __name__ == '__main__':
    main()
