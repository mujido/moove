/* C++ code produced by gperf version 3.0.2 */
/* Command-line: gperf -S 1 -t -D -C -L C++ -E -Z LexerSymbolTable lexer_symbols.hpp.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "lexer_symbols.hpp.gperf"

/**
 * \file
 */
#ifndef MOOVE_LEXER_SYMBOLS_HPP
#define MOOVE_LEXER_SYMBOLS_HPP

#include <cstring>

namespace {

#line 13 "lexer_symbols.hpp.gperf"
struct LexerSymbol { const char* name; int token; };
/* maximum key range = 71, duplicates = 0 */

class LexerSymbolTable
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct LexerSymbol *in_word_set (const char *str, unsigned int len);
};

inline unsigned int
LexerSymbolTable::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 46, 73, 73, 73, 36, 15,
      73, 73, 73, 25, 20, 20, 21, 15, 60, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 10,  0, 55, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 50, 73, 73, 15,  0,
       0,  0,  5,  0,  0, 73, 45, 15, 73, 73,
      10, 73, 73, 73, 15, 15,  0, 10, 73, 73,
       0, 10, 73, 73, 20, 20, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]+1];
}

const struct LexerSymbol *
LexerSymbolTable::in_word_set (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 39,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 9,
      MIN_HASH_VALUE = 2,
      MAX_HASH_VALUE = 72
    };

  static const struct LexerSymbol wordlist[] =
    {
#line 52 "lexer_symbols.hpp.gperf"
      {"=>",		tARROW},
#line 30 "lexer_symbols.hpp.gperf"
      {"case",		tCASE},
#line 19 "lexer_symbols.hpp.gperf"
      {"while",		tWHILE},
#line 29 "lexer_symbols.hpp.gperf"
      {"switch",		tSWITCH},
#line 31 "lexer_symbols.hpp.gperf"
      {"default",	tDEFAULT},
#line 34 "lexer_symbols.hpp.gperf"
      {"continue",	tCONTINUE},
#line 17 "lexer_symbols.hpp.gperf"
      {"else",		tELSE},
#line 26 "lexer_symbols.hpp.gperf"
      {"except",		tEXCEPT},
#line 46 "lexer_symbols.hpp.gperf"
      {"==",		tEQ},
#line 20 "lexer_symbols.hpp.gperf"
      {"endwhile",	tENDWHILE},
#line 32 "lexer_symbols.hpp.gperf"
      {"endswitch",	tENDSWITCH},
#line 18 "lexer_symbols.hpp.gperf"
      {"endif",		tENDIF},
#line 16 "lexer_symbols.hpp.gperf"
      {"elseif",		tELSEIF},
#line 27 "lexer_symbols.hpp.gperf"
      {"finally",	tFINALLY},
#line 21 "lexer_symbols.hpp.gperf"
      {"for",		tFOR},
#line 23 "lexer_symbols.hpp.gperf"
      {"fork",		tFORK},
#line 35 "lexer_symbols.hpp.gperf"
      {"break",		tBREAK},
#line 28 "lexer_symbols.hpp.gperf"
      {"endtry",		tENDTRY},
#line 48 "lexer_symbols.hpp.gperf"
      {"<=",		tLE},
#line 25 "lexer_symbols.hpp.gperf"
      {"try",		tTRY},
#line 22 "lexer_symbols.hpp.gperf"
      {"endfor",		tENDFOR},
#line 24 "lexer_symbols.hpp.gperf"
      {"endfork",	tENDFORK},
#line 36 "lexer_symbols.hpp.gperf"
      {"any",		tANY},
#line 33 "lexer_symbols.hpp.gperf"
      {"return",		tRETURN},
#line 38 "lexer_symbols.hpp.gperf"
      {"+=",		tADD_EQ},
#line 39 "lexer_symbols.hpp.gperf"
      {"-=",		tSUB_EQ},
#line 40 "lexer_symbols.hpp.gperf"
      {"*=",		tMUL_EQ},
#line 53 "lexer_symbols.hpp.gperf"
      {"..",		tTO},
#line 44 "lexer_symbols.hpp.gperf"
      {"||",		tOR},
#line 51 "lexer_symbols.hpp.gperf"
      {"--",		tDECREMENT},
#line 50 "lexer_symbols.hpp.gperf"
      {"++",		tINCREMENT},
#line 42 "lexer_symbols.hpp.gperf"
      {"%=",		tMOD_EQ},
#line 15 "lexer_symbols.hpp.gperf"
      {"if",		tIF},
#line 45 "lexer_symbols.hpp.gperf"
      {"&&",		tAND},
#line 37 "lexer_symbols.hpp.gperf"
      {"in",		tIN},
#line 47 "lexer_symbols.hpp.gperf"
      {"!=",		tNE},
#line 43 "lexer_symbols.hpp.gperf"
      {"^=",		tEXP_EQ},
#line 49 "lexer_symbols.hpp.gperf"
      {">=",		tGE},
#line 41 "lexer_symbols.hpp.gperf"
      {"/=",		tDIV_EQ}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          register const struct LexerSymbol *resword;

          switch (key - 2)
            {
              case 0:
                resword = &wordlist[0];
                goto compare;
              case 2:
                resword = &wordlist[1];
                goto compare;
              case 3:
                resword = &wordlist[2];
                goto compare;
              case 4:
                resword = &wordlist[3];
                goto compare;
              case 5:
                resword = &wordlist[4];
                goto compare;
              case 6:
                resword = &wordlist[5];
                goto compare;
              case 7:
                resword = &wordlist[6];
                goto compare;
              case 9:
                resword = &wordlist[7];
                goto compare;
              case 10:
                resword = &wordlist[8];
                goto compare;
              case 11:
                resword = &wordlist[9];
                goto compare;
              case 12:
                resword = &wordlist[10];
                goto compare;
              case 13:
                resword = &wordlist[11];
                goto compare;
              case 14:
                resword = &wordlist[12];
                goto compare;
              case 15:
                resword = &wordlist[13];
                goto compare;
              case 16:
                resword = &wordlist[14];
                goto compare;
              case 17:
                resword = &wordlist[15];
                goto compare;
              case 18:
                resword = &wordlist[16];
                goto compare;
              case 19:
                resword = &wordlist[17];
                goto compare;
              case 20:
                resword = &wordlist[18];
                goto compare;
              case 21:
                resword = &wordlist[19];
                goto compare;
              case 24:
                resword = &wordlist[20];
                goto compare;
              case 25:
                resword = &wordlist[21];
                goto compare;
              case 26:
                resword = &wordlist[22];
                goto compare;
              case 29:
                resword = &wordlist[23];
                goto compare;
              case 30:
                resword = &wordlist[24];
                goto compare;
              case 31:
                resword = &wordlist[25];
                goto compare;
              case 35:
                resword = &wordlist[26];
                goto compare;
              case 36:
                resword = &wordlist[27];
                goto compare;
              case 40:
                resword = &wordlist[28];
                goto compare;
              case 41:
                resword = &wordlist[29];
                goto compare;
              case 45:
                resword = &wordlist[30];
                goto compare;
              case 46:
                resword = &wordlist[31];
                goto compare;
              case 50:
                resword = &wordlist[32];
                goto compare;
              case 51:
                resword = &wordlist[33];
                goto compare;
              case 55:
                resword = &wordlist[34];
                goto compare;
              case 56:
                resword = &wordlist[35];
                goto compare;
              case 60:
                resword = &wordlist[36];
                goto compare;
              case 65:
                resword = &wordlist[37];
                goto compare;
              case 70:
                resword = &wordlist[38];
                goto compare;
            }
          return 0;
        compare:
          {
            register const char *s = resword->name;

            if (*str == *s && !std::strcmp (str + 1, s + 1))
              return resword;
          }
        }
    }
  return 0;
}
#line 54 "lexer_symbols.hpp.gperf"


}   //namespace

#endif   //MOOVE_LEXER_SYMBOLS_HPP
