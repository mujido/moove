/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tINT = 258,
     tOBJNUM = 259,
     tSTR = 260,
     tID = 261,
     tREAL = 262,
     tIF = 263,
     tELSEIF = 264,
     tELSE = 265,
     tENDIF = 266,
     tWHILE = 267,
     tENDWHILE = 268,
     tFOR = 269,
     tENDFOR = 270,
     tFORK = 271,
     tENDFORK = 272,
     tTRY = 273,
     tEXCEPT = 274,
     tFINALLY = 275,
     tENDTRY = 276,
     tSWITCH = 277,
     tCASE = 278,
     tDEFAULT = 279,
     tENDSWITCH = 280,
     tRETURN = 281,
     tCONTINUE = 282,
     tBREAK = 283,
     tANY = 284,
     tTO = 285,
     tARROW = 286,
     tIN = 287,
     tEXP_EQ = 288,
     tMOD_EQ = 289,
     tDIV_EQ = 290,
     tMUL_EQ = 291,
     tSUB_EQ = 292,
     tADD_EQ = 293,
     tAND = 294,
     tOR = 295,
     tGE = 296,
     tLE = 297,
     tNE = 298,
     tEQ = 299,
     tDECREMENT = 300,
     tINCREMENT = 301,
     tNEGATE = 302
   };
#endif
/* Tokens.  */
#define tINT 258
#define tOBJNUM 259
#define tSTR 260
#define tID 261
#define tREAL 262
#define tIF 263
#define tELSEIF 264
#define tELSE 265
#define tENDIF 266
#define tWHILE 267
#define tENDWHILE 268
#define tFOR 269
#define tENDFOR 270
#define tFORK 271
#define tENDFORK 272
#define tTRY 273
#define tEXCEPT 274
#define tFINALLY 275
#define tENDTRY 276
#define tSWITCH 277
#define tCASE 278
#define tDEFAULT 279
#define tENDSWITCH 280
#define tRETURN 281
#define tCONTINUE 282
#define tBREAK 283
#define tANY 284
#define tTO 285
#define tARROW 286
#define tIN 287
#define tEXP_EQ 288
#define tMOD_EQ 289
#define tDIV_EQ 290
#define tMUL_EQ 291
#define tSUB_EQ 292
#define tADD_EQ 293
#define tAND 294
#define tOR 295
#define tGE 296
#define tLE 297
#define tNE 298
#define tEQ 299
#define tDECREMENT 300
#define tINCREMENT 301
#define tNEGATE 302




/* Copy the first part of user declarations.  */
#line 1 "moove.y"


#include "expr_ast.hpp"
#include "lexer.hpp"
#include "parser_msgs.hpp"
#include "parser_state.hpp"
#include "stmt_ast.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <functional>
#include <string>

#define MAKE_BINARY_EXPR(T, left, right) \
   addToPool(new Expr::T(poolToAuto(left), \
			 poolToAuto(right)))

#define BEGIN_BLOCK() do {
#define END_BLOCK() } while(0)

