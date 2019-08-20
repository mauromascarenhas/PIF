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

#include "messagelogger.h"

MessageLogger* MessageLogger::current = nullptr;

MessageLogger::MessageLogger(CriticalMode mode) : CMODE(mode),
    errOutput(stderr, QIODevice::Unbuffered | QIODevice::ReadWrite), output(stdout, QIODevice::Unbuffered | QIODevice::ReadWrite)
{
    output.setCodec("UTF-8"); //ISO 8859-1
    errOutput.setCodec("UTF-8"); //ISO 8859-1
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
            errOutput << "[ ERRO ] " << message.toUtf8() << endl;
            break;
        case INFO:
            output << "[ INFO ] " << message.toUtf8() << endl;
            break;
        default:
            output << "[ AVISO ] " << message.toUtf8() << endl;
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
            case E_CANT_OPEN_FILE_O:
                logMessage = QString("[ ERRO ] Não foi escrever no arquivo de saída. Você tem as permissões necessárias? %1").arg(appendMsg);
                returnVal = 3;
                break;
            case E_EMPTY_JAVA_CLASS:
                logMessage = QString("[ ERRO ] O procedimento principal não foi nomeado. "
                                     "Para que a conversão possa ser feita para Java, um nome deve ser fornecido. %1").arg(appendMsg);
                returnVal = 3;
                break;
            case E_INVALID_ARGS:
                logMessage = QString("[ ERRO ] Argumentos de inicialização inválidos. %1").arg(appendMsg);
                returnVal = 1;
                break;
            case E_INVALID_JAVA_CLASS:
                logMessage = QString("[ ERRO ] O Nome do procedimento principal não corresponde ao nome base do arquivo fornecido como saída. %1").arg(appendMsg);
                returnVal = 3;
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
            case E_OUT_EXISTS:
                logMessage = QString("[ ERRO ] O arquivo fornecido como saída já existe. %1").arg(appendMsg);
                returnVal = 3;
                break;
            case E_OUT_CANT_REMOVE:
                logMessage = QString("[ ERRO ] Não foi possível sobrescrever o arquivo de saída %1.").arg(appendMsg);
                returnVal = 3;
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

        errOutput << logMessage.toUtf8() << endl;
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

        output << logMessage.toUtf8() << endl;
        if (CMODE == CRITICAL_Q) return -2;
        return 0;
    }
    else {
        output << QString("Compilado com sucesso!").toUtf8() << endl;
        return 0;
    }
}
