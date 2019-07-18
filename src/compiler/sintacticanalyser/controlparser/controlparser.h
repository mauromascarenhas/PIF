#ifndef CONTROLPARSER_H
#define CONTROLPARSER_H

#include <QList>

#include "../programitem.h"
#include "sintacticanalyser/lexicalanalyser/token.h"

class ControlParser : public ProgramItem
{
public:
    ControlParser();
    ~ControlParser();

    inline void addToken(const Token &token){ this->tokens.append(token); }

    bool isValid();

private:
    QList<Token> tokens;
};

#endif // CONTROLPARSER_H
