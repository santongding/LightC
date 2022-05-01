%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "tac.hpp"
#include "Checker.h"
#include "FlexLexer.h"
yyFlexLexer lex;
void yyerror(char* msg);
int yylex(){
return lex.yylex();
}

int user_lineno = -1;
int getyylineno() {
    if (user_lineno != -1)return user_lineno;
    return lex.lineno();
}

void setyylineno(int l) {
    user_lineno = l;
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
%type <sym> type_id identifier
%type <sym> var_type
%type <exp> expression_list expression
%type <tac> expression_statement return_statement member_declare_statement declare_statement if_statement while_statement statement_list statement block
%type <tac> function params param program class classes class_member class_members
/*
%type <tac>declaration_statement return_statement if_statement while_statement statement statement_list block
%type<tac> class_declarations class_declaration member_declarations member_declaration function parameter parameter_list
%type<tac> program*/
%%

program:classes{
	tac_last=$1;
	tac_complete();
}

classes:class| classes class{
	$$=join_tac($1,$2);
}

class:CLASS identifier '{' class_members '}'{
	$$=mk_class($2,$4);
}
class_members:class_member|class_members class_member{
	$$=join_tac($1,$2);
}
class_member:function|member_declare_statement;

function:var_type identifier '(' params ')' block{
	$$=mk_func($1,$2,$4,$6);
}
identifier:IDENTIFIER{
	typeManager.RecordIdentifier($1);
	$$=new SYM(SYM_SYMBOL,$1);
}

params:{
$$=NULL;
}
|param
|params ',' param{
	$$=join_tac($1,$3);
}
param:var_type identifier{
	$$=mk_tac(TAC_FORMAL,$1,$2,NULL,true);
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

type_id:identifier{

	$$=$1->Cast<SYM_TYPE>();
};
/*
| type_id ':' identifier{
	$$ =  new SYM(SYM_TYPE,$1->ToStr()+":"+$3);
}*/


var_type:type_id{
$$ = new SYM(SYM_TYPE,"ref|"+$1->ToStr());
}
| INT{
$$= new SYM(SYM_TYPE,"int|");
}

member_declare_statement:var_type identifier ';'{
	$$ = declare($1,$2);
}
|LINK var_type identifier ';'{
	$$ = declare_link($2,$3);
}
;

declare_statement:var_type identifier ';'{
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

return_statement:
/*RETURN ';'{
		TAC *t=mk_tac(TAC_RETURN, NULL, NULL, NULL,true);
        	$$=t;
}
|*/
RETURN expression ';'{
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
| identifier
{
	$$=mk_exp(NULL,$1,NULL);
}
| expression '.' identifier '(' expression_list ')'
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
	CheckTac(tac_first);
	tac_dump();

	return 0;
}
