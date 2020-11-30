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

#ifndef TOKEN_H
#define TOKEN_H

#include <QObject>
#include <QString>

class Token : public QObject
{
    Q_OBJECT
public:
    enum TokenType{
        ARITHMETIC_OP,
        ASSIGNMENT,
        BOOLEAN_OP,
        BOOLEAN_VAL,
        CONTROL,
        IDENTIFIER,
        INVALID,
        LITERAL,
        NUMERIC,
        PRIORITY_O,
        PRIORITY_C,
        RESERVED,
        TABULATION,
        UNDEFINED
    };
    Q_ENUM(TokenType)

    explicit Token(const QString &word = "", TokenType type = UNDEFINED,
                    long int position = -1);
    Token(const Token &other);
    ~Token();

    inline bool isEmpty() const { return this->cWord.isEmpty(); }

    inline QString word() const { return this->cWord; }
    inline TokenType type() const { return this->cType; }
    inline long int position() const { return this->cPos; }

    inline void setType(TokenType type) { this->cType = type; }
    inline void setWord(const QString &word) { this->cWord = word; }
    inline void setPosition(long int position) { this->cPos = position; }

    inline bool operator < (const Token &other){ return this->cPos < other.position(); }
    inline bool operator > (const Token &other){ return this->cPos > other.position(); }
    inline bool operator <= (const Token &other){ return this->cPos <= other.position(); }
    inline bool operator >= (const Token &other){ return this->cPos >= other.position(); }

    Token& operator =(const Token &other);

private:
    long int cPos;

    QString cWord;
    TokenType cType;
};

#endif // TOKEN_H
