Starting parse
Entering state 0
Reducing stack by rule 1 (line 190):
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token REM_TOKEN ()
Shifting token REM_TOKEN ()
Entering state 22
Reducing stack by rule 87 (line 609):
   $1 = token REM_TOKEN ()
-> $$ = nterm rem_keyword ()
Stack now 0 1 5 7
Entering state 50
Reducing stack by rule 86 (line 603):
   $1 = nterm rem_keyword ()
-> $$ = nterm REM_statement ()
Stack now 0 1 5 7
Entering state 49
Reducing stack by rule 26 (line 317):
   $1 = nterm REM_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
line 4: warning 1003 - 'averylongvername2!' & 'averylongvername!'
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ',' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token ',' ()
Shifting token ',' ()
Entering state 233
Reducing stack by rule 84 (line 587):
   $1 = token ',' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Reading a token: Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ';' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token ';' ()
Shifting token ';' ()
Entering state 234
Reducing stack by rule 85 (line 591):
   $1 = token ';' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Reading a token: Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token INTEGER_VARNAME ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token INTEGER_VARNAME ()
Shifting token INTEGER_VARNAME ()
Entering state 60
Reducing stack by rule 140 (line 902):
   $1 = token INTEGER_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
assign cast required
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token INTEGER_VARNAME ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token INTEGER_VARNAME ()
Shifting token INTEGER_VARNAME ()
Entering state 60
Reducing stack by rule 140 (line 902):
   $1 = token INTEGER_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token DOUBLE ()
Shifting token DOUBLE ()
Entering state 64
Reducing stack by rule 152 (line 974):
   $1 = token DOUBLE ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
assign cast required
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token STRING_VARNAME ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token STRING_VARNAME ()
Shifting token STRING_VARNAME ()
Entering state 102
Reducing stack by rule 145 (line 928):
   $1 = token STRING_VARNAME ()
-> $$ = nterm string_var_name ()
Stack now 0 1 5 7 45
Entering state 106
Reading a token: Next token is token '=' ()
Reducing stack by rule 143 (line 919):
   $1 = nterm string_var_name ()
-> $$ = nterm string_var_ref ()
Stack now 0 1 5 7 45
Entering state 105
Next token is token '=' ()
Shifting token '=' ()
Entering state 140
Reading a token: Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 45 105 140
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 45 105 140
Entering state 187
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 45 105 140
Entering state 184
Reducing stack by rule 69 (line 516):
   $1 = nterm string_var_ref ()
   $2 = token '=' ()
   $3 = nterm string_expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token SINGLE_VARNAME ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token SINGLE_VARNAME ()
Shifting token SINGLE_VARNAME ()
Entering state 61
Reducing stack by rule 141 (line 907):
   $1 = token SINGLE_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token SINGLE ()
Shifting token SINGLE ()
Entering state 65
Reducing stack by rule 153 (line 979):
   $1 = token SINGLE ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token DOUBLE_VARNAME ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token DOUBLE_VARNAME ()
Shifting token DOUBLE_VARNAME ()
Entering state 62
Reducing stack by rule 142 (line 912):
   $1 = token DOUBLE_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token SINGLE ()
Shifting token SINGLE ()
Entering state 65
Reducing stack by rule 153 (line 979):
   $1 = token SINGLE ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
assign cast required
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token INTEGER_VARNAME ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token INTEGER_VARNAME ()
Shifting token INTEGER_VARNAME ()
Entering state 60
Reducing stack by rule 140 (line 902):
   $1 = token INTEGER_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token STRING_VARNAME ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token STRING_VARNAME ()
Shifting token STRING_VARNAME ()
Entering state 102
Reducing stack by rule 145 (line 928):
   $1 = token STRING_VARNAME ()
-> $$ = nterm string_var_name ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 106
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 143 (line 919):
   $1 = nterm string_var_name ()
-> $$ = nterm string_var_ref ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 188
Reducing stack by rule 130 (line 851):
   $1 = nterm string_var_ref ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Next token is token EOL_TOKEN ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token SINGLE_VARNAME ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token SINGLE_VARNAME ()
Shifting token SINGLE_VARNAME ()
Entering state 61
Reducing stack by rule 141 (line 907):
   $1 = token SINGLE_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token DOUBLE_VARNAME ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token DOUBLE_VARNAME ()
Shifting token DOUBLE_VARNAME ()
Entering state 62
Reducing stack by rule 142 (line 912):
   $1 = token DOUBLE_VARNAME ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 187
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 5 (line 221):
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Next token is token EOL_TOKEN ()
Reducing stack by rule 9 (line 244):
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139
Entering state 79
Reading a token: Next token is token '+' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Next token is token '+' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token '+' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token '+' ()
Shifting token '+' ()
Entering state 124
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 75
Reading a token: Next token is token ':' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 74
Next token is token ':' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 73
Next token is token ':' ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 158
Reducing stack by rule 106 (line 720):
   $1 = nterm MULT_expression ()
   $2 = token '+' ()
   $3 = nterm ADD_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token ':' ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token ':' ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token ':' ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token ':' ()
