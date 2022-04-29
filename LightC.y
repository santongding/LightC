%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "tac.h"
#include "obj.h"

int yylex();
void yyerror(char* msg);

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
%token <string> INTEGER IDENTIFIER

%right '='
%left ','
%left EQ NE LT LE GT GE
%left '+' '-'
%left '*' '/'

%right UMINUS


%type <string> REF TYPE
%type <exp> expression_list expression call_expression
%type <tac> expression_statement declaration_statement return_statement if_statement while_statement statement statement_list block
%type<tac> class_declarations class_declaration member_declarations member_declaration function parameter parameter_list
%type<tac> program
%%

program: class_declarations

class_declarations:class_declaration
|class_declarations class_declaration{
	$$ = join_tac($1,$2);
}

class_declaration:CLASS IDENTIFIER '{' member_declarations '}'{
	$$ = make_class($2,$4);

}

member_declarations:member_declaration
|member_declarations member_declaration{
	$$ = join_tac($1,$2);
}

member_declaration:declaration_statement
|function;

parameter:TYPE IDENTIFIER{
	$$=make_param();
}

parameter_list:parameter|parameter_list ',' parameter{
	$$=join_tac($1,$3);
}

function : IDENTIFIER '(' parameter_list ')' block
{

	$$=do_func(declare_func($1_, $3, $5);
	scope_local=0; /* Leave local scope. */
	sym_tab_local=NULL; /* Clear local symbol table. */
}

block:'{' statement_list '}'{
 $$ = $2;
}| '{' '}'{
 $$ = NULL;
}

statement_list:statement| statement_list statement{
	$$ = join_tac($1,$2);
}

statement:declaration_statement|expression_statement|return_statement|if_statement|while_statement;

return_statement: RETURN expression_list ';'
{
TAC *t=mk_tac(TAC_RETURN, $2->ret, NULL, NULL);
t->prev=$2->tac;
$$=t;
}
;

if_statement : IF '(' expression_list ')' block
{
$$=do_if($3, $5);
}
| IF '(' expression_list ')' block ELSE block
{
$$=do_test($3, $5, $7);
}
;

while_statement : WHILE '(' expression_list ')' block
{
$$=do_while($3, $5);
}
;


declaration_statement: TYPE IDENTIFIER ';'{

}

expression_statement : expression_list ';'
{
	$$=($1).tac;
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
	$$=do_assign($1, $3);
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
| REF
{
	$$=mk_exp(NULL, get_var($1), NULL);
}
| call_expression
{
	$$=$1;
}
| error
{
	error("Bad expression syntax");
	$$=mk_exp(NULL, NULL, NULL);
}
;


call_expression : REF '(' expression_list ')'
{
	$$=do_call_ret($1, $3);
}
;

REF : IDENTIFIER
| IDENTIFIER '.' IDENTIFIER{
	$$ = $1 +'.'+$3;
}
TYPE: REF|LINK REF{
	TYPE = "<LINK>"+$2
}

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


	tac_init();

	yyparse();

	tac_print();

	return 0;
}
