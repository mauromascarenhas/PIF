#include "expressionparser.h"

ExpressionParser::ExpressionParser(const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem()
{
    this->declaredVars = globalVars;
    this->cOperationType = ControlParser::LITERAL;
}

ExpressionParser::~ExpressionParser(){

}

#include <QDebug>

ExpressionParser::Validity ExpressionParser::validity(){
    for (int i = 0; i < tokens.size(); ++i){
        qDebug() << tokens[i].word();
    }

    if (!tokens.size()) return INCOMPLETE;

    bool hasFinished = false;
    bool canReturnValid = true;
    bool invalidSemantic = false;
    Token current = tokens.first();
    Token oldToken = current;

    switch (oldToken.type()) {
        case Token::NUMERIC:
            cOperationType = ControlParser::NUMERIC;
            canReturnValid = false;
            break;
        case Token::BOOLEAN_VAL:
            cOperationType = ControlParser::BOOLEAN;
            canReturnValid = false;
            break;
        //TODO: Insert literal
        case Token::IDENTIFIER:
            if (declaredVars.contains(oldToken.word())){
                cOperationType = declaredVars.value(oldToken.word());
                break;
            }
        default:
            return INVALID;
    }

    if (tokens.length() == 1) return INCOMPLETE;
    current = tokens[1];
    oldToken = current;

    switch (oldToken.type()) {
        case Token::ARITHMETIC_OP:
            if (oldToken.word() != "="){
                canReturnValid = false;
                //TODO: Simplify operation (add semantic error message?)
                if (cOperationType != ControlParser::NUMERIC
                        || !(cOperationType == ControlParser::LITERAL
                             && oldToken.word() == "+")) return INVALID;
            }
            break;
        case Token::BOOLEAN_OP:
            if (cOperationType != ControlParser::BOOLEAN) return INVALID;
            break;
        default:
            return INVALID;
    }

    if (cOperationType == ControlParser::BOOLEAN){
        for (int i = 2; i < tokens.length(); ++i){
            current = tokens[i];

            if (hasFinished){
                //TODO: Show that it cannot have tokens after tabs?
                if (current.type() == Token::TABULATION) continue;
                else return INVALID;
            }

            switch (current.type()) {
                case Token::BOOLEAN_OP:
                    if (oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::BOOLEAN_VAL)
                        oldToken = current;
                    else return INVALID;
                    break;
                case Token::IDENTIFIER:
                    if (oldToken.type() == Token::BOOLEAN_OP){
                        if (declaredVars.contains(current.word())){
                            if (ControlParser::BOOLEAN != declaredVars.value(current.word())){
                                //TODO: Add semantic error message?
                                invalidSemantic = true;
                            }
                            oldToken = current;
                            break;
                        }
                        else {
                            //TODO: Display error (undeclared identifier)
                            return INVALID;
                        }
                    }
                    else {
                        //TODO: Display unexpected token
                        return INVALID;
                    }
                case Token::BOOLEAN_VAL:
                    if (oldToken.type() == Token::BOOLEAN_OP) oldToken = current;
                    else {
                        //TODO: Display unexpected token
                        return INVALID;
                    }
                    break;
                case Token::TABULATION:
                    hasFinished = true;
                    break;
                default:
                    //TODO: Display unexpected token
                    return INVALID;
            }
        }
    }
    else if (cOperationType == ControlParser::NUMERIC){
        for (int i = 2; i < tokens.length(); ++i){
            current = tokens[i];

            if (hasFinished){
                //TODO: Show that it cannot have tokens after tabs?
                if (current.type() == Token::TABULATION) continue;
                else return INVALID;
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
                    if (oldToken.type() == Token::ARITHMETIC_OP){
                        if (declaredVars.contains(current.word())){
                            if (ControlParser::NUMERIC != declaredVars.value(current.word())){
                                //TODO: Add semantic error message?
                                invalidSemantic = true;
                            }
                            oldToken = current;
                            break;
                        }
                        else {
                            //TODO: Display error (undeclared identifier)
                            return INVALID;
                        }
                    }
                    else {
                        //TODO: Display unexpected token
                        return INVALID;
                    }
                case Token::NUMERIC:
                    if (oldToken.type() == Token::ARITHMETIC_OP) oldToken = current;
                    else {
                        //TODO: Display unexpected token
                        return INVALID;
                    }
                    break;
                case Token::TABULATION:
                    hasFinished = true;
                    break;
                default:
                    //TODO: Display unexpected token
                    return INVALID;
            }
        }
    }
    else {
        for (int i = 2; i < tokens.length(); ++i){
            current = tokens[i];

            if (hasFinished){
                //TODO: Show that it cannot have tokens after tabs?
                if (current.type() == Token::TABULATION) continue;
                else return INVALID;
            }

            switch (current.type()) {
                case Token::ARITHMETIC_OP:
                    if (current.word() != "+") {
                        //TODO: Add error message?
                        return INVALID;
                    }
                    if (oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::LITERAL)
                        oldToken = current;
                    else return INVALID;
                    break;
                case Token::IDENTIFIER:
                    if (oldToken.type() == Token::ARITHMETIC_OP){
                        if (declaredVars.contains(current.word())){
                            if (ControlParser::LITERAL != declaredVars.value(current.word())){
                                //TODO: Add semantic error message?
                                invalidSemantic = true;
                            }
                            oldToken = current;
                            break;
                        }
                        else {
                            //TODO: Display error (undeclared identifier)
                            return INVALID;
                        }
                    }
                    else {
                        //TODO: Display unexpected token
                        return INVALID;
                    }
                case Token::NUMERIC:
                    if (oldToken.type() == Token::ARITHMETIC_OP) oldToken = current;
                    else {
                        //TODO: Display unexpected token
                        return INVALID;
                    }
                    break;
                case Token::TABULATION:
                    hasFinished = true;
                    break;
                default:
                    //TODO: Display unexpected token
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
            return canReturnValid ? VALID: INCOMPLETE;
        default:
            return INVALID;
    }
}
