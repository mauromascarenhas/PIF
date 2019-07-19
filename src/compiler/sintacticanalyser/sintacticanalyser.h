#ifndef SINTACTICANALYSER_H
#define SINTACTICANALYSER_H

#include <QFile>
#include <QDebug>
#include <QString>
#include <QHash>
#include <QTextStream>

#include "programitem.h"
#include "ioparser/ioparser.h"
#include "lexicalanalyser/token.h"
#include "blockparser/blockparser.h"
#include "controlparser/controlparser.h"
#include "lexicalanalyser/lexicalanalyser.h"
#include "mainprogramparser/mainprogramparser.h"

class SintacticAnalyser
{
public:
    enum OperationType{
        CONVERT_C,
        CONVERT_CPP,
        CONVERT_JAVA
    };

    explicit SintacticAnalyser(const QString &fileName = "",
                                const QString &outFileName = "",
                                OperationType operation = CONVERT_C);
    ~SintacticAnalyser();

    int execute();

private:
    int lineCounter;
    int indentFactor;

    bool programStarted;
    bool programFinished;

    const QString IN_FILE, OUT_FILE;
    const OperationType OPERATION;

    QHash<QString, ControlParser::IdentifierType> availableVars;

    BlockParser *currentBlock;
    MainProgramParser *mainProgram;
};

#endif // SINTACTICANALYSER_H
