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

void MessageLogger::clearInstance(){
    if (current){
        delete current;
        current = nullptr;
    }
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
        QString logMessage;
        switch (type) {
            case E_CANT_OPEN_FILE:
                logMessage = QString("[ ERRO ] Não foi possível abrir o código fonte. Você tem as permissões necessárias? %1").arg(appendMsg);
                returnVal = 1;
                break;
            case E_INVALID_ARGS:
                logMessage = QString("[ ERRO ] Argumentos de inicialização inválidos. %1").arg(appendMsg);
                returnVal = 1;
                break;
            case E_INVALID_T:
                logMessage = QString("[ ERRO ] Token inválido na linha %1. Token : '%2'. %3").arg(line).arg(token.word(), appendMsg);
                break;
            case E_UNEXPECTED_T:
                logMessage = QString("[ ERRO ] Token inesperado na linha %1. Token : '%2'. %3").arg(line).arg(token.word(), appendMsg);
                break;
            case E_UNEXPECTED_T_EXP:
                logMessage = QString("[ ERRO ] Token inesperado na expressão. Token : '%1'. %2").arg(token.word(), appendMsg);
                break;
            case E_EXPECTED_COND:
                logMessage = QString("[ ERRO ] Esperava-se uma expressão condicional na linha %1. Ao invés disso,"
                                      "foi obtido '%2'. %3").arg(line).arg(token.word(), appendMsg);
                break;
            case E_NO_END_OF_EXPR:
                logMessage = QString("[ ERRO ] Final de declaração de expressão não encontrado na linha %1. %2").arg(line).arg(appendMsg);
                break;
            case E_NO_MAIN_ROUTINE:
                logMessage = QString("[ ERRO ] Não há bloco de programa principal no arquivo fornecido como entrada! %1").arg(appendMsg);
                break;
            case E_NO_FILE:
                logMessage = QString("[ ERRO ] Nenhum arquivo especificado como entrada. %1").arg(appendMsg);
                returnVal = 1;
                break;
            case E_STRUCTURE_EXPECTED:
                logMessage = QString("[ ERRO ] Esperava-se a declaração de uma nova estrutura na linha %1. %2").arg(line).arg(appendMsg);
                break;
            case E_UNDECLARED_ID:
                logMessage = QString("[ ERRO ] Identificador '%1' não declarado. %2").arg(token.word()).arg(appendMsg);
                break;
            default:
                logMessage = QString("[ ERRO ] Erro na linha %1. %2").arg(line).arg(appendMsg);
                returnVal = -1;
                break;
        }

        qCritical() << logMessage;
        return returnVal;
    }
    else if (type < SUCCESS){
        QString logMessage;
        switch (type) {
            case W_INDENT_FACTOR:
                logMessage = QString("[ AVISO ] Problema de indentação na linha %1 - %2").arg(line).arg(appendMsg);
                break;
            default:
                logMessage = QString("[ AVISO ] Problema na linha %1. %2").arg(line).arg(appendMsg);
                break;
        }

        qWarning() << logMessage;
        if (CMODE == CRITICAL_Q) return -2;
        return 0;
    }
    else {
        qInfo() << "Compilado com sucesso!";
        return 0;
    }
}
