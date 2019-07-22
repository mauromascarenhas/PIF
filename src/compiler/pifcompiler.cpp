#include "pifcompiler.h"

PIFCompiler::PIFCompiler(QObject *parent) : QObject(parent)
{
    arguments = QCoreApplication::arguments();
    argCount = arguments.size();
}

PIFCompiler::~PIFCompiler(){
}

void PIFCompiler::execute(){
    qInfo() << "Parameters : " << arguments;
    qInfo() << "\n-----------------------";

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
        qInfo() << "  Settings\n";
        qInfo() << "* In file : " << fileName;
        qInfo() << "* Out file : " << outFileName;
        qInfo() << "* Convert op : " << operation;
        qInfo() << "* Critical mode : " << criticalMode;
        qInfo() << "* Overwrite : " << criticalMode;
        qInfo() << "-----------------------\n";

        MessageLogger::getInstance(criticalMode);

        SyntacticAnalyser sintacticAnalyser(fileName, outFileName, operation, overwriteFile);
        QCoreApplication::exit(sintacticAnalyser.execute());
    }
    else if (!hasFileName) QCoreApplication::exit(MessageLogger::getInstance().log(MessageLogger::E_NO_FILE));
    else QCoreApplication::exit(MessageLogger::getInstance().log(MessageLogger::E_INVALID_ARGS));
}
