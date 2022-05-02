#ifndef LIGHTC_TAC_HPP
#define LIGHTC_TAC_HPP

#include <string>
#include <cassert>
#include "def.h"

using std::string;
/* type of symbol */
enum SymbolType {
    SYM_UNKNOWN = 0,
    SYM_REF = 1,//value format: name = "x->y"
    SYM_CONST = 2,//value format: value = 233
    SYM_TYPE = 3,//value format: int| / ref|xxx / link|xxx
    SYM_LABEL = 4,//value format: name = "l2"
    SYM_SYMBOL = 5//value format: name = "adsdas"

};


/* type of tac */
enum TAC_TYPE {

    TAC_UNDEF = 0, /* undefined */
    TAC_ADD = 1, /* a=b+c */
    TAC_SUB = 2,/* a=b-c */
    TAC_MUL = 3,/* a=b*c */
    TAC_DIV = 4,/* a=b/c */
    TAC_EQ = 5,/* a=(b==c) */
    TAC_NE = 6,/* a=(b!=c) */
    TAC_LT = 7,/* a=(b<c) */
    TAC_LE = 8,/* a=(b<=c) */
    TAC_GT = 9,/* a=(b>c) */
    TAC_GE = 10,/* a=(b>=c) */
    TAC_NEG = 11,/* a=-b */
    TAC_COPY = 12,/* a=b */
    TAC_GOTO = 13,/* goto a */
    TAC_IFZ = 14,/* ifz b goto a */
    TAC_BEGINFUNC = 15,/* function begin */
    TAC_ENDFUNC = 16,/* function end */
    TAC_LABEL = 17,/* label a */
    TAC_DECLARE = 18,/* int a */
    TAC_FORMAL = 19,/* formal a */
    TAC_ACTUAL = 20,/* actual a */
    TAC_CALL = 21,/* a=call b */
    TAC_RETURN = 22,/* return a */
    TAC_BEGINCLASS = 23,/*class begin*/
    TAC_ENDCLASS = 24,/*class end*/
    TAC_LOCATE = 25,
    TAC_BEGINBLOCK = 26,
    TAC_ENDBLOCK = 27,
    TAC_NEW = 28,
    TAC_BIND = 29,
};

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


    SYM(SymbolType type, const string &name) : _type(type), _name(name), _value(0) {
        assert(name.length());
        assert(type != SymbolType::SYM_CONST);
    }

    const string &ToStr() {
        if (IsConst()) {
            _name = std::to_string(_value);
        }
        return _name;
    }

    template<SymbolType T>
    SYM *Cast() {
        if (T == SYM_CONST) {
            assert(!IsConst());
        }
        _type = T;
    }

    void SetStr(const string &s) {
        assert(_type != SYM_CONST);
        _name = s;
    }

    int &GetValue() {
        assert(IsConst());
        return _value;
    }

    bool IsConst() const {
        return _type == SYM_CONST;
    }

    template<SymbolType T>
    bool Is() const {
        return T == _type;
    }


private:
    SymbolType _type;
    string _name;
    int _value;
};

typedef struct tac /* TAC instruction node */
{
    int linenum;
    struct tac *next; /* Next instruction */
    struct tac *prev; /* Previous instruction */
    TAC_TYPE op; /* TAC instruction */
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
extern int next_tmp, next_label;
extern TAC *tac_first, *tac_last;

int getyylineno();

void setyylineno(int l);

/* function */
void tac_init();

void tac_complete();

void tac_dump();

void tac_print(TAC *i);

string mk_tmp();


SYM *mk_const(int n);

TAC *mk_tac(TAC_TYPE op, SYM *a, SYM *b, SYM *c, bool lineno);

EXP *mk_exp(EXP *next, SYM *ret, TAC *code);

TAC *join_tac(TAC *c1, TAC *c2);

EXP *do_assign(EXP *var, EXP *exp);

EXP *do_un(TAC_TYPE unop, EXP *exp);

EXP *do_bin(TAC_TYPE binop, EXP *exp1, EXP *exp2);

EXP *do_cmp(TAC_TYPE binop, EXP *exp1, EXP *exp2);

EXP *do_locate(EXP *x, SYM *y);

EXP *do_call_ret(EXP *obj, SYM *func, EXP *arglist);

EXP *do_exp_list(EXP *exps);

EXP *join_exp(EXP *x, EXP *y);

EXP *flush_exp(EXP *x);

TAC *mk_block(TAC *x);

void error(const char *str, int code);

void error(const string &str, int code);

TAC *do_test(EXP *exp, TAC *stmt1, TAC *stmt2);

TAC *do_while(EXP *exp, TAC *stmt);

TAC *declare(SYM *type, SYM *name);

TAC *declare_link(SYM *type, SYM *name);

TAC *declare_new(SYM *type, SYM *name);

TAC *mk_func(SYM *type, SYM *name, TAC *tac, TAC *block);

TAC *mk_class(SYM *name, TAC *tac);


inline void CheckStatus(STATUS s) {
    if (s != OK) {
        error(Status2Str(s), s);
    }
}

#endif //LIGHTC_TAC_HPP