/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "LightC.y"

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



#line 91 "bison.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "bison.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_EQ = 4,                         /* EQ  */
  YYSYMBOL_NE = 5,                         /* NE  */
  YYSYMBOL_LT = 6,                         /* LT  */
  YYSYMBOL_LE = 7,                         /* LE  */
  YYSYMBOL_GT = 8,                         /* GT  */
  YYSYMBOL_GE = 9,                         /* GE  */
  YYSYMBOL_UMINUS = 10,                    /* UMINUS  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_THEN = 12,                      /* THEN  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_FI = 14,                        /* FI  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_DO = 16,                        /* DO  */
  YYSYMBOL_DONE = 17,                      /* DONE  */
  YYSYMBOL_CONTINUE = 18,                  /* CONTINUE  */
  YYSYMBOL_FUNC = 19,                      /* FUNC  */
  YYSYMBOL_PRINT = 20,                     /* PRINT  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_CLASS = 22,                     /* CLASS  */
  YYSYMBOL_LINK = 23,                      /* LINK  */
  YYSYMBOL_NEW = 24,                       /* NEW  */
  YYSYMBOL_INTEGER = 25,                   /* INTEGER  */
  YYSYMBOL_IDENTIFIER = 26,                /* IDENTIFIER  */
  YYSYMBOL_TEXT = 27,                      /* TEXT  */
  YYSYMBOL_28_ = 28,                       /* '='  */
  YYSYMBOL_29_ = 29,                       /* ','  */
  YYSYMBOL_30_ = 30,                       /* '+'  */
  YYSYMBOL_31_ = 31,                       /* '-'  */
  YYSYMBOL_32_ = 32,                       /* '*'  */
  YYSYMBOL_33_ = 33,                       /* '/'  */
  YYSYMBOL_34_ = 34,                       /* '('  */
  YYSYMBOL_35_ = 35,                       /* ')'  */
  YYSYMBOL_36_ = 36,                       /* '.'  */
  YYSYMBOL_37_ = 37,                       /* '{'  */
  YYSYMBOL_38_ = 38,                       /* '}'  */
  YYSYMBOL_39_ = 39,                       /* ':'  */
  YYSYMBOL_40_ = 40,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_program = 42,                   /* program  */
  YYSYMBOL_classes = 43,                   /* classes  */
  YYSYMBOL_class = 44,                     /* class  */
  YYSYMBOL_class_members = 45,             /* class_members  */
  YYSYMBOL_class_member = 46,              /* class_member  */
  YYSYMBOL_function = 47,                  /* function  */
  YYSYMBOL_identifier = 48,                /* identifier  */
  YYSYMBOL_params = 49,                    /* params  */
  YYSYMBOL_param = 50,                     /* param  */
  YYSYMBOL_block = 51,                     /* block  */
  YYSYMBOL_statement_list = 52,            /* statement_list  */
  YYSYMBOL_statement = 53,                 /* statement  */
  YYSYMBOL_type_id = 54,                   /* type_id  */
  YYSYMBOL_var_type = 55,                  /* var_type  */
  YYSYMBOL_member_declare_statement = 56,  /* member_declare_statement  */
  YYSYMBOL_declare_statement = 57,         /* declare_statement  */
  YYSYMBOL_while_statement = 58,           /* while_statement  */
  YYSYMBOL_if_statement = 59,              /* if_statement  */
  YYSYMBOL_return_statement = 60,          /* return_statement  */
  YYSYMBOL_expression_statement = 61,      /* expression_statement  */
  YYSYMBOL_expression_list = 62,           /* expression_list  */
  YYSYMBOL_expression = 63                 /* expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   211

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  114

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    32,    30,    29,    31,    36,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    40,
       2,    28,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    52,    52,    57,    57,    61,    64,    68,    68,    72,
      72,    74,    77,    82,    85,    86,    89,    93,    96,    99,
     100,   106,   107,   108,   109,   110,   112,   122,   125,   129,
     132,   137,   139,   145,   151,   155,   167,   174,   183,   186,
     189,   195,   198,   201,   204,   208,   212,   216,   220,   224,
     228,   232,   236,   240,   244,   248,   252,   256,   260,   264
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "EQ", "NE",
  "LT", "LE", "GT", "GE", "UMINUS", "IF", "THEN", "ELSE", "FI", "WHILE",
  "DO", "DONE", "CONTINUE", "FUNC", "PRINT", "RETURN", "CLASS", "LINK",
  "NEW", "INTEGER", "IDENTIFIER", "TEXT", "'='", "','", "'+'", "'-'",
  "'*'", "'/'", "'('", "')'", "'.'", "'{'", "'}'", "':'", "';'", "$accept",
  "program", "classes", "class", "class_members", "class_member",
  "function", "identifier", "params", "param", "block", "statement_list",
  "statement", "type_id", "var_type", "member_declare_statement",
  "declare_statement", "while_statement", "if_statement",
  "return_statement", "expression_statement", "expression_list",
  "expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-83)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-39)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -15,   -24,    24,   -15,   -83,   -83,   -32,   -83,   -83,    12,
     -83,   -83,    14,   -83,   -83,   -30,   -83,   -24,   -83,   -83,
     -83,    20,    -7,     9,   -83,   -26,   -24,    -6,   -83,    10,
      42,   -83,   -83,     9,   -24,    33,   -83,   -83,    67,   -83,
     -83,    38,    39,     5,   -83,     5,   110,   -83,   -18,    88,
     -83,   -83,   -83,   -83,   -83,   -83,    40,   175,     5,     5,
      48,   125,    47,    46,     5,    41,   -83,   -83,     5,   -83,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     -24,   142,   162,   -83,   -83,   175,     9,    34,   175,   155,
     155,   155,   155,   155,   155,    91,    91,    47,    47,    56,
      33,    33,    54,   -83,     5,   110,    73,   -83,   -83,   175,
      62,    33,   -83,   -83
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     3,    12,     0,     1,     4,     0,
      28,     6,     0,     7,     9,    26,    27,     0,    10,     5,
       8,     0,     0,     0,    26,     0,    13,     0,    29,     0,
       0,    14,    30,     0,     0,     0,    16,    15,     0,    11,
      59,     0,     0,     0,    56,     0,     0,    18,    57,     0,
      19,    25,    24,    23,    22,    21,     0,    39,     0,     0,
      57,     0,    48,     0,     0,     0,    17,    20,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    36,    55,    41,     0,     0,    40,    49,
      50,    51,    52,    53,    54,    44,    45,    46,    47,    43,
       0,     0,     0,    31,     0,     0,    34,    33,    32,    42,
       0,     0,    58,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -83,   -83,   -83,    92,   -83,    90,   -83,    -1,   -83,    66,
     -82,   -83,    55,   -83,   -20,   -83,   -83,   -83,   -83,   -83,
     -83,   -42,   -17
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     4,    12,    13,    14,    60,    30,    31,
      39,    49,    50,    16,    17,    18,    51,    52,    53,    54,
      55,    56,    57
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       6,    25,     5,    27,    63,     9,    40,     1,    15,    21,
      64,    15,    10,    36,    28,    10,    22,    10,   106,   107,
      24,    65,    24,    10,     7,    29,    61,    26,    62,   113,
      44,     5,    24,    29,    32,     5,    45,    48,     5,    46,
       5,    81,    82,    23,    10,    87,     5,    85,    48,    33,
      11,    88,    19,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   110,    24,    86,   102,     5,    40,    68,
      38,    34,    58,    59,   103,    68,    64,    35,    41,    99,
      69,    84,    42,    80,   104,    24,   111,   109,    43,    40,
     105,    68,    44,     5,   108,     8,   -38,   112,    45,    41,
      37,    46,    20,    42,    67,    47,     0,   -38,     0,    43,
       0,    40,     0,    44,     5,     0,     0,   -38,     0,    45,
       0,     0,    46,    78,    79,     0,    66,    80,   -38,    70,
      71,    72,    73,    74,    75,    44,     5,     0,     0,   -38,
       0,    45,     0,     0,    46,   -38,    70,    71,    72,    73,
      74,    75,     0,     0,     0,    76,    77,    78,    79,     0,
       0,    80,     0,     0,     0,    83,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,     0,   100,    80,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,     0,
       0,    80,    76,    77,    78,    79,     0,   101,    80,     0,
       0,     0,     0,     0,     0,    76,    77,    78,    79,     0,
       0,    80
};

static const yytype_int8 yycheck[] =
{
       1,    21,    26,    23,    46,    37,     1,    22,     9,    39,
      28,    12,     3,    33,    40,     3,    17,     3,   100,   101,
      21,    39,    23,     3,     0,    26,    43,    34,    45,   111,
      25,    26,    33,    34,    40,    26,    31,    38,    26,    34,
      26,    58,    59,    23,     3,    65,    26,    64,    49,    39,
      38,    68,    38,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,   105,    65,    24,    86,    26,     1,    29,
      37,    29,    34,    34,    40,    29,    28,    35,    11,    80,
      40,    35,    15,    36,    28,    86,    13,   104,    21,     1,
      34,    29,    25,    26,    40,     3,    29,    35,    31,    11,
      34,    34,    12,    15,    49,    38,    -1,    40,    -1,    21,
      -1,     1,    -1,    25,    26,    -1,    -1,    29,    -1,    31,
      -1,    -1,    34,    32,    33,    -1,    38,    36,    40,     4,
       5,     6,     7,     8,     9,    25,    26,    -1,    -1,    29,
      -1,    31,    -1,    -1,    34,    35,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    30,    31,    32,    33,    -1,
      -1,    36,    -1,    -1,    -1,    40,     4,     5,     6,     7,
       8,     9,    30,    31,    32,    33,    -1,    35,    36,     4,
       5,     6,     7,     8,     9,    30,    31,    32,    33,    -1,
      -1,    36,    30,    31,    32,    33,    -1,    35,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    -1,
      -1,    36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    22,    42,    43,    44,    26,    48,     0,    44,    37,
       3,    38,    45,    46,    47,    48,    54,    55,    56,    38,
      46,    39,    48,    23,    48,    55,    34,    55,    40,    48,
      49,    50,    40,    39,    29,    35,    55,    50,    37,    51,
       1,    11,    15,    21,    25,    31,    34,    38,    48,    52,
      53,    57,    58,    59,    60,    61,    62,    63,    34,    34,
      48,    63,    63,    62,    28,    39,    38,    53,    29,    40,
       4,     5,     6,     7,     8,     9,    30,    31,    32,    33,
      36,    63,    63,    40,    35,    63,    24,    55,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    48,
      35,    35,    55,    40,    28,    34,    51,    51,    40,    63,
      62,    13,    35,    51
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    45,    45,    46,
      46,    47,    48,    49,    49,    49,    50,    51,    51,    52,
      52,    53,    53,    53,    53,    53,    54,    55,    55,    56,
      56,    57,    57,    58,    59,    59,    60,    61,    62,    62,
      62,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     5,     4,     1,     2,     1,
       1,     6,     1,     0,     1,     3,     3,     3,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       5,     4,     5,     5,     5,     7,     3,     2,     0,     1,
       3,     3,     5,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     6,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: classes  */
