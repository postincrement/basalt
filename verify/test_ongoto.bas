1 print "--START--"
10 x = 2
20 on x goto 100, 200, 300
30 print "fell through"
40 goto 9999
100 print "one"
110 goto 9999
200 print "two"
210 goto 9999
300 print "three"
9999 system
