%{
#include <string>
#include "../expression.h"
#include "expression.tab.h"
%}

%option outfile="lex.c" header-file="lex.h"


digit [0-9]
integer {digit}+
%%
"\n"  {return END;}
[[:space:]]+ /* Do nothing */;
{integer} {
    yylval.integer = new std::string(yytext);
       return INTEGER;
}
"w"    return LIMIT_ORDINAL;
"^"    return POW;
"*"    return MUL;
"+"    return ADD;
"("    return LEFT;
")"    return RIGHT;
"="    return EQUAL;
%%