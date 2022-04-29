#ifndef LIGHTC_TAC_HPP
#define LIGHTC_TAC_HPP

#include <string>
#include <cassert>

using std::string;
/* type of symbol */
enum SymbolType {
    SYM_UNKNOWN = 0,
    SYM_REF = 1,
    SYM_LINK = 2,
    SYM_VAR = 3,
    SYM_CONST = 4
};

/* type of tac */
#define TAC_UNDEF 0 /* undefined */
#define TAC_ADD 1 /* a=b+c */
#define TAC_SUB 2 /* a=b-c */
#define TAC_MUL 3 /* a=b*c */
#define TAC_DIV 4 /* a=b/c */
#define TAC_EQ 5 /* a=(b==c) */
#define TAC_NE 6 /* a=(b!=c) */
#define TAC_LT 7 /* a=(b<c) */
#define TAC_LE 8 /* a=(b<=c) */
#define TAC_GT 9 /* a=(b>c) */
#define TAC_GE 10 /* a=(b>=c) */
#define TAC_NEG 11 /* a=-b */
#define TAC_COPY 12 /* a=b */
#define TAC_GOTO 13 /* goto a */
#define TAC_IFZ 14 /* ifz b goto a */
#define TAC_BEGINFUNC 15 /* function begin */
#define TAC_ENDFUNC 16 /* function end */
#define TAC_LABEL 17 /* label a */
#define TAC_VAR 18 /* int a */
#define TAC_FORMAL 19 /* formal a */
#define TAC_ACTUAL 20 /* actual a */
#define TAC_CALL 21 /* a=call b */
#define TAC_RETURN 22 /* return a */
#define TAC_BEGINCLASS 23 /*class begin*/
#define TAC_ENDCLASS 24 /*class end*/
#define TAC_LOCATE 25

/* struct */
class SYM {
    /*
        SYM_VAR : value.value = 0x2333
        SYM_REF : value.name = ref var's name, ref cnt ++,
        SYM_LINK : value.name = ref var's name, ref cnt doesn't change
        SYM_UNKNOWN : will cast to other type when know its type
    */
public:
    SYM(SymbolType type, int value) : _type(type), _value(value) {
        assert(type == SymbolType::SYM_CONST);
    }


    SYM(SymbolType type, const string &name) : _type(type), _name(name), _value(0){
        assert(name.length());
        assert(type != SymbolType::SYM_CONST);
    }

    int &GetValue() {
        assert(IsConst());
        return _value;
    }

    bool IsConst() const {
        return _type == SYM_CONST;
    }


private:
    SymbolType _type;
    string _name;
    int _value;
};

typedef struct tac /* TAC instruction node */
{
    struct tac *next; /* Next instruction */
    struct tac *prev; /* Previous instruction */
    int op; /* TAC instruction */
    SYM *a;
    SYM *b;
    SYM *c;
} TAC;

typedef struct exp /* Parser expression */
{
    struct exp *next; /* For argument lists */
    TAC *tac; /* The code */
    SYM *ret; /* Where the result is */
} EXP;

/* global var */
extern int yylineno, scope_local, next_tmp, next_label;
extern SYM *sym_tab_global, *sym_tab_local;
extern TAC *tac_first, *tac_last;

/* function */
void tac_init();

void tac_complete();

void tac_dump();

void tac_print(TAC *i);

string mk_tmp();


SYM *mk_const(int n);

SYM *mk_text(const string &s);

TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c);

EXP *mk_exp(EXP *next, SYM *ret, TAC *code);

TAC *join_tac(TAC *c1, TAC *c2);

EXP *do_assign(SYM *var, EXP *exp);

EXP *do_un(int unop, EXP *exp);

EXP *do_bin(int binop, EXP *exp1, EXP *exp2);

EXP *do_cmp(int binop, EXP *exp1, EXP *exp2);

EXP *do_locate(EXP *x, EXP *y);

EXP *do_call_ret(SYM *sym, EXP *arglist);

void error(char *str) ;
#endif //LIGHTC_TAC_HPP