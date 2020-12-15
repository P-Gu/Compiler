#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <bits/stdc++.h> 
#include <exception>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Nodes.h"
using namespace std;

Parser::Parser(string filename) {
    this->scanner = new Scanner(filename);
    if (this->scanner->hasNextToken()){
        GetNextToken();
    } else {
        this->curToken = NULL;
    }
}

/*bool Parser::checkKind(int tokenKind) {
    return this->curToken->getKind() == tokenKind;
}*/

Node* Parser::Program() {
    if (this->curToken != NULL&&this->curToken->getKind() != Token::ENDOFF){
        Node *headDec = Dec(), *curDec = headDec;
        if (curDec->kind==Node::VARDEC||curDec->kind==Node::ARRDEC){
            expect(Token::SCOLON, "Expected ';'");
            GetNextToken();
        }
        while (this->curToken->getKind()!=Token::ENDOFF) {
            curDec->next = Dec();
            curDec = curDec->next;
            if (curDec->kind==Node::VARDEC||curDec->kind==Node::ARRDEC){
                expect(Token::SCOLON, "Expected ';'");
                GetNextToken();
            }
            cout << this->curToken->getKind();
        }
        return headDec;
    } else {
        cout << "Parsed zero tokens, check input file." << endl;
        Node* none = new Node(-1, Token::IGNORE, NULL);
        return none;
    }
}

Node *Parser::Dec() {  // After it returns, the curToken is either ';' or ','
    string type = "";
    bool isPointer = false;
    string id = "";
    Node *params;
    CompStmtNode *compStmtNode;
    Node *decNode;
    if (this->curToken->getKind()==Token::INT) {
        type = "int";
    } else if (this->curToken->getKind()==Token::STRING) {
        type = "string";
    } else if (this->curToken->getKind()==Token::VOID) {
        type = "void";
    } else throw ParserException(this->curToken->getLineNum(), "Expected type speficier");
    GetNextToken();
    if (this->curToken->getKind()==Token::MULTI) {
        isPointer = true;
        GetNextToken();
    }
    expect(Token::IDENTI, "Expected identifier");
    id = this->curToken->getValue();
    GetNextToken();
    if (this->curToken->getKind()==Token::LSQUAR) {
        GetNextToken();
        if (this->curToken->getKind()==Token::NUMBER) {
            int length = stoi(this->curToken->getValue());
            GetNextToken();
            expect(Token::RSQUAR, "Expected ']'");
            GetNextToken();
            decNode = new ArrDecNode(this->curToken->getLineNum(), Node::ARRDEC, NULL, type, isPointer, id, length);
        } else {
            expect(Token::RSQUAR, "Expected ']'");
            GetNextToken();
            decNode = new ArrDecNode(this->curToken->getLineNum(), Node::ARRDEC, NULL, type, isPointer, id, 0);
        }
    } else if (this->curToken->getKind()==Token::LPAREN) { // It is a funDecNode
        GetNextToken();
        if (this->curToken->getKind()==Token::VOID) {
            params = NULL;
            GetNextToken();
        } else {
            params = Dec();
            Node *curParam = params;
            while (this->curToken->getKind()==Token::COMMA) {
                GetNextToken();
                curParam->next = Dec();
                curParam = curParam->next;
            }
        }
        expect(Token::RPAREN, "Expected ')'");
        GetNextToken();
        compStmtNode = CompStmt();
        return new FunDecNode(this->curToken->getLineNum(), Node::FUNDEC, NULL, type, id, params, compStmtNode);
    } else {
        decNode = new VarDecNode(this->curToken->getLineNum(), Node::VARDEC, NULL, type, isPointer, id);
    }
    if (this->curToken->getKind()!=Token::SCOLON&&this->curToken->getKind()!=Token::COMMA&&this->curToken->getKind()!=Token::RPAREN) {
        throw new ParserException(this->curToken->getLineNum(), "Declaration didn't end properly");
    }
    return decNode;
}

Node* Parser::Statement(){
    if (this->curToken->getKind()==Token::LCURLY) {
        return CompStmt();
    } else if (this->curToken->getKind()==Token::IF) {
        return IfStmt();
    } else if (this->curToken->getKind()==Token::WHILE) {
        return WhileStmt();
    } else if (this->curToken->getKind()==Token::RETURN) {
        return ReturnStmt();
    } else if (this->curToken->getKind()==Token::WRITE) {
        return WriteStmt();
    } else if (this->curToken->getKind()==Token::WRITELN) {
        return WritelnStmt();
    }
    return ExpressionStmt();
}

