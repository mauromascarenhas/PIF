#ifndef PIFCOMPILER_H
#define PIFCOMPILER_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>

#include "sintacticanalyser/sintacticanalyser.h"
#include "sintacticanalyser/lexicalanalyser/lexicalanalyser.h"

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
    SintacticAnalyser::OperationType operation;
};

#endif // PIFCOMPILER_H
