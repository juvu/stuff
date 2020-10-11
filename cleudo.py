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

players = []

nameCards = ["GREEN", "MUSTARD", "PEACOCK", "PLUM", "SCARLET", "WHITE",
                    "WRENCH", "CANDLESTICK", "DAGGER", "PISTOL", "LEAD_PIPE", "ROPE",
                    "BATHROOM", "STUDY", "DINING_ROOM", "GAMES_ROOM", "GARAGE", "BEDROOM", "LIVING_ROOM", "KITCHEN", "COURTYARD"]

def getCard (data):
    if (data == "GR"):
        return (GREEN)
    elif (data == "MU"):
        return (MUSTARD)
    elif (data == "PE"):
        return (PEACOCK)
    elif (data == "PL"):
        return (PLUM)
    elif (data == "SC"):
        return (SCARLET)
    elif (data == "WH"):
        return (WHITE)
    elif (data == "WR"):
        return (WRENCH)
    elif (data == "CA"):
        return (CANDLESTICK)
    elif (data == "DA"):
        return (DAGGER)
    elif (data == "PI"):
        return (PISTOL)
    elif (data == "LE"):
        return (LEAD_PIPE)
    elif (data == "RO"):
        return (ROPE)
    elif (data == "BA"):
        return (BATHROOM)
    elif (data == "ST"):
        return (STUDY)
    elif (data == "DI"):
        return (DINING_ROOM)
    elif (data == "GM"):
        return (GAMES_ROOM)
    elif (data == "GA"):
        return (GARAGE)
    elif (data == "BE"):
        return (BEDROOM)
    elif (data == "LI"):
        return (LIVING_ROOM)
    elif (data == "KI"):
        return (KITCHEN)
    elif (data == "CO"):
        return (COURTYARD)
    return (-1)

class Player:
    def __init__(self, num):
        self.num = num
        # A set of cards that the player is known to have
        self.Cards = 0
        # A set of cards that the player is known not to have
        self.notCards = 0
        # A list of clauses.  Each clause is a set of cards, one of which
        # the player is known to have.
        self.possibleCards = []
        # a copy of the possible cards
        self.newPossible = []
        # The probability of having each card
        self.cardProb = []
        for x in range(numCards):
            self.cardProb.append(0.0)


    def addCard (self, card):
        self.Cards = self.Cards | (1 << card)

    def addNotCard (self, card):
        self.notCards = self.notCards | (1 << card)
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
        # we should rationalize the possibilities
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

    def setupProbabilities(self):
        numHaveCards = 0
        numHaveNotCards = 0
        for x in range(numCards):
            if (self.Cards & (1 << x)):
                numHaveCards += 1
            elif (self.notCards & (1 << x)):
                numHaveNotCards += 1
        for x in range(numCards):
            if (self.Cards & (1 << x)):
                self.cardProb[x] = 1.0
            elif (self.notCards & (1 << x)):
                self.cardProb[x] = 0.0
            else:
                self.cardProb[x] = (numPlayerCards - numHaveCards) / (numCards - (numHaveCards + numHaveNotCards))

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


def report(players):
    for x in players:
        if (x.num == 1):
            continue
        print ("\nPlayer {} CARDS -".format(x.num), end=" ")
        for y in range (numCards):
            if (x.Cards & (1 << y)):
                print (nameCards[y], end=" ")
        print ("\nPlayer {} NOT CARDS -".format(x.num), end=" ")
        for y in range (numCards):
            if (x.notCards & (1 << y)):
                print (nameCards[y], end=" ")
    print ("\n")


WhoMask = 63             # 000000000000000111111
WhoClearMask = ~WhoMask

WeaponMask = 4032        # 000000000111111000000
WeaponClearMask = ~WeaponMask

WhereMask = 2093056      # 111111111000000000000
WhereClearMask = ~WhereMask

AllMask = 2097151        # 111111111111111111111

def solution(players):
    solutionCards = 0
    for x in players:
        x.setupProbabilities()

    for x in players:
        solutionCards = solutionCards | x.Cards
    # Cards that may be in the solution are ones that we haven't found in any player hand
    solutionCards = ~solutionCards

    # We also have to take account of notCards
    # if no player has a particular card then the solution has this card

    notCombined = players[0].notCards
    for y in players:
        notCombined &= y.notCards

    notWho = notCombined & WhoMask
    if (notWho):
        # clear out everything we may currently have for Who
        solutionCards &= WhoClearMask
        # and insert the card that is not in any player hand
        solutionCards |= notWho

    notWeapon = notCombined & WeaponMask
    if (notWeapon):
        # clear out everything we may currently have for Weapon
        solutionCards &= WeaponClearMask
        # and insert the card that is not in any player hand
        solutionCards |= notWeapon

    notWhere = notCombined & WhereMask
    if (notWhere):
        # clear out everything we may currently have for Where
        solutionCards &= WhereClearMask
        # and insert the card that is not in any player hand
        solutionCards |= notWhere

    solutionProb = []
    for x in range(numCards):
        solutionProb.append(1.0)

    for x in range(numCards):
        if (solutionCards & (1 << x)):
            for y in players:
                solutionProb[x] *= (1.0 - y.cardProb[x])

    whoProbTotal = 0.0
    whatProbTotal = 0.0
    whereProbTotal = 0.0

    for x in range (numWho):
        if (solutionCards & (1 << x)):
            whoProbTotal += solutionProb[x]
    for x in range (numWho, numWhat):
        if (solutionCards & (1 << x)):
            whatProbTotal += solutionProb[x]
    for x in range (numWhat, numCards):
        if (solutionCards & (1 << x)):
            whereProbTotal += solutionProb[x]

    print ("Solution")
    print ("WHO -", end=" ")
    for x in range (numWho):
        if (solutionCards & (1 << x)):
            print ("{} {:.1f}%".format(nameCards[x], (solutionProb[x] / whoProbTotal) * 100.0), end=" ")
    print ("\nWHAT -", end=" ")
    for x in range (numWho, numWhat):
        if (solutionCards & (1 << x)):
            print ("{} {:.1f}%".format(nameCards[x], (solutionProb[x] / whatProbTotal) * 100.0), end=" ")
    print ("\nWHERE -", end=" ")
    for x in range (numWhat, numCards):
        if (solutionCards & (1 << x)):
            print ("{} {:.1f}%".format(nameCards[x], (solutionProb[x] / whereProbTotal) * 100.0), end=" ")
    print("\n")

