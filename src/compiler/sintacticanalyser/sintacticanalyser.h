#ifndef SINTACTICANALYSER_H
#define SINTACTICANALYSER_H

#include <QFile>
#include <QDebug>
#include <QString>
#include <QTextStream>

#include "programitem.h"
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
    const QString IN_FILE, OUT_FILE;
    const OperationType OPERATION;

    MainProgramParser *mainProgram;
    ProgramItem *currentItem;
};

#endif // SINTACTICANALYSER_H
