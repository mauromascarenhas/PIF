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

    QString fileName;
    QString outFileName;
    QStringList arguments;
    SyntacticAnalyser::OperationType operation;
};

#endif // PIFCOMPILER_H
