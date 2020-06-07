/* Generator by basalt */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

extern int basalt_init();
extern int basalt_print_tab();
extern int basalt_print_newline();
extern int basalt_print_string(const char *);
extern int basalt_print_int16(int);
extern int basalt_print_int32(int);
extern int basalt_print_single(float);
extern int basalt_print_double(double);
extern int basalt_strlen(const char *);

/* Vars */
const char * USER__string = 0; /*  */
float USER_A_single = 0; /* A */
float USER_AB_single = 0; /* AB! */
float USER_B3_single = 0; /* B3! */
float USER_B4_single = 0; /* B4! */
float USER_B5_single = 0; /* B5! */
float USER_B9_single = 0; /* B9! */
float USER_C_single = 0; /* C */
float USER_C1_single = 0; /* C1! */
float USER_D_single = 0; /* D */
float USER_D0_single = 0; /* D0! */
float USER_D1_single = 0; /* D1! */
float USER_D3_single = 0; /* D3! */
float USER_D4_single = 0; /* D4! */
float USER_D6_single = 0; /* D6! */
float USER_E_single = 0; /* E */
float USER_E0_single = 0; /* E0! */
float USER_FN_single = 0; /* FN! */
float USER_G_single = 0; /* G */
float USER_G5_single = 0; /* G5! */
float USER_H_single = 0; /* H */
float USER_H1_single = 0; /* H1! */
float USER_H8_single = 0; /* H8! */
float USER_I_single = 0; /* I */
float USER_J_single = 0; /* J */
float USER_J0_single = 0; /* J0! */
float USER_K_single = 0; /* K */
float USER_K3_single = 0; /* K3! */
float USER_K7_single = 0; /* K7! */
float USER_K9_single = 0; /* K9! */
float USER_L_single = 0; /* L */
float USER_N_single = 0; /* N */
float USER_P_single = 0; /* P */
float USER_P0_single = 0; /* P0! */
float USER_Q1_single = 0; /* Q1! */
float USER_Q2_single = 0; /* Q2! */
float USER_Q4_single = 0; /* Q4! */
float USER_Q5_single = 0; /* Q5! */
float USER_R_single = 0; /* R */
float USER_R1_single = 0; /* R1! */
float USER_R2_single = 0; /* R2! */
float USER_RN_single = 0; /* RN! */
float USER_S_single = 0; /* S */
float USER_S1_single = 0; /* S1! */
float USER_S2_single = 0; /* S2! */
float USER_S3_single = 0; /* S3! */
float USER_S8_single = 0; /* S8! */
float USER_S9_single = 0; /* S9! */
float USER_T_single = 0; /* T */
float USER_T0_single = 0; /* T0! */
float USER_T8_single = 0; /* T8! */
float USER_T9_single = 0; /* T9! */
float USER_W1_single = 0; /* W1! */
float USER_X_single = 0; /* X */
float USER_X1_single = 0; /* X1! */
float USER_X2_single = 0; /* X2! */
float USER_X3_single = 0; /* X3! */
float USER_X5_single = 0; /* X5! */
float USER_Y_single = 0; /* Y */
float USER_Y3_single = 0; /* Y3! */
float USER_YY_single = 0; /* YY! */
float USER_Z_single = 0; /* Z */
float USER_Z1_single = 0; /* Z1! */
float USER_Z2_single = 0; /* Z2! */
float USER_Z3_single = 0; /* Z3! */
float USER_Z4_single = 0; /* Z4! */
float USER_Z5_single = 0; /* Z5! */

