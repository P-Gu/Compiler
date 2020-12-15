#include <iostream>
#include <fstream>
#include <string>
#include "Nodes.h"
using namespace std;

string Node::kinds[] = {
  "PROGRAM", "VARDEC", "FUNDEC", "ARRDEC", "WRITELN", "WRITE", "READSTMT", "RETURN", "COMPSTMT", "WHILE", "IF", "EXPSTMT", 
  "EXP", "STMT", "DEC", "RELOPEXP", "ADDOPEXP", "VAR", "OPEXP", "FUNCALL", "READEXP", "ADDREXP", "DEREFEXP", "COMPEXP", 
  "MULTOPEXP", "FNODE", "TNODE", "ENODE", "FACTOR", "INTEXP", "STREXP", "ARREXP"
};

Node::Node(int lineNum, int kind, Node* next) {
    this->kind = kind;
    this->lineNum = lineNum;
    this->next = next;
}

void Node::printNode(){
    cout << "Kind: " << this->kinds[this->kind] << endl;
}

//
// Declaration Node:
//

VarDecNode::VarDecNode(int lineNum, int kind, Node* next, string type, bool isPointer, string var): Node(lineNum, kind, next) {
    this->type = type;
    this->isPointer = isPointer;
    this->id = id;
}

FunDecNode::FunDecNode(int lineNum, int kind, Node* next, string type, string id, Node* params, CompStmtNode *compStmtNode): Node(lineNum, kind, next){
    this->type = type;
    this->id = id;
    this->params = params;
    this->compStmtNode = compStmtNode;
}

ArrDecNode::ArrDecNode(int lineNum, int kind, Node* next, string type, bool isPointer, string var, int size): VarDecNode(lineNum, kind, next, type, isPointer, var){
    this->size = size;
}

DecNode::DecNode(int lineNum, int kind, Node* next, VarDecNode* varDecNode, FunDecNode* funDecNode): Node(lineNum, kind, next){
    this->varDecNode = varDecNode;
    this->funDecNode = funDecNode;
}



//
// Statement Nodes
//


WriteStmtNode::WriteStmtNode(int lineNum, int kind, Node* next, Node* expNode): Node(lineNum, kind, next){
    this->expNode = expNode;
}

ReturnStmtNode::ReturnStmtNode(int lineNum, int kind, Node* next, Node* expNode): Node(lineNum, kind, next){
    this->expNode = expNode;
}

CompStmtNode::CompStmtNode(int lineNum, int kind, Node* next, Node* stmtNode, Node* varDecNode): Node(lineNum, kind, next){
    this->stmtNode = stmtNode;
    this->varDecNode = varDecNode;
}

WhileStmtNode::WhileStmtNode(int lineNum, int kind, Node* next, Node* expNode, Node* stmtNode): Node(lineNum, kind, next) {
    this->stmtNode = stmtNode;
    this->expNode = expNode;
}

IfStmtNode::IfStmtNode(int lineNum, int kind, Node* next, Node* expNode, Node* stmtNode, Node* elseStmtNode): Node(lineNum, kind, next) {
    this->expNode = expNode;
    this->stmtNode = stmtNode;
    this->elseStmtNode = elseStmtNode;
}

ExpStmtNode::ExpStmtNode(int lineNum, int kind, Node* next, Node* expNode): Node(lineNum, kind, next) {
    this->expNode = expNode;
}

void ExpStmtNode::printNode(){
    Node::printNode();

    if (this->expNode!=NULL){
        this->expNode->printNode();
    }
}

StmtNode::StmtNode(int lineNum, int kind, Node* next, Node* stmtNode): Node(lineNum, kind, next) {
    this->stmtNode = stmtNode;
}

//
// Expression Nodes
//

ExpNode::ExpNode(int lineNum, int kind, Node* next, VarNode* varNode, ExpNode* expNode, CompExpNode* compExpNode): Node(lineNum, kind, next) {
    this->varNode = varNode;
    this->expNode = expNode;
    this->compExpNode = compExpNode;
}

void ExpNode::printNode(){
    Node::printNode();
    if (this->varNode!=NULL){
        this->varNode->printNode();
    }
    if (this->expNode!=NULL){
        this->expNode->printNode();
    }
    if (this->compExpNode!=NULL){
        this->compExpNode->printNode();
    }
}


VarNode::VarNode(int lineNum, int kind, Node* next, string id): Node(lineNum, kind, next) {
    this->id = id;
}

void VarNode::printNode(){
}


AddOpExpNode::AddOpExpNode(int lineNum, int kind, Node* next, char op): Node(lineNum, kind, next) {
    this->op = op;
}

void AddOpExpNode::printNode(){
    Node::printNode();
    
    cout << this->op << endl;
}

