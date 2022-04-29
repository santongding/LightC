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
%token <string> INTEGER IDENTIFIER TEXT

%left EQ NE LT LE GT GE
%left '+' '-'
%left '*' '/'
%right UMINUS

%type <string> TYPE
%type <string> REF
%type <tac> program declaration_list function_declaration function_declarations class_declaration function parameter_list variable_list statement declare_statement expression_statement return_statement null_statement if_statement while_statement block statement_list error
%type <exp> argument_list expression_list expression call_expression

%%

program : declaration_list
{
	tac_last=$1;
	tac_complete();
}
;

declaration_list : class_declaration | function_declaration
| declaration_list function_declaration
{
	$$=join_tac($1, $2);
}
| declaration_list class_declaration
{
	$$=join_tac($1, $2);
}
;

class_declaration: CLASS IDENTIFIER '{' function_declarations '}'{
	$$ = declare_class($2,$4);
};
function_declarations:function_declaration|
function_declarations function_declaration{
	$$ =join_tac($1, $2);
};

function_declaration : function
| declaration
;

REF : IDENTIFIER
| IDENTIFIER '.' IDENTIFIER{
	$$ = $1 +'.'+$3;
}

TYPE:REF;

declaration : TYPE variable_list ';'
{
	$$=$2;
}
;



variable_list : IDENTIFIER
{
	$$=declare_var($1);
}               
| variable_list ',' IDENTIFIER
{
	$$=join_tac($1, declare_var($3));
}               
;

function : IDENTIFIER '(' parameter_list ')' block
{

	$$=do_func(declare_func($1_, $3, $5);
	scope_local=0; /* Leave local scope. */
	sym_tab_local=NULL; /* Clear local symbol table. */
}
| error
{
	error("Bad function syntax");
	$$=NULL;
}
;
parameter : TYPE IDENTIFIER{
	$$=declare_para($2);

}


parameter_list : parameter
| parameter_list ',' parameter
{
	$$=join_tac($1, declare_para($3));
}               
|
{
	$$=NULL;
}
;

statement : expression_statement ';'
| delare_statement;
| return_statement ';'
| null_statement ';'
| if_statement
| while_statement
| block
;

block : '{' statement_list '}'
{
	$$=$2;
}               
;

statement_list : statement
| statement_list statement
{
	$$=join_tac($1, $2);
}               
;

expression_statement : expression
{
	$$=($1).tac;
}
;

expression : REF '=' expression{
	$$=do_assign(get_var($1), $3);
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
| '(' expression ')'
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
declare_statement: REF parameter_list
;
argument_list           :
{
	$$=NULL;
}
| expression_list
;

expression_list : expression
|  expression_list ',' expression
{
	$3->next=$1;
	$$=$3;
}
;


return_statement : RETURN expression
{
	TAC *t=mk_tac(TAC_RETURN, $2->ret, NULL, NULL);
	t->prev=$2->tac;
	$$=t;
}               
;

null_statement : CONTINUE
{
	$$=NULL;
}               
;

if_statement : IF '(' expression ')' block
{
	$$=do_if($3, $5);
}
| IF '(' expression ')' block ELSE block
{
	$$=do_test($3, $5, $7);
}
;

while_statement : WHILE '(' expression ')' block
{
	$$=do_while($3, $5);
}               
;

call_expression : REF '(' argument_list ')'
{
	$$=do_call_ret($1, $3);
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


	tac_init();

	yyparse();

	tac_print();

	return 0;
}