#line 52 "LightC.y"
               {
	tac_last=(yyvsp[0].tac);
	tac_complete();
}
#line 1236 "bison.cpp"
    break;

  case 4: /* classes: classes class  */
#line 57 "LightC.y"
                            {
	(yyval.tac)=join_tac((yyvsp[-1].tac),(yyvsp[0].tac));
}
#line 1244 "bison.cpp"
    break;

  case 5: /* class: CLASS identifier '{' class_members '}'  */
#line 61 "LightC.y"
                                            {
	(yyval.tac)=mk_class((yyvsp[-3].sym),(yyvsp[-1].tac));
}
#line 1252 "bison.cpp"
    break;

  case 6: /* class: CLASS identifier '{' '}'  */
#line 64 "LightC.y"
                         {
	(yyval.tac)=mk_class((yyvsp[-2].sym),NULL);
}
#line 1260 "bison.cpp"
    break;

  case 8: /* class_members: class_members class_member  */
#line 68 "LightC.y"
                                                     {
	(yyval.tac)=join_tac((yyvsp[-1].tac),(yyvsp[0].tac));
}
#line 1268 "bison.cpp"
    break;

  case 11: /* function: var_type identifier '(' params ')' block  */
#line 74 "LightC.y"
                                                 {
	(yyval.tac)=mk_func((yyvsp[-5].sym),(yyvsp[-4].sym),(yyvsp[-2].tac),(yyvsp[0].tac));
}
#line 1276 "bison.cpp"
    break;

  case 12: /* identifier: IDENTIFIER  */