Shifting token ':' ()
Entering state 101
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7 31 101
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 31 101 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 31 101 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 31 101 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ';' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token ';' ()
Shifting token ';' ()
Entering state 234
Reducing stack by rule 85 (line 591):
   $1 = token ';' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7 31 101
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7 31 101
Entering state 138
Reducing stack by rule 12 (line 258):
   $1 = nterm statement_list ()
   $2 = token ':' ()
   $3 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139
Entering state 79
Reading a token: Next token is token '-' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Next token is token '-' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token '-' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token '-' ()
Shifting token '-' ()
Entering state 123
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 75
Reading a token: Next token is token ':' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 74
Next token is token ':' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 73
Next token is token ':' ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139 73 123
Entering state 157
Reducing stack by rule 107 (line 726):
   $1 = nterm MULT_expression ()
   $2 = token '-' ()
   $3 = nterm ADD_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token ':' ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token ':' ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token ':' ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token ':' ()
Shifting token ':' ()
Entering state 101
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7 31 101
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 31 101 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 31 101 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 31 101 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ';' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token ';' ()
Shifting token ';' ()
Entering state 234
Reducing stack by rule 85 (line 591):
   $1 = token ';' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7 31 101
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7 31 101
Entering state 138
Reducing stack by rule 12 (line 258):
   $1 = nterm statement_list ()
   $2 = token ':' ()
   $3 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139
Entering state 79
Reading a token: Next token is token '*' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Next token is token '*' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token '*' ()
Shifting token '*' ()
Entering state 125
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 75
Reading a token: Next token is token ':' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 74
Next token is token ':' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 159
Reducing stack by rule 109 (line 738):
   $1 = nterm NEGATE_expression ()
   $2 = token '*' ()
   $3 = nterm MULT_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token ':' ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token ':' ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token ':' ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token ':' ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token ':' ()
Shifting token ':' ()
Entering state 101
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7 31 101
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 31 101 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 31 101 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 31 101 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ';' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token ';' ()
Shifting token ';' ()
Entering state 234
Reducing stack by rule 85 (line 591):
   $1 = token ';' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7 31 101
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7 31 101
Entering state 138
Reducing stack by rule 12 (line 258):
   $1 = nterm statement_list ()
   $2 = token ':' ()
   $3 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139
Entering state 79
Reading a token: Next token is token '/' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Next token is token '/' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token '/' ()
Shifting token '/' ()
Entering state 126
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139 74 126
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 74 126
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 74 126
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 74 126
Entering state 75
Reading a token: Next token is token ':' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 74 126
Entering state 74
Next token is token ':' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 74 126
Entering state 160
Reducing stack by rule 110 (line 744):
   $1 = nterm NEGATE_expression ()
   $2 = token '/' ()
   $3 = nterm MULT_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token ':' ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token ':' ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token ':' ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token ':' ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token ':' ()
Shifting token ':' ()
Entering state 101
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7 31 101
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 31 101 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 31 101 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 31 101 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ';' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token ';' ()
Shifting token ';' ()
Entering state 234
Reducing stack by rule 85 (line 591):
   $1 = token ';' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7 31 101
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7 31 101
Entering state 138
Reducing stack by rule 12 (line 258):
   $1 = nterm statement_list ()
   $2 = token ':' ()
   $3 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token '-' ()
Shifting token '-' ()
Entering state 67
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139 67
Entering state 79
Reading a token: Next token is token ':' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139 67
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 67
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 67
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 67
Entering state 114
Next token is token ':' ()
Reducing stack by rule 112 (line 756):
   $1 = token '-' ()
   $2 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token ':' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token ':' ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token ':' ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token ':' ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token ':' ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token ':' ()
Shifting token ':' ()
Entering state 101
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7 31 101
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 31 101 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 31 101 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 31 101 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 187
Reading a token: Next token is token ';' ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token ';' ()
Shifting token ';' ()
Entering state 234
Reducing stack by rule 85 (line 591):
   $1 = token ';' ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7 31 101
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7 31 101
Entering state 138
Reducing stack by rule 12 (line 258):
   $1 = nterm statement_list ()
   $2 = token ':' ()
   $3 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 3
Reducing stack by rule 7 (line 230):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm line_number ()
Stack now 0 1
Entering state 6
Reducing stack by rule 6 (line 224):
   $1 = nterm line_number ()
