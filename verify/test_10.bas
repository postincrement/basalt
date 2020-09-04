1 print "--START--"
10 GOSUB 100 : print "after gosub"
20 print "made it to end"
30 system
100 PRINT "made it to subroutine"
101 for i = 0 to 10 : print i 
102 if i = 5 then return
103 next
110 return
9999 system