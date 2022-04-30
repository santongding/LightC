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
int getyylineno(){
	return lex.lineno();
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

%token INT EQ NE LT LE GT GE UMINUS IF THEN ELSE FI WHILE DO DONE CONTINUE FUNC PRINT RETURN CLASS LINK VOID
%token <string> INTEGER IDENTIFIER TEXT

%right '='
%left ','
%left EQ NE LT LE GT GE
%left '+' '-'
%left '*' '/'
%right UMINUS
%nonassoc '(' ')'
%left '.'
%type <sym> type_id
%type <sym> type
%type <exp> expression_list expression
%type <tac> expression_statement return_statement declare_statement if_statement while_statement statement_list statement block
%type <tac> function params param program
/*
%type <tac>declaration_statement return_statement if_statement while_statement statement statement_list block
%type<tac> class_declarations class_declaration member_declarations member_declaration function parameter parameter_list
%type<tac> program*/
%%

program:function{
	tac_last=$1;
	tac_complete();
}

function:type IDENTIFIER '(' params ')' block{
	$$=mk_func($1,$2,$4,$6);
}

params:param
|params ',' param{
	$$=join_tac($1,$3);
}
param:type IDENTIFIER{
	$$=mk_tac(TAC_FORMAL,$1,new SYM(SYM_SYMBOL,$2),NULL,true);
}

block:'{' statement_list '}'{
	$$=mk_block($2);
}
| '{' '}'{
	$$=NULL;
}
statement_list:statement
| statement_list statement{

	$$ = join_tac($1,$2);

}

statement:expression_statement
|return_statement
|if_statement;
|while_statement;
|declare_statement;

type_id:IDENTIFIER{
	$$=new SYM(SYM_TYPE,$1);
}
| type_id ':' IDENTIFIER{
	$$ =  new SYM(SYM_TYPE,$1->ToStr()+":"+$3);
}

type:type_id{
	$$ = new SYM(SYM_TYPE,"ref|"+$1->ToStr());
}
| INT{
 	$$= new SYM(SYM_TYPE,"int|");
}
| VOID {
 	$$= new SYM(SYM_TYPE,"void|");
}
| LINK type_id{
	$$ = new SYM(SYM_TYPE,"link|"+$2->ToStr());
}

declare_statement:type IDENTIFIER ';'{
	$$ = declare($1,$2);
};


while_statement : WHILE '(' expression ')' block
{
	$$=do_while($3, $5);
}
;

if_statement : IF '(' expression ')' block
{
	$$=do_test($3, $5,NULL);
}
| IF '(' expression ')' block ELSE block
{
	$$=do_test($3, $5, $7);
}
;

return_statement:RETURN ';'{
		TAC *t=mk_tac(TAC_RETURN, NULL, NULL, NULL,true);
        	$$=t;
}
| RETURN expression ';'{
		TAC *t=mk_tac(TAC_RETURN, $2->ret, NULL, NULL,true);
        	t->prev=$2->tac;
        	$$=t;
}

expression_statement : expression_list ';'
{
	auto tp =do_exp_list($1);
	if(tp)
	$$ = do_exp_list($1)->tac;
	else $$ = NULL;
}
;

expression_list : {
	$$=NULL;
}
| expression
|  expression_list ',' expression
{
	$$=join_exp($1,$3);
}
;

expression : expression '=' expression {
	$$=do_assign($1, $3);
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
	$$=do_exp_list($2);
}
| INTEGER
{
	$$=mk_exp(NULL, mk_const(atoi($1)), NULL);
}
| IDENTIFIER
{
	$$=mk_exp(NULL,new SYM(SYM_SYMBOL,$1),NULL);
}
| expression '.' IDENTIFIER '(' expression_list ')'
{
	$$=do_call_ret($1,$3,$5);
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
	fprintf(stderr, "%s: line %d\n", msg, getyylineno());
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


	tac_init();

	yyparse();
	tac_dump();

	return 0;
}
