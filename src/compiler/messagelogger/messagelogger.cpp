#include "messagelogger.h"

MessageLogger* MessageLogger::current = nullptr;

MessageLogger::MessageLogger(CriticalMode mode) : CMODE(mode)
{

}

MessageLogger::~MessageLogger(){

}

MessageLogger& MessageLogger::getInstance(CriticalMode mode){
    if (!current) current = new MessageLogger(mode);
    return *current;
}

void MessageLogger::log(MessageType type, const QString &message){
    switch (type) {
        case ERROR:
            qCritical() << "[ ERRO ] " << message;
            break;
        case INFO:
            qInfo() << "[ INFO ] " << message;
            break;
        default:
            qWarning() << "[ AVISO ] " << message;
            break;
    }
}

int MessageLogger::log(LogType type, const QString &appendMsg,
                                        unsigned int line, const Token &token){
    if (type < W_INDENT_FACTOR){
        int returnVal = 2;
        QString logMessage = "[ ERRO ] ";
        switch (type) {
            case E_CANT_OPEN_FILE:
                logMessage += "Não foi possível abrir o código fonte. Você tem as permissões necessárias? ";
                returnVal = 1;
                break;
            case E_INVALID_T:
                logMessage += QString("Token inválido na linha %1. Token : \"%2\". ").arg(QString::number(line),
                                                                                      token.word());
                break;
            case E_UNEXPECTED_T:
                logMessage += QString("Token inesperado na linha %1. Token : \"%2\". ").arg(QString::number(line),
                                                                                       token.word());
                break;
            case E_UNEXPECTED_T_EXP:
                logMessage += QString("Token inesperado na expressão. Token : \"%2\". ").arg(token.word());
                break;
            case E_EXPECTED_COND:
                logMessage += QString("Esperava-se uma expressão condicional na linha %1. Ao invés disso,"
                                      "foi obtido \"%2\".").arg(QString::number(line), token.word());
                break;
            case E_NO_END_OF_EXPR:
                logMessage += QString("Final de declaração de expressão não encontrado na linha %1. ").arg(line);
                break;
            case E_NO_MAIN_ROUTINE:
                logMessage += "Não há bloco de programa principal no arquivo fornecido como entrada! ";
                break;
            case E_NO_FILE:
                logMessage += "Nenhum arquivo especificado como entrada.";
                break;
            case E_STRUCTURE_EXPECTED:
                logMessage += QString("Esperava-se a declaração de uma nova estrutura na linha %1. ").arg(line);
                break;
            case E_UNDECLARED_ID:
                logMessage += QString("Identificador %1 não declarado. ").arg(line);
                break;
            default:
                logMessage += QString("Erro na linha %1. ").arg(line);
                returnVal = -1;
                break;
        }
        logMessage += appendMsg;

        qCritical() << logMessage;
        return returnVal;
    }
    else if (type < SUCCESS){
        QString logMessage = "[ AVISO ] ";
        switch (type) {
            case W_INDENT_FACTOR:
                logMessage += QString("Problema de indentação na linha %1 - ").arg(line);
                break;
            default:
                logMessage += QString("Problema na linha %1. ").arg(line);
                break;
        }
        logMessage += appendMsg;

        qWarning() << logMessage;
        if (CMODE == CRITICAL_Q) return -2;
        return 0;
    }
    else {
        qInfo() << "Compilado com sucesso!";
        return 0;
    }
}
