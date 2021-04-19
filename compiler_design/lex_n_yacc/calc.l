%{
#include <stdio.h>                                                  //Standard input/output declarations
#include "y.tab.h"                                                  //File having list of tokens needed by grammar
int c;
%}

%%
" "       ;                                                         /*No action on whitespace*/
[a-z]     {
            /*
            yytext holds the text matched by following the regex specified on the LHS
            yytext[0] is the first character of the same. 
            This yytext is global and is accessible in t.tab.c as well as lex.yy.c.
            Similarly yylval is a global variable used to pass around the semantic value 
            of the currently matched token. The yylval is structure specified %union in calc.yacc.
            */
            c = yytext[0];
            yylval.a = c - 'a';
            return(LETTER);
          }
[0-9]     {
            c = yytext[0];
            yylval.a = c - '0';
            return(DIGIT);
          }
[^a-z0-9\b]    {
                 // The regex \b is for matching the beginning of a word.
                 c = yytext[0];
                 return(c);
               }
%%