%option prefix="MBASIC_"

%{

#include <iostream>

#include "basalt.h"
#include "mbasic.tab.hpp"

#define   YY_INPUT(buf,result,max_size)   MBASIC_yyinput(buf, &result, max_size)

%}

%option noyywrap

%x REM_MODE

%%

<REM_MODE>[^\n]* {
  MBASIC_lval.sval = new std::string(yytext);
  return COMMENT;
}

<REM_MODE>\n {
  BEGIN(INITIAL);
  g_lexLineNumber++;
  return EOL_TOKEN;
}

[Rr][Ee][Tt][Uu][Rr][Nn]  {
          return RETURN_TOKEN;
        }

[Ss][Yy][Ss][Tt][Ee][Mm]  {
          return SYSTEM_TOKEN;
        }

[Cc][Ll][Ee][Aa][Rr]  {
					return CLEAR_TOKEN;
				}

[Gg][Oo][Ss][Uu][Bb]  {
					return GOSUB_TOKEN;
				}

[Ii][Nn][Pp][Uu][Tt]  {
					return INPUT_TOKEN;
				}

[Pp][Rr][Ii][Nn][Tt]  {
          return PRINT_TOKEN;
        }

[Rr][Ii][Gg][Hh][Tt]"$"  {
          return RIGHT_TOKEN;
        }

[Uu][Ss][Ii][Nn][Gg] {
          return USING_TOKEN;
        }        

[Ww][Ii][Dd][Tt][Hh] {
          return WIDTH_TOKEN;
        }        

[Cc][Hh][Rr]"$"  {
					return CHR_TOKEN;
				}

[Ee][Ll][Ss][Ee]  {
					return ELSE_TOKEN;
				}

[Gg][Oo][Tt][Oo]  {
					return GOTO_TOKEN;
				}

[Ll][Ee][Ff][Tt]"$" {
          return LEFT_TOKEN;
        }
 
[Ll][Ee][Nn] {
          return LEN_TOKEN;
        }

[Ll][Ii][Nn][Ee] {
          return LINE_TOKEN;
        }

[Mm][Ii][Dd]"$"  {
					return MID_TOKEN;
				}
        
[Nn][Ee][Xx][Tt]  {
					return NEXT_TOKEN;
				}

[Ss][Tt][Ee][Pp]  {
          return STEP_TOKEN;
        }

[Ss][Tt][Oo][Pp]  {
          return STOP_TOKEN;
        }

[Ss][Tt][Rr]"$"  {
          return STR_TOKEN;
        }

[Tt][Hh][Ee][Nn]  {
          return THEN_TOKEN;
        }

[Aa][Nn][Dd] {
          return AND_TOKEN;
        }

[Dd][Ee][Ff]  {
					return DEF_TOKEN;
				}

[Ee][Nn][Dd]  {
					return END_TOKEN;
				}

[Dd][Ii][Mm]  {
					return DIM_TOKEN;
				}

[Ff][Oo][Rr]  {
					return FOR_TOKEN;
				}

[Rr][Ee][Mm]  {
          return REM_TOKEN;
        }

[Rr][Uu][Nn]  {
          return RUN_TOKEN;
        }

[Ss][Qq][Rr]  {
          return SQR_TOKEN;
        }

[Ll][Ee][Tt]  {
					return LET_TOKEN;
				}

[Ii][Nn][Tt]  {
					return INT_TOKEN;
				}

[Tt][Aa][Bb]  {
					return TAB_TOKEN;
				}

[Ii][Ff] {
          return IF_TOKEN;
        }

[Tt][Oo]  {
          return TO_TOKEN;
        }

[Oo][Rr] {
          return OR_TOKEN;
        }

[Oo][Nn]  {
          return ON_TOKEN;
        }
        
\"[^\"\n]*\" {
      MBASIC_lval.sval = new std::string(yytext+1, strlen(yytext)-2);
      return QUOTED_STRING;
    }

[a-zA-Z][a-zA-Z0-9\.]*\$ {
      MBASIC_lval.sval = new std::string(yytext);
      return STRING_VARNAME;
    }

[a-zA-Z][a-zA-Z0-9\.]*\% {
      MBASIC_lval.sval = new std::string(yytext);
      return INTEGER_VARNAME;
    }

[a-zA-Z][a-zA-Z0-9\.]*\! {
      MBASIC_lval.sval = new std::string(yytext);
      return SINGLE_VARNAME;
    }

[a-zA-Z][a-zA-Z0-9\.]*\# {
      MBASIC_lval.sval = new std::string(yytext);
      return DOUBLE_VARNAME;
    }

[+-]*[0-9][0-9]*\.*[0-9]*[eE][+-][0-9][0-9]*  {
		  MBASIC_lval.sfloat = new SingleFloat(yytext);
      return SINGLE;
		}

[+-]*[0-9]*\.[0-9]*\! {
		  MBASIC_lval.sfloat = new SingleFloat(yytext);
      return SINGLE;
		}

[+-]*[0-9][0-9]*\!	{
		  MBASIC_lval.sfloat = new SingleFloat(yytext);
      return SINGLE;
		}

[+-]*[0-9][0-9]*\.*[0-9]*[dD][+-][0-9][0-9]*  {
      MBASIC_lval.dfloat = new DoubleFloat(yytext);
      return DOUBLE;
    }

[+-]*[0-9]*\.[0-9]*\# {
      MBASIC_lval.dfloat = new DoubleFloat(yytext);
      return DOUBLE;
		}

[+-]*[0-9][0-9]*\#	{
      MBASIC_lval.dfloat = new DoubleFloat(yytext);
      return DOUBLE;
		}

[+-]*[0-9][0-9]*\%	{
      MBASIC_lval.ival = atoi(yytext);
      return INTEGER;
		}

[+-]*[0-9]*\.[0-9]* {
		  MBASIC_lval.sfloat = new SingleFloat(yytext);
      return SINGLE;
		}
    
[+-]*[0-9][0-9]* {
      MBASIC_lval.sval = new std::string(yytext);
      return DEF_INTEGER;
		}

[a-zA-Z][a-zA-Z0-9\.]* {
      MBASIC_lval.sval = new std::string(yytext);
      return IDENTIFIER;
    }

[ \t]*  	{}

[\n]	{
			  g_lexLineNumber++;
			  return EOL_TOKEN;
			}

">="   { return GREATERTHANEQUAL; }

"<="   { return LESSTHANEQUAL; }

"<>"   { return NOT_EQUAL; }

<<EOF>> { return 0; } //EOF_TOKEN; }

":"   { return yytext[0]; }

";"   { return yytext[0]; }

"="   { return yytext[0]; }

"?"   { return yytext[0]; }

","   { return yytext[0]; }

"'"   { return yytext[0]; }

"+"   { return yytext[0]; }

"-"   { return yytext[0]; }

"*"   { return yytext[0]; }

"/"   { return yytext[0]; }

"("   { return yytext[0]; }

")"   { return yytext[0]; }

"<"   { return yytext[0]; }

">"   { return yytext[0]; }

"^"   { return yytext[0]; }

.		{
		}

%%

void MBASIC_SetRemMode()
{
  BEGIN(REM_MODE);
}
