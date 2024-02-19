%{
#include <string>
#include "../expression.h"

int yylex(void);
void yyerror(const char *);

expression::expression* result1 = nullptr;
expression::expression* result2 = nullptr;
%}

%union {
    expression::expression* e;
    std::string* integer;
}

%token <integer> INTEGER
%token <limit_ordinal> LIMIT_ORDINAL
%token POW MUL ADD
%token LEFT RIGHT
%token END
%token EQUAL

%left ADD
%left MUL
%right POW


%type <e> Input Expression

%start Input

%%
Input: Expression EQUAL Expression END { result1 = $1; result2 = $3; }
;
Expression:
     INTEGER { $$=new expression::number(*$1); delete $1; }
| LIMIT_ORDINAL { $$=new expression::limit_ordinal(); }
| Expression POW Expression { $$=new expression::pow($1, $3); }
| Expression MUL Expression { $$=new expression::multiply($1, $3); }
| Expression ADD Expression { $$=new expression::add($1, $3); }
| LEFT Expression RIGHT { $$=$2; }
;

%%