int main(int argc, char * argv[])
{
  basalt_init();
  /* 10 REM SUPER STARTREK - MAY 16,1978 - REQUIRES 24K MEMORY (AT LEAST) */
  line_10:
  /* 30 REM */
  line_30:
  /* 50 REM **** SIMULATION OF A MISSION OF THE STARSHIP ENTERPRISE, */
  line_50:
  /* 60 REM **** AS SEEN ON THE STAR TREK TV SHOW. */
  line_60:
  /* 70 REM **** ORIGINAL PROGRAM BY MIKE MAYFIELD, MODIFIED VERSION */
  line_70:
  /* 80 REM **** PUBLISHED IN DEC'S "101 BASIC GAMES", BY DAVE AHL. */
  line_80:
  /* 90 REM **** MODIFICATIONS TO THE LATTER (PLUS DEBUGGING) BY BOB */
  line_90:
  /* 100 REM *** LEEDOM - APRIL & DECEMBER 1974, */
  line_100:
  /* 110 REM *** WITH A LITTLE HELP FROM HIS FRIENDS . . . */
  line_110:
  /* 120 REM *** COMMENTS, EPHITETS, AND SUGGESTIONS SOLICITED -- */
  line_120:
  /* 130 REM *** SEND TO: R.C. LEEDOM */
  line_130:
  /* 140 REM ***          WESTINGHOSE DEFENSE & ELECTRONICS SYSTEMS CNIR */
  line_140:
  /* 150 REM ***          BOX 746, M.S. 338 */
  line_150:
  /* 160 REM ***          BALTIMORE, MD 21203 */
  line_160:
  /* 170 REM *** */
  line_170:
  /* 180 REM *** CONVERTED TO MICROSOFT 8 K BASIC 3/16/78 BY JOHN BORDERS */
  line_180:
  /* 190 REM *** LINE NUMBERS FROm VERSION TREK7 OF 1/12/75 PRESERVED AS */
  line_190:
  /* 200 REM *** MUCH AS POSSIBLE WHILE USING MULTIPLE STATEMENTS PER LINE */
  line_200:
  /* 205 WIDTH 80 */
  line_205:
  /* 210 PRINT CHR$(26) */
  line_210:
  basalt_print_newline();
  /* 220 FOR XX=1 TO 6:PRINT:NEXT:PRINT TAB(20);"THE USS ENTERPRISE --- NCC-1701":PRINT:FOR YY=1 TO 40 STEP 2 */
  line_220:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  /* 221 PRINT TAB(YY);"                  ,------*------," */
  line_221:
  basalt_print_newline();
  /* 222 PRINT TAB(YY);"  ,-------------   '---  ------'" */
  line_222:
  basalt_print_newline();
  /* 223 PRINT TAB(YY);"   '-------- --'      / /" */
  line_223:
  basalt_print_newline();
  /* 224 PRINT TAB(YY);"       ,---' '-------/ /--," */
  line_224:
  basalt_print_newline();
  /* 225 PRINT TAB(YY);"        '----------------'" */
  line_225:
  basalt_print_newline();
  /* 226 PRINT:PRINT:FOR ZZ=1 TO 7:PRINT CHR$(11);:NEXT ZZ:NEXT YY */
  line_226:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  /* 227 PRINT:PRINT:PRINT:PRINT:PRINT */
  line_227:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  /* 260 CLEAR 600 */
  line_260:
  /* 270 Z$="                         " */
  line_270:
  /* 330 DIM G(8,8),C(9,2),K(3,3),N(3),Z(8,8),D(8) */
  line_330:
  /* 370 T=INT(RND(1)*20+20)*100:T0=T:T9=25+INT(RND(1)*10):D0=0:E=3000:E0=E */
  line_370:
  USER_T_single = (/* cast from 0 to 3 */ ((int)(((USER_RN_single * 20) + 20))) * 100);
  USER_T0_single = USER_T_single;
  USER_T9_single = (25 + /* cast from 0 to 3 */ ((int)((USER_RN_single * 10))));
  USER_D0_single = 0;
  USER_E_single = 3000;
  USER_E0_single = USER_E_single;
  /* 440 P=10:P0=P:S9=200:S=0:B9=0:K9=0:X$="":X0$=" IS " */
  line_440:
  USER_P_single = 10;
  USER_P0_single = USER_P_single;
  USER_S9_single = 200;
  USER_S_single = 0;
  USER_B9_single = 0;
  USER_K9_single = 0;
  /* 470 DEF FND(D)=SQR((K(I,1)-S1)^2+(K(I,2)-S2)^2) */
  line_470:
  /* 475 DEF FNR(R)=INT(RND(R)*7.98+1.01) */
  line_475:
  /* 490 Q1=FNR(1):Q2=FNR(1):S1=FNR(1):S2=FNR(1) */
  line_490:
  USER_Q1_single = USER_FN_single;
  USER_Q2_single = USER_FN_single;
  USER_S1_single = USER_FN_single;
  USER_S2_single = USER_FN_single;
  /* 530 FOR I=1 TO 9:C(I,1)=0:C(I,2)=0:NEXT I */
  line_530:
  USER_C_single = 0;
  USER_C_single = 0;
  /* 540 C(3,1)=-1:C(2,1)=-1:C(4,1)=-1:C(4,2)=-1:C(5,2)=-1:C(6,2)=-1 */
  line_540:
  USER_C_single = (- 1);
  USER_C_single = (- 1);
  USER_C_single = (- 1);
  USER_C_single = (- 1);
  USER_C_single = (- 1);
  USER_C_single = (- 1);
  /* 600 C(1,2)=1:C(2,2)=1:C(6,1)=1:C(7,1)=1:C(8,1)=1:C(8,2)=1:C(9,2)=1 */
  line_600:
  USER_C_single = 1;
  USER_C_single = 1;
  USER_C_single = 1;
  USER_C_single = 1;
  USER_C_single = 1;
  USER_C_single = 1;
  USER_C_single = 1;
  /* 670 FOR I=1 TO 8:D(I)=0:NEXT I */
  line_670:
  USER_D_single = 0;
  /* 710 A1$="NAVSRSLRSPHATORSHEDAMCOMXXX" */
  line_710:
  /* 820 FOR I=1 TO 8:FOR J=1 TO 8:K3=0:Z(I,J)=0:R1=RND(1) */
  line_820:
  USER_K3_single = 0;
  USER_Z_single = 0;
  USER_R1_single = USER_RN_single;
  /* 850 IF R1>.98 THEN K3=3:K9=K9+3:GOTO 980 */
  line_850:
  /* 860 IF R1>.95 THEN K3=2:K9=K9+2:GOTO 980 */
  line_860:
  /* 870 IF R1>.8 THEN K3=1:K9=K9+1 */
  line_870:
  /* 980 B3=0:IF RND(1)>.96 THEN B3=1:B9=B9+1 */
  line_980:
  USER_B3_single = 0;
  /* 1040 G(I,J)=K3*100+B3*10+FNR(1):NEXT J:NEXT I:IF K9>T9 THEN T9=K9+1 */
  line_1040:
  USER_G_single = ((USER_K3_single * 100) + ((USER_B3_single * 10) + USER_FN_single));
  /* 1100 IF B9<>0 THEN 1200 */
  line_1100:
  /* 1150 IF G(Q1,Q2)<200 THEN G(Q1,Q2)=G(Q1,Q2)+100:K9=K9+1 */
  line_1150:
  /* 1160 B9=1:G(Q1,Q2)=G(Q1,Q2)+10:Q1=FNR(1):Q2=FNR(1) */
  line_1160:
  USER_B9_single = 1;
  USER_G_single = (USER_G_single + 10);
  USER_Q1_single = USER_FN_single;
  USER_Q2_single = USER_FN_single;
  /* 1200 K7=K9:IF B9<>1 THEN X$="S":X0$=" ARE " */
  line_1200:
  USER_K7_single = USER_K9_single;
  /* 1230 PRINT"YOUR ORDERS ARE AS FOLLOWS:" */
  line_1230:
  basalt_print_newline();
  /* 1235 PRINT "--------------------------" */
  line_1235:
  basalt_print_newline();
  /* 1240 PRINT"   DESTROY THE";K9;"KLINGON WARSHIPS WHICH HAVE INVADED" */
  line_1240:
  basalt_print_single(USER_K9_single);
  basalt_print_newline();
  /* 1250 PRINT"   THE GALAXY BEFORE THEY CAN ATTACK FEDERATION HEADQUARTERS" */
  line_1250:
  basalt_print_newline();
  /* 1260 PRINT"   ON STARDATE";T0+T9;CHR$(8);". THIS GIVES YOU";T9;"DAYS. THERE";X0$ */
  line_1260:
  basalt_print_single(USER_T9_single);
  basalt_print_newline();
  /* 1270 PRINT"  ";B9;"STARBASE";X$;" IN THE GALAXY FOR RESUPPLYING YOUR SHIP." */
  line_1270:
  basalt_print_single(USER_B9_single);
  basalt_print_newline();
  /* 1280 PRINT:PRINT "ARE YOU READY TO ACCEPT COMMAND ('N' FOR INSTRUCTIONS)"; */
  line_1280:
  basalt_print_newline();
  basalt_print_newline();
  /* 1300 INPUT I5$:IF LEFT$(I5$,1)="N" OR LEFT$(I5$,1)="n" THEN RUN "TREKINST" ELSE IF LEFT$(I5$,1)="Y" OR LEFT$(I5$,1)="y" THEN 1310 ELSE 1280 */
  line_1300:
  /* 1310 PRINT CHR$(26) */
  line_1310:
  basalt_print_newline();
  /* 1320 Z4=Q1:Z5=Q2:K3=0:B3=0:S3=0:G5=0:D4=.5*RND(1):Z(Q1,Q2)=G(Q1,Q2) */
  line_1320:
  USER_Z4_single = USER_Q1_single;
  USER_Z5_single = USER_Q2_single;
  USER_K3_single = 0;
  USER_B3_single = 0;
  USER_S3_single = 0;
  USER_G5_single = 0;
  USER_D4_single = (0.5 * USER_RN_single);
  USER_Z_single = USER_G_single;
  /* 1390 IF Q1<1 OR Q1>8 OR Q2<1 OR Q2>8 THEN 1600 */
  line_1390:
  /* 1430 GOSUB 9030:PRINT:IF T0<>T THEN 1490 */
  line_1430:
  basalt_print_newline();
  /* 1460 PRINT"YOUR MISSION BEGINS WITH YOUR STARSHIP LOCATED" */
  line_1460:
  basalt_print_newline();
  /* 1470 PRINT"IN THE GALACTIC QUADRANT, '";G2$;"'.":GOTO 1500 */
  line_1470:
  basalt_print_newline();
  goto line_1500;
  /* 1490 PRINT"NOW ENTERING ";G2$;" QUADRANT . . ." */
  line_1490:
  basalt_print_newline();
  /* 1500 PRINT:K3=INT(G(Q1,Q2)*.01):B3=INT(G(Q1,Q2)*.1)-10*K3 */
  line_1500:
  basalt_print_newline();
  USER_K3_single = /* cast from 0 to 3 */ ((int)((USER_G_single * 0.01)));
  USER_B3_single = (/* cast from 0 to 3 */ ((int)((USER_G_single * 0.1))) - (10 * USER_K3_single));
  /* 1540 S3=G(Q1,Q2)-100*K3-10*B3:IF K3=0 THEN 1590 */
  line_1540:
  USER_S3_single = (USER_G_single - ((100 * USER_K3_single) - (10 * USER_B3_single)));
  /* 1560 PRINT TAB(3);CHR$(22);"  COMBAT AREA      CONDITION RED  ";CHR$(22):IF S>200 THEN PRINT:GOTO 1590 */
  line_1560:
  basalt_print_newline();
  /* 1580 PRINT TAB(3);CHR$(22);"      SHIELDS DANGEROUSLY LOW     ";CHR$(22):PRINT */
  line_1580:
  basalt_print_newline();
  basalt_print_newline();
  /* 1590 FOR I=1 TO 3:K(I,1)=0:K(I,2)=0:NEXT I */
  line_1590:
  USER_K_single = 0;
  USER_K_single = 0;
  /* 1600 FOR I=1 TO 3:K(I,3)=0:NEXT I:Q$=Z$+Z$+Z$+Z$+Z$+Z$+Z$+LEFT$(Z$,17) */
  line_1600:
  USER_K_single = 0;
  /* 1680 A$="<E>":Z1=S1:Z2=S2:GOSUB 8670:IF K3<1 THEN 1820 */
  line_1680:
  USER_Z1_single = USER_S1_single;
  USER_Z2_single = USER_S2_single;
  /* 1720 FOR I=1 TO K3:GOSUB 8590:A$="+K+":Z1=R1:Z2=R2 */
  line_1720:
  USER_Z1_single = USER_R1_single;
  USER_Z2_single = USER_R2_single;
  /* 1780 GOSUB 8670:K(I,1)=R1:K(I,2)=R2:K(I,3)=S9*(.5+RND(1)):NEXT I */
  line_1780:
  USER_K_single = USER_R1_single;
  USER_K_single = USER_R2_single;
  USER_K_single = (USER_S9_single * (0.5 + USER_RN_single));
  /* 1820 IF B3<1 THEN 1910 */
  line_1820:
  /* 1880 GOSUB 8590:A$=">B<":Z1=R1:B4=R1:Z2=R2:B5=R2:GOSUB 8670 */
  line_1880:
  USER_Z1_single = USER_R1_single;
  USER_B4_single = USER_R1_single;
  USER_Z2_single = USER_R2_single;
  USER_B5_single = USER_R2_single;
  /* 1910 FOR I=1 TO S3:GOSUB 8590:A$=" * ":Z1=R1:Z2=R2:GOSUB 8670:NEXT I */
  line_1910:
  USER_Z1_single = USER_R1_single;
  USER_Z2_single = USER_R2_single;
  /* 1980 GOSUB 6430 */
  line_1980:
  /* 1990 IF S+E>10 THEN IF E>10 OR D(7)=0 THEN 2060 */
  line_1990:
  /* 2020 PRINT:PRINT TAB(10);CHR$(22);"** FATAL ERROR **";CHR$(22):PRINT"YOU'VE JUST STRANDED YOUR SHIP IN SPACE." */
  line_2020:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  /* 2030 PRINT"YOU HAVE INSUFFICIENT MANEUVERING ENERGY," */
  line_2030:
  basalt_print_newline();
  /* 2040 PRINT"AND SHIELD CONTROL IS PRESENTLY INCAPABLE OF" */
  line_2040:
  basalt_print_newline();
  /* 2050 PRINT"CROSS-CIRCUITING TO ENGINE ROOM!!":PRINT:GOTO 6220 */
  line_2050:
  basalt_print_newline();
  basalt_print_newline();
  goto line_6220;
  /* 2060 PRINT:INPUT"COMMAND";A$:PRINT */
  line_2060:
  basalt_print_newline();
  basalt_print_newline();
  /* 2080 FOR I=1 TO 9:IF LEFT$(A$,3)<>MID$(A1$,3*I-2,3)THEN 2160 */
  line_2080:
  /* 2140 ON I GOTO 2300,1980,4000,4260,4700,5530,5690,7290,6270 */
  line_2140:
  /* 2160 NEXT I:PRINT"ENTER ONE OF THE FOLLOWING:" */
  line_2160:
  basalt_print_newline();
  /* 2170 PRINT "--------------------------" */
  line_2170:
  basalt_print_newline();
  /* 2180 PRINT"  NAV  (TO SET COURSE)" */
  line_2180:
  basalt_print_newline();
  /* 2190 PRINT"  SRS  (FOR SHORT RANGE SENSOR SCAN)" */
  line_2190:
  basalt_print_newline();
  /* 2200 PRINT"  LRS  (FOR LONG RANGE SENSOR SCAN)" */
  line_2200:
  basalt_print_newline();
  /* 2210 PRINT"  PHA  (TO FIRE PHASERS)" */
  line_2210:
  basalt_print_newline();
  /* 2220 PRINT"  TOR  (TO FIRE PHOTON TORPEDOES)" */
  line_2220:
  basalt_print_newline();
  /* 2230 PRINT"  SHE  (TO RAISE OR LOWER SHIELDS)" */
  line_2230:
  basalt_print_newline();
  /* 2240 PRINT"  DAM  (FOR DAMAGE CONTROL REPORTS)" */
  line_2240:
  basalt_print_newline();
  /* 2250 PRINT"  COM  (TO CALL ON LIBRARY-COMPUTER)" */
  line_2250:
  basalt_print_newline();
  /* 2260 PRINT"  XXX  (TO RESIGN YOUR COMMAND)":PRINT:GOTO 1990 */
  line_2260:
  basalt_print_newline();
  basalt_print_newline();
  goto line_1990;
  /* 2300 INPUT"COURSE (0-9)";C1:IF C1=9 THEN C1=1 */
  line_2300:
  /* 2310 IF C1>=1 AND C1<9 THEN 2350 */
  line_2310:
  /* 2330 PRINT"   LT. SULU: 'INCORRECT COURSE DATA, SIR!'":GOTO 1990 */
  line_2330:
  basalt_print_newline();
  goto line_1990;
  /* 2350 X$="8":IF D(1)<0 THEN X$="0.2" */
  line_2350:
  /* 2360 PRINT"WARP FACTOR (0-";X$;")";:INPUT W1:PRINT:IF D(1)<0 AND W1>.2 THEN 2470 */
  line_2360:
  basalt_print_newline();
  basalt_print_newline();
  /* 2380 IF W1>0 AND W1<=8 THEN 2490 */
  line_2380:
  /* 2390 IF W1=0 THEN 1990 */
  line_2390:
  /* 2420 PRINT"   CHIEF ENGINEER SCOTT: 'THE ENGINES WON'T TAKE"; */
  line_2420:
  basalt_print_newline();
  /* 2430 PRINT" WARP";W1;CHR$(8);"!'":GOTO 1990 */
  line_2430:
  basalt_print_single(USER_W1_single);
  basalt_print_newline();
  goto line_1990;
  /* 2470 PRINT"WARP ENGINES ARE DAMAGED.  MAXIUM SPEED = WARP 0.2":GOTO 1990 */
  line_2470:
  basalt_print_newline();
  goto line_1990;
  /* 2490 N=INT(W1*8+.5):IF E-N>=0 THEN 2590 */
  line_2490:
  USER_N_single = /* cast from 0 to 3 */ ((int)(((USER_W1_single * 8) + 0.5)));
  /* 2500 PRINT"ENGINEERING:  'INSUFFICIENT ENERGY AVAILABLE" */
  line_2500:
  basalt_print_newline();
  /* 2510 PRINT"               FOR MANEUVERING AT WARP";W1;CHR$(8);"!'" */
  line_2510:
  basalt_print_single(USER_W1_single);
  basalt_print_newline();
  /* 2530 IF S<N-E OR D(7)<0 THEN 1990 */
  line_2530:
  /* 2550 PRINT"DEFLECTOR CONTROL ROOM:  ";S;"UNITS OF ENERGY" */
  line_2550:
  basalt_print_single(USER_S_single);
  basalt_print_newline();
  /* 2560 PRINT"                          PRESENTLY DEPLOYED TO SHIELDS." */
  line_2560:
  basalt_print_newline();
  /* 2570 GOTO 1990 */
  line_2570:
  goto line_1990;
  /* 2590 FOR I=1 TO K3:IF K(I,3)=0 THEN 2700 */
  line_2590:
  /* 2610 A$="   ":Z1=K(I,1):Z2=K(I,2):GOSUB 8670:GOSUB 8590 */
  line_2610:
  USER_Z1_single = USER_K_single;
  USER_Z2_single = USER_K_single;
  /* 2660 K(I,1)=Z1:K(I,2)=Z2:A$="+K+":GOSUB 8670 */
  line_2660:
  USER_K_single = USER_Z1_single;
  USER_K_single = USER_Z2_single;
  /* 2700 NEXT I:GOSUB 6000:D1=0:D6=W1:IF W1>=1 THEN D6=1 */
  line_2700:
  USER_D1_single = 0;
  USER_D6_single = USER_W1_single;
  /* 2770 FOR I=1 TO 8:IF D(I)>=0 THEN 2880 */
  line_2770:
  /* 2790 D(I)=D(I)+D6:IF D(I)>-.1 AND D(I)<0 THEN D(I)=-.1:GOTO 2880 */
  line_2790:
  USER_D_single = (USER_D_single + USER_D6_single);
  /* 2800 IF D(I)<0 THEN 2880 */
  line_2800:
  /* 2810 IF D1<>1 THEN D1=1:PRINT"DAMAGE CONTROL REPORT:  "; */
  line_2810:
  /* 2840 PRINT TAB(8);:R1=I:GOSUB 8790:PRINT G2$;" REPAIR COMPLETED." */
  line_2840:
  basalt_print_newline();
  USER_R1_single = USER_I_single;
  basalt_print_newline();
  /* 2880 NEXT I:IF RND(1)>.2 THEN 3070 */
  line_2880:
  /* 2910 R1=FNR(1):IF RND(1)>=.6 THEN 3000 */
  line_2910:
  USER_R1_single = USER_FN_single;
  /* 2930 D(R1)=D(R1)-(RND(1)*5+1):PRINT"DAMAGE CONTROL REPORT:  "; */
  line_2930:
  USER_D_single = (USER_D_single - ((USER_RN_single * 5) + 1));
  basalt_print_newline();
  /* 2960 GOSUB 8790:PRINT G2$;" DAMAGED":PRINT:GOTO 3070 */
  line_2960:
  basalt_print_newline();
  basalt_print_newline();
  goto line_3070;
  /* 3000 D(R1)=D(R1)+RND(1)*3+1:PRINT"DAMAGE CONTROL REPORT:  "; */
  line_3000:
  USER_D_single = (USER_D_single + ((USER_RN_single * 3) + 1));
  basalt_print_newline();
  /* 3030 GOSUB 8790:PRINT G2$;" STATE OF REPAIR IMPROVED":PRINT */
  line_3030:
  basalt_print_newline();
  basalt_print_newline();
  /* 3070 A$="   ":Z1=INT(S1):Z2=INT(S2):GOSUB 8670 */
  line_3070:
  USER_Z1_single = /* cast from 0 to 3 */ ((int)(USER_S1_single));
  USER_Z2_single = /* cast from 0 to 3 */ ((int)(USER_S2_single));
  /* 3110 X1=C(C1,1)+(C(C1+1,1)-C(C1,1))*(C1-INT(C1)):X=S1:Y=S2 */
  line_3110:
  USER_X1_single = (USER_C_single + ((USER_C_single - USER_C_single) * (USER_C1_single - /* cast from 0 to 3 */ ((int)(USER_C1_single)))));
  USER_X_single = USER_S1_single;
  USER_Y_single = USER_S2_single;
  /* 3140 X2=C(C1,2)+(C(C1+1,2)-C(C1,2))*(C1-INT(C1)):Q4=Q1:Q5=Q2 */
  line_3140:
  USER_X2_single = (USER_C_single + ((USER_C_single - USER_C_single) * (USER_C1_single - /* cast from 0 to 3 */ ((int)(USER_C1_single)))));
  USER_Q4_single = USER_Q1_single;
  USER_Q5_single = USER_Q2_single;
  /* 3170 FOR I=1 TO N:S1=S1+X1:S2=S2+X2:IF S1<1 OR S1>=9 OR S2<1 OR S2>=9 THEN 3500 */
  line_3170:
  USER_S1_single = (USER_S1_single + USER_X1_single);
  USER_S2_single = (USER_S2_single + USER_X2_single);
  /* 3240 S8=INT(S1)*24+INT(S2)*3-26:IF MID$(Q$,S8,2)="  "THEN 3360 */
  line_3240:
  USER_S8_single = ((/* cast from 0 to 3 */ ((int)(USER_S1_single)) * 24) + ((/* cast from 0 to 3 */ ((int)(USER_S2_single)) * 3) - 26));
  /* 3320 S1=INT(S1-X1):S2=INT(S2-X2):PRINT"WARP ENGINES SHUT DOWN AT "; */
  line_3320:
  USER_S1_single = /* cast from 0 to 3 */ ((int)((USER_S1_single - USER_X1_single)));
  USER_S2_single = /* cast from 0 to 3 */ ((int)((USER_S2_single - USER_X2_single)));
  basalt_print_newline();
  /* 3350 PRINT"SECTOR";S1;CHR$(8);",";S2;"DUE TO BAD NAVAGATION":GOTO 3370 */
  line_3350:
  basalt_print_single(USER_S1_single);
  basalt_print_single(USER_S2_single);
  basalt_print_newline();
  goto line_3370;
  /* 3360 NEXT I:S1=INT(S1):S2=INT(S2) */
  line_3360:
  USER_S1_single = /* cast from 0 to 3 */ ((int)(USER_S1_single));
  USER_S2_single = /* cast from 0 to 3 */ ((int)(USER_S2_single));
  /* 3370 A$="<E>":Z1=INT(S1):Z2=INT(S2):GOSUB 8670:GOSUB 3910:T8=1 */
  line_3370:
  USER_Z1_single = /* cast from 0 to 3 */ ((int)(USER_S1_single));
  USER_Z2_single = /* cast from 0 to 3 */ ((int)(USER_S2_single));
  USER_T8_single = 1;
  /* 3430 IF W1<1 THEN T8=.1*INT(10*W1) */
  line_3430:
  /* 3450 T=T+T8:IF T>T0+T9 THEN 6220 */
  line_3450:
  USER_T_single = (USER_T_single + USER_T8_single);
  /* 3480 GOTO 1980 */
  line_3480:
  goto line_1980;
  /* 3500 X=8*Q1+X+N*X1:Y=8*Q2+Y+N*X2:Q1=INT(X/8):Q2=INT(Y/8):S1=INT(X-Q1*8) */
  line_3500:
  USER_X_single = ((8 * USER_Q1_single) + (USER_X_single + (USER_N_single * USER_X1_single)));
  USER_Y_single = ((8 * USER_Q2_single) + (USER_Y_single + (USER_N_single * USER_X2_single)));
  USER_Q1_single = /* cast from 0 to 3 */ ((int)((USER_X_single / 8)));
  USER_Q2_single = /* cast from 0 to 3 */ ((int)((USER_Y_single / 8)));
  USER_S1_single = /* cast from 0 to 3 */ ((int)((USER_X_single - (USER_Q1_single * 8))));
  /* 3550 S2=INT(Y-Q2*8):IF S1=0 THEN Q1=Q1-1:S1=8 */
  line_3550:
  USER_S2_single = /* cast from 0 to 3 */ ((int)((USER_Y_single - (USER_Q2_single * 8))));
  /* 3590 IF S2=0 THEN Q2=Q2-1:S2=8 */
  line_3590:
  /* 3620 X5=0:IF Q1<1 THEN X5=1:Q1=1:S1=1 */
  line_3620:
  USER_X5_single = 0;
  /* 3670 IF Q1>8 THEN X5=1:Q1=8:S1=8 */
  line_3670:
  /* 3710 IF Q2<1 THEN X5=1:Q2=1:S2=1 */
  line_3710:
  /* 3750 IF Q2>8 THEN X5=1:Q2=8:S2=8 */
  line_3750:
  /* 3790 IF X5=0 THEN 3860 */
  line_3790:
  /* 3800 PRINT"LT. UHURA: MESSAGE FROM STARFLEET COMMAND --" */
  line_3800:
  basalt_print_newline();
  /* 3810 PRINT"  'PERMISSION TO ATTEMPT CROSSING OF GALACTIC PERIMETER" */
  line_3810:
  basalt_print_newline();
  /* 3820 PRINT"  IS HEREBY *DENIED*.  SHUT DOWN YOUR ENGINES.'" */
  line_3820:
  basalt_print_newline();
  /* 3830 PRINT"CHIEF ENGINEER SCOTT:  'WARP ENGINES SHUT DOWN" */
  line_3830:
  basalt_print_newline();
  /* 3840 PRINT"  AT SECTOR";S1;CHR$(8);",";S2;"OF QUADRANT";Q1;CHR$(8);",";Q2;CHR$(8);".'" */
  line_3840:
  basalt_print_single(USER_S1_single);
  basalt_print_single(USER_S2_single);
  basalt_print_single(USER_Q1_single);
  basalt_print_single(USER_Q2_single);
  basalt_print_newline();
  /* 3850 IF T>T0+T9 THEN 6220 */
  line_3850:
  /* 3860 IF 8*Q1+Q2=8*Q4+Q5 THEN 3370 */
  line_3860:
  /* 3870 T=T+1:GOSUB 3910:GOTO 1320 */
  line_3870:
  USER_T_single = (USER_T_single + 1);
  goto line_1320;
  /* 3910 E=E-N-10:IF E>=0 THEN RETURN */
  line_3910:
  USER_E_single = (USER_E_single - (USER_N_single - 10));
  /* 3930 PRINT"SHIELD CONTROL SUPPLIES ENERGY TO COMPLETE THE MANEUVER." */
  line_3930:
  basalt_print_newline();
  /* 3940 S=S+E:E=0:IF S<=0 THEN S=0 */
  line_3940:
  USER_S_single = (USER_S_single + USER_E_single);
  USER_E_single = 0;
  /* 3980 RETURN */
  line_3980:
  /* 4000 IF D(3)<0 THEN PRINT"LONG RANGE SENSORS ARE INOPERABLE.":GOTO 1990 */
  line_4000:
  /* 4030 PRINT"LONG RANGE SCAN FOR QUADRANT";Q1;CHR$(8);",";Q2:PRINT */
  line_4030:
  basalt_print_single(USER_Q1_single);
  basalt_print_single(USER_Q2_single);
  basalt_print_newline();
  basalt_print_newline();
  /* 4040 O1$="-------------------":PRINT O1$ */
  line_4040:
  basalt_print_newline();
  /* 4060 FOR I=Q1-1 TO Q1+1:N(1)=-1:N(2)=-2:N(3)=-3:FOR J=Q2-1 TO Q2+1 */
  line_4060:
  USER_N_single = (- 1);
  USER_N_single = (- 2);
  USER_N_single = (- 3);
  /* 4120 IF I>0 AND I<9 AND J>0 AND J<9 THEN N(J-Q2+2)=G(I,J):Z(I,J)=G(I,J) */
  line_4120:
  /* 4180 NEXT J:FOR L=1 TO 3:PRINT"| ";:IF N(L)<0 THEN PRINT"*** ";:GOTO 4230 */
  line_4180:
  basalt_print_newline();
  /* 4210 PRINT RIGHT$(STR$(N(L)+1000),3);" "; */
  line_4210:
  basalt_print_newline();
  /* 4230 NEXT L:PRINT"|":PRINT O1$:NEXT I:GOTO 1990 */
  line_4230:
  basalt_print_newline();
  basalt_print_newline();
  goto line_1990;
  /* 4260 IF D(4)<0 THEN PRINT"PHASERS INOPERATIVE.":GOTO 1990 */
  line_4260:
  /* 4265 IF K3>0 THEN 4330 */
  line_4265:
  /* 4270 PRINT"SCIENCE OFFICER SPOCK:  'SENSORS SHOW NO ENEMY SHIPS" */
  line_4270:
  basalt_print_newline();
  /* 4280 PRINT"                         IN THIS QUADRANT'":GOTO 1990 */
  line_4280:
  basalt_print_newline();
  goto line_1990;
  /* 4330 IF D(8)<0 THEN PRINT"COMPUTER FAILURE HAMPERS ACCURACY." */
  line_4330:
  /* 4350 PRINT"PHASERS LOCKED ON TARGET;  "; */
  line_4350:
  basalt_print_newline();
  /* 4360 PRINT"ENERGY AVAILABLE =";E;"UNITS" */
  line_4360:
  basalt_print_single(USER_E_single);
  basalt_print_newline();
  /* 4370 INPUT"NUMBER OF UNITS TO FIRE";X:IF X<=0 THEN 1990 */
  line_4370:
  /* 4400 IF E-X<0 THEN 4360 */
  line_4400:
  /* 4410 E=E-X:IF D(7)<0 THEN X=X*RND(1) */
  line_4410:
  USER_E_single = (USER_E_single - USER_X_single);
  /* 4450 H1=INT(X/K3):FOR I=1 TO 3:IF K(I,3)<=0 THEN 4670 */
  line_4450:
  USER_H1_single = /* cast from 0 to 3 */ ((int)((USER_X_single / USER_K3_single)));
  /* 4480 H=INT((H1/FND(0))*(RND(1)+2)):IF H>.15*K(I,3)THEN 4530 */
  line_4480:
  USER_H_single = /* cast from 0 to 3 */ ((int)(((USER_H1_single / USER_FN_single) * (USER_RN_single + 2))));
  /* 4500 PRINT"SENSORS SHOW NO DAMAGE TO ENEMY AT";K(I,1);CHR$(8);",";K(I,2);CHR$(8);".":GOTO 4670 */
  line_4500:
  basalt_print_single(USER_K_single);
  basalt_print_single(USER_K_single);
  basalt_print_newline();
  goto line_4670;
  /* 4530 K(I,3)=K(I,3)-H:PRINT H;"UNIT HIT ON KLINGON AT SECTOR";K(I,1);CHR$(8);","; */
  line_4530:
  USER_K_single = (USER_K_single - USER_H_single);
  basalt_print_single(USER_H_single);
  basalt_print_single(USER_K_single);
  basalt_print_newline();
  /* 4550 PRINT K(I,2);CHR$(8);".":IF K(I,3)<=0 THEN PRINT:PRINT CHR$(22);"*** KLINGON DESTROYED ***";CHR$(22):PRINT:GOTO 4580 */
  line_4550:
  basalt_print_single(USER_K_single);
  basalt_print_newline();
  /* 4560 PRINT" (SENSORS SHOW";K(I,3);"UNITS REMAINING)":GOTO 4670 */
  line_4560:
  basalt_print_single(USER_K_single);
  basalt_print_newline();
  goto line_4670;
  /* 4580 K3=K3-1:K9=K9-1:Z1=K(I,1):Z2=K(I,2):A$="   ":GOSUB 8670 */
  line_4580:
  USER_K3_single = (USER_K3_single - 1);
  USER_K9_single = (USER_K9_single - 1);
  USER_Z1_single = USER_K_single;
  USER_Z2_single = USER_K_single;
  /* 4650 K(I,3)=0:G(Q1,Q2)=G(Q1,Q2)-100:Z(Q1,Q2)=G(Q1,Q2):IF K9<=0 THEN 6370 */
  line_4650:
  USER_K_single = 0;
  USER_G_single = (USER_G_single - 100);
  USER_Z_single = USER_G_single;
  /* 4670 NEXT I:GOSUB 6000:GOTO 1990 */
  line_4670:
  goto line_1990;
  /* 4700 IF P<=0 THEN PRINT"ALL PHOTON TORPEDOES EXPENDED.":GOTO 1990 */
  line_4700:
  /* 4730 IF D(5)<0 THEN PRINT"PHOTON TUBES ARE NOT OPERATIONAL.":GOTO 1990 */
  line_4730:
  /* 4760 INPUT"PHOTON TORPEDO COURSE (1-9)";C1:IF C1=9 THEN C1=1 */
  line_4760:
  /* 4780 IF C1>=1 AND C1<9 THEN 4850 */
  line_4780:
  /* 4790 PRINT"ENSIGN CHEKOV:  'INCORRECT COURSE DATA, SIR!'" */
  line_4790:
  basalt_print_newline();
  /* 4800 GOTO 1990 */
  line_4800:
  goto line_1990;
  /* 4850 X1=C(C1,1)+(C(C1+1,1)-C(C1,1))*(C1-INT(C1)):E=E-2:P=P-1 */
  line_4850:
  USER_X1_single = (USER_C_single + ((USER_C_single - USER_C_single) * (USER_C1_single - /* cast from 0 to 3 */ ((int)(USER_C1_single)))));
  USER_E_single = (USER_E_single - 2);
  USER_P_single = (USER_P_single - 1);
  /* 4860 X2=C(C1,2)+(C(C1+1,2)-C(C1,2))*(C1-INT(C1)):X=S1:Y=S2 */
  line_4860:
  USER_X2_single = (USER_C_single + ((USER_C_single - USER_C_single) * (USER_C1_single - /* cast from 0 to 3 */ ((int)(USER_C1_single)))));
  USER_X_single = USER_S1_single;
  USER_Y_single = USER_S2_single;
  /* 4910 PRINT"TORPEDO TRACK:" */
  line_4910:
  basalt_print_newline();
  /* 4920 X=X+X1:Y=Y+X2:X3=INT(X+.5):Y3=INT(Y+.5) */
  line_4920:
  USER_X_single = (USER_X_single + USER_X1_single);
  USER_Y_single = (USER_Y_single + USER_X2_single);
  USER_X3_single = /* cast from 0 to 3 */ ((int)((USER_X_single + 0.5)));
  USER_Y3_single = /* cast from 0 to 3 */ ((int)((USER_Y_single + 0.5)));
  /* 4960 IF X3<1 OR X3>8 OR Y3<1 OR Y3>8 THEN 5490 */
  line_4960:
  /* 5000 PRINT"               ";X3;CHR$(8);",";Y3:A$="   ":Z1=X:Z2=Y:GOSUB 8830 */
  line_5000:
  basalt_print_single(USER_X3_single);
  basalt_print_single(USER_Y3_single);
  basalt_print_newline();
  USER_Z1_single = USER_X_single;
  USER_Z2_single = USER_Y_single;
  /* 5050 IF Z3<>0 THEN 4920 */
  line_5050:
  /* 5060 A$="+K+":Z1=X:Z2=Y:GOSUB 8830:IF Z3=0 THEN 5210 */
  line_5060:
  USER_Z1_single = USER_X_single;
  USER_Z2_single = USER_Y_single;
  /* 5110 PRINT:PRINT CHR$(22);"*** KLINGON DESTROYED ***";CHR$(22):PRINT:K3=K3-1:K9=K9-1:IF K9<=0 THEN 6370 */
  line_5110:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  USER_K3_single = (USER_K3_single - 1);
  USER_K9_single = (USER_K9_single - 1);
  /* 5150 FOR I=1 TO 3:IF X3=K(I,1)AND Y3=K(I,2)THEN 5190 */
  line_5150:
  /* 5180 NEXT I:I=3 */
  line_5180:
  USER_I_single = 3;
  /* 5190 K(I,3)=0:GOTO 5430 */
  line_5190:
  USER_K_single = 0;
  goto line_5430;
  /* 5210 A$=" * ":Z1=X:Z2=Y:GOSUB 8830:IF Z3=0 THEN 5280 */
  line_5210:
  USER_Z1_single = USER_X_single;
  USER_Z2_single = USER_Y_single;
  /* 5260 PRINT"STAR AT";X3;",";Y3;"ABSORBED TORPEDO ENERGY.":GOSUB 6000:GOTO 1990 */
  line_5260:
  basalt_print_single(USER_X3_single);
  basalt_print_single(USER_Y3_single);
  basalt_print_newline();
  goto line_1990;
  /* 5280 A$=">!<":Z1=X:Z2=Y:GOSUB 8830:IF Z3=0 THEN 4760 */
  line_5280:
  USER_Z1_single = USER_X_single;
  USER_Z2_single = USER_Y_single;
  /* 5330 PRINT CHR$(22);"*** STARBASE DESTROYED ***";CHR$(22):B3=B3-1:B9=B9-1 */
  line_5330:
  basalt_print_newline();
  USER_B3_single = (USER_B3_single - 1);
  USER_B9_single = (USER_B9_single - 1);
  /* 5360 IF B9>0 OR K9>T-T0-T9 THEN 5400 */
  line_5360:
  /* 5370 PRINT"THAT DOES IT, CAPTAIN!!  YOU ARE HEREBY RELIEVED OF COMMAND" */
  line_5370:
  basalt_print_newline();
  /* 5380 PRINT"AND SENTENCED TO 99 STARDATES AT HARD LABOR ON CYGNUS 12!!" */
  line_5380:
  basalt_print_newline();
  /* 5390 GOTO 6270 */
  line_5390:
  goto line_6270;
  /* 5400 PRINT"STARFLEET COMMAND REVIEWING YOUR RECORD TO CONSIDER" */
  line_5400:
  basalt_print_newline();
  /* 5410 PRINT"COURT MARTIAL!":D0=0 */
  line_5410:
  basalt_print_newline();
  USER_D0_single = 0;
  /* 5430 Z1=X:Z2=Y:A$="   ":GOSUB 8670 */
  line_5430:
  USER_Z1_single = USER_X_single;
  USER_Z2_single = USER_Y_single;
  /* 5470 G(Q1,Q2)=K3*100+B3*10+S3:Z(Q1,Q2)=G(Q1,Q2):GOSUB 6000:GOTO 1990 */
  line_5470:
  USER_G_single = ((USER_K3_single * 100) + ((USER_B3_single * 10) + USER_S3_single));
  USER_Z_single = USER_G_single;
  goto line_1990;
  /* 5490 PRINT"TORPEDO MISSED.":PRINT:GOSUB 6000:GOTO 1990 */
  line_5490:
  basalt_print_newline();
  basalt_print_newline();
  goto line_1990;
  /* 5530 IF D(7)<0 THEN PRINT"SHIELD CONTROL INOPERABLE.":GOTO 1990 */
  line_5530:
  /* 5560 PRINT"ENERGY AVAILABLE =";E+S;:INPUT"NUMBER OF UNITS TO SHIELDS";X */
  line_5560:
  basalt_print_newline();
  /* 5580 IF X<0 OR S=X THEN PRINT"<SHIELDS UNCHANGED>":GOTO 1990 */
  line_5580:
  /* 5590 IF X<=E+S THEN 5630 */
  line_5590:
  /* 5600 PRINT"SHIELD CONTROL:  'THIS IS NOT THE FEDERATION TREASURY.'" */
  line_5600:
  basalt_print_newline();
  /* 5610 PRINT"<SHIELDS UNCHANGED>":GOTO 1990 */
  line_5610:
  basalt_print_newline();
  goto line_1990;
  /* 5630 E=E+S-X:S=X:PRINT"DEFLECTOR CONTROL ROOM:" */
  line_5630:
  USER_E_single = (USER_E_single + (USER_S_single - USER_X_single));
  USER_S_single = USER_X_single;
  basalt_print_newline();
  /* 5660 PRINT"  'SHIELDS NOW AT";INT(S);"UNITS PER YOUR COMMAND.'":GOTO 1990 */
  line_5660:
  basalt_print_newline();
  goto line_1990;
  /* 5690 IF D(6)>=0 THEN 5910 */
  line_5690:
  /* 5700 PRINT"DAMAGE CONTROL REPORT NOT AVAILABLE.":IF D0=0 THEN 1990 */
  line_5700:
  basalt_print_newline();
  /* 5720 D3=0:FOR I=1 TO 8:IF D(I)<0 THEN D3=D3+.1 */
  line_5720:
  USER_D3_single = 0;
  /* 5760 NEXT I:IF D3=0 THEN 1990 */
  line_5760:
  /* 5780 PRINT:D3=D3+D4:IF D3>=1 THEN D3=.9 */
  line_5780:
  basalt_print_newline();
  USER_D3_single = (USER_D3_single + USER_D4_single);
  /* 5810 PRINT"TECHNICIANS STANDING BY TO EFFECT REPAIRS TO YOUR SHIP;" */
  line_5810:
  basalt_print_newline();
  /* 5820 PRINT"ESTIMATED TIME TO REPAIR:";.01*INT(100*D3);"STARDATES." */
  line_5820:
  basalt_print_newline();
  /* 5840 INPUT"WILL YOU AUTHORIZE THE REPAIR ORDER (Y/N)";A$ */
  line_5840:
  /* 5860 IF A$<>"Y"THEN 1990 */
  line_5860:
  /* 5870 FOR I=1 TO 8:IF D(I)<0 THEN D(I)=0 */
  line_5870:
  /* 5890 NEXT I:T=T+D3+.1 */
  line_5890:
  USER_T_single = (USER_T_single + (USER_D3_single + 0.1));
  /* 5910 PRINT:PRINT"DEVICE             STATE OF REPAIR":PRINT"------             ---------------":FOR R1=1 TO 8 */
  line_5910:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  /* 5920 GOSUB 8790:PRINT G2$;LEFT$(Z$,25-LEN(G2$));INT(D(R1)*100)*.01 */
  line_5920:
  basalt_print_newline();
  /* 5950 NEXT R1:PRINT:IF D0<>0 THEN 5720 */
  line_5950:
  basalt_print_newline();
  /* 5980 GOTO 1990 */
  line_5980:
  goto line_1990;
  /* 6000 IF K3<=0 THEN RETURN */
  line_6000:
  /* 6010 IF D0<>0 THEN PRINT"STARBASE SHIELDS PROTECT THE ENTERPRISE.":RETURN */
  line_6010:
  /* 6040 FOR I=1 TO 3:IF K(I,3)<=0 THEN 6200 */
  line_6040:
  /* 6060 H=INT((K(I,3)/FND(1))*(2+RND(1))):S=S-H:K(I,3)=K(I,3)/(3+RND(0)) */
  line_6060:
  USER_H_single = /* cast from 0 to 3 */ ((int)(((USER_K_single / USER_FN_single) * (2 + USER_RN_single))));
  USER_S_single = (USER_S_single - USER_H_single);
  USER_K_single = (USER_K_single / (3 + USER_RN_single));
  /* 6080 PRINT:PRINT H;"UNIT HIT ON ENTERPRISE FROM SECTOR";K(I,1);CHR$(8);",";K(I,2);CHR$(8);"." */
  line_6080:
  basalt_print_newline();
  basalt_print_single(USER_H_single);
  basalt_print_single(USER_K_single);
  basalt_print_single(USER_K_single);
  basalt_print_newline();
  /* 6090 IF S<=0 THEN 6240 */
  line_6090:
  /* 6100 PRINT"      <SHIELDS DOWN TO";S;"UNITS>":IF H<20 THEN 6200 */
  line_6100:
  basalt_print_single(USER_S_single);
  basalt_print_newline();
  /* 6120 IF RND(1)>.6 OR H/S<=.02 THEN 6200 */
  line_6120:
  /* 6140 R1=FNR(1):D(R1)=D(R1)-H/S-.5*RND(1):GOSUB 8790 */
  line_6140:
  USER_R1_single = USER_FN_single;
  USER_D_single = (USER_D_single - ((USER_H_single / USER_S_single) - (0.5 * USER_RN_single)));
  /* 6170 PRINT"DAMAGE CONTROL: '";G2$;" DAMAGED BY THE HIT'" */
  line_6170:
  basalt_print_newline();
  /* 6200 NEXT I:RETURN */
  line_6200:
  /* 6220 PRINT:PRINT"IT IS STARDATE";T;CHR$(8);".":PRINT:GOTO 6270 */
  line_6220:
  basalt_print_newline();
  basalt_print_single(USER_T_single);
  basalt_print_newline();
  basalt_print_newline();
  goto line_6270;
  /* 6240 PRINT:PRINT"THE ENTERPRISE HAS BEEN DESTROYED.  THE FEDERATION "; */
  line_6240:
  basalt_print_newline();
  basalt_print_newline();
  /* 6250 PRINT"WILL BE CONQUERED.":GOTO 6220 */
  line_6250:
  basalt_print_newline();
  goto line_6220;
  /* 6270 PRINT"THERE WERE";K9;"KLINGON BATTLE CRUISERS LEFT AT" */
  line_6270:
  basalt_print_single(USER_K9_single);
  basalt_print_newline();
  /* 6280 PRINT"THE END OF YOUR MISSION." */
  line_6280:
  basalt_print_newline();
  /* 6290 PRINT:PRINT:IF B9=0 THEN 6360 */
  line_6290:
  basalt_print_newline();
  basalt_print_newline();
  /* 6310 PRINT"THE FEDERATION IS IN NEED OF A NEW STARSHIP COMMANDER" */
  line_6310:
  basalt_print_newline();
  /* 6320 PRINT"FOR A SIMILAR MISSION -- IF THERE IS A VOLUNTEER," */
  line_6320:
  basalt_print_newline();
  /* 6330 INPUT"LET HIM STEP FORWARD AND ENTER 'AYE'";A$:IF A$="AYE"THEN 10 */
  line_6330:
  /* 6360 PRINT:PRINT "BACK TO SYSTEM.":SYSTEM */
  line_6360:
  basalt_print_newline();
  basalt_print_newline();
  /* 6370 PRINT"CONGRATULATIONS, CAPTAIN!  THE LAST KLINGON BATTLE CRUISER" */
  line_6370:
  basalt_print_newline();
  /* 6380 PRINT"MENACING THE FEDERATION HAS BEEN DESTROYED.":PRINT */
  line_6380:
  basalt_print_newline();
  basalt_print_newline();
  /* 6400 PRINT"YOUR EFFICIENCY RATING IS";1000*(K7/(T-T0))^2:GOTO 6290 */
  line_6400:
  basalt_print_newline();
  goto line_6290;
  /* 6430 FOR I=S1-1 TO S1+1:FOR J=S2-1 TO S2+1 */
  line_6430:
  /* 6450 IF INT(I+.5)<1 OR INT(I+.5)>8 OR INT(J+.5)<1 OR INT(J+.5)>8 THEN 6540 */
  line_6450:
  /* 6490 A$=">B<":Z1=I:Z2=J:GOSUB 8830:IF Z3=1 THEN 6580 */
  line_6490:
  USER_Z1_single = USER_I_single;
  USER_Z2_single = USER_J_single;
  /* 6540 NEXT J:NEXT I:D0=0:GOTO 6650 */
  line_6540:
  USER_D0_single = 0;
  goto line_6650;
  /* 6580 D0=1:C$="DOCKED":E=E0:P=P0 */
  line_6580:
  USER_D0_single = 1;
  USER_E_single = USER_E0_single;
  USER_P_single = USER_P0_single;
  /* 6620 PRINT"SHIELDS DROPPED FOR DOCKING PURPOSES.":S=0:GOTO 6720 */
  line_6620:
  basalt_print_newline();
  USER_S_single = 0;
  goto line_6720;
  /* 6650 IF K3>0 THEN C$="*RED*":GOTO 6720 */
  line_6650:
  /* 6660 C$="GREEN":IF E<E0*.1 THEN C$="YELLOW" */
  line_6660:
  /* 6720 IF D(2)>=0 THEN 6770 */
  line_6720:
  /* 6730 PRINT:PRINT"*** SHORT RANGE SENSORS ARE OUT ***":PRINT:RETURN */
  line_6730:
  basalt_print_newline();
  basalt_print_newline();
  basalt_print_newline();
  /* 6770 O1$="   +--1---2---3---4---5---6---7---8-+":PRINT O1$:FOR I=1 TO 8:PRINT I;"|"; */
  line_6770:
  basalt_print_newline();
  basalt_print_single(USER_I_single);
  basalt_print_newline();
  /* 6820 FOR J=(I-1)*24+1 TO(I-1)*24+22 STEP 3:PRINT" ";MID$(Q$,J,3);:NEXT J:PRINT"|";I; */
  line_6820:
  basalt_print_newline();
  basalt_print_single(USER_I_single);
  basalt_print_newline();
  /* 6830 ON I GOTO 6850,6900,6960,7020,7070,7120,7180,7240 */
  line_6830:
  /* 6850 PRINT"        STARDATE          ";:PRINT USING"####.#";INT(T*10)*.1:GOTO 7260 */
  line_6850:
  basalt_print_newline();
  basalt_print_newline();
  goto line_7260;
  /* 6900 PRINT"        CONDITION          ";:IF C$="*RED*" THEN PRINT CHR$(22);"*RED*";CHR$(22) ELSE IF C$="DOCKED" THEN PRINT CHR$(22);"DOCKED";CHR$(22) ELSE PRINT C$ */
  line_6900:
  basalt_print_newline();
  /* 6902 GOTO 7260 */
  line_6902:
  goto line_7260;
  /* 6960 PRINT"        QUADRANT           ";Q1;CHR$(8);",";Q2;CHR$(8):GOTO 7260 */
  line_6960:
  basalt_print_single(USER_Q1_single);
  basalt_print_single(USER_Q2_single);
  basalt_print_newline();
  goto line_7260;
  /* 7020 PRINT"        SECTOR             ";S1;CHR$(8);",";S2;CHR$(8):GOTO 7260 */
  line_7020:
  basalt_print_single(USER_S1_single);
  basalt_print_single(USER_S2_single);
  basalt_print_newline();
  goto line_7260;
  /* 7070 PRINT"        PHOTON TORPEDOES  ";:PRINT USING"######";INT(P):GOTO 7260 */
  line_7070:
  basalt_print_newline();
  basalt_print_newline();
  goto line_7260;
  /* 7120 PRINT"        TOTAL ENERGY      ";:PRINT USING"######";INT(E+S):GOTO 7260 */
  line_7120:
  basalt_print_newline();
  basalt_print_newline();
  goto line_7260;
  /* 7180 PRINT"        SHIELDS           ";:PRINT USING"######";INT(S):GOTO 7260 */
  line_7180:
  basalt_print_newline();
  basalt_print_newline();
  goto line_7260;
  /* 7240 PRINT"        KLINGONS REMAINING";:PRINT USING"######";INT(K9) */
  line_7240:
  basalt_print_newline();
  basalt_print_newline();
  /* 7260 NEXT I:PRINT O1$:RETURN */
  line_7260:
  basalt_print_newline();
  /* 7290 IF D(8)<0 THEN PRINT"COMPUTER DISABLED.":GOTO 1990 */
  line_7290:
  /* 7320 INPUT"COMPUTER ACTIVE AND AWAITING COMMAND";A:IF A<0 THEN 1990 */
  line_7320:
  /* 7350 PRINT:H8=1:ON A+1 GOTO 7540,7900,8070,8500,8150,7400 */
  line_7350:
  basalt_print_newline();
  USER_H8_single = 1;
  /* 7360 PRINT"FUNCTIONS AVAILABLE FROM LIBRARY-COMPUTER:" */
  line_7360:
  basalt_print_newline();
  /* 7365 PRINT "-----------------------------------------":PRINT */
  line_7365:
  basalt_print_newline();
  basalt_print_newline();
  /* 7370 PRINT"   0 = CUMULATIVE GALTIC RECORD" */
  line_7370:
  basalt_print_newline();
  /* 7372 PRINT"   1 = STATUS REPORT" */
  line_7372:
  basalt_print_newline();
  /* 7374 PRINT"   2 = PHOTON TORPEDO DATA" */
  line_7374:
  basalt_print_newline();
  /* 7376 PRINT"   3 = STARBASE NAV DATA" */
  line_7376:
  basalt_print_newline();
  /* 7378 PRINT"   4 = DIRECTION/DISTANCE CALCULATOR" */
  line_7378:
  basalt_print_newline();
  /* 7380 PRINT"   5 = GALAXY 'REGION NAME' MAP":PRINT:GOTO 7320 */
  line_7380:
  basalt_print_newline();
  basalt_print_newline();
  goto line_7320;
  /* 7400 H8=0:G5=1:PRINT"                        THE GALAXY":GOTO 7550 */
  line_7400:
  USER_H8_single = 0;
  USER_G5_single = 1;
  basalt_print_newline();
  goto line_7550;
  /* 7540 REM */
  line_7540:
  /* 7543 PRINT:PRINT"       "; */
  line_7543:
  basalt_print_newline();
  basalt_print_newline();
  /* 7544 PRINT"COMPUTER RECORD OF GALAXY FOR QUADRANT";Q1;CHR$(8);",";Q2 */
  line_7544:
  basalt_print_single(USER_Q1_single);
  basalt_print_single(USER_Q2_single);
  basalt_print_newline();
  /* 7546 PRINT */
  line_7546:
  basalt_print_newline();
  /* 7550 PRINT"       1     2     3     4     5     6     7     8" */
  line_7550:
  basalt_print_newline();
  /* 7560 O1$="    +-----+-----+-----+-----+-----+-----+-----+-----+" */
  line_7560:
  /* 7570 PRINT O1$:FOR I=1 TO 8:PRINT I;" ";:IF H8=0 THEN 7740 */
  line_7570:
  basalt_print_newline();
  basalt_print_single(USER_I_single);
  basalt_print_newline();
  /* 7630 FOR J=1 TO 8:PRINT"| ";:IF Z(I,J)=0 THEN PRINT"*** ";:GOTO 7720 */
  line_7630:
  basalt_print_newline();
  /* 7700 PRINT RIGHT$(STR$(Z(I,J)+1000),3);" "; */
  line_7700:
  basalt_print_newline();
  /* 7720 IF J=8 THEN PRINT "|" */
  line_7720:
  /* 7721 NEXT J:GOTO 7850 */
  line_7721:
  goto line_7850;
  /* 7740 Z4=I:Z5=1:GOSUB 9030:J0=INT(15-.5*LEN(G2$)):PRINT TAB(J0);G2$; */
  line_7740:
  USER_Z4_single = USER_I_single;
  USER_Z5_single = 1;
  USER_J0_single = /* cast from 0 to 3 */ ((int)((15 - (0.5 * /* cast from 0 to 3 */ ()))));
  basalt_print_newline();
  /* 7800 Z5=5:GOSUB 9030:J0=INT(39-.5*LEN(G2$)):PRINT TAB(J0);G2$ */
  line_7800:
  USER_Z5_single = 5;
  USER_J0_single = /* cast from 0 to 3 */ ((int)((39 - (0.5 * /* cast from 0 to 3 */ ()))));
  basalt_print_newline();
  /* 7850 PRINT O1$:NEXT I:PRINT:GOTO 1990 */
  line_7850:
  basalt_print_newline();
  basalt_print_newline();
  goto line_1990;
  /* 7900 PRINT "   STATUS REPORT:":PRINT "   -------------":X$="":IF K9>1 THEN X$="S" */
  line_7900:
  basalt_print_newline();
  basalt_print_newline();
  /* 7940 PRINT K9;"KLINGON";X$;" LEFT." */
  line_7940:
  basalt_print_single(USER_K9_single);
  basalt_print_newline();
  /* 7960 PRINT" MISSION MUST BE COMPLETED IN";.1*INT((T0+T9-T)*10);"STARDATES." */
  line_7960:
  basalt_print_newline();
  /* 7970 X$="S":IF B9<2 THEN X$="":IF B9<1 THEN 8010 */
  line_7970:
  /* 7980 PRINT" THE FEDERATION IS MAINTAINING";B9;"STARBASE";X$;" IN THE GALAXY." */
  line_7980:
  basalt_print_single(USER_B9_single);
  basalt_print_newline();
  /* 7990 GOTO 5690 */
  line_7990:
  goto line_5690;
  /* 8010 PRINT"YOUR STUPIDITY HAS LEFT YOU ON YOUR OWN IN" */
  line_8010:
  basalt_print_newline();
  /* 8020 PRINT"  THE GALAXY -- YOU HAVE NO STARBASES LEFT!":GOTO 5690 */
  line_8020:
  basalt_print_newline();
  goto line_5690;
  /* 8070 IF K3<=0 THEN 4270 */
  line_8070:
  /* 8080 X$="":IF K3>1 THEN X$="S" */
  line_8080:
  /* 8090 PRINT"FROM ENTERPRISE TO KLINGON BATTLE CRUSER";X$ */
  line_8090:
  basalt_print_newline();
  /* 8100 H8=0:FOR I=1 TO 3:IF K(I,3)<=0 THEN 8480 */
  line_8100:
  USER_H8_single = 0;
  /* 8110 W1=K(I,1):X=K(I,2) */
  line_8110:
  USER_W1_single = USER_K_single;
  USER_X_single = USER_K_single;
  /* 8120 C1=S1:A=S2:GOTO 8220 */
  line_8120:
  USER_C1_single = USER_S1_single;
  USER_A_single = USER_S2_single;
  goto line_8220;
  /* 8150 PRINT"DIRECTION/DISTANCE CALCULATOR:" */
  line_8150:
  basalt_print_newline();
  /* 8160 PRINT"YOU ARE AT QUADRANT ";Q1;CHR$(8);",";Q2;" SECTOR ";S1;CHR$(8);",";S2;CHR$(8);"." */
  line_8160:
  basalt_print_single(USER_Q1_single);
  basalt_print_single(USER_Q2_single);
  basalt_print_single(USER_S1_single);
  basalt_print_single(USER_S2_single);
  basalt_print_newline();
  /* 8170 INPUT"PLEASE ENTER INITIAL COORDINATES (X,Y)";C1,A */
  line_8170:
  /* 8200 INPUT"FINAL COORDINATES (X,Y)";W1,X */
  line_8200:
  /* 8220 X=X-A:A=C1-W1:IF X<0 THEN 8350 */
  line_8220:
  USER_X_single = (USER_X_single - USER_A_single);
  USER_A_single = (USER_C1_single - USER_W1_single);
  /* 8250 IF A<0 THEN 8410 */
  line_8250:
  /* 8260 IF X>0 THEN 8280 */
  line_8260:
  /* 8270 IF A=0 THEN C1=5:GOTO 8290 */
  line_8270:
  /* 8280 C1=1 */
  line_8280:
  USER_C1_single = 1;
  /* 8290 IF ABS(A)<=ABS(X)THEN 8330 */
  line_8290:
  /* 8310 PRINT"DIRECTION =";C1+(((ABS(A)-ABS(X))+ABS(A))/ABS(A)):GOTO 8460 */
  line_8310:
  basalt_print_newline();
  goto line_8460;
  /* 8330 PRINT"DIRECTION =";C1+(ABS(A)/ABS(X)):GOTO 8460 */
  line_8330:
  basalt_print_newline();
  goto line_8460;
  /* 8350 IF A>0 THEN C1=3:GOTO 8420 */
  line_8350:
  /* 8360 IF X<>0 THEN C1=5:GOTO 8290 */
  line_8360:
  /* 8410 C1=7 */
  line_8410:
  USER_C1_single = 7;
  /* 8420 IF ABS(A)>=ABS(X)THEN 8450 */
  line_8420:
  /* 8430 PRINT"DIRECTION =";C1+(((ABS(X)-ABS(A))+ABS(X))/ABS(X)):GOTO 8460 */
  line_8430:
  basalt_print_newline();
  goto line_8460;
  /* 8450 PRINT"DIRECTION =";C1+(ABS(X)/ABS(A)) */
  line_8450:
  basalt_print_newline();
  /* 8460 PRINT"DISTANCE =";SQR(X^2+A^2):IF H8=1 THEN 1990 */
  line_8460:
  basalt_print_newline();
  /* 8480 NEXT I:GOTO 1990 */
  line_8480:
  goto line_1990;
  /* 8500 IF B3<>0 THEN PRINT"FROM ENTERPRISE TO STARBASE:":W1=B4:X=B5:GOTO 8120 */
  line_8500:
  /* 8510 PRINT"MR. SPOCK:  'SENSORS SHOW NO STARBASES IN THIS QUADRANT.'""; */
  line_8510:
  basalt_print_newline();
  /* 8520 GOTO 1990 */
  line_8520:
  goto line_1990;
  /* 8590 R1=FNR(1):R2=FNR(1):A$="   ":Z1=R1:Z2=R2:GOSUB 8830:IF Z3=0 THEN 8590 */
  line_8590:
  USER_R1_single = USER_FN_single;
  USER_R2_single = USER_FN_single;
  USER_Z1_single = USER_R1_single;
  USER_Z2_single = USER_R2_single;
  /* 8600 RETURN */
  line_8600:
  /* 8670 S8=INT(Z2-.5)*3+INT(Z1-.5)*24+1 */
  line_8670:
  USER_S8_single = ((/* cast from 0 to 3 */ ((int)((USER_Z2_single - 0.5))) * 3) + ((/* cast from 0 to 3 */ ((int)((USER_Z1_single - 0.5))) * 24) + 1));
  /* 8675 IF LEN(A$)<>3 THEN PRINT"ERROR":STOP */
  line_8675:
  /* 8680 IF S8=1 THEN Q$=A$+RIGHT$(Q$,189):RETURN */
  line_8680:
  /* 8690 IF S8=190 THEN Q$=LEFT$(Q$,189)+A$:RETURN */
  line_8690:
  /* 8700 Q$=LEFT$(Q$,S8-1)+A$+RIGHT$(Q$,190-S8):RETURN */
  line_8700:
  /* 8790 ON R1 GOTO 8792,8794,8796,8798,8800,8802,8804,8806 */
  line_8790:
  /* 8792 G2$="WARP ENGINES":RETURN */
  line_8792:
  /* 8794 G2$="SHORT RANGE SENSORS":RETURN */
  line_8794:
  /* 8796 G2$="LONG RANGE SENSORS":RETURN */
  line_8796:
  /* 8798 G2$="PHASER CONTROL":RETURN */
  line_8798:
  /* 8800 G2$="PHOTON TUBES":RETURN */
  line_8800:
  /* 8802 G2$="DAMAGE CONTROL":RETURN */
  line_8802:
  /* 8804 G2$="SHIELD CONTROL":RETURN */
  line_8804:
  /* 8806 G2$="LIBRARY-COMPUTER":RETURN */
  line_8806:
  /* 8830 Z1=INT(Z1+.5):Z2=INT(Z2+.5):S8=(Z2-1)*3+(Z1-1)*24+1:Z3=0 */
  line_8830:
  USER_Z1_single = /* cast from 0 to 3 */ ((int)((USER_Z1_single + 0.5)));
  USER_Z2_single = /* cast from 0 to 3 */ ((int)((USER_Z2_single + 0.5)));
  USER_S8_single = (((USER_Z2_single - 1) * 3) + (((USER_Z1_single - 1) * 24) + 1));
  USER_Z3_single = 0;
  /* 8890 IF MID$(Q$,S8,3)<>A$THEN RETURN */
  line_8890:
  /* 8900 Z3=1:RETURN */
  line_8900:
  USER_Z3_single = 1;
  /* 9030 IF Z5<=4 THEN ON Z4 GOTO 9040,9050,9060,9070,9080,9090,9100,9110 */
  line_9030:
  /* 9035 GOTO 9120 */
  line_9035:
  goto line_9120;
  /* 9040 G2$="ANTARES":GOTO 9210 */
  line_9040:
  goto line_9210;
  /* 9050 G2$="RIGEL":GOTO 9210 */
  line_9050:
  goto line_9210;
  /* 9060 G2$="PROCYON":GOTO 9210 */
  line_9060:
  goto line_9210;
  /* 9070 G2$="VEGA":GOTO 9210 */
  line_9070:
  goto line_9210;
  /* 9080 G2$="CANOPUS":GOTO 9210 */
  line_9080:
  goto line_9210;
  /* 9090 G2$="ALTAIR":GOTO 9210 */
  line_9090:
  goto line_9210;
  /* 9100 G2$="SAGITTARIUS":GOTO 9210 */
  line_9100:
  goto line_9210;
  /* 9110 G2$="POLLUX":GOTO 9210 */
  line_9110:
  goto line_9210;
  /* 9120 ON Z4 GOTO 9130,9140,9150,9160,9170,9180,9190,9200 */
  line_9120:
  /* 9130 G2$="SIRIUS":GOTO 9210 */
  line_9130:
  goto line_9210;
  /* 9140 G2$="DENEB":GOTO 9210 */
  line_9140:
  goto line_9210;
  /* 9150 G2$="CAPELLA":GOTO 9210 */
  line_9150:
  goto line_9210;
  /* 9160 G2$="BETELGEUSE":GOTO 9210 */
  line_9160:
  goto line_9210;
  /* 9170 G2$="ALDEBARAN":GOTO 9210 */
  line_9170:
  goto line_9210;
  /* 9180 G2$="REGULUS":GOTO 9210 */
  line_9180:
  goto line_9210;
  /* 9190 G2$="ARCTURUS":GOTO 9210 */
  line_9190:
  goto line_9210;
  /* 9200 G2$="SPICA" */
  line_9200:
  /* 9210 IF G5<>1 THEN ON Z5 GOTO 9230,9240,9250,9260,9230,9240,9250,9260 */
  line_9210:
  /* 9220 RETURN */
  line_9220:
  /* 9230 G2$=G2$+" I":RETURN */
  line_9230:
  /* 9240 G2$=G2$+" II":RETURN */
  line_9240:
  /* 9250 G2$=G2$+" III":RETURN */
  line_9250:
  /* 9260 G2$=G2$+" IV":RETURN */
  line_9260:
  /* 9999 END */
  line_9999:
  exit(0);
}
