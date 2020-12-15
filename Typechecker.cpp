#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <exception>
#include <unordered_map> 
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Nodes.h"
#include "Parser.h"
#include "Typechecker.h"
using namespace std;

Typechecker::Typechecker(string filename) {
    this->parser = new Parser(filename);
    this->f_count = 0;
}



// The keys in symbolTable are formatted, for example, as "t|x" or "f1|y" or "c1,2|z"
// "t|x" indicates x is a variable on top level
// "f1|y" indicates y is a parameter to the first function of the program
// "c1,2|z" indicates z is a variable in the second compound statement of the first compound statement of the program (Actually, the compound statements here can also be if or while statements)
//
void Typechecker::FindReferencesDec(Node *tree) {
    if (tree->kind==Node::VARDEC) {
        symbolTable["t|"+((VarDecNode *)tree)->id] = tree;
    } else if (tree->kind==Node::FUNDEC) {
        this->f_count++;
        symbolTable["t|"+((FunDecNode *)tree)->id] = tree;
        FindReferencesParams(((FunDecNode *)tree)->params, this->f_count);
        FindReferencesComp(((FunDecNode *)tree)->compStmtNode, "c"+to_string(this->f_count));
    }
    if (tree->next!=NULL) {
        FindReferencesDec(tree->next);
    }
}

void Typechecker::FindReferencesParams(Node *params, int f_count){
    while (params!=NULL) {
        symbolTable["f"+to_string(f_count)+"|"+((VarDecNode *)params)->id] = params;
        params = params ->next;
    }
}

void Typechecker::FindReferencesComp(Node *compStmtNode, string prefix){
    int c_count = 1;
    Node *varDecNode = ((CompStmtNode *)compStmtNode)->varDecNode;
    Node *stmtNode = ((CompStmtNode *)compStmtNode)->stmtNode;
    while (varDecNode!=NULL) {
        symbolTable[prefix+"|"+((VarDecNode *)varDecNode)->id] = varDecNode;
        varDecNode = varDecNode ->next;
    }
    while (stmtNode!=NULL) {
        if (stmtNode->kind==Node::COMPSTMT) {
            FindReferencesComp(stmtNode, prefix+","+to_string(c_count));
            c_count++;
        }
        else if (stmtNode->kind==Node::IF) {
            FindReferencesComp(((IfStmtNode *)stmtNode)->stmtNode, prefix+","+to_string(c_count));
            c_count++;
        } else if (stmtNode->kind==Node::WHILE) {
            FindReferencesComp(((WhileStmtNode *)stmtNode)->stmtNode, prefix+","+to_string(c_count));
            c_count++;
        }
        stmtNode = stmtNode->next;
    }
}


bool Typechecker::CheckDec(Node *tree) {
    while (tree!=null) {
        if (tree->kind==Node::FUNDEC) {
            if (!CheckFunc(Node *tree)) return false;
        }
        tree = tree->next;
    }
    return true;
}

bool Typechecker::CheckFunc(Node *tree) {
    string exp_check = CheckExp(((FunDecNode *)tree)->params);
    if (exp_check.compare("int")==0) {
        StmtNode *stmtNode = ((FunDecNode *)tree)->compStmtNode;
        while (stmtNode!=null) {
            stmtNode = stmtNode->next;
        }
    } else if (exp_check.compare("str")==0) {

    }
    return true;
}

string Typechecker::CheckExp(Node *tree, string prefix) {
    if (tree->kind==Node::OPEXP) {
        OpExpNode *opExpNode = (OpExpNode *)tree;
        if (opExpNode->expNode!=NULL) {
            if (CheckExp(opExpNode->expNode).compare("int")==0) {
                if (opExpNode->expNode2!=NULL) {
                    if (CheckExp(opExpNode2->expNode).compare("int")==0) {
                        return "int";
                    } else throw exception;
                }
                return "int";
            }
            else if (CheckExp(opExpNode->expNode).compare("str")==0) {
                if (opExpNode->expNode2!=NULL) {
                    if (CheckExp(opExpNode2->expNode).compare("str")==0) {
                        return "str";
                    } else throw exception;
                }
                return "str";
            }
        }
        
    }
    else if (tree->kind==Node::ENODE) {

    }
    return true;
}



int main () {
    try {
        Typechecker* checker = new Typechecker("example2.bpl");
        Node* root = checker->parser->Program();
        checker->FindReferencesDec(root);
        //cout << root->kind << "\n";
        return 0;
    } catch (ParserException* e) {
        cout << e->what() << "\n";
    }
}