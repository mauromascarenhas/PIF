#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <QList>

#include "../programitem.h"
#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/lexicalanalyser/token.h"
#include "syntacticanalyser/controlparser/controlparser.h"

class ExpressionParser : public ProgramItem
{
public:
    enum Validity{
        INVALID,
        INVALID_SEM,
        VALID_NO_ATTRIB,
        VALID_ATTRIB
    };

    ExpressionParser(const QHash<QString, ControlParser::IdentifierType> &globalVars = QHash<QString, ControlParser::IdentifierType>());
    ~ExpressionParser();

    inline void addToken(const Token &token){ this->tokens.append(token); }
    inline ControlParser::IdentifierType operationType(){ return this->cOperationType; }

    Validity validity();

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    int parenthesesCount;

    QList<Token> tokens;
    ControlParser::IdentifierType cOperationType;
    QHash<QString, ControlParser::IdentifierType> declaredVars;

    void toOutFileC(int indentFactor, QTextStream &stream);
    void toOutFileCPP(int indentFactor, QTextStream &stream);
    void toOutFileJAVA(int indentFactor, QTextStream &stream);
};

#endif // EXPRESSIONPARSER_H
