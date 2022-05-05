%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "tac.hpp"
#include "Checker.h"
#include "object.h"
#include "FlexLexer.h"
yyFlexLexer lex;
void yyerror(char* msg);
int yylex(){
int ans=lex.yylex();
setlineno(lex.lineno());
return ans;
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

%token INT EQ NE LT LE GT GE UMINUS IF THEN ELSE FI WHILE DO DONE CONTINUE FUNC PRINT RETURN CLASS LINK NEW
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
|CLASS identifier '{' '}'{
	$$=mk_class($2,NULL);
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
param:identifier ':' var_type{
	$$=mk_tac(TAC_FORMAL,$3,$1,NULL,true);
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

member_declare_statement: identifier ':' var_type';'{
	$$ = declare($3,$1);
}
| identifier ':' LINK var_type';'{
	$$ = declare_link($4,$1);
}
;

declare_statement:identifier ':' var_type';'{
	$$ = declare($3,$1);
}|identifier ':' NEW var_type';'{
	$$ = declare_new($4,$1);
}
;


while_statement : WHILE '(' expression ')' block
{
	$$=do_while(flush_exp($3), $5);
}
;

if_statement : IF '(' expression ')' block
{
	$$=do_test(flush_exp($3), $5,NULL);
}
| IF '(' expression ')' block ELSE block
{
	$$=do_test(flush_exp($3), $5, $7);
}
;

return_statement:
/*RETURN ';'{
		TAC *t=mk_tac(TAC_RETURN, NULL, NULL, NULL,true);
        	$$=t;
}
|*/
RETURN expression ';'{
		auto exp = flush_exp($2);
		TAC *t=mk_tac(TAC_RETURN, exp->ret, NULL, NULL,true);
        	t->prev=exp->tac;
        	$$=t;
}

expression_statement : expression_list ';'
{
	auto tp =do_exp_list($1,false);
	if(tp)
	$$ = tp->tac;
	else $$ = NULL;
}
;

expression_list : {
	$$=NULL;
}
| expression{
	$$=$1;
}
|  expression_list ',' expression
{
	$$=join_exp($1,$3);
}
;

expression : identifier '=' expression {
	$$=do_assign(mk_exp(NULL,$1,NULL),flush_exp($3));
}
|expression '.' identifier '=' expression {
 	$$=do_assign(do_locate(flush_exp($1),$3),flush_exp($5));
 }
| expression '.' identifier{
	$$=do_locate(flush_exp($1),$3);
}
| expression '+' expression
{
	$$=do_bin(TAC_ADD, flush_exp($1), flush_exp($3));
}
| expression '-' expression
{
	$$=do_bin(TAC_SUB, flush_exp($1), flush_exp($3));
}
| expression '*' expression
{
	$$=do_bin(TAC_MUL, flush_exp($1), flush_exp($3));
}
| expression '/' expression
{
	$$=do_bin(TAC_DIV, flush_exp($1), flush_exp($3));
}
| '-' expression  %prec UMINUS
{
	$$=do_un(TAC_NEG, flush_exp($2));
}
| expression EQ expression
{
	$$=do_cmp(TAC_EQ, flush_exp($1), flush_exp($3));
}
| expression NE expression
{
	$$=do_cmp(TAC_NE, flush_exp($1), flush_exp($3));
}
| expression LT expression
{
	$$=do_cmp(TAC_LT, flush_exp($1), ($3));
}
| expression LE expression
{
	$$=do_cmp(TAC_LE, flush_exp($1), flush_exp($3));
}
| expression GT expression
{
	$$=do_cmp(TAC_GT, flush_exp($1), flush_exp($3));
}
| expression GE expression
{
	$$=do_cmp(TAC_GE, flush_exp($1), flush_exp($3));
}
| '(' expression_list ')'
{
	$$=do_exp_list($2,false);
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
	$$=do_call_ret(flush_exp($1),$3,$5);
}
| error
{
	CheckStatus(BAD_SYNTAX);
	$$=mk_exp(NULL, NULL, NULL);
}
;


%%

void yyerror(char* msg)
{
	CheckStatus(BAD_SYNTAX);
}


int main(int argc,   char *argv[])
{
	if(argc != 3)
	{
		printf("usage: %s filename outputpath\n", argv[0]);
		exit(0);
	}

	char *input;

	input = argv[1];
	if(freopen(input, "r", stdin)==NULL)
	{
		printf("error: open %s failed\n", input);
		return 0;
	}

	string output =argv[2];
	if(freopen(output.c_str(), "w", stdout)==NULL)
	{
		printf("error: open %s failed\n", output.c_str());
		return 0;
	}


	tac_init();

	yyparse();
	tac_dump();
	CheckTac(tac_first);
	//tac_dump();
	tac_object_generate(tac_first);
	return 0;
}