#line 77 "LightC.y"
                     {
	typeManager.RecordIdentifier((yyvsp[0].string));
	(yyval.sym)=new SYM(SYM_SYMBOL,(yyvsp[0].string));
}
#line 1285 "bison.cpp"
    break;

  case 13: /* params: %empty  */
#line 82 "LightC.y"
       {
(yyval.tac)=NULL;
}
#line 1293 "bison.cpp"
    break;

  case 15: /* params: params ',' param  */
#line 86 "LightC.y"
                 {
	(yyval.tac)=join_tac((yyvsp[-2].tac),(yyvsp[0].tac));
}
#line 1301 "bison.cpp"
    break;

  case 16: /* param: identifier ':' var_type  */
#line 89 "LightC.y"
                             {
	(yyval.tac)=mk_tac(TAC_FORMAL,(yyvsp[0].sym),(yyvsp[-2].sym),NULL,true);
}
#line 1309 "bison.cpp"
    break;

  case 17: /* block: '{' statement_list '}'  */
#line 93 "LightC.y"
                            {
	(yyval.tac)=mk_block((yyvsp[-1].tac));
}
#line 1317 "bison.cpp"
    break;

  case 18: /* block: '{' '}'  */
#line 96 "LightC.y"
         {
	(yyval.tac)=NULL;
}
#line 1325 "bison.cpp"
    break;

  case 20: /* statement_list: statement_list statement  */
