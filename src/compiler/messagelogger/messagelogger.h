#ifndef MESSAGELOGGER_H
#define MESSAGELOGGER_H

#include <QString>
#include <QTextStream>
#include <QCoreApplication>

#include "syntacticanalyser/lexicalanalyser/token.h"

class MessageLogger
{
public:
    enum CriticalMode{
        CRITICAL_Q,
        WARNING_L
    };

    enum LogType{
        E_CANT_OPEN_FILE,
        E_CANT_OPEN_FILE_O,
        E_EMPTY_JAVA_CLASS,
        E_INVALID_ARGS,
        E_INVALID_JAVA_CLASS,
        E_INVALID_T,
        E_UNEXPECTED_T,
        E_UNEXPECTED_T_EXP,
        E_EXPECTED_COND,
        E_NO_END_OF_EXPR,
        E_NO_MAIN_ROUTINE,
        E_NO_FILE,
        E_OUT_EXISTS,
        E_OUT_CANT_REMOVE,
        E_STRUCTURE_EXPECTED,
        E_UNDECLARED_ID,
        E_UNDEFINED,
        W_INDENT_FACTOR,
        W_UNDEFINED,
        SUCCESS
    };

    enum MessageType{
        ERROR,
        INFO,
        WARNING
    };

    static MessageLogger& getInstance(CriticalMode mode = WARNING_L);
    static void clearInstance();
    ~MessageLogger();

    void log(MessageType type, const QString &message);
    int log(LogType type, const QString &appendMsg = "",
                unsigned int line = 0, const Token &token = Token());

private:
    explicit MessageLogger(CriticalMode mode = WARNING_L);

    static MessageLogger *current;

    const CriticalMode CMODE;

    QTextStream output;
    QTextStream errOutput;
};

#endif // MESSAGELOGGER_H
