/*
 *     This file is part of PIFC (Compiler).
 *
 *    PIFC (Compiler) is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Foobar is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "token.h"

Token::Token(const QString &word, TokenType type,
             long position) : QObject(nullptr){
    this->cPos = position;
    this->cType = type;
    this->cWord = word;
}

Token::Token(const Token &other) : QObject(nullptr){
    this->cPos = other.position();
    this->cType = other.type();
    this->cWord = other.word();
}

Token::~Token(){

}

Token& Token::operator =(const Token &other){
    this->cPos = other.position();
    this->cType = other.type();
    this->cWord = other.word();

    return *this;
}
