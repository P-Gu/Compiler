#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Token.h"
#include "Scanner.h"
using namespace std;

Token::Token() {
  this->value = "";
  this->kind = this->UNDEFI;
  this->lineNum = -1;
}

Token::Token(string value, int kind, int lineNum) {
  this->value = value;
  this->kind = kind;
  this->lineNum = lineNum;
}

char Token::specialChars[] = {
  '[',']','{','}','(',')','\'', '\"', ',', ';', '<', '=', '>', '!', '+', '-', '*', '/', '%', '&'
};

string Token::specialSymbols[] = {
  "int", "void", "string", "if", "else", "while", "return", "write", "writeln", "read"
};

string Token::kinds[] = {
  "IDENTI", "LSQUAR", "RSQUAR", "LCURLY", "RCURLY", "LPAREN", "RPAREN",
  "LESS", "LEQUAL", "TEQUAL", "NEQUAL", "GEQUAL", "GREAT", "PLUS", "MINUS", "MULTI", "DIVIDE", "PERCEN", "AND",
  "OR", "SCOLON", "COMMA", "UNDEFI", "ENDOFF", "KEYWOR", "NUMBER", "OEQUAL", "SQUOTE", "DQUOTE", "NOT", "INT", "VOID", "STRING",
  "IF", "ELSE", "WHILE", "RETURN", "WRITE", "WRITELN", "READ", "IGNORE"
};

string Token::toString(){
  return "KIND: " + this->kinds[this->kind] + " Value: " + this->value + " Line Number: " + to_string(this->lineNum);
}

int Token::getKind() {
  return this->kind;
}

int Token::getLineNum(){
  return this->lineNum;
}

string Token::getValue(){
  return this->value;
}

Scanner::Scanner(string filename) {
  this->file.open(filename);
  this->lineNum = 1;
  this->index = 0;
  getline(this->file, this->line);
  this->hasNext = true;
}


bool Scanner::hasNextToken() {
  return this->hasNext;
}

Token* Scanner::getNextToken() {
  string value = "";
  Token* token;
  char *charFound;
  string *strFound;
  while (this->index<this->line.length()&&this->line.at(this->index)==' ') {
    this->index++;
  }
  while (this->index>=this->line.length()) {
    this->index = 0;
    this->lineNum++;
    if (this->file.peek()!=EOF) {
      getline(this->file, this->line);
      while (this->index<this->line.length()&&this->line.at(this->index)==' ') {
        this->index++;
      }
    } else {
      token = new Token("", Token::ENDOFF, this->lineNum);
      this->hasNext = false;
      this->file.close();
      return token;
    }
  }
  charFound = find(begin(Token::specialChars), end(Token::specialChars), this->line.at(this->index));
  if (charFound != end(Token::specialChars)||this->line.at(this->index)=='&') {
    if (this->line.at(this->index)=='(' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::LPAREN, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)==')' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::RPAREN, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='[' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::LSQUAR, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)==']' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::RSQUAR, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='{' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::LCURLY, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='}' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::RCURLY, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)==',' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::COMMA, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)==';' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::SCOLON, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='\'' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::SQUOTE, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='\"' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::DQUOTE, this->lineNum);
      return token;
    }

    if (this->line.at(this->index)=='+' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::PLUS, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='-' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::MINUS, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='*' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::MULTI, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='/' ){
      if (this->line.at(this->index+1)=='*') {
        this->index += 2;
        while (this->index<this->line.length()) {
          if (index>0&&this->line.at(this->index)=='/'&&this->line.at(this->index-1)=='*'){
            this->index++;
            return getNextToken();
          }
          this->index++;
        }
        while (this->index>=this->line.length()) {
          this->index = 0;
          this->lineNum++;
          if (this->file.peek()!=EOF) {
            getline(this->file, this->line);
            while (this->index<this->line.length()) {
              this->index++;
              if (index>0&&this->line.at(this->index)=='/'&&this->line.at(this->index-1)=='*'){
                this->index++;
                return getNextToken();
              }
            }
          } else {
            token = new Token("", Token::ENDOFF, this->lineNum);
            this->hasNext = false;
            this->file.close();
            return token;
          }
        }
      }
      else {
        value += this->line.at(this->index);
        this->index++;
        token = new Token(value, Token::DIVIDE, this->lineNum);
        return token;
      }
    }
    if (this->line.at(this->index)=='&' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::AND, this->lineNum);
      return token;
    }
    if (this->line.at(this->index)=='|' ){
      value += this->line.at(this->index);
      this->index++;
      token = new Token(value, Token::OR, this->lineNum);
      return token;
    }

    if (this->line.at(this->index)=='=' ){
      if (this->line.at(this->index+1)=='=') {
        this->index += 2;
        value = "==";
        token = new Token(value, Token::TEQUAL, this->lineNum); 
      }
      else {
        this->index++;
        value = "=";
        token = new Token(value, Token::OEQUAL, this->lineNum); 
      }
      return token;
    }
    if (this->line.at(this->index)=='<' ){
      if (this->line.at(this->index+1)=='=') {
        this->index += 2;
        value = "<=";
        token = new Token(value, Token::LEQUAL, this->lineNum); 
      }
      else {
        this->index++;
        value = "<";
        token = new Token(value, Token::LESS, this->lineNum); 
      }
      return token;
    }
    if (this->line.at(this->index)=='>' ){
      if (this->line.at(this->index+1)=='=') {
        this->index += 2;
        value = ">=";
        token = new Token(value, Token::GEQUAL, this->lineNum); 
      }
      else {
        this->index++;
        value = ">";
        token = new Token(value, Token::GREAT, this->lineNum); 
      }
      return token;
    }
    if (this->line.at(this->index)=='!' ){
      if (this->line.at(this->index+1)=='=') {
        this->index += 2;
        value = "!=";
        token = new Token(value, Token::NEQUAL, this->lineNum); 
      }
      else {
        this->index++;
        value = "!";
        token = new Token(value, Token::NOT, this->lineNum); 
      }
      return token;
    }
  }
  else {
    while (this->index<this->line.length()&&this->line.at(this->index)!=' ') {
      charFound = find(begin(Token::specialChars), end(Token::specialChars), this->line.at(this->index));
      if (charFound != end(Token::specialChars)||this->line.at(this->index)=='&') break;
      value += this->line.at(this->index);
      this->index++;
    }
    strFound = find(begin(Token::specialSymbols), end(Token::specialSymbols), value);
    if (strFound != end(Token::specialSymbols)||value.compare("READ")==0){//Token::specialSymbols[end(Token::specialSymbols)])) {
      token = new Token(value, distance(begin(Token::specialSymbols), strFound)+30, this->lineNum);
    
    } else {
      for (int i=0; i<value.length(); i++) {
        if (!isdigit(value.at(i))) {
          token = new Token (value, Token::IDENTI, this->lineNum);
          return token;
        }
      }
      token = new Token (value, Token::NUMBER, this->lineNum);
    }
    return token;
  }
  
  token = new Token(value, Token::KEYWOR, this->lineNum);
  return token;
}


/*int main () {
  string line;

  Scanner* s = new Scanner("example2.bpl");

  Token* cur;
  while (s->hasNextToken()) {
    cur = s->getNextToken();
    cout << cur->toString() << '\n';
  }

  return 0;
}*/
