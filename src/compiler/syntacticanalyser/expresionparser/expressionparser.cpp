#include "expressionparser.h"

ExpressionParser::ExpressionParser(const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem()
{
    this->declaredVars = globalVars;
    this->cOperationType = ControlParser::LITERAL;
}

ExpressionParser::~ExpressionParser(){

}

ExpressionParser::Validity ExpressionParser::validity(){
    if (!tokens.size()) return VALID_NO_ATTRIB;

    //WARNING: Missing support for operation precedence
    bool hasFinished = false;
    bool canAttribute = true;
    bool invalidSemantic = false;
    Token current = tokens.first();
    Token oldToken = current;

    switch (oldToken.type()) {
        case Token::LITERAL:
        case Token::NUMERIC:
        case Token::BOOLEAN_VAL:
            cOperationType = ControlParser::BOOLEAN;
            canAttribute = false;
            break;
        case Token::IDENTIFIER:
            if (declaredVars.contains(oldToken.word())){
                cOperationType = declaredVars.value(oldToken.word());
                break;
            }
        default:
            return INVALID;
    }

    if (tokens.length() == 1){
        if (oldToken.type() == Token::IDENTIFIER
                && cOperationType == ControlParser::BOOLEAN)
            return VALID_NO_ATTRIB;
        else return INVALID;
    }

    current = tokens[1];    
    switch (current.type()) {
        case Token::BOOLEAN_OP:
            canAttribute = false;
            cOperationType = ControlParser::BOOLEAN;
            break;
        case Token::ASSIGNMENT:
            if (oldToken.type() == Token::IDENTIFIER) break;
        default:
            return INVALID;
    }
    oldToken = current;

    if (cOperationType == ControlParser::BOOLEAN){
        for (int i = 2; i < tokens.length(); ++i){
            current = tokens[i];

            if (hasFinished){
                if (current.type() == Token::TABULATION) continue;
                MessageLogger::getInstance().log(MessageLogger::ERROR,
                    "Não pode haver qualquer token após uma tabulação, pois, ela é tratada como término de expressão.");
                return INVALID;
            }

            switch (current.type()) {
                case Token::BOOLEAN_OP:
                    if (oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::BOOLEAN_VAL)
                        oldToken = current;
                    else return INVALID;
                    break;
                case Token::IDENTIFIER:
                    if (oldToken.type() == Token::BOOLEAN_OP
                            || oldToken.type() == Token::ASSIGNMENT){
                        if (declaredVars.contains(current.word())){
                            oldToken = current;
                            break;
                        }
                        else {
                            MessageLogger::getInstance().log(MessageLogger::E_UNDECLARED_ID,
                                                             "", 0, current);
                            return INVALID;
                        }
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                case Token::BOOLEAN_VAL:
                    if (oldToken.type() == Token::BOOLEAN_OP
                            || oldToken.type() == Token::ASSIGNMENT) oldToken = current;
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                    break;
                case Token::TABULATION:
                    hasFinished = true;
                    break;
                default:
                    MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                     "", 0, current);
                    return INVALID;
            }
        }
    }
    else if (cOperationType == ControlParser::NUMERIC){
        for (int i = 2; i < tokens.length(); ++i){
            current = tokens[i];

            if (hasFinished){
                if (current.type() == Token::TABULATION) continue;
                MessageLogger::getInstance().log(MessageLogger::ERROR,
                    "Não pode haver qualquer token após uma tabulação, pois, ela é tratada como término de expressão.");
                return INVALID;
            }

            switch (current.type()) {
                case Token::ARITHMETIC_OP:
                    if (current.word() == "=") return INVALID;
                    if (oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::NUMERIC)
                        oldToken = current;
                    else return INVALID;
                    break;
                case Token::IDENTIFIER:
                    if (oldToken.type() == Token::ARITHMETIC_OP
                            || oldToken.type() == Token::ASSIGNMENT){
                        if (declaredVars.contains(current.word())){
                            if (ControlParser::NUMERIC != declaredVars.value(current.word())){
                                MessageLogger::getInstance().log(MessageLogger::INFO,
                                                                 "Identificador de tipo não numérico em operação aritmética (poderá causar erro de execução).");
                                invalidSemantic = true;
                            }
                            oldToken = current;
                            break;
                        }
                        else {
                            MessageLogger::getInstance().log(MessageLogger::E_UNDECLARED_ID,
                                                             "", 0, current);
                            return INVALID;
                        }
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                case Token::NUMERIC:
                    if (oldToken.type() == Token::ARITHMETIC_OP
                            || oldToken.type() == Token::ASSIGNMENT) oldToken = current;
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                    break;
                case Token::TABULATION:
                    hasFinished = true;
                    break;
                default:
                    MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                     "", 0, current);
                    return INVALID;
            }
        }
    }
    else {
        for (int i = 2; i < tokens.length(); ++i){
            current = tokens[i];

            if (hasFinished){
                if (current.type() == Token::TABULATION) continue;
                MessageLogger::getInstance().log(MessageLogger::ERROR,
                    "Não pode haver qualquer token após uma tabulação, pois, ela é tratada como término de expressão.");
                return INVALID;
            }

            switch (current.type()) {
                case Token::ARITHMETIC_OP:
                    if (current.word() != "+") {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "Apenas operação de concatenação é permitida em literais",
                                                         0, current);
                        return INVALID;
                    }
                    oldToken = current;
                    break;
                case Token::IDENTIFIER:
                    if (oldToken.type() == Token::ARITHMETIC_OP
                            || oldToken.type() == Token::ASSIGNMENT){
                        if (declaredVars.contains(current.word())){
                            oldToken = current;
                            break;
                        }
                        else {
                            MessageLogger::getInstance().log(MessageLogger::E_UNDECLARED_ID,
                                                             "", 0, current);
                            return INVALID;
                        }
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                case Token::NUMERIC:
                    if (oldToken.type() == Token::ARITHMETIC_OP
                            || oldToken.type() == Token::ASSIGNMENT) oldToken = current;
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                    break;
                case Token::TABULATION:
                    hasFinished = true;
                    break;
                default:
                    MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                     "", 0, current);
                    return INVALID;
            }
        }
    }

    if (invalidSemantic) return INVALID_SEM;

    switch (current.type()) {
        case Token::IDENTIFIER:
        case Token::BOOLEAN_VAL:
        case Token::LITERAL:
        case Token::NUMERIC:
            return canAttribute ? VALID_ATTRIB : VALID_NO_ATTRIB;
        default:
            return INVALID;
    }
}

void ExpressionParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (this->validity()) {
        case VALID_ATTRIB:
            stream << tabs.toUtf8();
            for (int i = 0; i < tokens.size(); ++i)
                stream << QString("%1%2").arg(tokens[i].word(), i == tokens.size() - 1 ? "" : " ").toUtf8();
            stream << QString(";\n").toUtf8();
            break;
        case VALID_NO_ATTRIB:
            stream << tabs.toUtf8();
            for (int i = 0; i < tokens.size(); ++i)
                stream << QString("%1%2").arg(tokens[i].word(), i == tokens.size() - 1 ? "" : " ").toUtf8();
            break;
        default:
            break;
    }
}