namespace {

using namespace std;
using namespace Moove;

ParserState* parser;

/**
 * \internal
 * \brief Simplifies adding allocated objects to memory pool
 * \param ptr Pointer to add to pool and return
 *
 * Allows assigning to $$ and adding to memory pool in one line. Only
 * used for objects descended from MemoryPoolObject.
 */
template<class T>
inline T* addToPool(T* ptr)
{
   parser->addToPool(auto_ptr<ASTPoolObject>(ptr));
   return ptr;
}

/*
template<class T>
MemoryPoolPointer<T>* addPtrToPool(T* ptr)
{
   MemoryPoolPointer<T>* mpPtr = new MemoryPoolPointer<T>(auto_ptr<T>(ptr));
   parser->addToPool(auto_ptr<MemoryPoolObject>(mpPtr));
   return mpPtr;
}
*/

template<class T>
inline auto_ptr<T> poolToAuto(T* ptr)
{
   parser->removeFromPool(ptr);
   return auto_ptr<T>(ptr);
}

template<class T>
auto_ptr<T> poolPtrToAuto(ASTPoolPtr<T>* ptr)
{
   if(ptr) {
      parser->removeFromPool(ptr);
      auto_ptr<T> tmp(*ptr);
      delete ptr;
      return tmp;
   } else
      return auto_ptr<T>(0);
}

/**
 * \internal
 * \brief Calls cont.push_back(ptr) with exception safety
 * \param cont Container to insert into
 * \param ptr Pointer to a deletable object
 * 
 * If an exception is thrown while inserting \a ptr into \a cont, \a ptr
 * is deleted.
 */
template<class Container, class T>
void safePushBack(ASTAutoContainer<Container>& cont, T* ptr)
{
   auto_ptr<T> tmp(ptr);
   cont.push_back(ptr);
   tmp.release();
}

template<class Container, class T>
void safePushBack(ASTAutoContainer<Container>& cont, auto_ptr<T> ptr)
{
   cont.push_back(ptr.get());
   ptr.release();
}

inline void yyerror(const std::string& msg)
{
   parser->error(msg);
}

int yylex();

typedef Expr::Scatter::Target       ScatterTarget;
typedef Expr::Scatter::TargetList   ScatterTargetList;
typedef Stmt::TryExcept::Except     Except;
typedef Stmt::TryExcept::ExceptList ExceptList;

bool listToScatter(ScatterTargetList& targets, const Expr::ArgList& list);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 115 "moove.y"
{
   int                                    integer;
   Moove::ASTPoolPtr<std::string>* str;
   Moove::ASTPoolPtr<double>*      real;

   Moove::Expr::ArgList*                  list;
   Moove::Expr::Expr*                     expr;
   Moove::Expr::Scatter::Target*          target;
   Moove::Expr::Scatter::TargetList*      targetList;

   Moove::Stmt::Stmt*                     stmt;
   Moove::Stmt::Block*                    stmts;
   Moove::Stmt::If::ElseList*             elseList;
   Moove::Stmt::TryExcept::ExceptList*    exceptList;
   Moove::Stmt::Switch::CaseList*         caseList;
}
/* Line 187 of yacc.c.  */
#line 320 "moove.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 333 "moove.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1349

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNRULES -- Number of states.  */
#define YYNSTATES  235

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   302

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,     2,     2,    63,    54,     2,    73,
      64,    65,    52,    50,    68,    51,    60,    53,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    61,    67,
      44,    33,    45,    40,    69,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    62,     2,    66,    55,     2,    72,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,    41,    71,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    34,    35,
      36,    37,    38,    39,    42,    43,    46,    47,    48,    49,
      57,    58,    59
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    18,    19,    28,    29,
      39,    40,    52,    57,    63,    64,    73,    74,    82,    85,
      89,    93,    97,   100,   102,   103,   110,   111,   114,   115,
     117,   124,   132,   134,   136,   137,   144,   145,   150,   151,
     153,   155,   159,   161,   164,   166,   168,   170,   172,   176,
     178,   181,   184,   187,   190,   193,   196,   200,   206,   210,
     214,   218,   222,   226,   230,   234,   238,   242,   246,   250,
     254,   258,   262,   266,   270,   274,   278,   282,   286,   290,
     296,   300,   306,   313,   322,   328,   336,   338,   341,   347,
     352,   359,   363,   365,   369,   373,   378,   382,   385,   390,
     391,   392
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      75,     0,    -1,    76,    -1,    -1,    76,    77,    -1,     8,
      64,    93,    65,    76,    83,    84,    11,    -1,    -1,    12,
      85,    64,    93,    65,    78,    76,    13,    -1,    -1,    14,
       6,    32,    64,    93,    65,    79,    76,    15,    -1,    -1,
      14,     6,    32,    62,    93,    30,    93,    66,    80,    76,
      15,    -1,    18,    76,    86,    21,    -1,    18,    76,    20,
      76,    21,    -1,    -1,    16,    85,    64,    93,    65,    81,
      76,    17,    -1,    -1,    22,    64,    93,    65,    82,    88,
      25,    -1,    26,    67,    -1,    26,    93,    67,    -1,    27,
      85,    67,    -1,    28,    85,    67,    -1,    93,    67,    -1,
      67,    -1,    -1,    83,     9,    64,    93,    65,    76,    -1,
      -1,    10,    76,    -1,    -1,     6,    -1,    19,    85,    64,
      87,    65,    76,    -1,    86,    19,    85,    64,    87,    65,
      76,    -1,    29,    -1,    91,    -1,    -1,    88,    23,    64,
      93,    65,    76,    -1,    -1,    88,    24,    89,    76,    -1,
      -1,    91,    -1,    92,    -1,    91,    68,    92,    -1,    93,
      -1,    69,    93,    -1,     3,    -1,     4,    -1,     5,    -1,
       7,    -1,    70,    90,    71,    -1,     6,    -1,    56,    93,
      -1,    51,    93,    -1,    58,    93,    -1,    57,    93,    -1,
      93,    58,    -1,    93,    57,    -1,    93,    33,    93,    -1,
      70,    94,    71,    33,    93,    -1,    93,    43,    93,    -1,
      93,    42,    93,    -1,    93,    49,    93,    -1,    93,    48,
      93,    -1,    93,    44,    93,    -1,    93,    47,    93,    -1,
      93,    45,    93,    -1,    93,    46,    93,    -1,    93,    32,
      93,    -1,    93,    50,    93,    -1,    93,    51,    93,    -1,
      93,    52,    93,    -1,    93,    53,    93,    -1,    93,    54,
      93,    -1,    93,    55,    93,    -1,    93,    39,    93,    -1,
      93,    38,    93,    -1,    93,    37,    93,    -1,    93,    36,
      93,    -1,    93,    35,    93,    -1,    93,    34,    93,    -1,
      93,    40,    93,    41,    93,    -1,    93,    60,     6,    -1,
      93,    60,    64,    93,    65,    -1,    93,    61,     6,    64,
      90,    65,    -1,    93,    61,    64,    93,    65,    64,    90,
      65,    -1,    93,    62,    96,    93,    66,    -1,    93,    62,
      96,    93,    30,    93,    66,    -1,    63,    -1,    63,     6,
      -1,    63,     6,    64,    90,    65,    -1,     6,    64,    90,
      65,    -1,    72,    93,    56,    87,    97,    73,    -1,    64,
      93,    65,    -1,    95,    -1,    94,    68,    95,    -1,    94,
      68,     6,    -1,    94,    68,    69,     6,    -1,    91,    68,
      95,    -1,    40,     6,    -1,    40,     6,    33,    93,    -1,
      -1,    -1,    31,    93,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   168,   168,   175,   178,   188,   198,   197,   209,   208,
     220,   219,   231,   236,   242,   241,   262,   261,   271,   275,
     279,   297,   315,   319,   326,   329,   338,   341,   348,   351,
     357,   369,   386,   390,   397,   400,   408,   407,   422,   425,
     431,   436,   443,   447,   454,   458,   462,   466,   470,   474,
     481,   485,   489,   496,   503,   510,   519,   542,   550,   554,
     558,   562,   566,   570,   574,   578,   582,   586,   590,   594,
     598,   602,   606,   610,   614,   618,   622,   626,   630,   634,
     640,   648,   653,   662,   668,   674,   681,   688,   692,   697,
     702,   708,   715,   720,   725,   734,   743,   751,   759,   771,
     777,   780
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tINT", "tOBJNUM", "tSTR", "tID",
  "tREAL", "tIF", "tELSEIF", "tELSE", "tENDIF", "tWHILE", "tENDWHILE",
  "tFOR", "tENDFOR", "tFORK", "tENDFORK", "tTRY", "tEXCEPT", "tFINALLY",
  "tENDTRY", "tSWITCH", "tCASE", "tDEFAULT", "tENDSWITCH", "tRETURN",
  "tCONTINUE", "tBREAK", "tANY", "tTO", "tARROW", "tIN", "'='", "tEXP_EQ",
  "tMOD_EQ", "tDIV_EQ", "tMUL_EQ", "tSUB_EQ", "tADD_EQ", "'?'", "'|'",
  "tAND", "tOR", "'<'", "'>'", "tGE", "tLE", "tNE", "tEQ", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'^'", "'!'", "tDECREMENT", "tINCREMENT", "tNEGATE",
  "'.'", "':'", "'['", "'$'", "'('", "')'", "']'", "';'", "','", "'@'",
  "'{'", "'}'", "'`'", "'''", "$accept", "program", "stmts", "stmt", "@1",
  "@2", "@3", "@4", "@5", "elseifs", "else_block", "opt_id", "excepts",
  "except_codes", "switch_cases", "@6", "arglist", "ne_arglist",
  "argument", "expr", "scatter", "opt_target", "dollars_up",
  "catch_result", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    61,   288,   289,   290,   291,   292,   293,
      63,   124,   294,   295,    60,    62,   296,   297,   298,   299,
      43,    45,    42,    47,    37,    94,    33,   300,   301,   302,
      46,    58,    91,    36,    40,    41,    93,    59,    44,    64,
     123,   125,    96,    39
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    76,    76,    77,    78,    77,    79,    77,
      80,    77,    77,    77,    81,    77,    82,    77,    77,    77,
      77,    77,    77,    77,    83,    83,    84,    84,    85,    85,
      86,    86,    87,    87,    88,    88,    89,    88,    90,    90,
      91,    91,    92,    92,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    94,    94,    94,    94,    94,    95,    95,    96,
      97,    97
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     8,     0,     8,     0,     9,
       0,    11,     4,     5,     0,     8,     0,     7,     2,     3,
       3,     3,     2,     1,     0,     6,     0,     2,     0,     1,
       6,     7,     1,     1,     0,     6,     0,     4,     0,     1,
       1,     3,     1,     2,     1,     1,     1,     1,     3,     1,
       2,     2,     2,     2,     2,     2,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       3,     5,     6,     8,     5,     7,     1,     2,     5,     4,
       6,     3,     1,     3,     3,     4,     3,     2,     4,     0,
       0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,    44,    45,    46,    49,    47,     0,
      28,     0,    28,     3,     0,     0,    28,    28,     0,     0,
       0,     0,    86,     0,    23,    38,     0,     4,     0,    38,
       0,    29,     0,     0,     0,     0,     0,    18,     0,     0,
       0,    51,    50,    53,    52,    87,     0,     0,     0,     0,
      39,    40,    42,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      54,     0,     0,    99,    22,     0,    39,     0,     0,     0,
       0,    28,     3,     0,     0,    19,    20,    21,    38,    91,
      97,    43,    48,     0,     0,     0,     0,    66,    56,    78,
      77,    76,    75,    74,    73,     0,    59,    58,    62,    64,
      65,    63,    61,    60,    67,    68,    69,    70,    71,    72,
      80,     0,     0,     0,     0,    89,     0,     3,     0,     0,
       0,     0,     0,     0,    28,    12,    16,     0,     0,    41,
      96,    94,     0,    93,     0,    32,   100,    33,     0,     0,
      38,     0,     0,    24,     6,     0,     0,    14,     0,    13,
       0,    34,    88,    98,    95,    57,     0,     0,    79,    81,
       0,     0,     0,    84,    26,     3,     0,     8,     3,     0,
       0,     0,   101,    90,    82,    38,     0,     0,     3,     0,
       0,     0,     3,     0,     3,     0,     0,    36,    17,     0,
      85,     0,    27,     5,     7,    10,     0,    15,    30,     3,
       0,     3,    83,     0,     3,     9,    31,     0,    37,     3,
       0,     3,    25,    11,    35
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    27,   185,   202,   224,   188,   171,   184,
     199,    32,    93,   156,   191,   221,    49,    86,    51,    28,
      53,    54,   134,   177
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -168
static const yytype_int16 yypact[] =
{
    -168,     4,   504,  -168,  -168,  -168,  -168,   -50,  -168,   -45,
      21,    67,    21,  -168,    25,    30,    21,    21,   610,   610,
     610,   610,    78,   610,  -168,   107,   610,  -168,   651,   540,
     610,  -168,    27,    53,    28,   216,   610,  -168,   687,    29,
      32,   -17,   -17,   -17,   -17,    37,   793,    97,   610,    34,
      38,  -168,  1226,   -62,  -168,  1164,   610,   610,   610,   610,
     610,   610,   610,   610,   610,   610,   610,   610,   610,   610,
     610,   610,   610,   610,   610,   610,   610,   610,   610,  -168,
    -168,    10,    14,  -168,  -168,    42,    40,   827,   610,   -38,
     610,    21,  -168,    20,   861,  -168,  -168,  -168,   540,  -168,
      76,  1226,  -168,   107,     2,    83,   530,  1287,  1226,  1226,
    1226,  1226,  1226,  1226,  1226,  1195,  1274,  1274,  1287,  1287,
    1287,  1287,  1287,  1287,   140,   140,    60,    60,    60,    60,
    -168,   610,    55,   610,   610,  -168,   540,  -168,   895,   610,
     610,   929,    63,   242,    21,  -168,  -168,    64,   610,  -168,
    -168,  -168,   124,  -168,   610,  -168,   102,    40,   610,   963,
     540,   997,   588,   504,  -168,  1133,  1031,  -168,   530,  -168,
      71,  -168,  -168,  1226,  -168,  1226,   610,    65,  1250,  -168,
      72,    75,   610,  -168,    22,  -168,   610,  -168,  -168,    77,
     530,    52,  1226,  -168,  -168,   540,   723,    79,  -168,   125,
     312,   758,  -168,   338,  -168,    84,    80,  -168,  -168,    86,
    -168,   610,   504,  -168,  -168,  -168,   408,  -168,   504,  -168,
     610,  -168,  -168,  1065,  -168,  -168,   504,  1099,   504,  -168,
     434,  -168,   504,  -168,   504
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -168,  -168,   -13,  -168,  -168,  -168,  -168,  -168,  -168,  -168,
    -168,    13,  -168,  -167,  -168,  -168,   -26,   -23,   -98,    -8,
    -168,   -57,  -168,  -168
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      35,   189,    50,    85,     3,   149,   104,    38,   151,   105,
      41,    42,    43,    44,    29,    46,   130,    52,    55,    30,
     132,    52,    87,   205,   139,    34,   140,    31,    94,    39,
      40,   197,   198,     4,     5,     6,     7,     8,   149,   144,
     101,   145,    47,    81,    82,    83,   150,   153,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   152,   147,    33,   131,   206,   207,   208,   133,   143,
     138,    18,   141,   157,    45,    89,    19,    20,    21,    36,
      52,    88,    90,    22,    23,    52,    96,    37,    52,    97,
      25,    98,    26,   100,   142,   102,   103,   135,   136,   148,
       4,     5,     6,     7,     8,    78,   154,    79,    80,   160,
      81,    82,    83,   159,   163,   161,   162,   168,    52,   172,
     174,   165,   166,   176,   180,   190,   213,   194,   193,   195,
     173,     0,   204,   211,   220,   157,   175,    47,     0,   219,
     178,   222,    52,     0,     0,     0,     0,   170,    18,     0,
      52,     0,     0,    19,    20,    21,     0,   157,   192,   209,
      22,    23,   200,     0,   196,   203,    48,    25,   201,    26,
       0,     0,    52,     0,     0,   212,     0,    52,     0,   216,
       0,   218,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,   223,     0,     0,   226,     0,   228,     0,
       0,   230,   227,     0,     0,     0,   232,     0,   234,     4,
       5,     6,     7,     8,     9,     0,     0,     0,    10,     0,
      11,     0,    12,     0,    13,    91,    92,     0,    14,     0,
       0,     0,    15,    16,    17,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,     0,    11,     0,    12,     0,
      13,     0,     0,   169,    14,     0,     0,    18,    15,    16,
      17,     0,    19,    20,    21,     0,     0,     0,     0,    22,
      23,     0,     0,    24,     0,     0,    25,     0,    26,     0,
       0,     0,     0,    18,     0,     0,     0,     0,    19,    20,
      21,     0,     0,     0,     0,    22,    23,     0,     0,    24,
       0,     0,    25,     0,    26,     4,     5,     6,     7,     8,
       9,     0,     0,     0,    10,   214,    11,     0,    12,     0,
      13,     0,     0,     0,    14,     0,     0,     0,    15,    16,
      17,     4,     5,     6,     7,     8,     9,     0,     0,     0,
      10,     0,    11,     0,    12,   217,    13,     0,     0,     0,
      14,     0,     0,    18,    15,    16,    17,     0,    19,    20,
      21,     0,     0,     0,     0,    22,    23,     0,     0,    24,
       0,     0,    25,     0,    26,     0,     0,     0,     0,    18,
       0,     0,     0,     0,    19,    20,    21,     0,     0,     0,
       0,    22,    23,     0,     0,    24,     0,     0,    25,     0,
      26,     4,     5,     6,     7,     8,     9,     0,     0,     0,
      10,     0,    11,   225,    12,     0,    13,     0,     0,     0,
      14,     0,     0,     0,    15,    16,    17,     4,     5,     6,
       7,     8,     9,     0,     0,     0,    10,     0,    11,   233,
      12,     0,    13,     0,     0,     0,    14,     0,     0,    18,
      15,    16,    17,     0,    19,    20,    21,     0,     0,     0,
       0,    22,    23,     0,     0,    24,     0,     0,    25,     0,
      26,     0,     0,     0,     0,    18,     0,     0,     0,     0,
      19,    20,    21,     0,     0,     0,     0,    22,    23,     0,
       0,    24,     0,     0,    25,     0,    26,     4,     5,     6,
       7,     8,     9,     0,     0,     0,    10,     0,    11,     0,
      12,     0,    13,     0,     0,     0,    14,     0,     0,     0,
      15,    16,    17,     4,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     0,     0,    18,     0,     0,     0,   155,
      19,    20,    21,     0,     0,     0,     0,    22,    23,     0,
       0,    24,     0,     0,    25,     0,    26,     0,     0,     0,
       0,    18,     0,     0,     0,     0,    19,    20,    21,     0,
       0,    18,     0,    22,    23,     0,    19,    20,    21,    48,
      25,     0,    26,    22,    23,     0,     0,     0,     0,    48,
      25,     0,    26,     4,     5,     6,     7,     8,   182,     0,
      56,    57,    58,    59,    60,    61,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,    18,     0,     0,     0,     0,    19,    20,    21,     0,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
      25,     0,    26,    56,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
       0,    81,    82,    83,     0,     0,     0,     0,    84,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
       0,     0,     0,     0,    95,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,     0,     0,     0,   210,
      56,    57,    58,    59,    60,    61,    62,    63,    64,     0,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,     0,     0,     0,   215,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,     0,     0,    99,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
       0,     0,   137,    56,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
       0,    81,    82,    83,     0,     0,   146,    56,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,     0,     0,
     164,    56,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,     0,     0,   167,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,     0,     0,   179,    56,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
       0,     0,   181,    56,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
       0,    81,    82,    83,     0,     0,   187,    56,    57,    58,
      59,    60,    61,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,     0,     0,
     229,    56,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,     0,   186,   231,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    56,    57,    58,    59,
      60,    61,    62,    63,    64,     0,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
     106,    79,    80,     0,    81,    82,    83,    56,    57,    58,
      59,    60,    61,    62,    63,    64,   158,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    56,    57,
      58,    59,    60,    61,    62,    63,    64,     0,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    56,    79,    80,     0,    81,    82,    83,     0,
      -1,     0,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    56,    79,    80,     0,
      81,    82,    83,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83
};

static const yytype_int16 yycheck[] =
{
      13,   168,    25,    29,     0,   103,    68,    15,     6,    71,
      18,    19,    20,    21,    64,    23,     6,    25,    26,    64,
       6,    29,    30,   190,    62,    12,    64,     6,    36,    16,
      17,     9,    10,     3,     4,     5,     6,     7,   136,    19,
      48,    21,    40,    60,    61,    62,   103,   104,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    69,    98,     6,    64,    23,    24,    25,    64,    92,
      88,    51,    90,   106,     6,    32,    56,    57,    58,    64,
      98,    64,    64,    63,    64,   103,    67,    67,   106,    67,
      70,    64,    72,     6,    91,    71,    68,    65,    68,    33,
       3,     4,     5,     6,     7,    55,    33,    57,    58,    64,
      60,    61,    62,   131,   137,   133,   134,    64,   136,    65,
       6,   139,   140,    31,   160,    64,    11,    65,    73,    64,
     148,    -1,    65,    64,    64,   168,   154,    40,    -1,    65,
     158,    65,   160,    -1,    -1,    -1,    -1,   144,    51,    -1,
     168,    -1,    -1,    56,    57,    58,    -1,   190,   176,   195,
      63,    64,   185,    -1,   182,   188,    69,    70,   186,    72,
      -1,    -1,   190,    -1,    -1,   198,    -1,   195,    -1,   202,
      -1,   204,    52,    53,    54,    55,    -1,    57,    58,    -1,
      60,    61,    62,   211,    -1,    -1,   219,    -1,   221,    -1,
      -1,   224,   220,    -1,    -1,    -1,   229,    -1,   231,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      14,    -1,    16,    -1,    18,    19,    20,    -1,    22,    -1,
      -1,    -1,    26,    27,    28,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    14,    -1,    16,    -1,
      18,    -1,    -1,    21,    22,    -1,    -1,    51,    26,    27,
      28,    -1,    56,    57,    58,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    67,    -1,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,
      58,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    70,    -1,    72,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    -1,
      18,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      28,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    14,    -1,    16,    17,    18,    -1,    -1,    -1,
      22,    -1,    -1,    51,    26,    27,    28,    -1,    56,    57,
      58,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    70,    -1,
      72,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    14,    15,    16,    -1,    18,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    28,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    14,    15,
      16,    -1,    18,    -1,    -1,    -1,    22,    -1,    -1,    51,
      26,    27,    28,    -1,    56,    57,    58,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    70,    -1,
      72,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,    57,    58,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    67,    -1,    -1,    70,    -1,    72,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    14,    -1,
      16,    -1,    18,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    28,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    29,
      56,    57,    58,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    67,    -1,    -1,    70,    -1,    72,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,
      -1,    51,    -1,    63,    64,    -1,    56,    57,    58,    69,
      70,    -1,    72,    63,    64,    -1,    -1,    -1,    -1,    69,
      70,    -1,    72,     3,     4,     5,     6,     7,    30,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    72,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    -1,    -1,    67,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    -1,    -1,    67,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    -1,    -1,    -1,    66,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    -1,    66,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    -1,    -1,    65,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    65,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    65,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    -1,    -1,
      65,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    -1,    -1,    65,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    -1,    -1,    65,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      -1,    -1,    65,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    61,    62,    -1,    -1,    65,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    -1,    -1,
      65,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    -1,    30,    65,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    32,    57,    58,    -1,    60,    61,    62,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    32,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    75,    76,     0,     3,     4,     5,     6,     7,     8,
      12,    14,    16,    18,    22,    26,    27,    28,    51,    56,
      57,    58,    63,    64,    67,    70,    72,    77,    93,    64,
      64,     6,    85,     6,    85,    76,    64,    67,    93,    85,
      85,    93,    93,    93,    93,     6,    93,    40,    69,    90,
      91,    92,    93,    94,    95,    93,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    57,
      58,    60,    61,    62,    67,    90,    91,    93,    64,    32,
      64,    19,    20,    86,    93,    67,    67,    67,    64,    65,
       6,    93,    71,    68,    68,    71,    56,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
       6,    64,     6,    64,    96,    65,    68,    65,    93,    62,
      64,    93,    85,    76,    19,    21,    65,    90,    33,    92,
      95,     6,    69,    95,    33,    29,    87,    91,    41,    93,
      64,    93,    93,    76,    65,    93,    93,    65,    64,    21,
      85,    82,    65,    93,     6,    93,    31,    97,    93,    65,
      90,    65,    30,    66,    83,    78,    30,    65,    81,    87,
      64,    88,    93,    73,    65,    64,    93,     9,    10,    84,
      76,    93,    79,    76,    65,    87,    23,    24,    25,    90,
      66,    64,    76,    11,    13,    66,    76,    17,    76,    65,
      64,    89,    65,    93,    80,    15,    76,    93,    76,    65,
      76,    65,    76,    15,    76
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 169 "moove.y"
    {
			parser->setProgram(poolToAuto((yyvsp[(1) - (1)].stmts)));
                     ;}
    break;

  case 3:
#line 175 "moove.y"
    {
			(yyval.stmts) = addToPool(new Stmt::Block);
                     ;}
    break;

  case 4:
#line 179 "moove.y"
    {
			(yyval.stmts) = (yyvsp[(1) - (2)].stmts);

			if((yyvsp[(2) - (2)].stmt))
			   safePushBack(*(yyval.stmts), poolToAuto((yyvsp[(2) - (2)].stmt)));

                     ;}
    break;

  case 5:
#line 189 "moove.y"
    {
			if((yyvsp[(7) - (8)].stmts))
			   safePushBack(*(yyvsp[(6) - (8)].elseList), new Stmt::If::Else(poolToAuto((yyvsp[(7) - (8)].stmts))));

                        (yyval.stmt) = addToPool(new Stmt::If(poolToAuto((yyvsp[(3) - (8)].expr)), 
						    poolToAuto((yyvsp[(5) - (8)].stmts)), 
						    poolToAuto((yyvsp[(6) - (8)].elseList))));
                     ;}
    break;

  case 6:
#line 198 "moove.y"
    {
                        parser->beginLoop(poolPtrToAuto((yyvsp[(2) - (5)].str)).get());
                     ;}
    break;

  case 7:
#line 202 "moove.y"
    {
                            (yyval.stmt) = addToPool(new Stmt::While(parser->loopID(), 
							   poolToAuto((yyvsp[(4) - (8)].expr)), 
							   poolToAuto((yyvsp[(7) - (8)].stmts))));
                            parser->endLoop();
                         ;}
    break;

  case 8:
#line 209 "moove.y"
    {
			parser->beginLoop(poolPtrToAuto((yyvsp[(2) - (6)].str)).get());
                     ;}
    break;

  case 9:
#line 213 "moove.y"
    {
			    (yyval.stmt) = addToPool(new Stmt::ForList(parser->loopID(), 
							     poolToAuto((yyvsp[(5) - (9)].expr)), 
							     poolToAuto((yyvsp[(8) - (9)].stmts))));
			    parser->endLoop();
                         ;}
    break;

  case 10:
#line 220 "moove.y"
    {
                        parser->beginLoop(poolPtrToAuto((yyvsp[(2) - (8)].str)).get());
                     ;}
    break;

  case 11:
#line 224 "moove.y"
    {
			    (yyval.stmt) = addToPool(new Stmt::ForRange(parser->loopID(),
							      poolToAuto((yyvsp[(5) - (11)].expr)), 
							      poolToAuto((yyvsp[(7) - (11)].expr)), 
							      poolToAuto((yyvsp[(10) - (11)].stmts))));
			    parser->endLoop();
                         ;}
    break;

  case 12:
#line 232 "moove.y"
    {
			(yyval.stmt) = addToPool(new Stmt::TryExcept(poolToAuto((yyvsp[(2) - (4)].stmts)), 
							   poolToAuto((yyvsp[(3) - (4)].exceptList))));
                     ;}
    break;

  case 13:
#line 237 "moove.y"
    {
			(yyval.stmt) = addToPool(new Stmt::TryFinally(poolToAuto((yyvsp[(2) - (5)].stmts)), 
							    poolToAuto((yyvsp[(4) - (5)].stmts))));
                     ;}
    break;

  case 14:
#line 242 "moove.y"
    {
			parser->beginFork();

			if((yyvsp[(2) - (5)].str))
			   parser->addVar(**(yyvsp[(2) - (5)].str));
		     ;}
    break;

  case 15:
#line 249 "moove.y"
    {
			    BEGIN_BLOCK();
			    auto_ptr<string> id = poolPtrToAuto((yyvsp[(2) - (8)].str));
			    Symbol forkVar = (id.get() ? 
					      parser->findVar(*id) : 
					      Symbol());
			    (yyval.stmt) = addToPool(new Stmt::Fork(forkVar,
							  poolToAuto((yyvsp[(4) - (8)].expr)), 
							  poolToAuto((yyvsp[(7) - (8)].stmts))));
			    parser->endFork();
			    END_BLOCK();
			 ;}
    break;

  case 16:
#line 262 "moove.y"
    {
			parser->beginSwitch();
		     ;}
    break;

  case 17:
#line 266 "moove.y"
    {
			    (yyval.stmt) = addToPool(new Stmt::Switch(poolToAuto((yyvsp[(3) - (7)].expr)), 
							    poolToAuto((yyvsp[(6) - (7)].caseList))));
			    parser->endSwitch();
			 ;}
    break;

  case 18:
#line 272 "moove.y"
    {
			(yyval.stmt) = addToPool(new Stmt::Return);
		     ;}
    break;

  case 19:
#line 276 "moove.y"
    {
                        (yyval.stmt) = addToPool(new Stmt::Return(poolToAuto((yyvsp[(2) - (3)].expr))));
                     ;}
    break;

  case 20:
#line 280 "moove.y"
    {
			BEGIN_BLOCK();
			std::auto_ptr<std::string> id = poolPtrToAuto((yyvsp[(2) - (3)].str));
			Symbol sym = id.get() ? parser->findVar(*id) : Symbol();

			if(!parser->checkLoopID(sym) && !parser->switchDepth()) {
			   if(id.get())
			      yyerror("Invalid loop name in `continue' "
				      "statement: " + *id);
			   else
			      yyerror("No enclosing loop or switch for "
				      "`continue' statement");
			}

			(yyval.stmt) = addToPool(new Stmt::Continue(sym));
			END_BLOCK();
                     ;}
    break;

  case 21:
#line 298 "moove.y"
    {
			BEGIN_BLOCK();
			std::auto_ptr<std::string> id = poolPtrToAuto((yyvsp[(2) - (3)].str));
			Symbol sym = id.get() ? parser->findVar(*id) : Symbol();

			if(!parser->checkLoopID(sym) && !parser->switchDepth()) {
			   if(id.get())
			      yyerror("Invalid loop name in `break' "
				      "statement: " + *id);
			   else
			      yyerror("No enclosing loop or switch for "
				      "`break' statement");
			}

			(yyval.stmt) = addToPool(new Stmt::Break(sym));
			END_BLOCK();
                     ;}
    break;

  case 22:
#line 316 "moove.y"
    {
			(yyval.stmt) = addToPool(new Stmt::Expr(poolToAuto((yyvsp[(1) - (2)].expr))));
                     ;}
    break;

  case 23:
#line 320 "moove.y"
    {
                        (yyval.stmt) = 0;
                     ;}
    break;

  case 24:
#line 326 "moove.y"
    {
                        (yyval.elseList) = addToPool(new Stmt::If::ElseList);
                     ;}
    break;

  case 25:
#line 330 "moove.y"
    {
			(yyval.elseList) = (yyvsp[(1) - (6)].elseList);
			safePushBack(*(yyval.elseList), new Stmt::If::ElseIf(poolToAuto((yyvsp[(4) - (6)].expr)), 
							       poolToAuto((yyvsp[(6) - (6)].stmts))));
                     ;}
    break;

  case 26:
#line 338 "moove.y"
    {
                        (yyval.stmts) = 0;
                     ;}
    break;

  case 27:
#line 342 "moove.y"
    {
                        (yyval.stmts) = (yyvsp[(2) - (2)].stmts);
                     ;}
    break;

  case 28:
#line 348 "moove.y"
    {
                        (yyval.str) = 0;
                     ;}
    break;

  case 29:
#line 352 "moove.y"
    {
                        (yyval.str) = (yyvsp[(1) - (1)].str);
                     ;}
    break;

  case 30:
#line 358 "moove.y"
    {
			BEGIN_BLOCK();
                        (yyval.exceptList) = addToPool(new ExceptList);
			Symbol var = ((yyvsp[(2) - (6)].str) ? 
				      parser->addVar(*poolPtrToAuto((yyvsp[(2) - (6)].str))) : 
				      Symbol());
			safePushBack(*(yyval.exceptList), new Except(var,
						     poolToAuto((yyvsp[(4) - (6)].list)),
						     poolToAuto((yyvsp[(6) - (6)].stmts))));
			END_BLOCK();
                     ;}
    break;

  case 31:
#line 370 "moove.y"
    {
			BEGIN_BLOCK();
			if((yyvsp[(1) - (7)].exceptList)->back()->codes().empty())
                           yyerror("Unreachable EXCEPT clause");
			
			(yyval.exceptList) = (yyvsp[(1) - (7)].exceptList);
			Symbol var = ((yyvsp[(3) - (7)].str) ? 
				      parser->addVar(*poolPtrToAuto((yyvsp[(3) - (7)].str))) :
				      Symbol());
			safePushBack(*(yyval.exceptList), new Except(var,
						     poolToAuto((yyvsp[(5) - (7)].list)),
						     poolToAuto((yyvsp[(7) - (7)].stmts))));
			END_BLOCK();
		     ;}
    break;

  case 32:
#line 387 "moove.y"
    {
                        (yyval.list) = addToPool(new Expr::ArgList);
                     ;}
    break;

  case 33:
#line 391 "moove.y"
    {
                        (yyval.list) = (yyvsp[(1) - (1)].list);
                     ;}
    break;

  case 34:
#line 397 "moove.y"
    {
			(yyval.caseList) = addToPool(new Stmt::Switch::CaseList);
		     ;}
    break;

  case 35:
#line 401 "moove.y"
    {
			(yyval.caseList) = (yyvsp[(1) - (6)].caseList);
			safePushBack(*(yyval.caseList), 
				     new Stmt::Switch::Case(poolToAuto((yyvsp[(4) - (6)].expr)), 
							    poolToAuto((yyvsp[(6) - (6)].stmts))));
		     ;}
    break;

  case 36:
#line 408 "moove.y"
    {
			if(find_if((yyvsp[(1) - (2)].caseList)->begin(), (yyvsp[(1) - (2)].caseList)->end(),
				   mem_fun(&Stmt::Switch::Case::isDefault)) != (yyvsp[(1) - (2)].caseList)->end())
			   yyerror("Multiple default clauses in switch statement");
		     ;}
    break;

  case 37:
#line 414 "moove.y"
    {
			    (yyval.caseList) = (yyvsp[(1) - (4)].caseList);
			    safePushBack(*(yyval.caseList), 
					 new Stmt::Switch::Default(poolToAuto((yyvsp[(4) - (4)].stmts))));
			 ;}
    break;

  case 38:
#line 422 "moove.y"
    {
			(yyval.list) = addToPool(new Expr::ArgList);
                     ;}
    break;

  case 39:
#line 426 "moove.y"
    {
                        (yyval.list) = (yyvsp[(1) - (1)].list);
                     ;}
    break;

  case 40:
#line 432 "moove.y"
    {
			(yyval.list) = addToPool(new Expr::ArgList);
			safePushBack(*(yyval.list), poolToAuto((yyvsp[(1) - (1)].expr)));
                     ;}
    break;

  case 41:
#line 437 "moove.y"
    {
			(yyval.list) = (yyvsp[(1) - (3)].list);
			safePushBack(*(yyval.list), poolToAuto((yyvsp[(3) - (3)].expr)));
                     ;}
    break;

  case 42:
#line 444 "moove.y"
    {
			(yyval.expr) = (yyvsp[(1) - (1)].expr);
                     ;}
    break;

  case 43:
#line 448 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Splice(poolToAuto((yyvsp[(2) - (2)].expr))));
                     ;}
    break;

