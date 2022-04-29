%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "tac.hpp"
#include "FlexLexer.h"
yyFlexLexer lex;
void yyerror(char* msg);
int yylex(){
return lex.yylex();
}

%}

%union
{
	char character;
	char *string;
	SYM *sym;
	TAC *tac;
	EXP *exp;
}

%token INT EQ NE LT LE GT GE UMINUS IF THEN ELSE FI WHILE DO DONE CONTINUE FUNC PRINT RETURN CLASS LINK
%token <string> INTEGER IDENTIFIER TEXT

%right '='
%left ','
%left EQ NE LT LE GT GE
%left '+' '-'
%left '*' '/'
%right UMINUS
%nonassoc '(' ')'
%left '.'


%type <exp> expression_list expression
%type <tac> expression_statement
/*
%type <tac>declaration_statement return_statement if_statement while_statement statement statement_list block
%type<tac> class_declarations class_declaration member_declarations member_declaration function parameter parameter_list
%type<tac> program*/
%%


expression_statement : expression_list ';'
{
	$$=($1)->tac;
}
;

expression_list : expression
|  expression_list ',' expression
{
	$3->next=$1;
	$$=$3;
}
;

expression : expression '=' expression {
	$$=do_assign($1->ret, $3);
}
| expression '.' expression{
	$$=do_locate($1,$3);
}
| expression '+' expression
{
	$$=do_bin(TAC_ADD, $1, $3);
}
| expression '-' expression
{
	$$=do_bin(TAC_SUB, $1, $3);
}
| expression '*' expression
{
	$$=do_bin(TAC_MUL, $1, $3);
}
| expression '/' expression
{
	$$=do_bin(TAC_DIV, $1, $3);
}
| '-' expression  %prec UMINUS
{
	$$=do_un(TAC_NEG, $2);
}
| expression EQ expression
{
	$$=do_cmp(TAC_EQ, $1, $3);
}
| expression NE expression
{
	$$=do_cmp(TAC_NE, $1, $3);
}
| expression LT expression
{
	$$=do_cmp(TAC_LT, $1, $3);
}
| expression LE expression
{
	$$=do_cmp(TAC_LE, $1, $3);
}
| expression GT expression
{
	$$=do_cmp(TAC_GT, $1, $3);
}
| expression GE expression
{
	$$=do_cmp(TAC_GE, $1, $3);
}
| '(' expression_list ')'
{
	$$=$2;
}
| INTEGER
{
	$$=mk_exp(NULL, mk_const(atoi($1)), NULL);
}
| IDENTIFIER
{
	$$=mk_exp(NULL,new SYM(SYM_LINK,$1),NULL);
}
| expression '(' expression_list ')'
{
$$=do_call_ret($1->ret, $3);
}
| error
{
	error("Bad expression syntax");
	$$=mk_exp(NULL, NULL, NULL);
}
;


%%

void yyerror(char* msg)
{
	fprintf(stderr, "%s: line %d\n", msg, yylineno);
	exit(0);
}


int main(int argc,   char *argv[])
{
	if(argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
		exit(0);
	}

	char *input, *output;

	input = argv[1];
	if(freopen(input, "r", stdin)==NULL)
	{
		printf("error: open %s failed\n", input);
		return 0;
	}

	output=(char *)malloc(strlen(input + 10));
	strcpy(output,input);
	strcat(output,".s");

	if(freopen(output, "w", stdout)==NULL)
	{
		printf("error: open %s failed\n", output);
		return 0;
	}


	//tac_init();

	yyparse();

	//tac_print(tac_last);

	return 0;
}
