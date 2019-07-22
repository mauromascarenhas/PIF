#ifndef MAINPROGRAMPARSER_H
#define MAINPROGRAMPARSER_H

#include <QList>
#include <QString>

#include "../blockparser/blockparser.h"
#include "syntacticanalyser/programitem.h"

class MainProgramParser : public BlockParser
{
public:
    MainProgramParser(const QString &programName = "");
    ~MainProgramParser();

    inline void closeProgramBlock() { this->closedProgramBlock = true; }
    inline bool isProgramBlockClosed() { return this->closedProgramBlock; }

    inline QString programName() { return this->PROGRAM_NAME; }

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    using BlockParser::getBlockExpression;
    using BlockParser::setBlockExpression;

    bool closedProgramBlock;

    const QString PROGRAM_NAME;

    void toOutFileC(int indentFactor, QTextStream &stream);
    void toOutFileCPP(int indentFactor, QTextStream &stream);
    void toOutFileJAVA(int indentFactor, QTextStream &stream);
};

#endif // MAINPROGRAMPARSER_H
