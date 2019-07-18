#ifndef IOPARSER_H
#define IOPARSER_H

#include <QList>

#include "../programitem.h"
#include "../lexicalanalyser/token.h"

class IOParser : public ProgramItem
{
public:
    enum IOType{
        INPUT,
        OUTPUT
    };

    IOParser(IOType type = OUTPUT);
    ~IOParser();

    inline void addToken(const Token &token){ this->arguments.append(token); }

    inline void setType(IOType type) { this->ioType = type; }
    inline IOType type(){ return this->ioType; }

private:
    IOType ioType;
    QList<Token> arguments;
};

#endif // IOPARSER_H
