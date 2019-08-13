#include "pifcompiler.h"

PIFCompiler::PIFCompiler(QObject *parent) : QObject(parent),
    output(stdout, QIODevice::Unbuffered | QIODevice::ReadWrite | QIODevice::Text)
{
    arguments = QCoreApplication::arguments();
    argCount = arguments.size();

    output.setGenerateByteOrderMark(true);
    output.setCodec("UTF-8"); //ISO 8859-1
}

PIFCompiler::~PIFCompiler(){
}

void PIFCompiler::execute(){
    bool hasValidArgs = true;
    bool overwriteFile = false;
    MessageLogger::CriticalMode criticalMode = MessageLogger::WARNING_L;
    SyntacticAnalyser::OperationType operation = SyntacticAnalyser::CONVERT_C;

    bool hasFileName = false;
    for (int i = 1; i < arguments.size(); ++i){
        if (hasFileName){
            if (arguments[i].toLower() == "-o" &&
                    i == arguments.size() - 2)
                outFileName = arguments[++i];
            else hasValidArgs = false;
            continue;
        }

        if (arguments[i].toLower() == "-f") overwriteFile = true;
        else if (arguments[i].toLower() == "-c") criticalMode = MessageLogger::CRITICAL_Q;
        else if(arguments[i].toLower() == "-l"){
            if (i < arguments.size() - 2){
                if (arguments[++i] == "c") operation = SyntacticAnalyser::CONVERT_C;
                else if (arguments[i] == "cpp") operation = SyntacticAnalyser::CONVERT_CPP;
                else if (arguments[i] == "java") operation = SyntacticAnalyser::CONVERT_JAVA;
                else hasValidArgs = false;

                fileName = arguments[++i];
                hasFileName = true;
            }
            else {
                hasValidArgs = false;
                break;
            }
        }
        else {
            fileName = arguments[i];
            hasFileName = true;
        }
    }

    if (hasFileName && hasValidArgs){
        output << QString("\n-----------------------").toUtf8() << endl;
        output << QString("* Arquivo de entrada : ").toUtf8() << fileName.toUtf8() << endl;
        output << QString("* Arquivo de saída : ").toUtf8() << outFileName.toUtf8() << endl;
        output << QString("* Operação de conversão : ").toUtf8() << operation << endl;
        output << QString("* Modo crítico : ").toUtf8() << criticalMode << endl;
        output << QString("* Sobrescrever : ").toUtf8() << overwriteFile << endl;
        output << QString("-----------------------\n").toUtf8() << endl;

        MessageLogger::getInstance(criticalMode);

        SyntacticAnalyser sintacticAnalyser(fileName, outFileName, operation, overwriteFile);
        QCoreApplication::exit(sintacticAnalyser.execute());
    }
    else if (!hasFileName) QCoreApplication::exit(MessageLogger::getInstance().log(MessageLogger::E_NO_FILE));
    else{
        output << QString("Argumentos : ").toUtf8();
        for (int i = 0; i < arguments.length(); ++i)
             output << arguments[i].toUtf8() << QString(i == arguments.length() - 1 ? "\n" : ", ").toUtf8();
        output << QString("\n-----------------------").toUtf8() << endl;
        QCoreApplication::exit(MessageLogger::getInstance().log(MessageLogger::E_INVALID_ARGS));
    }
}
