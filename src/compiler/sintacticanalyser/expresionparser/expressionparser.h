#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <QList>

#include "../programitem.h"
#include "sintacticanalyser/lexicalanalyser/token.h"

class ExpressionParser : public ProgramItem
{
public:
    enum Validity{
        INVALID,
        VALID,
        INCOMPLETE
    };

    ExpressionParser();
    ~ExpressionParser();

    inline void addToken(const Token &token){ this->tokens.append(token); }

    Validity isValid();

private:
    QList<Token> tokens;
};

#endif // EXPRESSIONPARSER_H