-> $$ = nterm opt_line_number ()
Stack now 0 1
Entering state 5
Reducing stack by rule 3 (line 201):
-> $$ = nterm @1 ()
Stack now 0 1 5
Entering state 7
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 66 (line 504):
-> $$ = nterm opt_LET_TOKEN ()
Stack now 0 1 5 7
Entering state 45
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45
Entering state 79
Reading a token: Next token is token '=' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45
Entering state 104
Next token is token '=' ()
Shifting token '=' ()
Entering state 139
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139
Entering state 79
Reading a token: Next token is token '*' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 75
Next token is token '*' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 74
Next token is token '*' ()
Shifting token '*' ()
Entering state 125
Reading a token: Next token is token DEF_NUMERIC ()
Shifting token DEF_NUMERIC ()
Entering state 59
Reducing stack by rule 151 (line 969):
   $1 = token DEF_NUMERIC ()
-> $$ = nterm numeric_const_expr ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 80
Reducing stack by rule 122 (line 803):
   $1 = nterm numeric_const_expr ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 75
Reading a token: Next token is token '+' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 74
Next token is token '+' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 74 125
Entering state 159
Reducing stack by rule 109 (line 738):
   $1 = nterm NEGATE_expression ()
   $2 = token '*' ()
   $3 = nterm MULT_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 73
Next token is token '+' ()
Shifting token '+' ()
Entering state 124
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 79
Reading a token: Next token is token '+' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 75
Next token is token '+' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 74
Next token is token '+' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 73
Next token is token '+' ()
Shifting token '+' ()
Entering state 124
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 79
Reading a token: Next token is token '/' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 75
Next token is token '/' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 74
Next token is token '/' ()
Shifting token '/' ()
Entering state 126
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 79
Reading a token: Next token is token '*' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 75
Next token is token '*' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 74
Next token is token '*' ()
Shifting token '*' ()
Entering state 125
Reading a token: Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 79
Reading a token: Next token is token ':' ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 75
Next token is token ':' ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 74
Next token is token ':' ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126 74 125
Entering state 159
Reducing stack by rule 109 (line 738):
   $1 = nterm NEGATE_expression ()
   $2 = token '*' ()
   $3 = nterm MULT_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124 74 126
Entering state 160
Reducing stack by rule 110 (line 744):
   $1 = nterm NEGATE_expression ()
   $2 = token '/' ()
   $3 = nterm MULT_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 73
Next token is token ':' ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139 73 124 73 124
Entering state 158
Reducing stack by rule 106 (line 720):
   $1 = nterm MULT_expression ()
   $2 = token '+' ()
   $3 = nterm ADD_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139 73 124
Entering state 158
Reducing stack by rule 106 (line 720):
   $1 = nterm MULT_expression ()
   $2 = token '+' ()
   $3 = nterm ADD_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 72
Next token is token ':' ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 70
Next token is token ':' ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 45 104 139
Entering state 69
Next token is token ':' ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 45 104 139
Entering state 176
Reducing stack by rule 68 (line 508):
   $1 = nterm numeric_var_ref ()
   $2 = token '=' ()
   $3 = nterm expression ()
-> $$ = nterm assign_expression ()
Stack now 0 1 5 7 45
Entering state 103
Reducing stack by rule 65 (line 497):
   $1 = nterm opt_LET_TOKEN ()
   $2 = nterm assign_expression ()
-> $$ = nterm LET_statement ()
Stack now 0 1 5 7
Entering state 44
Reducing stack by rule 22 (line 301):
   $1 = nterm LET_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7
Entering state 32
Reducing stack by rule 11 (line 253):
   $1 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token ':' ()
Shifting token ':' ()
Entering state 101
Reading a token: Next token is token PRINT_TOKEN ()
Shifting token PRINT_TOKEN ()
Entering state 21
Reducing stack by rule 76 (line 556):
   $1 = token PRINT_TOKEN ()
-> $$ = nterm print_keyword ()
Stack now 0 1 5 7 31 101
Entering state 48
Reducing stack by rule 73 (line 545):
-> $$ = nterm $@2 ()
Stack now 0 1 5 7 31 101 48
Entering state 107
Reading a token: Next token is token QUOTED_STRING ()
Reducing stack by rule 78 (line 562):
-> $$ = nterm opt_USING ()
Stack now 0 1 5 7 31 101 48 107
Entering state 143
Reducing stack by rule 74 (line 548):
-> $$ = nterm $@3 ()
Stack now 0 1 5 7 31 101 48 107 143
Entering state 191
Reducing stack by rule 80 (line 567):
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token QUOTED_STRING ()
Shifting token QUOTED_STRING ()
Entering state 182
Reducing stack by rule 154 (line 986):
   $1 = token QUOTED_STRING ()
-> $$ = nterm string_const_expr ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 189
Reducing stack by rule 136 (line 876):
   $1 = nterm string_const_expr ()
