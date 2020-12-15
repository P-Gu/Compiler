#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <exception>
#include "Token.h"
#include "Scanner.h"
#include "Nodes.h"
using namespace std;


class ParserException: public exception {
  string returnMsg;
public:
  ParserException(int lineNum, string message);
  virtual const char* what() const throw();
};

class Parser {
  Scanner* scanner;
  Token* curToken;
  //map<int, string> varTable;  Can keep track of variable types, not sure if needed
  
public:
  Parser(string filename);

  Node* Program();

  Node *Dec();

  Node* Statement();

  CompStmtNode* CompStmt();

  IfStmtNode* IfStmt();

  WhileStmtNode* WhileStmt();

  ReturnStmtNode* ReturnStmt();

  WriteStmtNode* WriteStmt();

  Node* WritelnStmt();
  
  Node* ExpressionStmt();

  Node* Expression();

  VarNode* Var();

  //CompExpNode* CompExp();

  Node* E(Node* curFactor);

  Node* T(Node* curFactor);

  Node* Factor();

  ArrExpNode* ArrExp(VarNode* varNode);

  FunCallNode* FunCall(VarNode* varNode);

  AddrExpNode* AddrExp();

  DerefExpNode* DerefExp();

  IntExpNode* IntExp();

  StrExpNode* StrExp();

  Node* Read();

  void GetNextToken();

  void expect(int tokenKind, string message);
};

#endif