#line 100 "LightC.y"
                          {

	(yyval.tac) = join_tac((yyvsp[-1].tac),(yyvsp[0].tac));

}
#line 1335 "bison.cpp"
    break;

  case 26: /* type_id: identifier  */
#line 112 "LightC.y"
                  {

	(yyval.sym)=(yyvsp[0].sym)->Cast<SYM_TYPE>();
}
#line 1344 "bison.cpp"
    break;

  case 27: /* var_type: type_id  */
#line 122 "LightC.y"
                {
(yyval.sym) = new SYM(SYM_TYPE,"ref|"+(yyvsp[0].sym)->ToStr());
}
#line 1352 "bison.cpp"
    break;

  case 28: /* var_type: INT  */
#line 125 "LightC.y"
     {
(yyval.sym)= new SYM(SYM_TYPE,"int|");
}
#line 1360 "bison.cpp"
    break;

  case 29: /* member_declare_statement: identifier ':' var_type ';'  */
#line 129 "LightC.y"
                                                    {
	(yyval.tac) = declare((yyvsp[-1].sym),(yyvsp[-3].sym));
}
#line 1368 "bison.cpp"
    break;

  case 30: /* member_declare_statement: identifier ':' LINK var_type ';'  */
#line 132 "LightC.y"
                                 {
	(yyval.tac) = declare_link((yyvsp[-1].sym),(yyvsp[-4].sym));
}
#line 1376 "bison.cpp"
    break;

  case 31: /* declare_statement: identifier ':' var_type ';'  */
#line 137 "LightC.y"
                                            {
	(yyval.tac) = declare((yyvsp[-1].sym),(yyvsp[-3].sym));
}
#line 1384 "bison.cpp"
    break;

  case 32: /* declare_statement: identifier ':' NEW var_type ';'  */
#line 139 "LightC.y"
                                {
	(yyval.tac) = declare_new((yyvsp[-1].sym),(yyvsp[-4].sym));
}
#line 1392 "bison.cpp"
    break;

  case 33: /* while_statement: WHILE '(' expression ')' block  */
#line 146 "LightC.y"
{
	(yyval.tac)=do_while(flush_exp((yyvsp[-2].exp)), (yyvsp[0].tac));
}
#line 1400 "bison.cpp"
    break;

  case 34: /* if_statement: IF '(' expression ')' block  */
#line 152 "LightC.y"
{
	(yyval.tac)=do_test(flush_exp((yyvsp[-2].exp)), (yyvsp[0].tac),NULL);
}
#line 1408 "bison.cpp"
    break;

  case 35: /* if_statement: IF '(' expression ')' block ELSE block  */
#line 156 "LightC.y"
{
	(yyval.tac)=do_test(flush_exp((yyvsp[-4].exp)), (yyvsp[-2].tac), (yyvsp[0].tac));
}
#line 1416 "bison.cpp"
    break;

  case 36: /* return_statement: RETURN expression ';'  */
#line 167 "LightC.y"
                     {
		auto exp = flush_exp((yyvsp[-1].exp));
		TAC *t=mk_tac(TAC_RETURN, exp->ret, NULL, NULL,true);
        	t->prev=exp->tac;
        	(yyval.tac)=t;
}
#line 1427 "bison.cpp"
    break;

  case 37: /* expression_statement: expression_list ';'  */
#line 175 "LightC.y"
{
	auto tp =do_exp_list((yyvsp[-1].exp),false);
	if(tp)
	(yyval.tac) = tp->tac;
	else (yyval.tac) = NULL;
}
#line 1438 "bison.cpp"
    break;

  case 38: /* expression_list: %empty  */
#line 183 "LightC.y"
                  {
	(yyval.exp)=NULL;
}
#line 1446 "bison.cpp"
    break;

  case 39: /* expression_list: expression  */
#line 186 "LightC.y"
            {
	(yyval.exp)=(yyvsp[0].exp);
}
#line 1454 "bison.cpp"
    break;

  case 40: /* expression_list: expression_list ',' expression  */
#line 190 "LightC.y"
{
	(yyval.exp)=join_exp((yyvsp[-2].exp),(yyvsp[0].exp));
}
#line 1462 "bison.cpp"
    break;

  case 41: /* expression: identifier '=' expression  */
