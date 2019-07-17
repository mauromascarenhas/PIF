#include "sintacticanalyser.h"

SintacticAnalyser::SintacticAnalyser(const QString &fileName,
    const QString &outFileName, OperationType operation) :
    IN_FILE(fileName), OUT_FILE(outFileName), OPERATION(operation)
{
    currentBlock = nullptr;
    mainProgram = nullptr;

    programFinished = false;

    lineCounter = 0;
    indentFactor = 0;
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
                    currentBlock = mainProgram = new MainProgramParser(currentToken.word());
                    programID = true;
                    indentFactor++;
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

        if (!mainProgram){
            qCritical() << "Não há corpo de programa principal.";
            //TODO: Return what?
            return 0;
        }

        while(!stream.atEnd()){
            lineCounter++;

            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            int i;
            Token currentToken;
            //Verifica a corretude da tabulação
            for(i = 0; i < lLine.tokenCount() && i < indentFactor - 1; ++i){
                currentToken = lLine.nextToken();
                qInfo() << "Token atual : " << currentToken.word()
                        << " : " << currentToken.type();
                if (currentToken.type() == Token::INVALID){
                    qCritical() << "Token inválido detectado : " << currentToken.word();
                    //TODO: Return what?
                    return 0;
                }
                else if (currentToken.type() != Token::TABULATION){
                    if (currentToken.type() == Token::RESERVED){
                        if (currentToken.word().startsWith("fim-")){
                            if (dynamic_cast<MainProgramParser *>(currentBlock) &&
                                    currentToken.word() == "fim-programa"){

                            }
                            else if (dynamic_cast<MainProgramParser *>(currentBlock) &&
                                     currentToken.word() == "fim-programa"){
                                programFinished = true;
                                indentFactor--;
                            }
                            else {
                                //TODO: fix here?
                                QString endOfBlock;
                                switch (currentBlock->blockType()) {
                                    case BlockParser::IF:
                                    case BlockParser::ELSE:
                                    case BlockParser::ELSE_IF:
                                        endOfBlock = "fim-se";
                                        break;
                                    case BlockParser::WHILE:
                                        endOfBlock = "fim-enquanto";
                                        break;
                                    case BlockParser::DO_WHILE:
                                        endOfBlock = "enquanto";
                                    default:
                                        qCritical() << "Sequência de controle inválida.";
                                        //TODO: Return what?
                                        return 0;
                                }
                                if (currentToken.word() == endOfBlock){
                                    currentBlock = currentBlock->parent();
                                    indentFactor--;
                                }
                                else if (endOfBlock == "fim-se" && currentBlock->blockType() != BlockParser::ELSE){
                                    //TODO: Nest another block? + Evaluate expression
                                }
                                if (currentToken.word() == "enquanto"){
                                    //TODO: Evaluate expression
                                }
                            }
                        }
                        else {
                            qCritical() << "Token inesperado!";
                            //TODO: Return what?
                            return 0;
                        }
                    }
                    else{
                        qWarning() << "Erro de tabulação!";
                        //TODO: Return what?
                        return 0;
                    }
                }
            }

            if (programFinished){
                while (i < lLine.tokenCount()){
                    if (lLine.nextToken().type() != Token::TABULATION){
                        qCritical() << "Token inesperado após término do programa!";
                        //TODO: Return what?
                        return 0;
                    }
                    ++i;
                }
            }

            while (i < lLine.tokenCount()){
                //TODO: Q?
                currentToken = lLine.nextToken();
                qInfo() << "Token atual : " << currentToken.word()
                        << " : " << currentToken.type();
                i++;

                switch (currentToken.type()) {
                    case Token::IDENTIFIER:
                        Token prevToken = currentToken;
                        currentToken = lLine.nextToken();
                        qInfo() << "Token atual : " << currentToken.word()
                                << " : " << currentToken.type();
                        if (currentToken.type() == Token::ARITHIMETIC_OP){
                            //TODO: Create arithmetic/boolean expression -> Check;
                        }
                        else if (currentToken.type() == Token::CONTROL){
                            //TODO: Create control sequence (declare)
                        }
                        else {
                            qCritical() << "Sequência de tokens inválida.";
                            //TODO: Return what?
                            return 0;
                        }
                        i++;
                        break;
                    default:
                        break;
                }
            }

            qInfo() << "------------------------------\n";
        }
        sourceFile.close();

        if (indentFactor){
            qCritical() << "Há algum bloco de código no programa que não foi encerrado.";
            //TODO: Return what?
            return 0;
        }

        //TODO: Convert file?

        return 0;
    }

    qCritical() << "Não foi possível abrir o código fonte. Você tem as permissões necessárias?";
    return 0; //TODO: Return what?
}
