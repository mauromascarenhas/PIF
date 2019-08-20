/*
 *     This file is part of PIFC (Compiler).
 *
 *    PIFC (Compiler) is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Foobar is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 */

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
