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