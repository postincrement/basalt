#include <iostream>

// global variables
float A__single = 0;
char * A__string = nullptr;
char * A1__string = nullptr;
float B3__single = 0;
float B9__single = 0;
float * C__single_array_2;
int C__single_array_2_dim[2];
char * C__string = nullptr;
float C1__single = 0;
float * D__single_array_1;
int D__single_array_1_dim[1];
float D0__single = 0;
float D3__single = 0;
float D4__single = 0;
float E__single = 0;
float E0__single = 0;
float * G__single_array_2;
int G__single_array_2_dim[2];
char * G2__string = nullptr;
float G5__single = 0;
float H__single = 0;
float H1__single = 0;
float H8__single = 0;
float J0__single = 0;
float * K__single_array_2;
int K__single_array_2_dim[2];
float K3__single = 0;
float K7__single = 0;
float K9__single = 0;
float N__single = 0;
char * O1__string = nullptr;
float P__single = 0;
float P0__single = 0;
char * Q__string = nullptr;
float Q1__single = 0;
float Q2__single = 0;
float Q4__single = 0;
float Q5__single = 0;
float R1__single = 0;
float R2__single = 0;
float S__single = 0;
float S1__single = 0;
float S2__single = 0;
float S3__single = 0;
float S8__single = 0;
float S9__single = 0;
float T__single = 0;
float T0__single = 0;
float T8__single = 0;
float T9__single = 0;
float W1__single = 0;
float X__single = 0;
char * X__string = nullptr;
char * X0__string = nullptr;
float X1__single = 0;
float X2__single = 0;
float X3__single = 0;
float X5__single = 0;
float Y__single = 0;
float Y3__single = 0;
float * Z__single_array_2;
int Z__single_array_2_dim[2];
char * Z__string = nullptr;
float Z1__single = 0;
float Z2__single = 0;
float Z3__single = 0;
float Z4__single = 0;
float Z5__single = 0;

