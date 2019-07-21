#ifndef IOPARSER_H
#define IOPARSER_H

#include <QList>
#include <QHash>
#include <QString>

#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/programitem.h"
#include "syntacticanalyser/lexicalanalyser/token.h"
#include "syntacticanalyser/controlparser/controlparser.h"

class IOParser : public ProgramItem
{
public:
    enum IOType{
        INPUT,
        OUTPUT
    };

    IOParser(IOType type = OUTPUT,
             const QHash<QString, ControlParser::IdentifierType> &globalVars = QHash<QString, ControlParser::IdentifierType>());
    ~IOParser();

    inline void addToken(const Token &token){ this->arguments.append(token); }
    inline bool isValid() { return ioType == OUTPUT ? outValidity() : inValidity(); }

    inline void setType(IOType type) { this->ioType = type; }
    inline IOType type(){ return this->ioType; }

private:
    IOType ioType;
    QList<Token> arguments;
    QHash<QString, ControlParser::IdentifierType> globalVars;

    bool inValidity();
    bool outValidity();
};

#endif // IOPARSER_H