#line 195 "LightC.y"
                                       {
	(yyval.exp)=do_assign(mk_exp(NULL,(yyvsp[-2].sym),NULL),flush_exp((yyvsp[0].exp)));
}
#line 1470 "bison.cpp"
    break;

  case 42: /* expression: expression '.' identifier '=' expression  */
#line 198 "LightC.y"
                                          {
 	(yyval.exp)=do_assign(do_locate(flush_exp((yyvsp[-4].exp)),(yyvsp[-2].sym)),flush_exp((yyvsp[0].exp)));
 }
#line 1478 "bison.cpp"
    break;

  case 43: /* expression: expression '.' identifier  */
#line 201 "LightC.y"
                           {
	(yyval.exp)=do_locate(flush_exp((yyvsp[-2].exp)),(yyvsp[0].sym));
}
#line 1486 "bison.cpp"
    break;

  case 44: /* expression: expression '+' expression  */
#line 205 "LightC.y"
{
	(yyval.exp)=do_bin(TAC_ADD, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1494 "bison.cpp"
    break;

  case 45: /* expression: expression '-' expression  */
#line 209 "LightC.y"
{
	(yyval.exp)=do_bin(TAC_SUB, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1502 "bison.cpp"
    break;

  case 46: /* expression: expression '*' expression  */
#line 213 "LightC.y"
{
	(yyval.exp)=do_bin(TAC_MUL, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1510 "bison.cpp"
    break;

  case 47: /* expression: expression '/' expression  */
#line 217 "LightC.y"
{
	(yyval.exp)=do_bin(TAC_DIV, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1518 "bison.cpp"
    break;

  case 48: /* expression: '-' expression  */
#line 221 "LightC.y"
{
	(yyval.exp)=do_un(TAC_NEG, flush_exp((yyvsp[0].exp)));
}
#line 1526 "bison.cpp"
    break;

  case 49: /* expression: expression EQ expression  */
#line 225 "LightC.y"
{
	(yyval.exp)=do_cmp(TAC_EQ, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1534 "bison.cpp"
    break;

  case 50: /* expression: expression NE expression  */
#line 229 "LightC.y"
{
	(yyval.exp)=do_cmp(TAC_NE, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1542 "bison.cpp"
    break;

  case 51: /* expression: expression LT expression  */
#line 233 "LightC.y"
{
	(yyval.exp)=do_cmp(TAC_LT, flush_exp((yyvsp[-2].exp)), ((yyvsp[0].exp)));
}
#line 1550 "bison.cpp"
    break;

  case 52: /* expression: expression LE expression  */
#line 237 "LightC.y"
{
	(yyval.exp)=do_cmp(TAC_LE, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1558 "bison.cpp"
    break;

  case 53: /* expression: expression GT expression  */
#line 241 "LightC.y"
{
	(yyval.exp)=do_cmp(TAC_GT, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1566 "bison.cpp"
    break;

  case 54: /* expression: expression GE expression  */
#line 245 "LightC.y"
{
	(yyval.exp)=do_cmp(TAC_GE, flush_exp((yyvsp[-2].exp)), flush_exp((yyvsp[0].exp)));
}
#line 1574 "bison.cpp"
    break;

  case 55: /* expression: '(' expression_list ')'  */
#line 249 "LightC.y"
{
	(yyval.exp)=do_exp_list((yyvsp[-1].exp),false);
}
#line 1582 "bison.cpp"
    break;

  case 56: /* expression: INTEGER  */
#line 253 "LightC.y"
{
	(yyval.exp)=mk_exp(NULL, mk_const(atoi((yyvsp[0].string))), NULL);
}
#line 1590 "bison.cpp"
    break;

  case 57: /* expression: identifier  */
#line 257 "LightC.y"
{
	(yyval.exp)=mk_exp(NULL,(yyvsp[0].sym),NULL);
}
#line 1598 "bison.cpp"
    break;

  case 58: /* expression: expression '.' identifier '(' expression_list ')'  */
#line 261 "LightC.y"
{
	(yyval.exp)=do_call_ret(flush_exp((yyvsp[-5].exp)),(yyvsp[-3].sym),(yyvsp[-1].exp));
}
#line 1606 "bison.cpp"
    break;

  case 59: /* expression: error  */
#line 265 "LightC.y"
{
	CheckStatus(BAD_SYNTAX);
	(yyval.exp)=mk_exp(NULL, NULL, NULL);
}
#line 1615 "bison.cpp"
    break;


#line 1619 "bison.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 272 "LightC.y"


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