MultOpExpNode::MultOpExpNode(int lineNum, int kind, Node* next, char op): Node(lineNum, kind, next) {
    this->op = op;
}

RelOpExpNode::RelOpExpNode(int lineNum, int kind, Node* next, string op): Node(lineNum, kind, next) {
    this->op = op;
}

void RelOpExpNode::printNode(){
    Node::printNode();
    
    cout << this->op << endl;
}

OpExpNode::OpExpNode(int lineNum, int kind, Node* next, string op, Node* expNode, Node* expNode2): Node(lineNum, kind, next) {
    this->op = op;
    this->expNode = expNode;
    this->expNode2 = expNode2;
}

FactorNode::FactorNode(int lineNum, int kind, Node* next, ExpNode* exp, FunCallNode* funCall, Node* read, bool isPointer, string id, string str, int num): Node(lineNum, kind, next) {
    this->exp = exp;     
    this->funCall = funCall;
    this->isPointer = isPointer;
    this->read = read;
    this->id = id;
    this->str = str;
    this->num = num;
}

void FactorNode::printNode(){
    Node::printNode();
    
    if (this->read!=NULL){
        this->read->printNode();
    }
    /*cout << (this->minus ? "minus" : "noMinus") << endl;
    cout << (this->isPointer ? "isPointer" : "notPointer") << endl;
    cout << (this->isAddr ? "isAddress" : "notAddress") << endl;*/
}

FNode::FNode(int lineNum, int kind, Node* next, FactorNode* factor, bool minus, bool isPointer, bool isAddr): Node(lineNum, kind, next) {
    this->factor = factor;     
    this->minus = minus;
    this->isPointer = isPointer;
    this->isAddr = isAddr;
}

void FNode::printNode(){
    Node::printNode();
    
    if (this->factor!=NULL){
        this->factor->printNode();
    }
    cout << (this->minus ? "minus" : "noMinus") << endl;
    cout << (this->isPointer ? "isPointer" : "notPointer") << endl;
    cout << (this->isAddr ? "isAddress" : "notAddress") << endl;
}

TNode::TNode(int lineNum, int kind, Node* next, TNode* t, MultOpExpNode* multOpExpNode, FNode* f): Node(lineNum, kind, next) {
    this->t = t;
    this->multOpExpNode = multOpExpNode;
    this->f = f;
}

void TNode::printNode(){
    Node::printNode();
    
    if (this->t!=NULL){
        this->t->printNode();
    }
    if (this->multOpExpNode!=NULL){
        this->multOpExpNode->printNode();
    }
    if (this->f!=NULL){
        this->f->printNode();
    }
}

ENode::ENode(int lineNum, int kind, Node* next, ENode* e, AddOpExpNode* addOpExpNode, TNode* t): Node(lineNum, kind, next) {
    this->e = e;
    this->addOpExpNode = addOpExpNode;
    this->t = t;
}

void ENode::printNode(){
    Node::printNode();
    
    if (this->e!=NULL){
        this->e->printNode();
    }
    if (this->addOpExpNode!=NULL){
        this->addOpExpNode->printNode();
    }
    if (this->t!=NULL){
        this->t->printNode();
    }
}

CompExpNode::CompExpNode(int lineNum, int kind, Node* next, ENode* e, RelOpExpNode* relOpExpNode, ENode* secE): Node(lineNum, kind, next) {
    this->e = e;
    this->secE = secE;
    this->relOpExpNode = relOpExpNode;
}

void CompExpNode::printNode(){
    Node::printNode();
    
    if (this->e!=NULL){
        this->e->printNode();
    }
    if (this->relOpExpNode!=NULL){
        this->relOpExpNode->printNode();
    }
    if (this->secE!=NULL){
        this->secE->printNode();
    }
}

FunCallNode::FunCallNode(int lineNum, int kind, Node* next, VarNode* varNode, Node* expNode): Node(lineNum, kind, next) {
    this->varNode = varNode;
    this->expNode = expNode;
}

ArrExpNode::ArrExpNode(int lineNum, int kind, Node* next, VarNode* varNode, Node* expNode): Node(lineNum, kind, next) {
    this->varNode = varNode;
    this->expNode = expNode;
}

AddrExpNode::AddrExpNode(int lineNum, int kind, Node* next, Node* expNode): Node(lineNum, kind, next) {
    this->expNode = expNode;
}

DerefExpNode::DerefExpNode(int lineNum, int kind, Node* next, Node* expNode): Node(lineNum, kind, next) {
    this->expNode = expNode;
}

IntExpNode::IntExpNode(int lineNum, int kind, Node* next, int val): Node(lineNum, kind, next) {
    this->val = val;
}

StrExpNode::StrExpNode(int lineNum, int kind, Node* next, string str): Node(lineNum, kind, next) {
    this->str = str;
}



