#include "sintacticanalyser.h"

SintacticAnalyser::SintacticAnalyser(const QString &fileName,
    const QString &outFileName, OperationType operation) :
    IN_FILE(fileName), OUT_FILE(outFileName), OPERATION(operation)
{
    currentItem = nullptr;
    mainProgram = nullptr;
}

SintacticAnalyser::~SintacticAnalyser(){
    if (mainProgram) delete mainProgram;
}

int SintacticAnalyser::execute(){
    QFile sourceFile(IN_FILE);
    if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&sourceFile);
        stream.setCodec("UTF-8"); //ISO 8859-1

        while(!stream.atEnd()){
            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            bool startedProgram = false;
            bool programID = false;
            for (int i = 0; i < lLine.tokenCount(); ++i){
                Token currentToken = lLine.nextToken();
                qInfo() << "Token atual : " << currentToken.word()
                        << " : " << currentToken.type();
                if (currentToken.type() == Token::RESERVED && !startedProgram){
                    if (currentToken.word() == "programa") startedProgram = true;
                    else {
                        qCritical() << "Token inesperado na inicialização do programa : "
                                    << currentToken.word();
                        //TODO: Return what?
                        return 0;
                    }
                }
                else if (currentToken.type() == Token::IDENTIFIER && startedProgram){
                    currentItem = mainProgram = new MainProgramParser(currentToken.word());
                    programID = true;
                }
                else if (currentToken.type() != Token::TABULATION){
                    qCritical() << "Token inesperado na inicialização do programa : "
                                << currentToken.word();
                    //TODO: Return what?
                    return 0;
                }
            }
            qInfo() << "------------------------------\n";
            if (programID) break;
        }

        while(!stream.atEnd()){
            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            for(int i = 0; i < lLine.tokenCount(); ++i){
                Token currentToken = lLine.nextToken();
                qInfo() << "Token atual : " << currentToken.word()
                        << " : " << currentToken.type();
            }
            qInfo() << "------------------------------\n";
        }
        sourceFile.close();

        return 0;
    }

    qCritical() << "Não foi possível abrir o código fonte. Você tem as permissões necessárias?";
    return 0; //TODO: Return what?
}
