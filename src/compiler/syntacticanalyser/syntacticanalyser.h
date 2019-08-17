#ifndef SINTACTICANALYSER_H
#define SINTACTICANALYSER_H

#include <QDir>
#include <QFile>
#include <QHash>
#include <QString>
#include <QFileInfo>
#include <QTextStream>

#include "programitem.h"
#include "ioparser/ioparser.h"
#include "blankitem/blankitem.h"
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
                                OperationType operation = CONVERT_C,
                                bool forceOverwrite = false);
    ~SyntacticAnalyser();

    int execute();

private:
    int indentFactor;
    unsigned int lineCounter;

    bool programStarted;
    bool programFinished;

    const bool OVERWRITE;
    const QString IN_FILE, OUT_FILE;
    const OperationType OPERATION;

    QHash<QString, ControlParser::IdentifierType> availableVars;

    BlockParser *currentBlock;
    MainProgramParser *mainProgram;
};

#endif // SINTACTICANALYSER_H