def addACard(player, data):
    player = player - 1
    card = getCard(data.upper())
    if (card == -1):
        data = input ("{} Unrecognized - reenter".format(data))
        card = getCard(data.upper())
    players[player].addCard(card)
    for y in range (0, numPlayers):
        if (y != player):
            players[y].addNotCard(card)

def addANotCard(player, data):
    player = player - 1
    card = getCard(data.upper())
    if (card == -1):
        data = input ("{} Unrecognized - reenter".format(data))
        card = getCard(data.upper())
    players[player].addNotCard(card)

def addAPossibility (player, suspect, weapon, room):
    player = player - 1
    suspect = getCard(suspect.upper())
    if (suspect == -1):
        data = input ("{} Unrecognized - reenter".format(data))
        suspect = getCard(data.upper())
    weapon = getCard(weapon.upper())
    if (weapon == -1):
        data = input ("{} Unrecognized - reenter".format(data))
        weapon = getCard(data.upper())
    room = getCard(room.upper())
    if (room == -1):
        data = input ("{} Unrecognized - reenter".format(data))
        room = getCard(data.upper())
    players[player].addPossibility(suspect, weapon, room)


def RoomSelection():
    suspect = input ("Enter Suspect. First 2 letters eg GR = GREEN")
    weapon = input ("Enter Weapon. First 2 letters eg CA = CANDLESTICK")
    room = input ("Enter Room. First 2 letters eg DI = DINING_ROOM. NOTE - Games Room is GM")
    while (1):
        data = input ("Enter operation. eg 3P = Possibility for player 3, 2N = NotCards for player 2, 3R = Player 3 has the room card, 2S is player 2 has the suspect. 0 to exit")
        op = data.upper()
        try:
            player = int(op[0])
        except:
            break
        if (player == 0):
            break
        if (op[1] == 'P'):
            print ("Possibility for {}".format(player))
            addAPossibility (player, suspect, weapon, room)
        elif (op[1] == 'N'):
            print ("NotCards for {}".format(player))
            addANotCard (player, suspect)
            addANotCard (player, weapon)
            addANotCard (player, room)
        elif (op[1] == 'S'):
            print ("Suspect for {}".format(player))
            addACard (player, suspect)
        elif (op[1] == 'W'):
            print ("Weapon for {}".format(player))
            addACard (player, weapon)
        elif (op[1] == 'R'):
            print ("Room for {}".format(player))
            addACard (player, room)

def loadGame(players):
    curPlayer = 0
    fp = open("cleudo.txt")
    for line in fp:
        cards, notCards, *rest = line.split(" ")
        players[curPlayer].Cards = int(cards)
        players[curPlayer].notCards = int(notCards)
        for x in rest:
            players[curPlayer].possibleCards.append(int(x))
        players[curPlayer].rationalizePossibilities()
        curPlayer = curPlayer + 1


def saveGame(players):
    f = open("cleudo.txt", "w")
    for x in players:
        f.write("{} {}".format(x.Cards, x.notCards))
        for y in x.possibleCards:
            f.write(" {}".format(y))
        f.write ("\n")
    f.close()

def main():
    for x in range(numPlayers):
        players.append(Player(x+1))

    data = input ("Load game? Y/N")
    if (data == "Y" or data == "y"):
        loadGame(players)
        solution(players)
    else:
        for x in range(numPlayerCards):
            data = input ("Enter P1 Starting Hand Card {}. First 2 letters eg GR = GREEN or RO = ROPE. NOTE - Games Room is GM".format(x))
            addACard (1, data)

    # player1 not cards are the complement of the cards
    players[0].notCards = ~players[0].Cards
    # only use the least significant numCards bits
    players[0].notCards &= AllMask

    while (1):
        data = input ("1 or return = play another round. 0 to exit")
        try:
            op = int(data)
        except:
            op = 1
        if (op == 0):
            break

        RoomSelection()
        solution(players)
        #report(players)
        saveGame(players)

if __name__ == '__main__':
    main()