CompStmtNode* Parser::CompStmt(){
    CompStmtNode* compStmtNode = new CompStmtNode (this->curToken->getLineNum(), Node::IF, NULL, NULL, NULL);
    GetNextToken();
    if (this->curToken->getKind()>=Token::INT&&this->curToken->getKind()<=Token::STRING) {
        Node *head = Dec(), *cur = head;
        expect(Token::SCOLON, "Expected ';'");
        GetNextToken();
        while (this->curToken->getKind()>=Token::INT&&this->curToken->getKind()<=Token::STRING) {
            cur->next = Dec();
            cur = cur->next;
            expect(Token::SCOLON, "Expected ';'");
            GetNextToken();
        }
        compStmtNode->varDecNode = head;
    }
    Node *headStmt = Statement(), *curStmt = headStmt;
    while (this->curToken->getKind()!=Token::RCURLY) {
        curStmt->next = Statement();
        curStmt = curStmt->next;
    }
    GetNextToken();
    compStmtNode->stmtNode = headStmt;
    return compStmtNode;
}

IfStmtNode* Parser::IfStmt(){
    IfStmtNode* ifStmtNode = new IfStmtNode (this->curToken->getLineNum(), Node::IF, NULL, NULL, NULL, NULL);
    GetNextToken();
    expect(Token::LPAREN, "Expected '('");
    GetNextToken();
    ifStmtNode->expNode = Expression();
    expect(Token::RPAREN, "Expected ')'");
    GetNextToken();
    ifStmtNode->stmtNode = Statement();
    if (this->curToken->getKind()==Token::ELSE) ifStmtNode->elseStmtNode = Statement();
    return ifStmtNode;
}

WhileStmtNode* Parser::WhileStmt(){
    WhileStmtNode* whileStmtNode = new WhileStmtNode (this->curToken->getLineNum(), Node::WHILE, NULL, NULL, NULL);
    GetNextToken();
    expect(Token::LPAREN, "Expected '('");
    GetNextToken();
    whileStmtNode->expNode = Expression();
    expect(Token::RPAREN, "Expected ')'");
    GetNextToken();
    whileStmtNode->stmtNode = Statement();
    return whileStmtNode;
}

ReturnStmtNode* Parser::ReturnStmt(){
    ReturnStmtNode* returnStmtNode = new ReturnStmtNode (this->curToken->getLineNum(), Node::RETURN, NULL, NULL);
    GetNextToken();
    if (this->curToken->getKind()==Token::SCOLON) return returnStmtNode;
    returnStmtNode->expNode = Expression();
    expect(Token::SCOLON, "Expected ';'");
    GetNextToken();
    return returnStmtNode;
}

WriteStmtNode* Parser::WriteStmt(){
    WriteStmtNode* writeStmtNode = new WriteStmtNode (this->curToken->getLineNum(), Node::WRITE, NULL, NULL);
    GetNextToken();
    expect(Token::LPAREN, "Expected '('");
    GetNextToken();
    writeStmtNode->expNode = Expression();
    expect(Token::RPAREN, "Expected ')'");
    GetNextToken();
    expect(Token::SCOLON, "Expected ';'");
    GetNextToken();
    return writeStmtNode;
}

Node* Parser::WritelnStmt(){
    Node* writelnNode = new Node (this->curToken->getLineNum(), Node::WRITELN, NULL);
    GetNextToken();
    expect(Token::LPAREN, "Expected '('");
    GetNextToken();
    expect(Token::RPAREN, "Expected ')'");
    GetNextToken();
    expect(Token::SCOLON, "Expected ';'");
    GetNextToken();
    return writelnNode;
}
  
Node* Parser::ExpressionStmt(){
    Node* expNode = Expression();
    expect(Token::SCOLON, "Expected ';'");
    GetNextToken();
    return expNode;
}