  case 44:
#line 455 "moove.y"
    {
                        (yyval.expr) = addToPool(new Expr::Integer((yyvsp[(1) - (1)].integer)));
                     ;}
    break;

  case 45:
#line 459 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Objnum((yyvsp[(1) - (1)].integer)));
		     ;}
    break;

  case 46:
#line 463 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Str(poolPtrToAuto((yyvsp[(1) - (1)].str))));
                     ;}
    break;

  case 47:
#line 467 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Real(poolPtrToAuto((yyvsp[(1) - (1)].real))));
                     ;}
    break;

  case 48:
#line 471 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::List(poolToAuto((yyvsp[(2) - (3)].list))));
                     ;}
    break;

  case 49:
#line 475 "moove.y"
    {
			BEGIN_BLOCK();
			Symbol var = parser->addVar(*poolPtrToAuto((yyvsp[(1) - (1)].str)));
			(yyval.expr) = addToPool(new Expr::Variable(var));
			END_BLOCK();
                     ;}
    break;

  case 50:
#line 482 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Not(poolToAuto((yyvsp[(2) - (2)].expr))));
                     ;}
    break;

  case 51:
#line 486 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Negate(poolToAuto((yyvsp[(2) - (2)].expr))));
                     ;}
    break;

  case 52:
#line 490 "moove.y"
    {
			if(!(yyvsp[(2) - (2)].expr)->assignable())
			   yyerror("Invalid operand to ++ operator");

			(yyval.expr) = addToPool(new Expr::PreInc(poolToAuto((yyvsp[(2) - (2)].expr))));
                     ;}
    break;

  case 53:
#line 497 "moove.y"
    {
			if(!(yyvsp[(2) - (2)].expr)->assignable())
			   yyerror("Invalid operand to -- operator");

			(yyval.expr) = addToPool(new Expr::PreDec(poolToAuto((yyvsp[(2) - (2)].expr))));
                     ;}
    break;

  case 54:
#line 504 "moove.y"
    {
			if(!(yyvsp[(1) - (2)].expr)->assignable())
			   yyerror("Invalid operand to ++ operator");

			(yyval.expr) = addToPool(new Expr::PostInc(poolToAuto((yyvsp[(1) - (2)].expr))));
                     ;}
    break;

  case 55:
#line 512 "moove.y"
    {
			if(!(yyvsp[(1) - (2)].expr)->assignable())
			   yyerror("Invalid operand to -- operator");

			(yyval.expr) = addToPool(new Expr::PostDec(poolToAuto((yyvsp[(1) - (2)].expr))));

                     ;}
    break;

  case 56:
#line 520 "moove.y"
    {
			BEGIN_BLOCK();
			std::auto_ptr<Expr::Expr> dest = poolToAuto((yyvsp[(1) - (3)].expr));

			if(!dest->assignable()) {
			   bool converted = false;
			   Expr::List* list;

			   if((list = dynamic_cast<Expr::List*>(dest.get()))) {
			      std::auto_ptr<ScatterTargetList> targets(new ScatterTargetList);
			      converted = listToScatter(*targets, 
							list->elements());
			      dest.reset(new Expr::Scatter(targets));
			   } 
			   
			   if(!converted)
			      yyerror("Invalid expression on left side of assignment");
			}

                        (yyval.expr) = addToPool(new Expr::Assign(dest, poolToAuto((yyvsp[(3) - (3)].expr))));
			END_BLOCK();
                     ;}
    break;

  case 57:
#line 543 "moove.y"
    {
			BEGIN_BLOCK();
			auto_ptr<Expr::Expr> scatter(new Expr::Scatter(poolToAuto((yyvsp[(2) - (5)].targetList))));
			(yyval.expr) = addToPool(new Expr::Assign(scatter,
							poolToAuto((yyvsp[(5) - (5)].expr))));
			END_BLOCK();
                     ;}
    break;

  case 58:
#line 551 "moove.y"
    {
			(yyval.expr) = MAKE_BINARY_EXPR(Or, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 59:
#line 555 "moove.y"
    {
			(yyval.expr) = MAKE_BINARY_EXPR(And, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 60:
#line 559 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Equal, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 61:
#line 563 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(NotEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 62:
#line 567 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Less, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 63:
#line 571 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(LessEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 64:
#line 575 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Greater, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 65:
#line 579 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(GreaterEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 66:
#line 583 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(In, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 67:
#line 587 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Add, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 68:
#line 591 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Sub, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 69:
#line 595 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Mul, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 70:
#line 599 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Div, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 71:
#line 603 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Mod, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 72:
#line 607 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(Exp, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 73:
#line 611 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(AddEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 74:
#line 615 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(SubEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 75:
#line 619 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(MulEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 76:
#line 623 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(DivEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 77:
#line 627 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(ModEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 78:
#line 631 "moove.y"
    {
                        (yyval.expr) = MAKE_BINARY_EXPR(ExpEqual, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
                     ;}
    break;

  case 79:
#line 635 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Conditional(poolToAuto((yyvsp[(1) - (5)].expr)), 
							     poolToAuto((yyvsp[(3) - (5)].expr)), 
							     poolToAuto((yyvsp[(5) - (5)].expr))));
                     ;}
    break;

  case 80:
#line 641 "moove.y"
    {
			BEGIN_BLOCK();
			auto_ptr<Expr::Expr> name(new Expr::Str(poolPtrToAuto((yyvsp[(3) - (3)].str))));
			(yyval.expr) = addToPool(new Expr::Prop(poolToAuto((yyvsp[(1) - (3)].expr)),
						      name));
			END_BLOCK();
                     ;}
    break;

  case 81:
#line 649 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Prop(poolToAuto((yyvsp[(1) - (5)].expr)), 
						      poolToAuto((yyvsp[(4) - (5)].expr))));
                     ;}
    break;

  case 82:
