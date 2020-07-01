from pycipher import SimpleSubstitution as SimpleSub
import random
import re

print ("Enter text to encode")

ctext = input()

parentkey = list('12345FGHIJKLMNOPQRSTUVWXYZ')

random.shuffle(parentkey)
ciphered = SimpleSub(parentkey).encipher(ctext, True)

stext = list(ciphered)
k = 0
for t in ctext:
    if t.islower():
        stext[k]=stext[k].lower()
    k = k + 1

print (''.join(stext))

#print (ciphered)