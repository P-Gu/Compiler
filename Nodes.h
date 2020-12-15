#ifndef NODES_H
#define NODES_H

#include <string>
using namespace std;

class CompExpNode;
class FunCallNode;
class ExpNode;
class StrExpNode;
class VarNode;
class CompStmtNode;


class Node {
public:
  // Whenever you change something below, make sure to go to Nodes.c and change kinds[]
  static const int PROGRAM = 0, VARDEC = 1, FUNDEC = 2, ARRDEC = 3, WRITELN = 4, WRITE = 5, READSTMT = 6, RETURN = 7, COMPSTMT = 8,
    WHILE = 9, IF = 10, EXPSTMT = 11, EXP = 12, STMT = 13, DEC = 14, RELOPEXP = 15, ADDOPEXP = 16, VAR = 17, OPEXP = 18,
    FUNCALL = 19, READEXP = 20, ADDREXP = 21, DEREFEXP = 22, COMPEXP = 23, MULTOPEXP = 24, FNODE = 25, TNODE = 26, ENODE = 27,
    FACTOR = 28, INTEXP = 29, STREXP = 30, ARREXP = 31;
  
  static string kinds[];
  Node* next;
  int lineNum;
  int kind;

  Node(int lineNum, int kind, Node* next);

  void printNode();
};

//
// Declaration Node:
//

class VarDecNode: public Node {
public:
  string type;
  bool isPointer;
  string id;
  VarDecNode(int lineNum, int kind, Node* next, string type, bool isPointer, string id);
  //void printNode();
};

class FunDecNode: public Node {
public:
  string type;
  string id;
  Node* params;
  CompStmtNode *compStmtNode;
  //void printNode();
  FunDecNode(int lineNum, int kind, Node* next, string type, string id, Node* params, CompStmtNode *compStmtNode);
};

class ArrDecNode: public VarDecNode {
public:
  int size;
  //void printNode();
  ArrDecNode(int lineNum, int kind, Node* next, string type, bool isPointer, string var, int size);
};

class DecNode: public Node {
  VarDecNode* varDecNode;
  FunDecNode* funDecNode;

public:
  //void printNode();
  DecNode(int lineNum, int kind, Node* next, VarDecNode* varDecNode, FunDecNode* funDecNode);
};

//
// Statement Nodes
//


class WriteStmtNode: public Node {
public:
  Node* expNode;
  //void printNode();
  WriteStmtNode(int lineNum, int kind, Node* next, Node* expNode);
};

class ReturnStmtNode: public Node {
public:
  Node* expNode;
  //void printNode();
  ReturnStmtNode(int lineNum, int kind, Node* next, Node* expNode);
};

class CompStmtNode: public Node {
public:
  Node* stmtNode;
  Node* varDecNode;
  //void printNode();
  CompStmtNode(int lineNum, int kind, Node* next, Node* stmtNode, Node* varDecNode);
};


class WhileStmtNode: public Node {
public:
  Node* expNode;
  Node* stmtNode;
  //void printNode();
  WhileStmtNode(int lineNum, int kind, Node* next, Node* expNode, Node* stmtNode);
};

class IfStmtNode: public Node {
public:
  //void printNode();
  Node* expNode;
  Node* stmtNode;
  Node* elseStmtNode;
  IfStmtNode(int lineNum, int kind, Node* next, Node* expNode, Node* stmtNode, Node* elseStmtNode);
};

class ExpStmtNode: public Node {
public:
  Node* expNode;

  void printNode();
  ExpStmtNode(int lineNum, int kind, Node* next, Node* expNode);
};

class StmtNode: public Node {
public:
  //void printNode();
  Node* stmtNode;
  StmtNode(int lineNum, int kind, Node* next, Node* stmtNode);
};


//
// Expression Nodes
//

class VarNode: public Node {
public:
  string id;

  void printNode();
  VarNode(int lineNum, int kind, Node* next, string id);
};

class ExpNode: public Node {
public:
  VarNode* varNode;
  ExpNode* expNode;
  CompExpNode* compExpNode;
  
  void printNode();
  ExpNode(int lineNum, int kind, Node* next, VarNode* varNode, ExpNode* expNode, CompExpNode* compExpNode);
};

class AddOpExpNode: public Node {
  char op;
public:

  void printNode();
  AddOpExpNode(int lineNum, int kind, Node* next, char op);
};

class MultOpExpNode: public Node {
  char op;
public:
  
  //void printNode();
  MultOpExpNode(int lineNum, int kind, Node* next, char op);
};

class RelOpExpNode: public Node {

public:
  string op;
  
  void printNode();
  RelOpExpNode(int lineNum, int kind, Node* next, string op);
};

class OpExpNode: public Node {

public:
  string op;
  Node* expNode;
  Node* expNode2;
  
  //void printNode();
  OpExpNode(int lineNum, int kind, Node* next, string op, Node* expNode, Node* expNode2);
};

class FactorNode: public Node {
public:
  ExpNode* exp;
  FunCallNode* funCall;
  Node* read;
  bool isPointer;
  string id;
  string str;
  int num;
  
  void printNode();
  FactorNode(int lineNum, int kind, Node* next, ExpNode* exp, FunCallNode* funCall, Node* read, bool isPointer, string id, string str, int num);
};

class FNode: public Node {
public:
  FactorNode* factor;
  bool isPointer;
  bool isAddr;
  bool minus;
  
  void printNode();
  FNode(int lineNum, int kind, Node* next, FactorNode* factor, bool minus, bool isPointer, bool isAddr);
};

class TNode: public Node {
public:
  TNode* t;
  FNode* f;
  MultOpExpNode* multOpExpNode;
  
  void printNode();
  TNode(int lineNum, int kind, Node* next, TNode* t, MultOpExpNode* multOpExpNode, FNode* f);
};

class ENode: public Node {
public:
  ENode* e;
  TNode* t;
  AddOpExpNode* addOpExpNode;
  
  void printNode();
  ENode(int lineNum, int kind, Node* next, ENode* e, AddOpExpNode* addOpExpNode, TNode* t);
};

class CompExpNode: public Node {
public:
  ENode* e;
  ENode* secE;
  RelOpExpNode* relOpExpNode;
  
  void printNode();
  CompExpNode(int lineNum, int kind, Node* next, ENode* e, RelOpExpNode* relOpExpNode, ENode* secE);
};

class FunCallNode: public Node {

public:
  VarNode* varNode;
  Node* expNode;
  //void printNode();
  FunCallNode(int lineNum, int kind, Node* next, VarNode* varNode, Node* expNode);
};

class ArrExpNode: public Node {

public:
  VarNode* varNode;
  Node* expNode;
  //void printNode();
  ArrExpNode(int lineNum, int kind, Node* next, VarNode* varNode, Node* expNode);
};

class AddrExpNode: public Node {
public:
  Node* expNode;
  //void printNode();
  AddrExpNode(int lineNum, int kind, Node* next, Node* expNode);
};

class DerefExpNode: public Node {
public:
  Node* expNode;
  //void printNode();
  DerefExpNode(int lineNum, int kind, Node* next, Node* expNode);
};

class IntExpNode: public Node {
public:

  int val;
  //void printNode();
  IntExpNode(int lineNum, int kind, Node* next, int val);
};

class StrExpNode: public Node {
public:

  string str;
  //void printNode();
  StrExpNode(int lineNum, int kind, Node* next, string str);
};



#endif