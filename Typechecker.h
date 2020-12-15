#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <exception>
#include <unordered_map> 
#include "Token.h"
#include "Scanner.h"
#include "Nodes.h"
#include "Parser.h"
using namespace std;



class Typechecker {
  unordered_map<string, Node*> symbolTable;
  int f_count;
public:
  Parser* parser;
  Typechecker(string filename);
  void FindReferencesDec(Node *tree);
  void FindReferencesParams(Node *params, int f_count);
  void FindReferencesComp(Node *compStmtNode, string prefix);
};

#endif