Node* Parser::Expression(){
    Node* curFactor = Factor();
    int lineNum = this->curToken->getLineNum();
    string value = this->curToken->getValue();
    OpExpNode* rootOpExp = NULL;
    OpExpNode* curOpExp = NULL;
    while (this->curToken->getKind()==Token::OEQUAL) {
        if (rootOpExp == NULL) {
            rootOpExp = new OpExpNode(this->curToken->getLineNum(), Node::OPEXP, NULL, "=", curFactor, NULL);
            curOpExp = rootOpExp;
        } else {
            curOpExp->expNode2 = new OpExpNode(this->curToken->getLineNum(), Node::OPEXP, NULL, "=", curFactor, NULL);
            curOpExp = (OpExpNode *)curOpExp->expNode2;
        }
        GetNextToken();
        curFactor = Factor();
    }
    
    if (this->curToken->getKind()==Token::SCOLON) {
        if (rootOpExp!=NULL) {
            curOpExp->expNode2 = curFactor;
            return rootOpExp;
        } else {
            return curFactor;
        }
    }
    
    Node* e = E(curFactor);
    if (this->curToken->getKind()>=Token::LESS&&this->curToken->getKind()<=Token::GREAT) {
        OpExpNode* relOpExp = new OpExpNode(this->curToken->getLineNum(), Node::RELOPEXP, NULL, this->curToken->getValue(), e, NULL);
        GetNextToken();
        relOpExp->expNode2 = E(Factor());
        return relOpExp;
    } else {
        if (rootOpExp!=NULL) {
            curOpExp->expNode2 = e;
            return rootOpExp;
        } else {
            return e;
        }
    }

}

VarNode* Parser::Var() {
    VarNode *varNode = new VarNode(this->curToken->getLineNum(), Node::VAR, NULL, this->curToken->getValue());
    GetNextToken();
    return varNode;
}

Node* Parser::E(Node* curFactor){
    Node* t = T(curFactor);
    OpExpNode* rootOpExp = NULL;
    OpExpNode *curOpExp = NULL;
    OpExpNode *newOpExp = NULL;
    while (this->curToken->getKind()!=Token::SCOLON&&this->curToken->getKind()!=Token::COMMA&&this->curToken->getKind()!=Token::RSQUAR&&this->curToken->getKind()!=Token::RPAREN&&!(this->curToken->getKind()>=Token::LESS&&this->curToken->getKind()<=Token::GREAT)) {
        if (this->curToken->getKind()==Token::MULTI||this->curToken->getKind()==Token::DIVIDE||this->curToken->getKind()==Token::PERCEN) {
            if (rootOpExp == NULL) {
                rootOpExp = new OpExpNode(this->curToken->getLineNum(), Node::MULTOPEXP, NULL, this->curToken->getValue(), t, NULL);
                curOpExp = rootOpExp;
            } else {
                newOpExp = new OpExpNode(this->curToken->getLineNum(), Node::MULTOPEXP, NULL, this->curToken->getValue(), curOpExp, NULL);
                curOpExp = newOpExp;
            }
            GetNextToken();
            curFactor = Factor();
            curOpExp->expNode2 = T(curFactor);
        } else {
            throw new ParserException(this->curToken->getLineNum(), "Cannot parse E node");
        }
    }
    return (rootOpExp==NULL ? t : rootOpExp);
}

Node* Parser::T(Node* curFactor){
    OpExpNode* rootOpExp = NULL, *curOpExp = NULL, *newOpExp = NULL;
    while (this->curToken->getKind()!=Token::SCOLON&&this->curToken->getKind()!=Token::COMMA&&this->curToken->getKind()!=Token::RSQUAR&&this->curToken->getKind()!=Token::RPAREN&&
    !(this->curToken->getKind()>=Token::LESS&&this->curToken->getKind()<=Token::GREAT)&&this->curToken->getKind()!=Token::DIVIDE&&this->curToken->getKind()!=Token::MULTI&&this->curToken->getKind()!=Token::PERCEN) {
        if (this->curToken->getKind()==Token::PLUS||this->curToken->getKind()==Token::MINUS) {
            if (rootOpExp == NULL) {
                rootOpExp = new OpExpNode(this->curToken->getLineNum(), Node::ADDOPEXP, NULL, this->curToken->getValue(), curFactor, NULL);
                curOpExp = rootOpExp;
            } else {
                newOpExp = new OpExpNode(this->curToken->getLineNum(), Node::ADDOPEXP, NULL, this->curToken->getValue(), curOpExp, NULL);
                curOpExp = newOpExp;
            }
            GetNextToken();
            curOpExp->expNode2 = Factor();
        } else {
            throw new ParserException(this->curToken->getLineNum(), "Cannot parse T node");
        }
    }
    return (rootOpExp==NULL ? curFactor : rootOpExp);
}