#line 654 "moove.y"
    {
			BEGIN_BLOCK();
			auto_ptr<Expr::Expr> name(new Expr::Str(poolPtrToAuto((yyvsp[(3) - (6)].str))));
			(yyval.expr) = addToPool(new Expr::VerbCall(poolToAuto((yyvsp[(1) - (6)].expr)), 
							  name, 
							  poolToAuto((yyvsp[(5) - (6)].list))));
			END_BLOCK();
		     ;}
    break;

  case 83:
#line 663 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::VerbCall(poolToAuto((yyvsp[(1) - (8)].expr)),
							  poolToAuto((yyvsp[(4) - (8)].expr)),
							  poolToAuto((yyvsp[(7) - (8)].list))));
		     ;}
    break;

  case 84:
#line 669 "moove.y"
    {
			parser->decDollarDepth();
			(yyval.expr) = addToPool(new Expr::Index(poolToAuto((yyvsp[(1) - (5)].expr)),
						       poolToAuto((yyvsp[(4) - (5)].expr))));
		     ;}
    break;

  case 85:
#line 675 "moove.y"
    {
			parser->decDollarDepth();
			(yyval.expr) = addToPool(new Expr::Range(poolToAuto((yyvsp[(1) - (7)].expr)), 
						       poolToAuto((yyvsp[(4) - (7)].expr)), 
						       poolToAuto((yyvsp[(6) - (7)].expr))));
		     ;}
    break;

  case 86:
