%option prefix="DBC_"

%{

#include <iostream>

#include "dbc.hpp"
#include "dbc.tab.hpp"

int g_dbcLexMode = eLexMode_Normal;

%}

%option noyywrap

%x MUXSTATE
%x QUOTEDSTRING

%%

"\""		{
		  if (g_dbcLexMode == eLexMode_AllowQuote)
		    return '"';
		  BEGIN(QUOTEDSTRING);
		}

<QUOTEDSTRING>[^\"]*\" {
                  BEGIN(INITIAL);
		  DBC_lval.sval = new std::string(yytext, strlen(yytext)-1);
		  return QUOTED_STRING;
		}

":"		{ return yytext[0]; }

";"		{ return yytext[0]; }

"|"		{ return yytext[0]; }

"("		{ return yytext[0]; }

")"		{ return yytext[0]; }

"["		{ return yytext[0]; }

"]"		{ return yytext[0]; }

"+"		{ return yytext[0]; }

"-"		{ return yytext[0]; }

"@"		{ return yytext[0]; }

","		{ return yytext[0]; }

"VERSION"	{ return TOKEN_VERSION; }

"ENUM"		{ return TOKEN_ENUM; }

"FLOAT"		{ return TOKEN_FLOAT; }

"HEX"		{ return TOKEN_HEX; }

"INT"		{ return TOKEN_INT; }

"STRING"	{ return TOKEN_STRING; }

"NS_"		{ return TOKEN_NS_; }

"BS_"		{ return TOKEN_BS_; }

"BO_"		{ return TOKEN_BO_; }

"BU_"		{ return TOKEN_BU_; }

"SG_"		{ return TOKEN_SG_; }

"EV_"		{ return TOKEN_EV_; }

"CM_"		{ return TOKEN_CM_; }

"VAL_"		{ return TOKEN_VAL_; }

"BA_DEF_DEF_"	{ return TOKEN_BA_DEF_DEF_; }

"BA_DEF_"	{ return TOKEN_BA_DEF_; }

"NS_DESC_"          { return TOKEN_NS_DESC_; }
"CAT_DEF_"          { return TOKEN_CAT_DEF_; }
"CAT_"              { return TOKEN_CAT_; }
"FILTER"            { return TOKEN_FILTER; }
"EV_DATA_"          { return TOKEN_EV_DATA_; }
"ENVVAR_DATA_"      { return TOKEN_ENVVAR_DATA_; }
"SGTYPE_"           { return TOKEN_SGTYPE_; }
"SGTYPE_VAL_"       { return TOKEN_SGTYPE_VAL_; }
"BA_DEF_SGTYPE_"    { return TOKEN_BA_DEF_SGTYPE_; }
"BA_SGTYPE_"        { return TOKEN_BA_SGTYPE_; }
"SIG_TYPE_REF_"     { return TOKEN_SIG_TYPE_REF_; }
"VAL_TABLE_"        { return TOKEN_VAL_TABLE_; }
"SIG_GROUP_"        { return TOKEN_SIG_GROUP_; }
"SIG_VALTYPE_"      { return TOKEN_SIG_VALTYPE_; }
"SIGTYPE_VALTYPE_"  { return TOKEN_SIGTYPE_VALTYPE_; }
"BO_TX_BU_"         { return TOKEN_BO_TX_BU_; }
"BA_REL_"           { return TOKEN_BA_REL_; }
"BA_DEF_REL_"       { return TOKEN_BA_DEF_REL_; }
"BA_DEF_DEF_REL_"   { return TOKEN_BA_DEF_DEF_REL_; }
"BU_SG_REL_"        { return TOKEN_BU_SG_REL_; }
"BU_EV_REL_"        { return TOKEN_BU_EV_REL_; }
"BU_BO_REL_"        { return TOKEN_BU_BO_REL_; }
"SG_MUL_VAL_"       { return TOKEN_SG_MUL_VAL_; }

"BA_"		{ return TOKEN_BA_; }


"M"		{
		  if (g_dbcLexMode == eLexMode_MuxIndicator)
                    return 'M';
                }

"m"		{
		  if (g_dbcLexMode == eLexMode_MuxIndicator)
                    BEGIN(MUXSTATE);
                }

<MUXSTATE>[0-9][0-9]* {
		  DBC_lval.ival = atoi(yytext);
                  BEGIN(INITIAL);
                  return MUX_SWITCH;
		}
		    


[+-]*[0-9][0-9]*\.*[0-9]*[eE][+-][0-9][0-9]*  {
		  DBC_lval.fval = atof(yytext);
                  return DOUBLE;
		}

[+-]*[0-9]*\.[0-9]* {
		  DBC_lval.fval = atof(yytext);
                  return DOUBLE;
		}

\-[0-9][0-9]*	{
                  DBC_lval.ival = atoi(yytext);
                  return NEGATIVE_INT;
		}

\+*[0-9][0-9]*	{
                  DBC_lval.ival = atoi(yytext);
                  return POSITIVE_INT;
		}

[_a-zA-Z0-9]+ {
		  DBC_lval.sval = new std::string(yytext);
		  return IDENTIFIER;
		}
		
[ \t]*  	{}

[\n]	 	{
		  g_lineNumber++;
		}

.		{
		}

%%