Node* Parser::Factor(){  // This method reads basic individual expression nodes
    if (this->curToken->getKind()==Token::LPAREN) { // -> (EXPRESSION)
        GetNextToken();
        Node* expNode = Expression();
        expect(Token::RPAREN, "Expected ')'");
        GetNextToken();
        return expNode;
    }
    else if (this->curToken->getKind()==Token::READ) { // -> read()
        return Read();
    } else if (this->curToken->getKind()==Token::MULTI) { // -> *Factor
        return AddrExp();
    } else if (this->curToken->getKind()==Token::AND) { // -> &Factor
        return DerefExp();
    } else if (this->curToken->getKind()==Token::NUMBER) { // -> <num>
        return IntExp();
    } else if (this->curToken->getKind()==Token::DQUOTE) { // -> <string>
        return StrExp();
    } else if (this->curToken->getKind()==Token::IDENTI) { // -> FUN_CALL or <id>[expression] or <id>
        VarNode* varNode = Var();
        if (this->curToken->getKind()==Token::LSQUAR) {
            return ArrExp(varNode);
        } else if (this->curToken->getKind()==Token::LPAREN) {
            return FunCall(varNode);
        }
        return varNode;
    } else {
        throw new ParserException(this->curToken->getLineNum(), "Cannot parse Factor");
    }
}

ArrExpNode* Parser::ArrExp(VarNode* varNode){
    ArrExpNode* arrExpNode = new ArrExpNode(varNode->lineNum, Node::ARREXP, NULL, varNode, NULL);
    GetNextToken();
    arrExpNode->expNode = Expression();
    expect(Token::RSQUAR, "Expected ']'");
    GetNextToken();
    return arrExpNode;
}

FunCallNode* Parser::FunCall(VarNode* varNode){
    FunCallNode* funCallNode = new FunCallNode(varNode->lineNum, Node::FUNCALL, NULL, varNode, NULL);
    GetNextToken();
    Node *headExp = Expression();
    Node *curExp = headExp;
    while (this->curToken->getKind()==Token::COMMA) {
        GetNextToken();
        curExp->next = Expression();
        curExp = curExp->next;
    }
    expect(Token::RPAREN, "Expected ')'");
    GetNextToken();
    return funCallNode;
}

AddrExpNode* Parser::AddrExp(){
    AddrExpNode* addrExpNode = new AddrExpNode(this->curToken->getLineNum(), Node::ADDREXP, NULL, NULL);
    GetNextToken();
    addrExpNode->expNode = Factor();
    return addrExpNode;
}

DerefExpNode* Parser::DerefExp(){
    DerefExpNode* derefExpNode = new DerefExpNode(this->curToken->getLineNum(), Node::DEREFEXP, NULL, NULL);
    GetNextToken();
    derefExpNode->expNode = Factor();
    return derefExpNode;
}

Node* Parser::Read(){
    Node* readNode = new Node(this->curToken->getLineNum(), Node::READEXP, NULL);
    GetNextToken();
    expect(Token::LPAREN, "Expected '('");
    GetNextToken();
    expect(Token::RPAREN, "Expected ')'");
    GetNextToken();
    return readNode;
}

IntExpNode* Parser::IntExp() {
    IntExpNode* intExpNode = new IntExpNode(this->curToken->getLineNum(), Node::STREXP, NULL, 0);
    expect(Token::NUMBER, "Expected an integer");
    intExpNode->val = stoi(this->curToken->getValue());
    GetNextToken();
    return intExpNode;
}

StrExpNode* Parser::StrExp() {
    StrExpNode* strExpNode = new StrExpNode(this->curToken->getLineNum(), Node::STREXP, NULL, "");
    expect(Token::DQUOTE, "Expected '\"'");
    GetNextToken();
    expect(Token::IDENTI, "Expected a string after '\"'");
    strExpNode->str = this->curToken->getValue();
    GetNextToken();
    expect(Token::DQUOTE, "Expected '\"'");
    GetNextToken();
    return strExpNode;
}

void Parser::GetNextToken() {
    this->curToken = scanner->getNextToken();
    cout << this->curToken->toString() << "\n";
}

ParserException::ParserException(int lineNum, string message):returnMsg(message+" on line "+to_string(lineNum)){}

const char* ParserException::what() const throw(){
    return returnMsg.c_str();
}

void Parser::expect(int tokenKind, string message) {
    if (this->curToken->getKind()!=tokenKind) {
        throw new ParserException(this->curToken->getLineNum(), message);
    }
}

/*int main () {
    Scanner* s = new Scanner("example2.bpl");

    Token* cur;
    while (s->hasNextToken()) {
        cur = s->getNextToken();
        cout << cur->toString() << '\n';
    }
    cout << "\n\n\n";
    try {
        Parser* parser = new Parser("example.bpl");
        Node* root = parser->Program();
        Node *voidFun = root->next;
        Node *voidFun2 = voidFun->next;
        if (voidFun2->next==NULL) cout << "good\n";

        return 0;
    } catch (ParserException* e) {
        cout << e->what() << "\n";
    }
}*/