#line 682 "moove.y"
    {
			if(!parser->dollarDepth())
			   yyerror("Illegal context for `$' expression");

			(yyval.expr) = addToPool(new Expr::Length);
		     ;}
    break;

  case 87:
#line 689 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::SystemProp(poolPtrToAuto((yyvsp[(2) - (2)].str))));
		     ;}
    break;

  case 88:
#line 693 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::SystemCall(poolPtrToAuto((yyvsp[(2) - (5)].str)),
							    poolToAuto((yyvsp[(4) - (5)].list))));
		     ;}
    break;

  case 89:
#line 698 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Builtin(poolPtrToAuto((yyvsp[(1) - (4)].str)), 
							 poolToAuto((yyvsp[(3) - (4)].list))));
		     ;}
    break;

  case 90:
#line 703 "moove.y"
    {
			(yyval.expr) = addToPool(new Expr::Catch(poolToAuto((yyvsp[(2) - (6)].expr)), 
						       poolToAuto((yyvsp[(4) - (6)].list)),
						       poolToAuto((yyvsp[(5) - (6)].expr))));
		     ;}
    break;

  case 91:
#line 709 "moove.y"
    {
			 (yyval.expr) = (yyvsp[(2) - (3)].expr);
		     ;}
    break;

  case 92:
#line 716 "moove.y"
    {
			(yyval.targetList) = addToPool(new ScatterTargetList);
			safePushBack(*(yyval.targetList), poolToAuto((yyvsp[(1) - (1)].target)));
		     ;}
    break;

  case 93:
#line 721 "moove.y"
    {
			(yyval.targetList) = (yyvsp[(1) - (3)].targetList);
			safePushBack(*(yyval.targetList), poolToAuto((yyvsp[(3) - (3)].target)));
		     ;}
    break;

  case 94:
#line 726 "moove.y"
    {
			BEGIN_BLOCK();
			(yyval.targetList) = (yyvsp[(1) - (3)].targetList);
			Symbol var = parser->addVar(*poolPtrToAuto((yyvsp[(3) - (3)].str)));
			safePushBack(*(yyval.targetList), 
				     new ScatterTarget(ScatterTarget::REQUIRED, var));
			END_BLOCK();
		     ;}
    break;

  case 95:
#line 735 "moove.y"
    {
			BEGIN_BLOCK();
			(yyval.targetList) = (yyvsp[(1) - (4)].targetList);
			Symbol var = parser->addVar(*poolPtrToAuto((yyvsp[(4) - (4)].str)));
			safePushBack(*(yyval.targetList), 
				     new ScatterTarget(ScatterTarget::REST, var));
			END_BLOCK();
		     ;}
    break;

  case 96:
#line 744 "moove.y"
    {
			(yyval.targetList) = addToPool(new ScatterTargetList);
			listToScatter(*(yyval.targetList), *poolToAuto((yyvsp[(1) - (3)].list)));
			safePushBack(*(yyval.targetList), poolToAuto((yyvsp[(3) - (3)].target)));
		     ;}
    break;

  case 97:
#line 752 "moove.y"
    {
			BEGIN_BLOCK();
			Symbol var = parser->addVar(*poolPtrToAuto((yyvsp[(2) - (2)].str)));
			(yyval.target) = addToPool(new ScatterTarget(ScatterTarget::OPTIONAL,
							 var));
			END_BLOCK();
		     ;}
    break;

  case 98:
#line 760 "moove.y"
    {
			BEGIN_BLOCK();
			Symbol var = parser->addVar(*poolPtrToAuto((yyvsp[(2) - (4)].str)));
			(yyval.target) = addToPool(new ScatterTarget(ScatterTarget::OPTIONAL,
							 var,
							 poolToAuto((yyvsp[(4) - (4)].expr))));
			END_BLOCK();
		     ;}
    break;

  case 99:
#line 771 "moove.y"
    {
			parser->incDollarDepth();
		     ;}
    break;

  case 100:
#line 777 "moove.y"
    {
			(yyval.expr) = 0;
		     ;}
    break;

  case 101:
#line 781 "moove.y"
    {
			(yyval.expr) = (yyvsp[(2) - (2)].expr);
		     ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2837 "moove.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 786 "moove.y"

 
}   //namespace

#include "lexer_symbols.hpp"

