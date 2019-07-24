#include "expressionparser.h"

ExpressionParser::ExpressionParser(const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem()
{
    this->declaredVars = globalVars;
    this->cOperationType = ControlParser::LITERAL;
    this->parenthesesCount = 0;
}

ExpressionParser::~ExpressionParser(){

}

ExpressionParser::Validity ExpressionParser::validity(){
    if (!tokens.size()) return INVALID;

    bool hasFinished = false;
    bool canAttribute = true;
    bool invalidSemantic = false;
    bool startedWPrecedence = false;
    Token current = tokens.first();
    Token oldToken = current;

    switch (oldToken.type()) {
        case Token::PRIORITY_O:
            parenthesesCount++;
            startedWPrecedence = true;
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
        if ((oldToken.type() == Token::IDENTIFIER
                || oldToken.type() == Token::BOOLEAN_VAL)
                    && cOperationType == ControlParser::BOOLEAN)
            return VALID_NO_ATTRIB;
        else return INVALID;
    }

    int k = 1;
    if (startedWPrecedence)
        for (k = 1; k < tokens.length() && current.type() == Token::PRIORITY_O; ++k){
            oldToken = current;
            current = tokens[k];
        }
    else current = tokens[k++];

    if (k == tokens.length()) return INVALID;

    switch (current.type()) {
        case Token::BOOLEAN_OP:
            canAttribute = false;
            cOperationType = ControlParser::BOOLEAN;
            break;
        case Token::IDENTIFIER:
        case Token::NUMERIC:
        case Token::BOOLEAN_VAL:
            if (startedWPrecedence) break;
        case Token::ASSIGNMENT:
            if (oldToken.type() == Token::IDENTIFIER) break;
        default:
            return INVALID;
    }
    oldToken = current;

    //FIXME: It does not supports negative constants
    //NOTE: It does not accept literals (each language has its own implementation)

    if (cOperationType == ControlParser::BOOLEAN){
        for (int i = k; i < tokens.length(); ++i){
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
                            || oldToken.type() == Token::BOOLEAN_VAL
                            || oldToken.type() == Token::NUMERIC
                            || oldToken.type() == Token::PRIORITY_C)
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
                case Token::NUMERIC:
                case Token::BOOLEAN_VAL:
                    if (oldToken.type() == Token::BOOLEAN_OP
                            || oldToken.type() == Token::ASSIGNMENT
                            || oldToken.type() == Token::PRIORITY_O) oldToken = current;
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                    break;
                case Token::PRIORITY_O:
                    if (oldToken.type() == Token::BOOLEAN_OP
                            || oldToken.type() == Token::ASSIGNMENT
                            || oldToken.type() == Token::PRIORITY_O){
                        oldToken = current;
                        parenthesesCount++;
                        break;
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                case Token::PRIORITY_C:
                    if (oldToken.type() == Token::BOOLEAN_VAL
                            || oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::NUMERIC
                            || oldToken.type() == Token::PRIORITY_C){
                        oldToken = current;
                        parenthesesCount--;
                        break;
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
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
                    if (oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::NUMERIC
                            || oldToken.type() == Token::PRIORITY_C)
                        oldToken = current;
                    else return INVALID;
                    break;
                case Token::IDENTIFIER:
                    if (oldToken.type() == Token::ARITHMETIC_OP
                            || oldToken.type() == Token::ASSIGNMENT
                            || oldToken.type() == Token::PRIORITY_O){
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
                            || oldToken.type() == Token::ASSIGNMENT
                            || oldToken.type() == Token::PRIORITY_O) oldToken = current;
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                    break;
                case Token::PRIORITY_O:
                    if (oldToken.type() == Token::ARITHMETIC_OP
                            || oldToken.type() == Token::PRIORITY_O
                            || oldToken.type() == Token::ASSIGNMENT){
                        oldToken = current;
                        parenthesesCount++;
                        break;
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
                case Token::PRIORITY_C:
                    if (oldToken.type() == Token::NUMERIC
                            || oldToken.type() == Token::IDENTIFIER
                            || oldToken.type() == Token::PRIORITY_C){
                        oldToken = current;
                        parenthesesCount--;
                        if(parenthesesCount < 0){
                            MessageLogger::getInstance().log(MessageLogger::ERROR,
                                                             QString("Há mais parênteses fechados que abertos em determinada região da expressão."));
                            return INVALID;
                        }
                        break;
                    }
                    else {
                        MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                         "", 0, current);
                        return INVALID;
                    }
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
                case Token::BOOLEAN_VAL:
                case Token::LITERAL:
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

    if (parenthesesCount) return INVALID;
    if (invalidSemantic) return INVALID_SEM;

    switch (current.type()) {
        case Token::IDENTIFIER:
        case Token::BOOLEAN_VAL:
        case Token::PRIORITY_C:
        case Token::LITERAL:
        case Token::NUMERIC:
            return canAttribute ? VALID_ATTRIB : VALID_NO_ATTRIB;
        default:
            return INVALID;
    }
}

void ExpressionParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang conv){
    switch (conv) {
        case CPP:
            toOutFileCPP(indentFactor, stream);
            break;
        case JAVA:
            toOutFileJAVA(indentFactor, stream);
            break;
        default:
            toOutFileC(indentFactor, stream);
            break;
    }
}

void ExpressionParser::toOutFileC(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (this->validity()) {
        case VALID_ATTRIB:
            switch (cOperationType) {
                case ControlParser::BOOLEAN:
                case ControlParser::NUMERIC:
                    stream << tabs.toUtf8();
                    for (int i = 0; i < tokens.size(); ++i)
                        stream << QString("%1%2").arg(tokens[i].word(), i == tokens.size() - 1 ? "" : " ").toUtf8();
                    stream << QString(";\n").toUtf8();
                    break;
                default:
                {
                    stream << QString("%1snprintf(%2, 1024, \"").arg(tabs, tokens.first().word()).toUtf8();
                    for (int i = 2; i < tokens.size(); ++i){
                        switch (tokens[i].type()) {
                            case Token::BOOLEAN_VAL:
                                stream << QString("%d").toUtf8();
                                break;
                            case Token::NUMERIC:
                                stream << QString("%lf").toUtf8();
                                break;
                            case Token::LITERAL:
                                stream << QString("%s").toUtf8();
                                break;
                            case Token::IDENTIFIER:
                                switch (declaredVars.value(tokens[i].word())) {
                                    case ControlParser::BOOLEAN:
                                        stream << QString("%d").toUtf8();
                                        break;
                                    case ControlParser::NUMERIC:
                                        stream << QString("%lf").toUtf8();
                                        break;
                                    default:
                                        stream << QString("%s").toUtf8();
                                        break;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    stream << QString("\", ").toUtf8();
                    for(int i = 2; i < tokens.size(); ++i){
                        switch (tokens[i].type()) {
                        case Token::ARITHMETIC_OP:
                            break;
                        case Token::BOOLEAN_VAL:
                            stream << QString("%1%2").arg(tokens[i].word() == "verdadeiro" ? "true" : "false").arg(i == tokens.size() - 1 ? ");\n" : ", ").toUtf8();
                            break;
                        default:
                            stream << QString("%1%2").arg(tokens[i].word()).arg(i == tokens.size() - 1 ? ");\n" : ", ").toUtf8();
                            break;
                        }
                    }
                    break;
                }
            }
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

void ExpressionParser::toOutFileCPP(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (this->validity()) {
        case VALID_ATTRIB:
            switch (cOperationType) {
                case ControlParser::BOOLEAN:
                case ControlParser::NUMERIC:
                    stream << tabs.toUtf8();
                    for (int i = 0; i < tokens.size(); ++i)
                        stream << QString("%1%2").arg(tokens[i].word(), i == tokens.size() - 1 ? "" : " ").toUtf8();
                    stream << QString(";\n").toUtf8();
                    break;
                default:
                {
                    stream << QString("%1__sstrm__ << boolalpha << ").arg(tabs).toUtf8();
                    for (int i = 2; i < tokens.size(); ++i){
                        switch (tokens[i].type()) {
                            case Token::ARITHMETIC_OP:
                                break;
                            case Token::BOOLEAN_VAL:
                                stream << QString("%1%2").arg(tokens[i].word() == "verdadeiro" ? "true" : "false", i == tokens.size() - 1 ? ";\n" : " << ").toUtf8();
                                break;
                            default:
                                stream << QString("%1%2").arg(tokens[i].word(), i == tokens.size() - 1 ? ";\n" : " << ").toUtf8();
                                break;
                        }
                    }
                    stream << QString("%1%2 = __sstrm__.str();\n"
                                      "%1__sstrm__.str(string());\n").arg(tabs, tokens.first().word()).toUtf8();
                    break;
                }
            }
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

void ExpressionParser::toOutFileJAVA(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (this->validity()) {
        case VALID_ATTRIB:
            switch (cOperationType) {
                case ControlParser::BOOLEAN:
                case ControlParser::NUMERIC:
                    stream << tabs.toUtf8();
                    for (int i = 0; i < tokens.size(); ++i)
                        stream << QString("%1%2").arg(tokens[i].word(), i == tokens.size() - 1 ? "" : " ").toUtf8();
                    stream << QString(";\n").toUtf8();
                    break;
                default:
                {
                    stream << QString("%1%2 = ").arg(tabs, tokens.first().word()).toUtf8();
                    for (int i = 2; i < tokens.size(); ++i){
                        QString concat = i == tokens.size() - 1 ? ";\n" : " + ";
                        switch (tokens[i].type()) {
                            case Token::BOOLEAN_VAL:
                                stream << QString("Boolean.toString(%1)%2").arg(tokens[i].word() == "verdadeiro" ? "true" : "false",
                                                                                    concat).toUtf8();
                                break;
                            case Token::NUMERIC:
                                stream << QString("Double.toString(%1)%2").arg(tokens[i].word() == "verdadeiro" ? "true" : "false",
                                                                                    concat).toUtf8();
                                break;
                            case Token::LITERAL:
                                stream << QString("%1%2").arg(tokens[i].word(), concat).toUtf8();
                                break;
                            case Token::IDENTIFIER:
                                switch (declaredVars.value(tokens[i].word())) {
                                    case ControlParser::BOOLEAN:
                                        stream << QString("Boolean.toString(%1)%2").arg(tokens[i].word(), concat).toUtf8();
                                        break;
                                    case ControlParser::NUMERIC:
                                        stream << QString("Double.toString(%1)%2").arg(tokens[i].word(), concat).toUtf8();
                                        break;
                                    default:
                                        stream << QString("%1%2").arg(tokens[i].word(), concat).toUtf8();
                                        break;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                }
            }
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
