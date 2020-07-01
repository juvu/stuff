#-------------------------------------------------------------------------------
# Name:        module2
# Purpose:
#
# Author:      antdi
#
# Created:     25/06/2020
# Copyright:   (c) antdi 2020
# Licence:     <your licence>
#-------------------------------------------------------------------------------


filepath = 'C:\DATA\personal\ITTestFile.txt'
fp = open(filepath)
for line in fp:
    z = line.split()
    if (len(z) >= 8):
        txt = "{0} {1} {2}".format(z[2],z[4],z[7])
    elif (len(z) >= 5):
        txt = "{0} {1}".format(z[2],z[4])
    elif (len(z) >= 3):
        txt = "{0}".format(z[2])
    else:
        txt ="less than 3 words!"
    print (txt)


