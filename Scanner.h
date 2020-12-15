#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Token.h"
using namespace std;

class Scanner {
  ifstream file;
  int lineNum;
  string line;
  int index;
  bool hasNext;
  
public:
  Token* getNextToken();
  bool hasNextToken();
  Scanner(string filename);
};

#endif