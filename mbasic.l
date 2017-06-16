%option prefix="MBASIC_"

%{

#include <iostream>

#include "mbasic.tab.hpp"
#include "basalt.h"

%}

%option noyywrap

%%

":"		{ return yytext[0]; }

"="   { return yytext[0]; }

[Ll][Ee][Tt]  {
					return LET_TOKEN;
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