#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class Token {
  string value;
  int kind;
  int lineNum;

public:
  static const int IDENTI = 0, LSQUAR = 1, RSQUAR = 2, LCURLY = 3, RCURLY = 4, LPAREN = 5, RPAREN = 6,
  LESS = 7, LEQUAL = 8, TEQUAL = 9, NEQUAL = 10, GEQUAL = 11, GREAT = 12,
  PLUS = 13, MINUS = 14, MULTI = 15, DIVIDE = 16, PERCEN = 17, AND = 18,
  OR = 19, SCOLON = 20, COMMA = 21, UNDEFI = 22, ENDOFF = 23, KEYWOR = 24, NUMBER = 25, OEQUAL = 26, SQUOTE = 27,
  DQUOTE = 28 /* two equal signs */, NOT = 29, INT = 30, VOID = 31, STRING = 32, IF = 33, ELSE = 34, WHILE = 35, 
  RETURN = 36, WRITE = 37, WRITELN = 38, READ = 39, IGNORE = 40;  // Names are no more than 6 char                                        

  static string kinds[];
  static char specialChars[];
  static string specialSymbols[];

  string toString();

  int getKind();

  int getLineNum();

  string getValue();

  Token();

  Token(string value, int kind, int lineNum);

};

#endif