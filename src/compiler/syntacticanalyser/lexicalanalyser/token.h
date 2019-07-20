#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

class Token
{
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

    void operator = (const Token &other);

private:
    long int cPos;

    QString cWord;
    TokenType cType;
};

#endif // TOKEN_H
