#ifndef CONTROLPARSER_H
#define CONTROLPARSER_H

#include <QList>
#include <QHash>
#include <QString>

#include "../programitem.h"
#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/lexicalanalyser/token.h"

class ControlParser : public ProgramItem
{
public:
    ControlParser();
    ~ControlParser();

    enum IdentifierType {
        NUMERIC,
        LITERAL,
        BOOLEAN
    };

    inline void addToken(const Token &token){ this->tokens.append(token); }

    bool isValid();
    bool insertNewVars(QHash<QString, IdentifierType> &globalVars);

    inline QStringList getDeclaredIDs() { return this->declaredIDs; }

private:
    QList<Token> tokens;
    QStringList declaredIDs;
    IdentifierType currentType;
};

#endif // CONTROLPARSER_H