// constant strings
const char * const_string_8 = "";
const char * const_string_91 = " ";
const char * const_string_21 = "  ";
const char * const_string_69 = "   ";
const char * const_string_178 = "       ";
const char * const_string_110 = "               ";
const char * const_string_7 = "                         ";
const char * const_string_68 = "                          PRESENTLY DEPLOYED TO SHIELDS.";
const char * const_string_95 = "                         IN THIS QUADRANT'";
const char * const_string_177 = "                        THE GALAXY";
const char * const_string_2 = "                  ,------*------,";
const char * const_string_65 = "               FOR MANEUVERING AT WARP";
const char * const_string_6 = "        '----------------'";
const char * const_string_160 = "        CONDITION          ";
const char * const_string_167 = "        KLINGONS REMAINING";
const char * const_string_163 = "        PHOTON TORPEDOES  ";
const char * const_string_161 = "        QUADRANT           ";
const char * const_string_162 = "        SECTOR             ";
const char * const_string_166 = "        SHIELDS           ";
const char * const_string_158 = "        STARDATE          ";
const char * const_string_165 = "        TOTAL ENERGY      ";
const char * const_string_5 = "       ,---' '-------/ /--,";
const char * const_string_180 = "       1     2     3     4     5     6     7     8";
const char * const_string_134 = "      <SHIELDS DOWN TO";
const char * const_string_35 = "      SHIELDS DANGEROUSLY LOW     ";
const char * const_string_181 = "    +-----+-----+-----+-----+-----+-----+-----+-----+";
const char * const_string_4 = "   '-------- --'      / /";
const char * const_string_157 = "   +--1---2---3---4---5---6---7---8-+";
const char * const_string_183 = "   -------------";
const char * const_string_171 = "   0 = CUMULATIVE GALTIC RECORD";
const char * const_string_172 = "   1 = STATUS REPORT";
const char * const_string_173 = "   2 = PHOTON TORPEDO DATA";
const char * const_string_174 = "   3 = STARBASE NAV DATA";
const char * const_string_175 = "   4 = DIRECTION/DISTANCE CALCULATOR";
const char * const_string_176 = "   5 = GALAXY 'REGION NAME' MAP";
const char * const_string_60 = "   CHIEF ENGINEER SCOTT: 'THE ENGINES WON'T TAKE";
const char * const_string_15 = "   DESTROY THE";
const char * const_string_55 = "   LT. SULU: 'INCORRECT COURSE DATA, SIR!'";
const char * const_string_18 = "   ON STARDATE";
const char * const_string_182 = "   STATUS REPORT:";
const char * const_string_17 = "   THE GALAXY BEFORE THEY CAN ATTACK FEDERATION HEADQUARTERS";
const char * const_string_79 = "  'PERMISSION TO ATTEMPT CROSSING OF GALACTIC PERIMETER";
const char * const_string_124 = "  'SHIELDS NOW AT";
const char * const_string_3 = "  ,-------------   '---  ------'";
const char * const_string_82 = "  AT SECTOR";
const char * const_string_53 = "  COM  (TO CALL ON LIBRARY-COMPUTER)";
const char * const_string_34 = "  COMBAT AREA      CONDITION RED  ";
const char * const_string_52 = "  DAM  (FOR DAMAGE CONTROL REPORTS)";
const char * const_string_80 = "  IS HEREBY *DENIED*.  SHUT DOWN YOUR ENGINES.'";
const char * const_string_48 = "  LRS  (FOR LONG RANGE SENSOR SCAN)";
const char * const_string_46 = "  NAV  (TO SET COURSE)";
const char * const_string_49 = "  PHA  (TO FIRE PHASERS)";
const char * const_string_51 = "  SHE  (TO RAISE OR LOWER SHIELDS)";
const char * const_string_47 = "  SRS  (FOR SHORT RANGE SENSOR SCAN)";
const char * const_string_190 = "  THE GALAXY -- YOU HAVE NO STARBASES LEFT!";
const char * const_string_50 = "  TOR  (TO FIRE PHOTON TORPEDOES)";
const char * const_string_54 = "  XXX  (TO RESIGN YOUR COMMAND)";
const char * const_string_104 = " (SENSORS SHOW";
const char * const_string_39 = " * ";
const char * const_string_12 = " ARE ";
const char * const_string_72 = " DAMAGED";
const char * const_string_137 = " DAMAGED BY THE HIT'";
const char * const_string_224 = " I";
const char * const_string_225 = " II";
const char * const_string_226 = " III";
const char * const_string_23 = " IN THE GALAXY FOR RESUPPLYING YOUR SHIP.";
const char * const_string_188 = " IN THE GALAXY.";
const char * const_string_9 = " IS ";
const char * const_string_227 = " IV";
const char * const_string_185 = " LEFT.";
const char * const_string_186 = " MISSION MUST BE COMPLETED IN";
const char * const_string_33 = " QUADRANT . . .";
const char * const_string_71 = " REPAIR COMPLETED.";
const char * const_string_194 = " SECTOR ";
const char * const_string_73 = " STATE OF REPAIR IMPROVED";
const char * const_string_187 = " THE FEDERATION IS MAINTAINING";
const char * const_string_61 = " WARP";
const char * const_string_62 = "!'";
const char * const_string_164 = "######";
const char * const_string_159 = "####.#";
const char * const_string_31 = "'.";
const char * const_string_59 = ")";
const char * const_string_40 = "** FATAL ERROR **";
const char * const_string_90 = "*** ";
const char * const_string_103 = "*** KLINGON DESTROYED ***";
const char * const_string_156 = "*** SHORT RANGE SENSORS ARE OUT ***";
const char * const_string_114 = "*** STARBASE DESTROYED ***";
const char * const_string_153 = "*RED*";
const char * const_string_37 = "+K+";
const char * const_string_76 = ",";
const char * const_string_131 = "------             ---------------";
const char * const_string_88 = "-------------------";
const char * const_string_14 = "--------------------------";
const char * const_string_170 = "-----------------------------------------";
const char * const_string_101 = ".";
const char * const_string_19 = ". THIS GIVES YOU";
const char * const_string_84 = ".'";
const char * const_string_57 = "0.2";
const char * const_string_56 = "8";
const char * const_string_36 = "<E>";
const char * const_string_121 = "<SHIELDS UNCHANGED>";
const char * const_string_113 = ">!<";
const char * const_string_38 = ">B<";
const char * const_string_112 = "ABSORBED TORPEDO ENERGY.";
const char * const_string_220 = "ALDEBARAN";
const char * const_string_106 = "ALL PHOTON TORPEDOES EXPENDED.";
const char * const_string_213 = "ALTAIR";
const char * const_string_116 = "AND SENTENCED TO 99 STARDATES AT HARD LABOR ON CYGNUS 12!!";
const char * const_string_43 = "AND SHIELD CONTROL IS PRESENTLY INCAPABLE OF";
const char * const_string_208 = "ANTARES";
const char * const_string_222 = "ARCTURUS";
const char * const_string_24 = "ARE YOU READY TO ACCEPT COMMAND ('N' FOR INSTRUCTIONS)";
const char * const_string_146 = "AYE";
const char * const_string_147 = "BACK TO SYSTEM.";
const char * const_string_219 = "BETELGEUSE";
const char * const_string_212 = "CANOPUS";
const char * const_string_218 = "CAPELLA";
const char * const_string_81 = "CHIEF ENGINEER SCOTT:  'WARP ENGINES SHUT DOWN";
const char * const_string_168 = "COMPUTER DISABLED.";
const char * const_string_96 = "COMPUTER FAILURE HAMPERS ACCURACY.";
const char * const_string_179 = "COMPUTER RECORD OF GALAXY FOR QUADRANT";
const char * const_string_148 = "CONGRATULATIONS, CAPTAIN!  THE LAST KLINGON BATTLE CRUISER";
const char * const_string_118 = "COURT MARTIAL!";
const char * const_string_44 = "CROSS-CIRCUITING TO ENGINE ROOM!!";
const char * const_string_205 = "DAMAGE CONTROL";
const char * const_string_126 = "DAMAGE CONTROL REPORT NOT AVAILABLE.";
const char * const_string_70 = "DAMAGE CONTROL REPORT:  ";
const char * const_string_136 = "DAMAGE CONTROL: '";
const char * const_string_20 = "DAYS. THERE";
const char * const_string_123 = "DEFLECTOR CONTROL ROOM:";
const char * const_string_66 = "DEFLECTOR CONTROL ROOM:  ";
const char * const_string_217 = "DENEB";
const char * const_string_130 = "DEVICE             STATE OF REPAIR";
const char * const_string_195 = "DIRECTION =";
const char * const_string_192 = "DIRECTION/DISTANCE CALCULATOR:";
const char * const_string_196 = "DISTANCE =";
const char * const_string_151 = "DOCKED";
const char * const_string_77 = "DUE TO BAD NAVAGATION";
const char * const_string_98 = "ENERGY AVAILABLE =";
const char * const_string_64 = "ENGINEERING:  'INSUFFICIENT ENERGY AVAILABLE";
const char * const_string_108 = "ENSIGN CHEKOV:  'INCORRECT COURSE DATA, SIR!'";
const char * const_string_45 = "ENTER ONE OF THE FOLLOWING:";
const char * const_string_199 = "ERROR";
const char * const_string_128 = "ESTIMATED TIME TO REPAIR:";
const char * const_string_145 = "FOR A SIMILAR MISSION -- IF THERE IS A VOLUNTEER,";
const char * const_string_191 = "FROM ENTERPRISE TO KLINGON BATTLE CRUSER";
const char * const_string_197 = "FROM ENTERPRISE TO STARBASE:";
const char * const_string_169 = "FUNCTIONS AVAILABLE FROM LIBRARY-COMPUTER:";
const char * const_string_154 = "GREEN";
const char * const_string_30 = "IN THE GALACTIC QUADRANT, '";
const char * const_string_138 = "IT IS STARDATE";
const char * const_string_184 = "KLINGON";
const char * const_string_142 = "KLINGON BATTLE CRUISERS LEFT AT";
const char * const_string_16 = "KLINGON WARSHIPS WHICH HAVE INVADED";
const char * const_string_207 = "LIBRARY-COMPUTER";
const char * const_string_87 = "LONG RANGE SCAN FOR QUADRANT";
const char * const_string_202 = "LONG RANGE SENSORS";
const char * const_string_86 = "LONG RANGE SENSORS ARE INOPERABLE.";
const char * const_string_78 = "LT. UHURA: MESSAGE FROM STARFLEET COMMAND --";
const char * const_string_149 = "MENACING THE FEDERATION HAS BEEN DESTROYED.";
const char * const_string_198 = "MR. SPOCK:  'SENSORS SHOW NO STARBASES IN THIS QUADRANT.'";
const char * const_string_25 = "N";
const char * const_string_10 = "NAVSRSLRSPHATORSHEDAMCOMXXX";
const char * const_string_32 = "NOW ENTERING ";
const char * const_string_83 = "OF QUADRANT";
const char * const_string_203 = "PHASER CONTROL";
const char * const_string_93 = "PHASERS INOPERATIVE.";
const char * const_string_97 = "PHASERS LOCKED ON TARGET;  ";
const char * const_string_204 = "PHOTON TUBES";
const char * const_string_107 = "PHOTON TUBES ARE NOT OPERATIONAL.";
const char * const_string_215 = "POLLUX";
const char * const_string_210 = "PROCYON";
const char * const_string_221 = "REGULUS";
const char * const_string_209 = "RIGEL";
const char * const_string_11 = "S";
const char * const_string_214 = "SAGITTARIUS";
const char * const_string_94 = "SCIENCE OFFICER SPOCK:  'SENSORS SHOW NO ENEMY SHIPS";
const char * const_string_75 = "SECTOR";
const char * const_string_100 = "SENSORS SHOW NO DAMAGE TO ENEMY AT";
const char * const_string_206 = "SHIELD CONTROL";
const char * const_string_120 = "SHIELD CONTROL INOPERABLE.";
const char * const_string_85 = "SHIELD CONTROL SUPPLIES ENERGY TO COMPLETE THE MANEUVER.";
const char * const_string_122 = "SHIELD CONTROL:  'THIS IS NOT THE FEDERATION TREASURY.'";
const char * const_string_152 = "SHIELDS DROPPED FOR DOCKING PURPOSES.";
const char * const_string_201 = "SHORT RANGE SENSORS";
const char * const_string_216 = "SIRIUS";
const char * const_string_223 = "SPICA";
const char * const_string_111 = "STAR AT";
const char * const_string_22 = "STARBASE";
const char * const_string_132 = "STARBASE SHIELDS PROTECT THE ENTERPRISE.";
const char * const_string_129 = "STARDATES.";
const char * const_string_117 = "STARFLEET COMMAND REVIEWING YOUR RECORD TO CONSIDER";
const char * const_string_127 = "TECHNICIANS STANDING BY TO EFFECT REPAIRS TO YOUR SHIP;";
const char * const_string_115 = "THAT DOES IT, CAPTAIN!!  YOU ARE HEREBY RELIEVED OF COMMAND";
const char * const_string_143 = "THE END OF YOUR MISSION.";
const char * const_string_139 = "THE ENTERPRISE HAS BEEN DESTROYED.  THE FEDERATION ";
const char * const_string_144 = "THE FEDERATION IS IN NEED OF A NEW STARSHIP COMMANDER";
const char * const_string_1 = "THE USS ENTERPRISE --- NCC-1701";
const char * const_string_141 = "THERE WERE";
const char * const_string_119 = "TORPEDO MISSED.";
const char * const_string_109 = "TORPEDO TRACK:";
const char * const_string_133 = "UNIT HIT ON ENTERPRISE FROM SECTOR";
const char * const_string_102 = "UNIT HIT ON KLINGON AT SECTOR";
const char * const_string_99 = "UNITS";
const char * const_string_67 = "UNITS OF ENERGY";
const char * const_string_125 = "UNITS PER YOUR COMMAND.'";
const char * const_string_105 = "UNITS REMAINING)";
const char * const_string_135 = "UNITS>";
const char * const_string_211 = "VEGA";
const char * const_string_200 = "WARP ENGINES";
const char * const_string_63 = "WARP ENGINES ARE DAMAGED.  MAXIUM SPEED = WARP 0.2";
const char * const_string_74 = "WARP ENGINES SHUT DOWN AT ";
const char * const_string_58 = "WARP FACTOR (0-";
const char * const_string_140 = "WILL BE CONQUERED.";
const char * const_string_27 = "Y";
const char * const_string_155 = "YELLOW";
const char * const_string_193 = "YOU ARE AT QUADRANT ";
const char * const_string_42 = "YOU HAVE INSUFFICIENT MANEUVERING ENERGY,";
const char * const_string_41 = "YOU'VE JUST STRANDED YOUR SHIP IN SPACE.";
const char * const_string_150 = "YOUR EFFICIENCY RATING IS";
const char * const_string_29 = "YOUR MISSION BEGINS WITH YOUR STARSHIP LOCATED";
const char * const_string_13 = "YOUR ORDERS ARE AS FOLLOWS:";
const char * const_string_189 = "YOUR STUPIDITY HAS LEFT YOU ON YOUR OWN IN";
const char * const_string_26 = "n";
const char * const_string_28 = "y";
const char * const_string_92 = "|";
const char * const_string_89 = "| ";

