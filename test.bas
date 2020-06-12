5 rem This is a test
6 averylongvername = 23
7 averylongvername2 = 24

10 print "hello" , "world"
20 print "hello" ; "world"
30 print "hello, world"

40 a% = 2
41 b% = 6.4#

50 a$ = "fred"

60 a! = 3.1

70 a# = 3.1456
80 a = 12345

90 print "this is a% => " a%
91 print "this is a$ => " a$
92 print "this is a! => " a!
93 print "this is a# => " a#
94 print "this is a => " a

100 m = m + 1   : print "m "; m 
101 n = m - 10  : print "n "; n
102 o = m * 23  : print "o "; o
103 p = o / 9  : print "p "; p
104 q = -m      : print "q" ; q
110 z = m * 4 + n + o / p * m : print "z " z

200 goto 1205
201

300 r$ = a$
301 r = len(a$)

310 s$ = str$(r)
311 s$ = "xxx" + "   "
312 s$ = s$ + "   "
313 print s$
314 s$ = s$ + "   " + s$ + a$ + "fred" : print s$
315 s$ = s$
316 rem test

400 s = sqr(300)

999 end

1201 goto 1210
1205 goto 1201
1210 print "made it!" : goto 201
