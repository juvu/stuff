#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      antdi
#
# Created:     03/07/2020
# Copyright:   (c) antdi 2020
# Licence:     <your licence>
#-------------------------------------------------------------------------------

import kivy
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.spinner import Spinner
from kivy.uix.widget import Widget
from kivy.properties import ObjectProperty



class MyGrid(Widget):
    red = ObjectProperty(None)
    yellow = ObjectProperty(None)
    blue=ObjectProperty(None)
    black=ObjectProperty(None)
    win=ObjectProperty(None)
    score=ObjectProperty(None)
    draw=ObjectProperty(None)

    redVal = 0
    yellowVal = 0
    blueVal = 0
    blackVal = 0
    RYscore = 0
    BKscore = 0

    def hoopName(self, hoop):
        if (hoop == 0):
            return ("1")
        elif (hoop == 1):
            return ("2")
        elif (hoop == 2):
            return ("3")
        elif (hoop == 3):
            return ("4")
        elif (hoop == 4):
            return ("5")
        elif (hoop == 5):
            return ("6")
        elif (hoop == 6):
            return ("1 Back")
        elif (hoop == 7):
            return ("2 Back")
        elif (hoop == 8):
            return ("3 Back")
        elif (hoop == 9):
            return ("4 Back")
        elif (hoop == 10):
            return ("Penult")
        elif (hoop == 11):
            return ("Rover")
        elif (hoop == 12):
            return ("Peg")
        elif (hoop == 13):
            return ("Box")
        else:
            return ("??")

    def hoopVal(self, hoop):
        if (hoop == "1"):
            return (0)
        elif (hoop == "2"):
            return (1)
        elif (hoop == "3"):
            return (2)
        elif (hoop == "4"):
            return (3)
        elif (hoop == "5"):
            return (4)
        elif (hoop == "6"):
            return (5)
        elif (hoop == "1 Back"):
            return (6)
        elif (hoop == "2 Back"):
            return (7)
        elif (hoop == "3 Back"):
            return (8)
        elif (hoop == "4 Back"):
            return (9)
        elif (hoop == "Penult"):
            return (10)
        elif (hoop == "Rover"):
            return (11)
        elif (hoop == "Peg"):
            return (12)
        elif (hoop == "Box"):
            return (13)
        else:
            return (14)

    def spinnerRed(self, value):
        self.redVal = self.hoopVal(value)
        self.RYscore = self.redVal + self.yellowVal

    def spinnerYellow(self, value):
        self.yellowVal = self.hoopVal(value)
        self.RYscore = self.redVal + self.yellowVal

    def spinnerBlue(self, value):
        self.blueVal = self.hoopVal(value)
        self.BKscore = self.blueVal + self.blackVal

    def spinnerBlack(self, value):
        self.blackVal = self.hoopVal(value)
        self.BKscore = self.blueVal + self.blackVal

    def btn(self):
        if (self.RYscore > self.BKscore):
            self.score.text = "Score:\nRY {0} BK {1}      RY +{2}".format(self.RYscore, self.BKscore, self.RYscore - self.BKscore)
            pointsNeeded = self.RYscore - self.BKscore + 1
            toWin1 = "none"
            toWin2 = "none"
            toWin3 = "none"
            toWin4 = "none"
            if (self.blueVal + pointsNeeded < 13):
                finalPos = self.hoopName(self.blueVal + pointsNeeded)
                toWin1 = "B to {0}".format(finalPos)
            elif (self.blueVal + pointsNeeded == 13):
                toWin1 = "B to Box"
                peelFinalPos = self.hoopName(self.blackVal + 1)
                toWin3 = "B to Peg, K 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.blueVal + pointsNeeded - 13;
                peelFinalPos = self.hoopName(self.blackVal + peelsNeeded);
                toWin1 = "B to Box, K {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.blackVal + peelsNeeded + 1)
                toWin3 = "B to Peg, K {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (self.blackVal + pointsNeeded <= 13):
                finalPos = self.hoopName(self.blackVal + pointsNeeded)
                toWin2 = "K to {0}".format(finalPos)
            elif (self.blackVal + pointsNeeded == 13):
                toWin2 = "K to Box";
                peelFinalPos = self.hoopName(self.blueVal + 1)
                toWin4 = "K to Peg, B 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.blackVal + pointsNeeded - 13
                peelFinalPos = self.hoopName(self.blueVal + peelsNeeded)
                toWin2 = "K to Box, B {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.blueVal + peelsNeeded + 1)
                toWin4 = "K to Peg, B {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (toWin3 != "none" and toWin4 != "none"):
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n3) {2}\n4) {3}".format(toWin1, toWin2, toWin3, toWin4)
            elif (toWin3 != "none"):
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin3)
            elif (toWin4 != "none"):
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin4)
            else:
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n".format(toWin1, toWin2)

        elif (self.BKscore > self.RYscore):
            self.score.text = "Score:\nRY {0} BK {1}      BK +{2}".format(self.RYscore, self.BKscore, self.BKscore - self.RYscore)
            pointsNeeded = self.BKscore - self.RYscore + 1
            toWin1 = "none"
            toWin2 = "none"
            toWin3 = "none"
            toWin4 = "none"
            if (self.redVal + pointsNeeded < 13):
                finalPos = self.hoopName(self.redVal + pointsNeeded)
                toWin1 = "R to {0}".format(finalPos)
            elif (self.redVal + pointsNeeded == 13):
                toWin1 = "R to Box"
                peelFinalPos = self.hoopName(self.yellowVal + 1)
                toWin3 = "R to Peg, Y 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.redVal + pointsNeeded - 13;
                peelFinalPos = self.hoopName(self.yellowVal + peelsNeeded);
                toWin1 = "R to Box, Y {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.yellowkVal + peelsNeeded + 1)
                toWin3 = "R to Peg, Y {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (self.yellowVal + pointsNeeded <= 13):
                finalPos = self.hoopName(self.yellowVal + pointsNeeded)
                toWin2 = "Y to {0}".format(finalPos)
            elif (self.yellowVal + pointsNeeded == 13):
                toWin2 = "Y to Box";
                peelFinalPos = self.hoopName(self.redVal + 1)
                toWin4 = "Y to Peg, R 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.yellowVal + pointsNeeded - 13
                peelFinalPos = self.hoopName(self.redVal + peelsNeeded)
                toWin2 = "Y to Box, R {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.blueVal + peelsNeeded + 1)
                toWin4 = "Y to Peg, R {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (toWin3 != "none" and toWin4 != "none"):
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n3) {2}\n4) {3}".format(toWin1, toWin2, toWin3, toWin4)
            elif (toWin3 != "none"):
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin3)
            elif (toWin4 != "none"):
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin4)
            else:
                self.win.text = "To win by 1:\n1) {0}\n2) {1}\n".format(toWin1, toWin2)

        elif (self.BKscore == self.RYscore):
            self.score.text = "Score:\nRY {0} BK {1}      Score tied".format(self.RYscore, self.BKscore)
            self.win.text = "Golden Hoop\n"


        if (self.RYscore > self.BKscore):
            pointsNeeded = self.RYscore - self.BKscore
            toWin1 = "none"
            toWin2 = "none"
            toWin3 = "none"
            toWin4 = "none"
            if (self.blueVal + pointsNeeded < 13):
                finalPos = self.hoopName(self.blueVal + pointsNeeded)
                toWin1 = "B to {0}".format(finalPos)
            elif (self.blueVal + pointsNeeded == 13):
                toWin1 = "B to Box"
                peelFinalPos = self.hoopName(self.blackVal + 1)
                toWin3 = "B to Peg, K 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.blueVal + pointsNeeded - 13;
                peelFinalPos = self.hoopName(self.blackVal + peelsNeeded);
                toWin1 = "B to Box, K {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.blackVal + peelsNeeded + 1)
                toWin3 = "B to Peg, K {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (self.blackVal + pointsNeeded <= 13):
                finalPos = self.hoopName(self.blackVal + pointsNeeded)
                toWin2 = "K to {0}".format(finalPos)
            elif (self.blackVal + pointsNeeded == 13):
                toWin2 = "K to Box";
                peelFinalPos = self.hoopName(self.blueVal + 1)
                toWin4 = "K to Peg, B 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.blackVal + pointsNeeded - 13
                peelFinalPos = self.hoopName(self.blueVal + peelsNeeded)
                toWin2 = "K to Box, B {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.blueVal + peelsNeeded + 1)
                toWin4 = "K to Peg, B {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (toWin3 != "none" and toWin4 != "none"):
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n3) {2}\n4) {3}".format(toWin1, toWin2, toWin3, toWin4)
            elif (toWin3 != "none"):
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin3)
            elif (toWin4 != "none"):
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin4)
            else:
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n".format(toWin1, toWin2)

        elif (self.BKscore > self.RYscore):
            pointsNeeded = self.BKscore - self.RYscore
            toWin1 = "none"
            toWin2 = "none"
            toWin3 = "none"
            toWin4 = "none"
            if (self.redVal + pointsNeeded < 13):
                finalPos = self.hoopName(self.redVal + pointsNeeded)
                toWin1 = "R to {0}".format(finalPos)
            elif (self.redVal + pointsNeeded == 13):
                toWin1 = "R to Box"
                peelFinalPos = self.hoopName(self.yellowVal + 1)
                toWin3 = "R to Peg, Y 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.redVal + pointsNeeded - 13;
                peelFinalPos = self.hoopName(self.yellowVal + peelsNeeded);
                toWin1 = "R to Box, Y {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.yellowkVal + peelsNeeded + 1)
                toWin3 = "R to Peg, Y {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (self.yellowVal + pointsNeeded <= 13):
                finalPos = self.hoopName(self.yellowVal + pointsNeeded)
                toWin2 = "Y to {0}".format(finalPos)
            elif (self.yellowVal + pointsNeeded == 13):
                toWin2 = "Y to Box";
                peelFinalPos = self.hoopName(self.redVal + 1)
                toWin4 = "Y to Peg, R 1 peel to {0}".format(peelFinalPos)
            else:
                peelsNeeded = self.yellowVal + pointsNeeded - 13
                peelFinalPos = self.hoopName(self.redVal + peelsNeeded)
                toWin2 = "Y to Box, R {0} peels to {1}".format(peelsNeeded, peelFinalPos)
                peelFinalPos = self.hoopName(self.blueVal + peelsNeeded + 1)
                toWin4 = "Y to Peg, R {0} peels to {1}".format(peelsNeeded + 1, peelFinalPos)

            if (toWin3 != "none" and toWin4 != "none"):
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n3) {2}\n4) {3}".format(toWin1, toWin2, toWin3, toWin4)
            elif (toWin3 != "none"):
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin3)
            elif (toWin4 != "none"):
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n3) {2}".format(toWin1, toWin2, toWin4)
            else:
                self.draw.text = "To draw level:\n1) {0}\n2) {1}\n".format(toWin1, toWin2)

class MyApp(App):
    def build(self):
        return MyGrid()

if __name__ == '__main__':
    MyApp().run()