int main(int argc, char *argv[])
{
  // 1: 10 REM SUPER STARTREK - MAY 16,1978 - REQUIRES 24K MEMORY (AT LEAST)
  // 2: 30 REM
  // 3: 40 REM ****        **** STAR TREK ****      ****
  // 4: 50 REM **** SIMULATION OF A MISSION OF THE STARSHIP ENTERPRISE,
  // 5: 60 REM **** AS SEEN ON THE STAR TREK TV SHOW.
  // 6: 70 REM **** ORIGINAL PROGRAM BY MIKE MAYFIELD, MODIFIED VERSION
  // 7: 80 REM **** PUBLISHED IN DEC'S "101 BASIC GAMES", BY DAVE AHL.
  // 8: 90 REM **** MODIFICATIONS TO THE LATTER (PLUS DEBUGGING) BY BOB
  // 9: 100 REM *** LEEDOM - APRIL & DECEMBER 1974,
  // 10: 110 REM *** WITH A LITTLE HELP FROM HIS FRIENDS . . .
  // 11: 120 REM *** COMMENTS, EPHITETS, AND SUGGESTIONS SOLICITED --
  // 12: 130 REM *** SEND TO: R.C. LEEDOM
  // 13: 140 REM ***          WESTINGHOSE DEFENSE & ELECTRONICS SYSTEMS CNIR
  // 14: 150 REM ***          BOX 746, M.S. 338
  // 15: 160 REM ***          BALTIMORE, MD 21203
  // 16: 170 REM ***
  // 17: 180 REM *** CONVERTED TO MICROSOFT 8 K BASIC 3/16/78 BY JOHN BORDERS
  // 18: 190 REM *** LINE NUMBERS FROm VERSION TREK7 OF 1/12/75 PRESERVED AS
  // 19: 200 REM *** MUCH AS POSSIBLE WHILE USING MULTIPLE STATEMENTS PER LINE
  // 20: 205 WIDTH 80
  // 21: 210 PRINT CHR$(26)
  // 22: 220 FOR XX=1 TO 6:PRINT:NEXT:PRINT TAB(20);"THE USS ENTERPRISE --- NCC-1701":PRINT:FOR YY=1 TO 40 STEP 2
  // reference to XX!
  // reference to YY!
  // 23: 221 PRINT TAB(YY);"                  ,------*------,"
  // 24: 222 PRINT TAB(YY);"  ,-------------   '---  ------'"
  // 25: 223 PRINT TAB(YY);"   '-------- --'      / /"
  // 26: 224 PRINT TAB(YY);"       ,---' '-------/ /--,"
  // 27: 225 PRINT TAB(YY);"        '----------------'"
  // 28: 226 PRINT:PRINT:FOR ZZ=1 TO 7:PRINT CHR$(11);:NEXT ZZ:NEXT YY
  // reference to ZZ!
  // 29: 227 PRINT:PRINT:PRINT:PRINT:PRINT
  // 30: 260 CLEAR 600
  // 31: 270 Z$="                         "
  // reference to Z$
  // 32: 330 DIM G(8,8),C(9,2),K(3,3),N(3),Z(8,8),D(8)
  // reference to G!
  // reference to C!
  // reference to K!
  // reference to N!
  // reference to Z!
  // reference to D!
  // 33: 370 T=INT(RND(1)*20+20)*100:T0=T:T9=25+INT(RND(1)*10):D0=0:E=3000:E0=E
  // reference to T!
  // reference to T0!
  // reference to T9!
  // reference to D0!
  // reference to E!
  // reference to E0!
  // 34: 440 P=10:P0=P:S9=200:S=0:B9=0:K9=0:X$="":X0$=" IS "
  // reference to P!
  // reference to P0!
  // reference to S9!
  // reference to S!
  // reference to B9!
  // reference to K9!
  // reference to X$
  // reference to X0$
  // 35: 470 DEF FND(D)=SQR((K(I,1)-S1)^2+(K(I,2)-S2)^2)
  // 36: 475 DEF FNR(R)=INT(RND(R)*7.98+1.01)
  // 37: 490 Q1=FNR(1):Q2=FNR(1):S1=FNR(1):S2=FNR(1)
  // reference to Q1!
  // reference to Q2!
  // reference to S1!
  // reference to S2!
  // 38: 530 FOR I=1 TO 9:C(I,1)=0:C(I,2)=0:NEXT I
  // reference to I!
  // reference to C!
  // reference to C!
  // 39: 540 C(3,1)=-1:C(2,1)=-1:C(4,1)=-1:C(4,2)=-1:C(5,2)=-1:C(6,2)=-1
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // 40: 600 C(1,2)=1:C(2,2)=1:C(6,1)=1:C(7,1)=1:C(8,1)=1:C(8,2)=1:C(9,2)=1
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // reference to C!
  // 41: 670 FOR I=1 TO 8:D(I)=0:NEXT I
  // reference to I!
  // reference to D!
  // 42: 710 A1$="NAVSRSLRSPHATORSHEDAMCOMXXX"
  // reference to A1$
  // 43: 820 FOR I=1 TO 8:FOR J=1 TO 8:K3=0:Z(I,J)=0:R1=RND(1)
  // reference to I!
  // reference to J!
  // reference to K3!
  // reference to Z!
  // reference to R1!
  // 44: 850 IF R1>.98 THEN K3=3:K9=K9+3:GOTO 980
  // 45: 860 IF R1>.95 THEN K3=2:K9=K9+2:GOTO 980
  // 46: 870 IF R1>.8 THEN K3=1:K9=K9+1
  // 47: 980 B3=0:IF RND(1)>.96 THEN B3=1:B9=B9+1
  // reference to B3!
  // 48: 1040 G(I,J)=K3*100+B3*10+FNR(1):NEXT J:NEXT I:IF K9>T9 THEN T9=K9+1
  // reference to G!
  // 49: 1100 IF B9<>0 THEN 1200
  // 50: 1150 IF G(Q1,Q2)<200 THEN G(Q1,Q2)=G(Q1,Q2)+100:K9=K9+1
  // 51: 1160 B9=1:G(Q1,Q2)=G(Q1,Q2)+10:Q1=FNR(1):Q2=FNR(1)
  // reference to B9!
  // reference to G!
  // reference to Q1!
  // reference to Q2!
  // 52: 1200 K7=K9:IF B9<>1 THEN X$="S":X0$=" ARE "
  // reference to K7!
  // 53: 1230 PRINT"YOUR ORDERS ARE AS FOLLOWS:"
  // 54: 1235 PRINT "--------------------------"
  // 55: 1240 PRINT"   DESTROY THE";K9;"KLINGON WARSHIPS WHICH HAVE INVADED"
  // 56: 1250 PRINT"   THE GALAXY BEFORE THEY CAN ATTACK FEDERATION HEADQUARTERS"
  // 57: 1260 PRINT"   ON STARDATE";T0+T9;CHR$(8);". THIS GIVES YOU";T9;"DAYS. THERE";X0$
  // 58: 1270 PRINT"  ";B9;"STARBASE";X$;" IN THE GALAXY FOR RESUPPLYING YOUR SHIP."
  // 59: 1280 PRINT:PRINT "ARE YOU READY TO ACCEPT COMMAND ('N' FOR INSTRUCTIONS)";
  // 60: 1300 INPUT I5$:IF LEFT$(I5$,1)="N" OR LEFT$(I5$,1)="n" THEN RUN "TREKINST" ELSE IF LEFT$(I5$,1)="Y" OR LEFT$(I5$,1)="y" THEN 1310 ELSE 1280
  // 61: 1310 PRINT CHR$(26)
  // 62: 1320 Z4=Q1:Z5=Q2:K3=0:B3=0:S3=0:G5=0:D4=.5*RND(1):Z(Q1,Q2)=G(Q1,Q2)
  // reference to Z4!
  // reference to Z5!
  // reference to K3!
  // reference to B3!
  // reference to S3!
  // reference to G5!
  // reference to D4!
  // reference to Z!
  // 63: 1390 IF Q1<1 OR Q1>8 OR Q2<1 OR Q2>8 THEN 1600
  // 64: 1430 GOSUB 9030:PRINT:IF T0<>T THEN 1490
  // 65: 1460 PRINT"YOUR MISSION BEGINS WITH YOUR STARSHIP LOCATED"
  // 66: 1470 PRINT"IN THE GALACTIC QUADRANT, '";G2$;"'.":GOTO 1500
  // 67: 1490 PRINT"NOW ENTERING ";G2$;" QUADRANT . . ."
  // 68: 1500 PRINT:K3=INT(G(Q1,Q2)*.01):B3=INT(G(Q1,Q2)*.1)-10*K3
  // reference to K3!
  // reference to B3!
  // 69: 1540 S3=G(Q1,Q2)-100*K3-10*B3:IF K3=0 THEN 1590
  // reference to S3!
  // 70: 1560 PRINT TAB(3);CHR$(22);"  COMBAT AREA      CONDITION RED  ";CHR$(22):IF S>200 THEN PRINT:GOTO 1590
  // 71: 1580 PRINT TAB(3);CHR$(22);"      SHIELDS DANGEROUSLY LOW     ";CHR$(22):PRINT
  // 72: 1590 FOR I=1 TO 3:K(I,1)=0:K(I,2)=0:NEXT I
  // reference to I!
  // reference to K!
  // reference to K!
  // 73: 1600 FOR I=1 TO 3:K(I,3)=0:NEXT I:Q$=Z$+Z$+Z$+Z$+Z$+Z$+Z$+LEFT$(Z$,17)
  // reference to I!
  // reference to K!
  // reference to Q$
  // 74: 1680 A$="<E>":Z1=S1:Z2=S2:GOSUB 8670:IF K3<1 THEN 1820
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 75: 1720 FOR I=1 TO K3:GOSUB 8590:A$="+K+":Z1=R1:Z2=R2
  // reference to I!
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 76: 1780 GOSUB 8670:K(I,1)=R1:K(I,2)=R2:K(I,3)=S9*(.5+RND(1)):NEXT I
  // 77: 1820 IF B3<1 THEN 1910
  // 78: 1880 GOSUB 8590:A$=">B<":Z1=R1:B4=R1:Z2=R2:B5=R2:GOSUB 8670
  // 79: 1910 FOR I=1 TO S3:GOSUB 8590:A$=" * ":Z1=R1:Z2=R2:GOSUB 8670:NEXT I
  // reference to I!
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 80: 1980 GOSUB 6430
  // 81: 1990 IF S+E>10 THEN IF E>10 OR D(7)=0 THEN 2060
  // 82: 2020 PRINT:PRINT TAB(10);CHR$(22);"** FATAL ERROR **";CHR$(22):PRINT"YOU'VE JUST STRANDED YOUR SHIP IN SPACE."
  // 83: 2030 PRINT"YOU HAVE INSUFFICIENT MANEUVERING ENERGY,"
  // 84: 2040 PRINT"AND SHIELD CONTROL IS PRESENTLY INCAPABLE OF"
  // 85: 2050 PRINT"CROSS-CIRCUITING TO ENGINE ROOM!!":PRINT:GOTO 6220
  // 86: 2060 PRINT:INPUT"COMMAND";A$:PRINT
  // 87: 2080 FOR I=1 TO 9:IF LEFT$(A$,3)<>MID$(A1$,3*I-2,3)THEN 2160
  // reference to I!
  // 88: 2140 ON I GOTO 2300,1980,4000,4260,4700,5530,5690,7290,6270
  // 89: 2160 NEXT I:PRINT"ENTER ONE OF THE FOLLOWING:"
  // 90: 2170 PRINT "--------------------------"
  // 91: 2180 PRINT"  NAV  (TO SET COURSE)"
  // 92: 2190 PRINT"  SRS  (FOR SHORT RANGE SENSOR SCAN)"
  // 93: 2200 PRINT"  LRS  (FOR LONG RANGE SENSOR SCAN)"
  // 94: 2210 PRINT"  PHA  (TO FIRE PHASERS)"
  // 95: 2220 PRINT"  TOR  (TO FIRE PHOTON TORPEDOES)"
  // 96: 2230 PRINT"  SHE  (TO RAISE OR LOWER SHIELDS)"
  // 97: 2240 PRINT"  DAM  (FOR DAMAGE CONTROL REPORTS)"
  // 98: 2250 PRINT"  COM  (TO CALL ON LIBRARY-COMPUTER)"
  // 99: 2260 PRINT"  XXX  (TO RESIGN YOUR COMMAND)":PRINT:GOTO 1990
  // 100: 2300 INPUT"COURSE (0-9)";C1:IF C1=9 THEN C1=1
  // 101: 2310 IF C1>=1 AND C1<9 THEN 2350
  // 102: 2330 PRINT"   LT. SULU: 'INCORRECT COURSE DATA, SIR!'":GOTO 1990
  // 103: 2350 X$="8":IF D(1)<0 THEN X$="0.2"
  // reference to X$
  // 104: 2360 PRINT"WARP FACTOR (0-";X$;")";:INPUT W1:PRINT:IF D(1)<0 AND W1>.2 THEN 2470
  // 105: 2380 IF W1>0 AND W1<=8 THEN 2490
  // 106: 2390 IF W1=0 THEN 1990
  // 107: 2420 PRINT"   CHIEF ENGINEER SCOTT: 'THE ENGINES WON'T TAKE";
  // 108: 2430 PRINT" WARP";W1;CHR$(8);"!'":GOTO 1990
  // 109: 2470 PRINT"WARP ENGINES ARE DAMAGED.  MAXIUM SPEED = WARP 0.2":GOTO 1990
  // 110: 2490 N=INT(W1*8+.5):IF E-N>=0 THEN 2590
  // reference to N!
  // 111: 2500 PRINT"ENGINEERING:  'INSUFFICIENT ENERGY AVAILABLE"
  // 112: 2510 PRINT"               FOR MANEUVERING AT WARP";W1;CHR$(8);"!'"
  // 113: 2530 IF S<N-E OR D(7)<0 THEN 1990
  // 114: 2550 PRINT"DEFLECTOR CONTROL ROOM:  ";S;"UNITS OF ENERGY"
  // 115: 2560 PRINT"                          PRESENTLY DEPLOYED TO SHIELDS."
  // 116: 2570 GOTO 1990
  // 117: 2590 FOR I=1 TO K3:IF K(I,3)=0 THEN 2700
  // reference to I!
  // 118: 2610 A$="   ":Z1=K(I,1):Z2=K(I,2):GOSUB 8670:GOSUB 8590
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 119: 2660 K(I,1)=Z1:K(I,2)=Z2:A$="+K+":GOSUB 8670
  // reference to K!
  // reference to K!
  // reference to A$
  // 120: 2700 NEXT I:GOSUB 6000:D1=0:D6=W1:IF W1>=1 THEN D6=1
  // 121: 2770 FOR I=1 TO 8:IF D(I)>=0 THEN 2880
  // reference to I!
  // 122: 2790 D(I)=D(I)+D6:IF D(I)>-.1 AND D(I)<0 THEN D(I)=-.1:GOTO 2880
  // reference to D!
  // 123: 2800 IF D(I)<0 THEN 2880
  // 124: 2810 IF D1<>1 THEN D1=1:PRINT"DAMAGE CONTROL REPORT:  ";
  // 125: 2840 PRINT TAB(8);:R1=I:GOSUB 8790:PRINT G2$;" REPAIR COMPLETED."
  // reference to R1!
  // 126: 2880 NEXT I:IF RND(1)>.2 THEN 3070
  // 127: 2910 R1=FNR(1):IF RND(1)>=.6 THEN 3000
  // reference to R1!
  // 128: 2930 D(R1)=D(R1)-(RND(1)*5+1):PRINT"DAMAGE CONTROL REPORT:  ";
  // reference to D!
  // 129: 2960 GOSUB 8790:PRINT G2$;" DAMAGED":PRINT:GOTO 3070
  // 130: 3000 D(R1)=D(R1)+RND(1)*3+1:PRINT"DAMAGE CONTROL REPORT:  ";
  // reference to D!
  // 131: 3030 GOSUB 8790:PRINT G2$;" STATE OF REPAIR IMPROVED":PRINT
  // 132: 3070 A$="   ":Z1=INT(S1):Z2=INT(S2):GOSUB 8670
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 133: 3110 X1=C(C1,1)+(C(C1+1,1)-C(C1,1))*(C1-INT(C1)):X=S1:Y=S2
  // reference to X1!
  // reference to X!
  // reference to Y!
  // 134: 3140 X2=C(C1,2)+(C(C1+1,2)-C(C1,2))*(C1-INT(C1)):Q4=Q1:Q5=Q2
  // reference to X2!
  // reference to Q4!
  // reference to Q5!
  // 135: 3170 FOR I=1 TO N:S1=S1+X1:S2=S2+X2:IF S1<1 OR S1>=9 OR S2<1 OR S2>=9 THEN 3500
  // reference to I!
  // reference to S1!
  // reference to S2!
  // 136: 3240 S8=INT(S1)*24+INT(S2)*3-26:IF MID$(Q$,S8,2)="  "THEN 3360
  // reference to S8!
  // 137: 3320 S1=INT(S1-X1):S2=INT(S2-X2):PRINT"WARP ENGINES SHUT DOWN AT ";
  // reference to S1!
  // reference to S2!
  // 138: 3350 PRINT"SECTOR";S1;CHR$(8);",";S2;"DUE TO BAD NAVAGATION":GOTO 3370
  // 139: 3360 NEXT I:S1=INT(S1):S2=INT(S2)
  // 140: 3370 A$="<E>":Z1=INT(S1):Z2=INT(S2):GOSUB 8670:GOSUB 3910:T8=1
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // reference to T8!
  // 141: 3430 IF W1<1 THEN T8=.1*INT(10*W1)
  // 142: 3450 T=T+T8:IF T>T0+T9 THEN 6220
  // reference to T!
  // 143: 3480 GOTO 1980
  // 144: 3500 X=8*Q1+X+N*X1:Y=8*Q2+Y+N*X2:Q1=INT(X/8):Q2=INT(Y/8):S1=INT(X-Q1*8)
  // reference to X!
  // reference to Y!
  // reference to Q1!
  // reference to Q2!
  // reference to S1!
  // 145: 3550 S2=INT(Y-Q2*8):IF S1=0 THEN Q1=Q1-1:S1=8
  // reference to S2!
  // 146: 3590 IF S2=0 THEN Q2=Q2-1:S2=8
  // 147: 3620 X5=0:IF Q1<1 THEN X5=1:Q1=1:S1=1
  // reference to X5!
  // 148: 3670 IF Q1>8 THEN X5=1:Q1=8:S1=8
  // 149: 3710 IF Q2<1 THEN X5=1:Q2=1:S2=1
  // 150: 3750 IF Q2>8 THEN X5=1:Q2=8:S2=8
  // 151: 3790 IF X5=0 THEN 3860
  // 152: 3800 PRINT"LT. UHURA: MESSAGE FROM STARFLEET COMMAND --"
  // 153: 3810 PRINT"  'PERMISSION TO ATTEMPT CROSSING OF GALACTIC PERIMETER"
  // 154: 3820 PRINT"  IS HEREBY *DENIED*.  SHUT DOWN YOUR ENGINES.'"
  // 155: 3830 PRINT"CHIEF ENGINEER SCOTT:  'WARP ENGINES SHUT DOWN"
  // 156: 3840 PRINT"  AT SECTOR";S1;CHR$(8);",";S2;"OF QUADRANT";Q1;CHR$(8);",";Q2;CHR$(8);".'"
  // 157: 3850 IF T>T0+T9 THEN 6220
  // 158: 3860 IF 8*Q1+Q2=8*Q4+Q5 THEN 3370
  // 159: 3870 T=T+1:GOSUB 3910:GOTO 1320
  // reference to T!
  // 160: 3910 E=E-N-10:IF E>=0 THEN RETURN
  // reference to E!
  // 161: 3930 PRINT"SHIELD CONTROL SUPPLIES ENERGY TO COMPLETE THE MANEUVER."
  // 162: 3940 S=S+E:E=0:IF S<=0 THEN S=0
  // reference to S!
  // reference to E!
  // 163: 3980 RETURN
  // 164: 4000 IF D(3)<0 THEN PRINT"LONG RANGE SENSORS ARE INOPERABLE.":GOTO 1990
  // 165: 4030 PRINT"LONG RANGE SCAN FOR QUADRANT";Q1;CHR$(8);",";Q2:PRINT
  // 166: 4040 O1$="-------------------":PRINT O1$
  // reference to O1$
  // 167: 4060 FOR I=Q1-1 TO Q1+1:N(1)=-1:N(2)=-2:N(3)=-3:FOR J=Q2-1 TO Q2+1
  // reference to I!
  // reference to N!
  // reference to N!
  // reference to N!
  // reference to J!
  // 168: 4120 IF I>0 AND I<9 AND J>0 AND J<9 THEN N(J-Q2+2)=G(I,J):Z(I,J)=G(I,J)
  // 169: 4180 NEXT J:FOR L=1 TO 3:PRINT"| ";:IF N(L)<0 THEN PRINT"*** ";:GOTO 4230
  // 170: 4210 PRINT RIGHT$(STR$(N(L)+1000),3);" ";
  // 171: 4230 NEXT L:PRINT"|":PRINT O1$:NEXT I:GOTO 1990
  // 172: 4260 IF D(4)<0 THEN PRINT"PHASERS INOPERATIVE.":GOTO 1990
  // 173: 4265 IF K3>0 THEN 4330
  // 174: 4270 PRINT"SCIENCE OFFICER SPOCK:  'SENSORS SHOW NO ENEMY SHIPS"
  // 175: 4280 PRINT"                         IN THIS QUADRANT'":GOTO 1990
  // 176: 4330 IF D(8)<0 THEN PRINT"COMPUTER FAILURE HAMPERS ACCURACY."
  // 177: 4350 PRINT"PHASERS LOCKED ON TARGET;  ";
  // 178: 4360 PRINT"ENERGY AVAILABLE =";E;"UNITS"
  // 179: 4370 INPUT"NUMBER OF UNITS TO FIRE";X:IF X<=0 THEN 1990
  // 180: 4400 IF E-X<0 THEN 4360
  // 181: 4410 E=E-X:IF D(7)<0 THEN X=X*RND(1)
  // reference to E!
  // 182: 4450 H1=INT(X/K3):FOR I=1 TO 3:IF K(I,3)<=0 THEN 4670
  // reference to H1!
  // reference to I!
  // 183: 4480 H=INT((H1/FND(0))*(RND(1)+2)):IF H>.15*K(I,3)THEN 4530
  // reference to H!
  // 184: 4500 PRINT"SENSORS SHOW NO DAMAGE TO ENEMY AT";K(I,1);CHR$(8);",";K(I,2);CHR$(8);".":GOTO 4670
  // 185: 4530 K(I,3)=K(I,3)-H:PRINT H;"UNIT HIT ON KLINGON AT SECTOR";K(I,1);CHR$(8);",";
  // reference to K!
  // 186: 4550 PRINT K(I,2);CHR$(8);".":IF K(I,3)<=0 THEN PRINT:PRINT CHR$(22);"*** KLINGON DESTROYED ***";CHR$(22):PRINT:GOTO 4580
  // 187: 4560 PRINT" (SENSORS SHOW";K(I,3);"UNITS REMAINING)":GOTO 4670
  // 188: 4580 K3=K3-1:K9=K9-1:Z1=K(I,1):Z2=K(I,2):A$="   ":GOSUB 8670
  // reference to K3!
  // reference to K9!
  // reference to Z1!
  // reference to Z2!
  // reference to A$
  // 189: 4650 K(I,3)=0:G(Q1,Q2)=G(Q1,Q2)-100:Z(Q1,Q2)=G(Q1,Q2):IF K9<=0 THEN 6370
  // reference to K!
  // reference to G!
  // reference to Z!
  // 190: 4670 NEXT I:GOSUB 6000:GOTO 1990
  // 191: 4700 IF P<=0 THEN PRINT"ALL PHOTON TORPEDOES EXPENDED.":GOTO 1990
  // 192: 4730 IF D(5)<0 THEN PRINT"PHOTON TUBES ARE NOT OPERATIONAL.":GOTO 1990
  // 193: 4760 INPUT"PHOTON TORPEDO COURSE (1-9)";C1:IF C1=9 THEN C1=1
  // 194: 4780 IF C1>=1 AND C1<9 THEN 4850
  // 195: 4790 PRINT"ENSIGN CHEKOV:  'INCORRECT COURSE DATA, SIR!'"
  // 196: 4800 GOTO 1990
  // 197: 4850 X1=C(C1,1)+(C(C1+1,1)-C(C1,1))*(C1-INT(C1)):E=E-2:P=P-1
  // reference to X1!
  // reference to E!
  // reference to P!
  // 198: 4860 X2=C(C1,2)+(C(C1+1,2)-C(C1,2))*(C1-INT(C1)):X=S1:Y=S2
  // reference to X2!
  // reference to X!
  // reference to Y!
  // 199: 4910 PRINT"TORPEDO TRACK:"
  // 200: 4920 X=X+X1:Y=Y+X2:X3=INT(X+.5):Y3=INT(Y+.5)
  // reference to X!
  // reference to Y!
  // reference to X3!
  // reference to Y3!
  // 201: 4960 IF X3<1 OR X3>8 OR Y3<1 OR Y3>8 THEN 5490
  // 202: 5000 PRINT"               ";X3;CHR$(8);",";Y3:A$="   ":Z1=X:Z2=Y:GOSUB 8830
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 203: 5050 IF Z3<>0 THEN 4920
  // 204: 5060 A$="+K+":Z1=X:Z2=Y:GOSUB 8830:IF Z3=0 THEN 5210
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 205: 5110 PRINT:PRINT CHR$(22);"*** KLINGON DESTROYED ***";CHR$(22):PRINT:K3=K3-1:K9=K9-1:IF K9<=0 THEN 6370
  // reference to K3!
  // reference to K9!
  // 206: 5150 FOR I=1 TO 3:IF X3=K(I,1)AND Y3=K(I,2)THEN 5190
  // reference to I!
  // 207: 5180 NEXT I:I=3
  // 208: 5190 K(I,3)=0:GOTO 5430
  // reference to K!
  // 209: 5210 A$=" * ":Z1=X:Z2=Y:GOSUB 8830:IF Z3=0 THEN 5280
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 210: 5260 PRINT"STAR AT";X3;",";Y3;"ABSORBED TORPEDO ENERGY.":GOSUB 6000:GOTO 1990
  // 211: 5280 A$=">!<":Z1=X:Z2=Y:GOSUB 8830:IF Z3=0 THEN 4760
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 212: 5330 PRINT CHR$(22);"*** STARBASE DESTROYED ***";CHR$(22):B3=B3-1:B9=B9-1
  // reference to B3!
  // reference to B9!
  // 213: 5360 IF B9>0 OR K9>T-T0-T9 THEN 5400
  // 214: 5370 PRINT"THAT DOES IT, CAPTAIN!!  YOU ARE HEREBY RELIEVED OF COMMAND"
  // 215: 5380 PRINT"AND SENTENCED TO 99 STARDATES AT HARD LABOR ON CYGNUS 12!!"
  // 216: 5390 GOTO 6270
  // 217: 5400 PRINT"STARFLEET COMMAND REVIEWING YOUR RECORD TO CONSIDER"
  // 218: 5410 PRINT"COURT MARTIAL!":D0=0
  // reference to D0!
  // 219: 5430 Z1=X:Z2=Y:A$="   ":GOSUB 8670
  // reference to Z1!
  // reference to Z2!
  // reference to A$
  // 220: 5470 G(Q1,Q2)=K3*100+B3*10+S3:Z(Q1,Q2)=G(Q1,Q2):GOSUB 6000:GOTO 1990
  // reference to G!
  // reference to Z!
  // 221: 5490 PRINT"TORPEDO MISSED.":PRINT:GOSUB 6000:GOTO 1990
  // 222: 5530 IF D(7)<0 THEN PRINT"SHIELD CONTROL INOPERABLE.":GOTO 1990
  // 223: 5560 PRINT"ENERGY AVAILABLE =";E+S;:INPUT"NUMBER OF UNITS TO SHIELDS";X
  // 224: 5580 IF X<0 OR S=X THEN PRINT"<SHIELDS UNCHANGED>":GOTO 1990
  // 225: 5590 IF X<=E+S THEN 5630
  // 226: 5600 PRINT"SHIELD CONTROL:  'THIS IS NOT THE FEDERATION TREASURY.'" 
  // 227: 5610 PRINT"<SHIELDS UNCHANGED>":GOTO 1990
  // 228: 5630 E=E+S-X:S=X:PRINT"DEFLECTOR CONTROL ROOM:"
  // reference to E!
  // reference to S!
  // 229: 5660 PRINT"  'SHIELDS NOW AT";INT(S);"UNITS PER YOUR COMMAND.'":GOTO 1990
  // 230: 5690 IF D(6)>=0 THEN 5910
  // 231: 5700 PRINT"DAMAGE CONTROL REPORT NOT AVAILABLE.":IF D0=0 THEN 1990
  // 232: 5720 D3=0:FOR I=1 TO 8:IF D(I)<0 THEN D3=D3+.1
  // reference to D3!
  // reference to I!
  // 233: 5760 NEXT I:IF D3=0 THEN 1990
  // 234: 5780 PRINT:D3=D3+D4:IF D3>=1 THEN D3=.9
  // reference to D3!
  // 235: 5810 PRINT"TECHNICIANS STANDING BY TO EFFECT REPAIRS TO YOUR SHIP;"
  // 236: 5820 PRINT"ESTIMATED TIME TO REPAIR:";.01*INT(100*D3);"STARDATES."
  // 237: 5840 INPUT"WILL YOU AUTHORIZE THE REPAIR ORDER (Y/N)";A$
  // 238: 5860 IF A$<>"Y"THEN 1990
  // 239: 5870 FOR I=1 TO 8:IF D(I)<0 THEN D(I)=0
  // reference to I!
  // 240: 5890 NEXT I:T=T+D3+.1
  // 241: 5910 PRINT:PRINT"DEVICE             STATE OF REPAIR":PRINT"------             ---------------":FOR R1=1 TO 8
  // reference to R1!
  // 242: 5920 GOSUB 8790:PRINT G2$;LEFT$(Z$,25-LEN(G2$));INT(D(R1)*100)*.01
  // 243: 5950 NEXT R1:PRINT:IF D0<>0 THEN 5720
  // 244: 5980 GOTO 1990
  // 245: 6000 IF K3<=0 THEN RETURN
  // 246: 6010 IF D0<>0 THEN PRINT"STARBASE SHIELDS PROTECT THE ENTERPRISE.":RETURN
  // 247: 6040 FOR I=1 TO 3:IF K(I,3)<=0 THEN 6200
  // reference to I!
  // 248: 6060 H=INT((K(I,3)/FND(1))*(2+RND(1))):S=S-H:K(I,3)=K(I,3)/(3+RND(0))
  // reference to H!
  // reference to S!
  // reference to K!
  // 249: 6080 PRINT:PRINT H;"UNIT HIT ON ENTERPRISE FROM SECTOR";K(I,1);CHR$(8);",";K(I,2);CHR$(8);"."
  // 250: 6090 IF S<=0 THEN 6240
  // 251: 6100 PRINT"      <SHIELDS DOWN TO";S;"UNITS>":IF H<20 THEN 6200
  // 252: 6120 IF RND(1)>.6 OR H/S<=.02 THEN 6200
  // 253: 6140 R1=FNR(1):D(R1)=D(R1)-H/S-.5*RND(1):GOSUB 8790
  // reference to R1!
  // reference to D!
  // 254: 6170 PRINT"DAMAGE CONTROL: '";G2$;" DAMAGED BY THE HIT'"
  // 255: 6200 NEXT I:RETURN
  // 256: 6220 PRINT:PRINT"IT IS STARDATE";T;CHR$(8);".":PRINT:GOTO 6270
  // 257: 6240 PRINT:PRINT"THE ENTERPRISE HAS BEEN DESTROYED.  THE FEDERATION ";
  // 258: 6250 PRINT"WILL BE CONQUERED.":GOTO 6220
  // 259: 6270 PRINT"THERE WERE";K9;"KLINGON BATTLE CRUISERS LEFT AT"
  // 260: 6280 PRINT"THE END OF YOUR MISSION."
  // 261: 6290 PRINT:PRINT:IF B9=0 THEN 6360
  // 262: 6310 PRINT"THE FEDERATION IS IN NEED OF A NEW STARSHIP COMMANDER"
  // 263: 6320 PRINT"FOR A SIMILAR MISSION -- IF THERE IS A VOLUNTEER,"
  // 264: 6330 INPUT"LET HIM STEP FORWARD AND ENTER 'AYE'";A$:IF A$="AYE"THEN 10
  // 265: 6360 PRINT:PRINT "BACK TO SYSTEM.":SYSTEM
  // 266: 6370 PRINT"CONGRATULATIONS, CAPTAIN!  THE LAST KLINGON BATTLE CRUISER"
  // 267: 6380 PRINT"MENACING THE FEDERATION HAS BEEN DESTROYED.":PRINT
  // 268: 6400 PRINT"YOUR EFFICIENCY RATING IS";1000*(K7/(T-T0))^2:GOTO 6290
  // 269: 6430 FOR I=S1-1 TO S1+1:FOR J=S2-1 TO S2+1
  // reference to I!
  // reference to J!
  // 270: 6450 IF INT(I+.5)<1 OR INT(I+.5)>8 OR INT(J+.5)<1 OR INT(J+.5)>8 THEN 6540
  // 271: 6490 A$=">B<":Z1=I:Z2=J:GOSUB 8830:IF Z3=1 THEN 6580
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 272: 6540 NEXT J:NEXT I:D0=0:GOTO 6650
  // 273: 6580 D0=1:C$="DOCKED":E=E0:P=P0
  // reference to D0!
  // reference to C$
  // reference to E!
  // reference to P!
  // 274: 6620 PRINT"SHIELDS DROPPED FOR DOCKING PURPOSES.":S=0:GOTO 6720
  // reference to S!
  // 275: 6650 IF K3>0 THEN C$="*RED*":GOTO 6720
  // 276: 6660 C$="GREEN":IF E<E0*.1 THEN C$="YELLOW"
  // reference to C$
  // 277: 6720 IF D(2)>=0 THEN 6770
  // 278: 6730 PRINT:PRINT"*** SHORT RANGE SENSORS ARE OUT ***":PRINT:RETURN
  // 279: 6770 O1$="   +--1---2---3---4---5---6---7---8-+":PRINT O1$:FOR I=1 TO 8:PRINT I;"|";
  // reference to O1$
  // reference to I!
  // 280: 6820 FOR J=(I-1)*24+1 TO(I-1)*24+22 STEP 3:PRINT" ";MID$(Q$,J,3);:NEXT J:PRINT"|";I;
  // reference to J!
  // 281: 6830 ON I GOTO 6850,6900,6960,7020,7070,7120,7180,7240
  // 282: 6850 PRINT"        STARDATE          ";:PRINT USING"####.#";INT(T*10)*.1:GOTO 7260
  // 283: 6900 PRINT"        CONDITION          ";:IF C$="*RED*" THEN PRINT CHR$(22);"*RED*";CHR$(22) ELSE IF C$="DOCKED" THEN PRINT CHR$(22);"DOCKED";CHR$(22) ELSE PRINT C$
  // 284: 6902 GOTO 7260
  // 285: 6960 PRINT"        QUADRANT           ";Q1;CHR$(8);",";Q2;CHR$(8):GOTO 7260
  // 286: 7020 PRINT"        SECTOR             ";S1;CHR$(8);",";S2;CHR$(8):GOTO 7260
  // 287: 7070 PRINT"        PHOTON TORPEDOES  ";:PRINT USING"######";INT(P):GOTO 7260
  // 288: 7120 PRINT"        TOTAL ENERGY      ";:PRINT USING"######";INT(E+S):GOTO 7260
  // 289: 7180 PRINT"        SHIELDS           ";:PRINT USING"######";INT(S):GOTO 7260
  // 290: 7240 PRINT"        KLINGONS REMAINING";:PRINT USING"######";INT(K9)
  // 291: 7260 NEXT I:PRINT O1$:RETURN
  // 292: 7290 IF D(8)<0 THEN PRINT"COMPUTER DISABLED.":GOTO 1990
  // 293: 7320 INPUT"COMPUTER ACTIVE AND AWAITING COMMAND";A:IF A<0 THEN 1990
  // 294: 7350 PRINT:H8=1:ON A+1 GOTO 7540,7900,8070,8500,8150,7400
  // reference to H8!
  // 295: 7360 PRINT"FUNCTIONS AVAILABLE FROM LIBRARY-COMPUTER:"
  // 296: 7365 PRINT "-----------------------------------------":PRINT
  // 297: 7370 PRINT"   0 = CUMULATIVE GALTIC RECORD"
  // 298: 7372 PRINT"   1 = STATUS REPORT"
  // 299: 7374 PRINT"   2 = PHOTON TORPEDO DATA"
  // 300: 7376 PRINT"   3 = STARBASE NAV DATA"
  // 301: 7378 PRINT"   4 = DIRECTION/DISTANCE CALCULATOR"
  // 302: 7380 PRINT"   5 = GALAXY 'REGION NAME' MAP":PRINT:GOTO 7320
  // 303: 7400 H8=0:G5=1:PRINT"                        THE GALAXY":GOTO 7550
  // reference to H8!
  // reference to G5!
  // 304: 7540 REM
  // 305: 7542 REM
  // 306: 7543 PRINT:PRINT"       ";
  // 307: 7544 PRINT"COMPUTER RECORD OF GALAXY FOR QUADRANT";Q1;CHR$(8);",";Q2
  // 308: 7546 PRINT
  // 309: 7550 PRINT"       1     2     3     4     5     6     7     8"
  // 310: 7560 O1$="    +-----+-----+-----+-----+-----+-----+-----+-----+"
  // reference to O1$
  // 311: 7570 PRINT O1$:FOR I=1 TO 8:PRINT I;" ";:IF H8=0 THEN 7740
  // reference to I!
  // 312: 7630 FOR J=1 TO 8:PRINT"| ";:IF Z(I,J)=0 THEN PRINT"*** ";:GOTO 7720
  // reference to J!
  // 313: 7700 PRINT RIGHT$(STR$(Z(I,J)+1000),3);" ";
  // 314: 7720 IF J=8 THEN PRINT "|"
  // 315: 7721 NEXT J:GOTO 7850
  // 316: 7740 Z4=I:Z5=1:GOSUB 9030:J0=INT(15-.5*LEN(G2$)):PRINT TAB(J0);G2$;
  // reference to Z4!
  // reference to Z5!
  // reference to J0!
  // 317: 7800 Z5=5:GOSUB 9030:J0=INT(39-.5*LEN(G2$)):PRINT TAB(J0);G2$
  // reference to Z5!
  // reference to J0!
  // 318: 7850 PRINT O1$:NEXT I:PRINT:GOTO 1990
  // 319: 7900 PRINT "   STATUS REPORT:":PRINT "   -------------":X$="":IF K9>1 THEN X$="S"
  // reference to X$
  // 320: 7940 PRINT K9;"KLINGON";X$;" LEFT."
  // 321: 7960 PRINT" MISSION MUST BE COMPLETED IN";.1*INT((T0+T9-T)*10);"STARDATES."
  // 322: 7970 X$="S":IF B9<2 THEN X$="":IF B9<1 THEN 8010
  // reference to X$
  // 323: 7980 PRINT" THE FEDERATION IS MAINTAINING";B9;"STARBASE";X$;" IN THE GALAXY."
  // 324: 7990 GOTO 5690
  // 325: 8010 PRINT"YOUR STUPIDITY HAS LEFT YOU ON YOUR OWN IN"
  // 326: 8020 PRINT"  THE GALAXY -- YOU HAVE NO STARBASES LEFT!":GOTO 5690
  // 327: 8070 IF K3<=0 THEN 4270
  // 328: 8080 X$="":IF K3>1 THEN X$="S"
  // reference to X$
  // 329: 8090 PRINT"FROM ENTERPRISE TO KLINGON BATTLE CRUSER";X$
  // 330: 8100 H8=0:FOR I=1 TO 3:IF K(I,3)<=0 THEN 8480
  // reference to H8!
  // reference to I!
  // 331: 8110 W1=K(I,1):X=K(I,2)
  // reference to W1!
  // reference to X!
  // 332: 8120 C1=S1:A=S2:GOTO 8220
  // reference to C1!
  // reference to A!
  // 333: 8150 PRINT"DIRECTION/DISTANCE CALCULATOR:"
  // 334: 8160 PRINT"YOU ARE AT QUADRANT ";Q1;CHR$(8);",";Q2;" SECTOR ";S1;CHR$(8);",";S2;CHR$(8);"."
  // 335: 8170 INPUT"PLEASE ENTER INITIAL COORDINATES (X,Y)";C1,A
  // 336: 8200 INPUT"FINAL COORDINATES (X,Y)";W1,X
  // 337: 8220 X=X-A:A=C1-W1:IF X<0 THEN 8350
  // reference to X!
  // reference to A!
  // 338: 8250 IF A<0 THEN 8410
  // 339: 8260 IF X>0 THEN 8280
  // 340: 8270 IF A=0 THEN C1=5:GOTO 8290
  // 341: 8280 C1=1
  // reference to C1!
  // 342: 8290 IF ABS(A)<=ABS(X)THEN 8330
  // 343: 8310 PRINT"DIRECTION =";C1+(((ABS(A)-ABS(X))+ABS(A))/ABS(A)):GOTO 8460
  // 344: 8330 PRINT"DIRECTION =";C1+(ABS(A)/ABS(X)):GOTO 8460
  // 345: 8350 IF A>0 THEN C1=3:GOTO 8420
  // 346: 8360 IF X<>0 THEN C1=5:GOTO 8290
  // 347: 8410 C1=7
  // reference to C1!
  // 348: 8420 IF ABS(A)>=ABS(X)THEN 8450
  // 349: 8430 PRINT"DIRECTION =";C1+(((ABS(X)-ABS(A))+ABS(X))/ABS(X)):GOTO 8460
  // 350: 8450 PRINT"DIRECTION =";C1+(ABS(X)/ABS(A))
  // 351: 8460 PRINT"DISTANCE =";SQR(X^2+A^2):IF H8=1 THEN 1990
  // 352: 8480 NEXT I:GOTO 1990
  // 353: 8500 IF B3<>0 THEN PRINT"FROM ENTERPRISE TO STARBASE:":W1=B4:X=B5:GOTO 8120
  // 354: 8510 PRINT"MR. SPOCK:  'SENSORS SHOW NO STARBASES IN THIS QUADRANT.'"";
  // 355: 8520 GOTO 1990
  // 356: 8590 R1=FNR(1):R2=FNR(1):A$="   ":Z1=R1:Z2=R2:GOSUB 8830:IF Z3=0 THEN 8590
  // reference to R1!
  // reference to R2!
  // reference to A$
  // reference to Z1!
  // reference to Z2!
  // 357: 8600 RETURN
  // 358: 8670 S8=INT(Z2-.5)*3+INT(Z1-.5)*24+1
  // reference to S8!
  // 359: 8675 IF LEN(A$)<>3 THEN PRINT"ERROR":STOP
  // 360: 8680 IF S8=1 THEN Q$=A$+RIGHT$(Q$,189):RETURN
  // 361: 8690 IF S8=190 THEN Q$=LEFT$(Q$,189)+A$:RETURN
  // 362: 8700 Q$=LEFT$(Q$,S8-1)+A$+RIGHT$(Q$,190-S8):RETURN
  // reference to Q$
  // 363: 8790 ON R1 GOTO 8792,8794,8796,8798,8800,8802,8804,8806
  // 364: 8792 G2$="WARP ENGINES":RETURN
  // reference to G2$
  // 365: 8794 G2$="SHORT RANGE SENSORS":RETURN
  // reference to G2$
  // 366: 8796 G2$="LONG RANGE SENSORS":RETURN
  // reference to G2$
  // 367: 8798 G2$="PHASER CONTROL":RETURN
  // reference to G2$
  // 368: 8800 G2$="PHOTON TUBES":RETURN
  // reference to G2$
  // 369: 8802 G2$="DAMAGE CONTROL":RETURN
  // reference to G2$
  // 370: 8804 G2$="SHIELD CONTROL":RETURN
  // reference to G2$
  // 371: 8806 G2$="LIBRARY-COMPUTER":RETURN
  // reference to G2$
  // 372: 8830 Z1=INT(Z1+.5):Z2=INT(Z2+.5):S8=(Z2-1)*3+(Z1-1)*24+1:Z3=0
  // reference to Z1!
  // reference to Z2!
  // reference to S8!
  // reference to Z3!
  // 373: 8890 IF MID$(Q$,S8,3)<>A$THEN RETURN
  // 374: 8900 Z3=1:RETURN
  // reference to Z3!
  // 375: 9030 IF Z5<=4 THEN ON Z4 GOTO 9040,9050,9060,9070,9080,9090,9100,9110
  // 376: 9035 GOTO 9120
  // 377: 9040 G2$="ANTARES":GOTO 9210
  // reference to G2$
  // 378: 9050 G2$="RIGEL":GOTO 9210
  // reference to G2$
  // 379: 9060 G2$="PROCYON":GOTO 9210
  // reference to G2$
  // 380: 9070 G2$="VEGA":GOTO 9210
  // reference to G2$
  // 381: 9080 G2$="CANOPUS":GOTO 9210
  // reference to G2$
  // 382: 9090 G2$="ALTAIR":GOTO 9210
  // reference to G2$
  // 383: 9100 G2$="SAGITTARIUS":GOTO 9210
  // reference to G2$
  // 384: 9110 G2$="POLLUX":GOTO 9210
  // reference to G2$
  // 385: 9120 ON Z4 GOTO 9130,9140,9150,9160,9170,9180,9190,9200
  // 386: 9130 G2$="SIRIUS":GOTO 9210
  // reference to G2$
  // 387: 9140 G2$="DENEB":GOTO 9210
  // reference to G2$
  // 388: 9150 G2$="CAPELLA":GOTO 9210
  // reference to G2$
  // 389: 9160 G2$="BETELGEUSE":GOTO 9210
  // reference to G2$
  // 390: 9170 G2$="ALDEBARAN":GOTO 9210
  // reference to G2$
  // 391: 9180 G2$="REGULUS":GOTO 9210
  // reference to G2$
  // 392: 9190 G2$="ARCTURUS":GOTO 9210
  // reference to G2$
  // 393: 9200 G2$="SPICA"
  // reference to G2$
  // 394: 9210 IF G5<>1 THEN ON Z5 GOTO 9230,9240,9250,9260,9230,9240,9250,9260
  // 395: 9220 RETURN
  // 396: 9230 G2$=G2$+" I":RETURN
  // reference to G2$
  // 397: 9240 G2$=G2$+" II":RETURN
  // reference to G2$
  // 398: 9250 G2$=G2$+" III":RETURN
  // reference to G2$
  // 399: 9260 G2$=G2$+" IV":RETURN
  // reference to G2$
  // 400: 9999 END
}
