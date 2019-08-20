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

#ifndef PIFCOMPILER_H
#define PIFCOMPILER_H

#include <QFile>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QCoreApplication>

#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/syntacticanalyser.h"

class PIFCompiler : public QObject
{
    Q_OBJECT
public:
    PIFCompiler(QObject *parent = nullptr);
    ~PIFCompiler();

public slots:
    void execute();

private:
    int argCount;

    QFile currentFile;
    QTextStream output;

    QString fileName;
    QString outFileName;
    QStringList arguments;
    SyntacticAnalyser::OperationType operation;
};

#endif // PIFCOMPILER_H
