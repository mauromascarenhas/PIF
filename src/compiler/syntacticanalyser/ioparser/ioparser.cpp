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
            case Token::ARITHMETIC_OP:
                if (arguments[i].word() == "+" && valid){
                    valid = false;
                    break;
                }
                else if (arguments[i].word() != "+") {
                    MessageLogger::getInstance().log(MessageLogger::ERROR,
                        QString("Apenas identificadores, constantes e o caractere '+' são permitidos em operações de saída. Token fornecido : '%1'.")
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
                    QString("Apenas identificadores, constantes e o operador '+' são permitidos em operações de saída (de forma intercalada). Token fornecido : '%1'.")
                        .arg(arguments[i].word()));
                return false;
        }
    }

    if (!valid && arguments.size()) MessageLogger::getInstance().log(MessageLogger::ERROR, "O último argumento não pode ser ',' .");
    return valid;
}

QList<Token> IOParser::validArgs(){
    QList<Token> validArguments;
    for (int i = 0; i < arguments.length(); ++i)
        switch (ioType) {
            case OUTPUT:
                if (arguments[i].type() != Token::ARITHMETIC_OP) validArguments.append(arguments[i]);
                break;
            default:
                if (arguments[i].type() != Token::CONTROL) validArguments.append(arguments[i]);
                break;
        }
    return validArguments;
}

void IOParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang conv){
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

void IOParser::toOutFileC(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    QList<Token> arguments = validArgs();

    if (ioType == INPUT){
        stream << QString("%1scanf(\"").arg(tabs).toUtf8();
        for (int i = 0; i < arguments.size(); ++i){
            switch (globalVars.value(arguments[i].word())) {
                case ControlParser::BOOLEAN:
                    stream << QString("%d%1").arg(i == arguments.size() - 1 ? "\", " : " ").toUtf8();
                    break;
                case ControlParser::NUMERIC:
                    stream << QString("%lf%1").arg(i == arguments.size() - 1 ? "\", " : " ").toUtf8();
                    break;
                default:
                    stream << QString("%s%1").arg(i == arguments.size() - 1 ? "\", " : " ").toUtf8();
                    break;
            }
        }
        for (int i = 0; i < arguments.size(); ++i){
            switch (globalVars.value(arguments[i].word())) {
                case ControlParser::BOOLEAN:
                case ControlParser::NUMERIC:
                    stream << QString("&%1%2").arg(arguments[i].word()).arg(i == arguments.size() - 1 ? ");\n" : ", ").toUtf8();
                    break;
                default:
                    stream << QString("%1%2").arg(arguments[i].word()).arg(i == arguments.size() - 1 ? ");\n" : ", ").toUtf8();
                    break;
            }
        }
    }
    else {
        stream << QString("%1printf(\"").arg(tabs).toUtf8();
        for(int i = 0; i < arguments.size(); ++i){
            switch (arguments[i].type()) {
                case Token::BOOLEAN_VAL:
                    stream << QString("%d").toUtf8();
                    break;
                case Token::NUMERIC:
                    stream << QString("%lf").toUtf8();
                    break;
                case Token::LITERAL:
                    stream << QString("%s").toUtf8();
                    break;
                default:
                    switch (globalVars.value(arguments[i].word())) {
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
            }
        }
        stream << QString("\\n\", ").toUtf8();
        for(int i = 0; i < arguments.size(); ++i)
            stream << QString("%1%2").arg(arguments[i].word()).arg(i == arguments.size() - 1 ? ");\n" : ", ").toUtf8();
    }
}

void IOParser::toOutFileCPP(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    QList<Token> arguments = validArgs();

    if (ioType == INPUT){
        stream << QString("%1cin >> ").arg(tabs).toUtf8();
        for (int i = 0; i < arguments.size(); ++i)
            stream << QString("%1%2").arg(arguments[i].word(), i == arguments.size() - 1 ? ";\n" : " >> ").toUtf8();
    }
    else {
        stream << QString("%1cout << boolalpha << ").arg(tabs).toUtf8();
        for(int i = 0; i < arguments.size(); ++i){
            switch (arguments[i].type()) {
            case Token::BOOLEAN_VAL:
                stream << QString("%1%2").arg(arguments[i].word() == "verdadeiro" ? "true" : "false", i == arguments.size() - 1 ? " << endl;\n" : " << ").toUtf8();
                break;
            default:
                stream << QString("%1%2").arg(arguments[i].word(), i == arguments.size() - 1 ? " << endl;\n" : " << ").toUtf8();
                break;
            }
        }
    }
}

void IOParser::toOutFileJAVA(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    QList<Token> arguments = validArgs();

    if (ioType == INPUT){
        for (int i = 0; i < arguments.size(); ++i){
            switch (globalVars.value(arguments[i].word())) {
                case ControlParser::BOOLEAN:
                    stream << QString("%1%2 = scanner.nextBoolean();\n").arg(tabs, arguments[i].word()).toUtf8();
                    break;
                case ControlParser::NUMERIC:
                    stream << QString("%1%2 = scanner.nextDouble();\n").arg(tabs, arguments[i].word()).toUtf8();
                    break;
                default:
                    stream << QString("%1%2 = scanner.next();\n").arg(tabs, arguments[i].word()).toUtf8();
                    break;
            }
        }
    }
    else {
        stream << QString("%1System.out.println(").arg(tabs).toUtf8();
        for(int i = 0; i < arguments.size(); ++i){
            QString concat = i == arguments.size() - 1 ? ");\n" : " + ";
            switch (arguments[i].type()) {
                case Token::BOOLEAN_VAL:
                    stream << QString("Boolean.toString(%1)%2").arg(arguments[i].word() == "verdadeiro" ? "true" : "false",
                                                                        concat).toUtf8();
                    break;
                case Token::NUMERIC:
                    stream << QString("Double.toString(%1)%2").arg(arguments[i].word(), concat).toUtf8();
                    break;
                case Token::LITERAL:
                    stream << QString("%1%2").arg(arguments[i].word(), concat).toUtf8();
                    break;
                default:
                    switch (globalVars.value(arguments[i].word())) {
                        case ControlParser::BOOLEAN:
                            stream << QString("Boolean.toString(%1)%2").arg(arguments[i].word(), concat).toUtf8();
                            break;
                        case ControlParser::NUMERIC:
                            stream << QString("Double.toString(%1)%2").arg(arguments[i].word(), concat).toUtf8();
                            break;
                        default:
                            stream << QString("%1%2").arg(arguments[i].word(), concat).toUtf8();
                            break;
                    }
                    break;
            }
        }
    }
}
