#ifndef SINTACTICANALYSER_H
#define SINTACTICANALYSER_H

#include <QFile>
#include <QHash>
#include <QString>
#include <QTextStream>

#include "programitem.h"
#include "ioparser/ioparser.h"
#include "lexicalanalyser/token.h"
#include "blockparser/blockparser.h"
#include "controlparser/controlparser.h"
#include "messagelogger/messagelogger.h"
#include "lexicalanalyser/lexicalanalyser.h"
#include "mainprogramparser/mainprogramparser.h"

class SyntacticAnalyser
{
public:
    enum OperationType{
        CONVERT_C,
        CONVERT_CPP,
        CONVERT_JAVA
    };

    explicit SyntacticAnalyser(const QString &fileName = "",
                                const QString &outFileName = "",
                                OperationType operation = CONVERT_C);
    ~SyntacticAnalyser();

    int execute();
    void freeReferences();
    void freeReferences(BlockParser *block);

private:
    int indentFactor;
    unsigned int lineCounter;

    bool programStarted;
    bool programFinished;

    const QString IN_FILE, OUT_FILE;
    const OperationType OPERATION;

    QHash<QString, ControlParser::IdentifierType> availableVars;

    BlockParser *currentBlock;
    MainProgramParser *mainProgram;
};

#endif // SINTACTICANALYSER_H
