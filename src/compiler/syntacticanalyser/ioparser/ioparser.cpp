#include "ioparser.h"

IOParser::IOParser(IOType type,
                   const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem()
{
    this->ioType = type;
    this->globalVars = globalVars;
}

IOParser::~IOParser(){

}

bool IOParser::inValidity(){
    bool valid = false;
    for (int i = 0; i < arguments.size(); ++i){
        switch (arguments[i].type()) {
            case Token::IDENTIFIER:
            {
                bool exists = globalVars.contains(arguments[i].word());
                if (exists && !valid){
                    valid = true;
                    break;
                }
                else if (!exists){
                    MessageLogger::getInstance().log(MessageLogger::E_UNDECLARED_ID,
                                                     "", 0, arguments[i]);
                    return false;
                }
                valid = false;
            }
            case Token::CONTROL:
                if (arguments[i].word() == "," && valid){
                    valid = false;
                    break;
                }
                else if (arguments[i].word() != ",") {
                    MessageLogger::getInstance().log(MessageLogger::ERROR,
                        QString("Apenas identificadores e o caractere ',' são permitidos em operações de entrada. Token fornecido : '%1'.")
                            .arg(arguments[i].word()));
                    return false;
                }
            default:
                MessageLogger::getInstance().log(MessageLogger::ERROR,
                    QString("Apenas identificadores e o caractere ',' são permitidos em operações de entrada (de forma intercalada). Token fornecido : '%1'.")
                        .arg(arguments[i].word()));
                return false;
        }
    }

    if (!valid && arguments.size()) MessageLogger::getInstance().log(MessageLogger::ERROR, "O último argumento não pode ser ',' .");
    return valid;
}

bool IOParser::outValidity(){
    bool valid = false;
    for (int i = 0; i < arguments.size(); ++i){
        switch (arguments[i].type()) {
            case Token::IDENTIFIER:
            {
                bool exists = globalVars.contains(arguments[i].word());
                if (exists && !valid){
                    valid = true;
                    break;
                }
                else if (!exists){
                    MessageLogger::getInstance().log(MessageLogger::E_UNDECLARED_ID,
                                                     "", 0, arguments[i]);
                    return false;
                }
                valid = false;
            }
            case Token::CONTROL:
                if (arguments[i].word() == "+" && valid){
                    valid = false;
                    break;
                }
                else if (arguments[i].word() != "+") {
                    MessageLogger::getInstance().log(MessageLogger::ERROR,
                        QString("Apenas identificadores e o caractere ',' são permitidos em operações de entrada. Token fornecido : '%1'.")
                            .arg(arguments[i].word()));
                    return false;
                }
                valid = true;
            case Token::NUMERIC:
            case Token::BOOLEAN_VAL:
            case Token::LITERAL:
                if (!valid){
                    valid = true;
                    break;
                }
            default:
                MessageLogger::getInstance().log(MessageLogger::ERROR,
                    QString("Apenas identificadores e o operador '+' são permitidos em operações de entrada (de forma intercalada). Token fornecido : '%1'.")
                        .arg(arguments[i].word()));
                return false;
        }
    }

    if (!valid && arguments.size()) MessageLogger::getInstance().log(MessageLogger::ERROR, "O último argumento não pode ser ',' .");
    return valid;
}
