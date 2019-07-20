#include "token.h"

Token::Token(const QString &word, TokenType type, long position){
    this->cPos = position;
    this->cType = type;
    this->cWord = word;
}

Token::Token(const Token &other){
    this->cPos = other.position();
    this->cType = other.type();
    this->cWord = other.word();
}

Token::~Token(){

}

void Token::operator=(const Token &other){
    this->cPos = other.position();
    this->cType = other.type();
    this->cWord = other.word();
}
