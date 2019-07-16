#ifndef MAINPROGRAMPARSER_H
#define MAINPROGRAMPARSER_H

#include <QList>
#include <QString>

#include "../blockparser/blockparser.h"
#include "sintacticanalyser/programitem.h"

class MainProgramParser : public BlockParser
{
public:
    MainProgramParser(const QString &programName = "");
    ~MainProgramParser();

private:
    const QString PROGRAM_NAME;
};

#endif // MAINPROGRAMPARSER_H
