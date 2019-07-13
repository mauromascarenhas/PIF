#include "token.h"

Token::Token(const std::string &token, TokenType type){
    this->word = token;
    this->currentType = type;
}

Token::~Token(){}

void Token::setToken(const std::string &token){
    this->word = token;
}

void Token::setType(TokenType type){
    this->currentType = type;
}

std::string Token::token(){
    return this->token;
}

Token::TokenType Token::type(){
    return this->currentType;
}