-> $$ = nterm string_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 187
Reading a token: Next token is token IDENTIFIER ()
Reducing stack by rule 123 (line 815):
   $1 = nterm string_value ()
-> $$ = nterm string_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 237
Reducing stack by rule 83 (line 583):
   $1 = nterm string_expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token IDENTIFIER ()
Shifting token IDENTIFIER ()
Entering state 58
Reducing stack by rule 139 (line 897):
   $1 = token IDENTIFIER ()
-> $$ = nterm numeric_var_name ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 79
Reading a token: Next token is token EOL_TOKEN ()
Reducing stack by rule 137 (line 888):
   $1 = nterm numeric_var_name ()
-> $$ = nterm numeric_var_ref ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 78
Reducing stack by rule 118 (line 786):
   $1 = nterm numeric_var_ref ()
-> $$ = nterm numeric_value ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 77
Reducing stack by rule 117 (line 780):
   $1 = nterm numeric_value ()
-> $$ = nterm subexpression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 76
Reducing stack by rule 115 (line 770):
   $1 = nterm subexpression ()
-> $$ = nterm POWER_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 75
Next token is token EOL_TOKEN ()
Reducing stack by rule 113 (line 760):
   $1 = nterm POWER_expression ()
-> $$ = nterm NEGATE_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 74
Next token is token EOL_TOKEN ()
Reducing stack by rule 111 (line 750):
   $1 = nterm NEGATE_expression ()
-> $$ = nterm MULT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 73
Next token is token EOL_TOKEN ()
Reducing stack by rule 108 (line 732):
   $1 = nterm MULT_expression ()
-> $$ = nterm ADD_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 72
Next token is token EOL_TOKEN ()
Reducing stack by rule 105 (line 714):
   $1 = nterm ADD_expression ()
-> $$ = nterm compare_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 71
Reducing stack by rule 98 (line 684):
   $1 = nterm compare_expression ()
-> $$ = nterm NOT_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 70
Next token is token EOL_TOKEN ()
Reducing stack by rule 96 (line 674):
   $1 = nterm NOT_expression ()
-> $$ = nterm AND_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 69
Next token is token EOL_TOKEN ()
Reducing stack by rule 94 (line 664):
   $1 = nterm AND_expression ()
-> $$ = nterm expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 236
Reducing stack by rule 82 (line 579):
   $1 = nterm expression ()
-> $$ = nterm print_expression ()
Stack now 0 1 5 7 31 101 48 107 143 191 216
Entering state 235
Reducing stack by rule 81 (line 570):
   $1 = nterm print_expression_list ()
   $2 = nterm print_expression ()
-> $$ = nterm print_expression_list ()
Stack now 0 1 5 7 31 101 48 107 143 191
Entering state 216
Next token is token EOL_TOKEN ()
Reducing stack by rule 75 (line 544):
   $1 = nterm print_keyword ()
   $2 = nterm $@2 ()
   $3 = nterm opt_USING ()
   $4 = nterm $@3 ()
   $5 = nterm print_expression_list ()
-> $$ = nterm PRINT_statement ()
Stack now 0 1 5 7 31 101
Entering state 47
Reducing stack by rule 25 (line 313):
   $1 = nterm PRINT_statement ()
-> $$ = nterm statement ()
Stack now 0 1 5 7 31 101
Entering state 138
Reducing stack by rule 12 (line 258):
   $1 = nterm statement_list ()
   $2 = token ':' ()
   $3 = nterm statement ()
-> $$ = nterm statement_list ()
Stack now 0 1 5 7
Entering state 31
Next token is token EOL_TOKEN ()
Reducing stack by rule 10 (line 247):
   $1 = nterm statement_list ()
-> $$ = nterm opt_statement_list ()
Stack now 0 1 5 7
Entering state 30
Next token is token EOL_TOKEN ()
Shifting token EOL_TOKEN ()
Entering state 98
Reducing stack by rule 33 (line 347):
   $1 = token EOL_TOKEN ()
-> $$ = nterm eol ()
Stack now 0 1 5 7 30
Entering state 100
Reducing stack by rule 4 (line 200):
   $1 = nterm opt_line_number ()
   $2 = nterm @1 ()
   $3 = nterm opt_statement_list ()
   $4 = nterm eol ()
-> $$ = nterm line ()
Stack now 0 1
Entering state 4
Reducing stack by rule 2 (line 191):
   $1 = nterm file ()
   $2 = nterm line ()
-> $$ = nterm file ()
Stack now 0
Entering state 1
Reading a token: Now at end of input.
Shifting token $end ()
Entering state 2
Stack now 0 1 2
Cleanup: popping token $end ()
Cleanup: popping nterm file ()
line 13: warning 1002 - 
line 3: warning 1002 - av!
line 11: warning 1002 - b%
program has 30 elements
