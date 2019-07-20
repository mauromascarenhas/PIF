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
    qInfo() << "\n";

    //TODO: Parse arguments

    if (argCount > 1){
        SyntacticAnalyser sintacticAnalyser(arguments[1]);
        QCoreApplication::exit(sintacticAnalyser.execute());
    }
    else {
        //TODO: Return error?
        qCritical() << "Nenhum arquivo especificado.";

        QCoreApplication::exit(0);
    }
}