namespace {

Symbol isRestTarget(const Expr::Expr* expr)
{
   const Expr::Splice* splice;
   const Expr::Variable* var;

   if((splice = dynamic_cast<const Expr::Splice*>(expr)) &&
      (var = dynamic_cast<const Expr::Variable*>(&splice->operand())))
      return var->id();
   else
      return Symbol();
}
   
bool listToScatter(ScatterTargetList& targets, const Expr::ArgList& list)
{
   typedef Expr::ArgList ArgList;
   typedef Expr::Variable Variable;
   typedef Expr::Splice Splice;
   typedef ScatterTarget Target;

   bool success = true;

   ArgList::const_iterator end = list.end();
   for(ArgList::const_iterator item = list.begin(); item != end; ++item) {
      Symbol id;

      if(const Variable* var = dynamic_cast<const Variable*>(*item)) {
	 std::auto_ptr<Target> target(new ScatterTarget(Target::REQUIRED,
							var->id()));
	 targets.push_back(target.get());
	 target.release();
      } else if((id = isRestTarget(*item))) {
	 std::auto_ptr<Target> target(new ScatterTarget(Target::REST,
							id));
	 targets.push_back(target.get());
	 target.release();
      } else {
	 yyerror("Scattering assignment targets must be simple variables.");
	 success = false;
      }
   }

   return success;
}

void checkAssignable(const Expr::Expr& expr, const char* assignType)
{
   if(!expr.assignable())
      yyerror(string("Illegal expression on left side of ") +
	      assignType + '.');
}

int yylex()
{
   return parser->lexer().nextToken();
}

}    //namespace

namespace Moove {

void Lexer::error(const string& msg)
{
   m_messages.error(msg, m_line);
}

void Lexer::warning(const string& msg)
{
   m_messages.warning(msg, m_line);
}

bool Lexer::isWS(char ch)
{
   return ch == ' ' || ch == '\t' || ch == '\r';
}

bool Lexer::myIsDigit(char ch)
{
   return isdigit(ch);
}

bool Lexer::isIDSuffixChar(char ch)
{ 
   return isalnum(ch) || ch == '_';
}

void Lexer::skipWS()
{
   m_pos = find_if(m_pos, m_end, not1(ptr_fun(&isWS)));
}

void Lexer::skipDigits()
{
   m_pos = find_if(m_pos, m_end, not1(ptr_fun(&myIsDigit)));
}

void Lexer::skipID()
{
   m_pos = find_if(m_pos, m_end, not1(ptr_fun(&isIDSuffixChar)));
}

int Lexer::parseNumber(bool realOK)
{
   string::const_iterator start = m_pos;
   
   //integer portion
   skipDigits();

   char ch = peekCur();
   if(realOK && 
      ((ch == '.' && peekNext() != '.') || ch == 'e' || ch == 'E')) {
      //fractional portion
      if(ch == '.') {
	 ++m_pos;
	 skipDigits();
	 ch = peekCur();
      }

      //exponent portion
      if(ch == 'e' || ch == 'E') {
	 ++m_pos;
	 ch = peekCur();
	 if(ch == '-' || ch == '+')
	    ++m_pos;
	 
	 if(isdigit(peekCur()))
	    skipDigits();
	 else {
	    error("Malformed floating-point literal");
	    return tINT;
	 }
      }

      double value = strtod(string(start, m_pos).c_str(), 0);
      if(errno != ERANGE) {
	 yylval.real = addToPool(new ASTPoolPtr<double>(new double(value)));
	 return tREAL;
      } else {
	 error("Real value out of range");
	 return tINT;
      }
   } else {
      long value = strtol(string(start, m_pos).c_str(), 0, 10);
      if(errno != ERANGE && static_cast<int>(value) == value) {
	 yylval.integer = value;
	 return tINT;
      } else {
	 error("Integer literal out of range");
	 return tINT;
      }
   }
}   

int Lexer::parseObjnum()
{
   bool negative = false;

   ++m_pos;
   if(peekCur() == '-') {
      negative = true;
      ++m_pos;
   }
   
   int ret = parseNumber(false) != -1 ? tOBJNUM : -1;
   
   if(negative && ret == tOBJNUM)
      yylval.integer = -yylval.integer;

   return ret;
}

int Lexer::parseString()
{
   yylval.str = addToPool(new ASTPoolPtr<string>(new string));
   
   for(string::const_iterator start = ++m_pos; m_pos != m_end; ++m_pos) {
      if(*m_pos == '"') {
	 (*yylval.str)->append(start, m_pos);
	 ++m_pos;
	 return tSTR;
      } else if(*m_pos == '\\') {
	 if(++m_pos != m_end && *m_pos != '\n') {
	    (*yylval.str)->append(start, m_pos - 1);
	    **yylval.str += *m_pos;
	    start = m_pos + 1;
	 } else
	    break;
      } else if(*m_pos == '\n')
	 break;
   }

   error("Missing quote");
   if(m_pos != m_end)
      ++m_pos;
   
   return tSTR;
}

void Lexer::parseComment()
{
   const char* commentEnd = "*/";
	    
   string::const_iterator pos = search(m_pos + 2, m_end,
				       commentEnd, commentEnd + 2);
   if(pos == m_end)
      error("End of program while in a comment");
   else
      m_pos = pos + 2;
}

int Lexer::parseID()
{
   string::const_iterator start = m_pos++;
   skipID();

   auto_ptr<string> id(new string(start, m_pos));
	 
   //Check ID in reserved word set
   const LexerSymbol* sym = LexerSymbolTable::in_word_set(id->c_str(),
							  id->size());
   if(sym) {
      //ID is a reserved word. Return appropriate token
      return sym->token;
   } else {
      //ID is not a reserved word
      yylval.str = addToPool(new ASTPoolPtr<string>(id));
      return tID;
   }
}

int Lexer::parseOp()
{
   if(m_end - m_pos >= 2) {
      //First check the next 2 characters for a 2-char operator stored
      //in the LexerSymbolTable
      string op(m_pos, m_pos + 2);
      const LexerSymbol* sym = LexerSymbolTable::in_word_set(op.c_str(),
							  op.size());
      if(sym) {
	 //It is a 2-char operator. Return appropriate token
	 m_pos += 2;
	 return sym->token;
      }
   }
    
   //Assume character is a 1-char operator
   return *m_pos++;
}

Lexer::Lexer(const std::string& str, ParserMessages& msgs, bool enableObjnums) : 
   m_source(str), m_pos(m_source.begin()), m_end(m_source.end()),
   m_line(1), m_messages(msgs), m_enableObjnums(enableObjnums)
{}

Lexer::Lexer(const char* str, ParserMessages& msgs, bool enableObjnums) :
   m_source(str), m_pos(m_source.begin()), m_end(m_source.end()),
   m_line(1), m_messages(msgs), m_enableObjnums(enableObjnums)
{}

int Lexer::nextToken()
{
   skipWS();

   while(m_pos != m_end) {
      char ch = *m_pos;

      if(ch == '\n') {
	 ++m_line;
	 ++m_pos;
	 skipWS();
      } else if(ch == '/' && peekNext() == '*') {
	 parseComment();
      } else if(ch == '"') {
	 return parseString();
      } else if(isdigit(ch) || (ch == '.' && isdigit(peekNext()))) {
	 return parseNumber(true);
      } else if(m_enableObjnums && ch == '#') {
	 return parseObjnum();
      } else if(isalpha(ch) || ch == '_') {
	 return parseID();
      } else
	 return parseOp();
   }
      
   return -1;
}

/**
 * \brief Type used for communication with parser sub-system
 *
 * An instance of this type will both provide a method of communication
 * with the calling Compiler object as well as hold parsing state
 * information such as loop depth, variables, and constants.
 */
int parseSource(ParserState& newParser)
{
   parser = &newParser;
   int ret = yyparse();
   newParser.parseFinished();
   return ret;
}

}   //namespace Moove

