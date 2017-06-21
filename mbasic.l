%option prefix="MBASIC_"

%{

#include <iostream>

#include "basalt.h"
#include "mbasic.tab.hpp"

%}

%option noyywrap

%%

":"		{ return yytext[0]; }

";"   { return yytext[0]; }

"="   { return yytext[0]; }

[Ll][Ee][Tt]  {
					return LET_TOKEN;
				}

[Pp][Rr][Ii][Nn][Tt]  {
          return PRINT_TOKEN;
        }

        
\"[^\"\n]*\" {
      MBASIC_lval.sval = new std::string(yytext+1, strlen(yytext)-2);
      return QUOTED_STRING;
    }

[+-]*[0-9][0-9]*\.*[0-9]*[eE][+-][0-9][0-9]*  {
		  MBASIC_lval.fval = atof(yytext);
      return DOUBLE;
		}

[+-]*[0-9]*\.[0-9]* {
		  MBASIC_lval.fval = atof(yytext);
      return DOUBLE;
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

[_a-zA-Z0-9]+ {
		  MBASIC_lval.sval = new std::string(yytext);
		  return IDENTIFIER;
		}
		
[ \t]*  	{}

[\n]	 	{
			  g_lineNumber++;
			  return EOL;
			}

.		{
		}

%%