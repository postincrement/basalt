%option prefix="MBASIC_"

%{

#include <iostream>

#include "basalt.h"
#include "mbasic.tab.hpp"

#define   YY_INPUT(buf,result,max_size)   MBASIC_yyinput(buf, &result, max_size)

/*
[_a-zA-Z0-9]+ {
		  MBASIC_lval.sval = new std::string(yytext);
		  return IDENTIFIER;
		}
*/    


%}

%option noyywrap

%x REM_MODE

%%

<REM_MODE>[^\n]* {
    BEGIN(INITIAL);
  }

[Cc][Ll][Ee][Aa][Rr]  {
					return CLEAR_TOKEN;
				}

[Dd][Ii][Mm]  {
					return DIM_TOKEN;
				}

[Ee][Nn][Dd]  {
					return END_TOKEN;
				}

[Ff][Oo][Rr]  {
					return FOR_TOKEN;
				}

[Gg][Oo][Ss][Uu][Bb]  {
					return GOSUB_TOKEN;
				}

[Gg][Oo][Tt][Oo]  {
					return GOTO_TOKEN;
				}

[Ll][Ee][Tt]  {
					return LET_TOKEN;
				}

[Nn][Ee][Xx][Tt]  {
					return NEXT_TOKEN;
				}

[Pp][Rr][Ii][Nn][Tt]  {
          return PRINT_TOKEN;
        }

[Rr][Ee][Mm]  {
          return REM_TOKEN;
        }

[Rr][Ee][Tt][Uu][Rr][Nn]  {
          return RETURN_TOKEN;
        }

[Ss][Tt][Ee][Pp]  {
          return STEP_TOKEN;
        }

[Tt][Oo]  {
          return TO_TOKEN;
        }

[Ww][Ii][Dd][Tt][Hh] {
          return WIDTH_TOKEN;
        }        
        
\"[^\"\n]*\" {
      MBASIC_lval.sval = new std::string(yytext+1, strlen(yytext)-2);
      return QUOTED_STRING;
    }

[+-]*[0-9][0-9]*\.*[0-9]*[eE][+-][0-9][0-9]*  {
		  MBASIC_lval.sfloat = new SingleFloat(yytext);
      return SINGLE;
		}

[+-]*[0-9][0-9]*\.*[0-9]*[dD][+-][0-9][0-9]*  {
      MBASIC_lval.dfloat = new DoubleFloat(yytext);
      return DOUBLE;
    }

[+-]*[0-9]*\.[0-9]* {
      MBASIC_lval.sfloat = new SingleFloat(yytext);
      return SINGLE;
		}

\-[0-9][0-9]*	{
                  MBASIC_lval.ival = atoi(yytext);
                  return NEGATIVE_INT;
		}

\+*[0-9][0-9]*	{
                  MBASIC_lval.ival = atoi(yytext);
                  return POSITIVE_INT;
		}

[a-zA-Z0-9]+\$ {
      MBASIC_lval.sval = new std::string(yytext);
      return STRING_VARNAME;
    }

[a-zA-Z0-9]+\% {
      MBASIC_lval.sval = new std::string(yytext);
      return INTEGER_VARNAME;
    }

[a-zA-Z0-9]+\! {
      MBASIC_lval.sval = new std::string(yytext);
      return SINGLE_VARNAME;
    }

[a-zA-Z0-9]+\# {
      MBASIC_lval.sval = new std::string(yytext);
      return DOUBLE_VARNAME;
    }

[a-zA-Z0-9]+ {
      MBASIC_lval.sval = new std::string(yytext);
      return INTEGER_VARNAME;
    }

		
[ \t]*  	{}

[\n]	 	{
			  g_lineNumber++;
			  return EOL_TOKEN;
			}

<<EOF>>     return EOF_TOKEN;      

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

.		{
		}

%%


void MBASIC_SetRemMode()
{
  BEGIN(REM_MODE);
}
