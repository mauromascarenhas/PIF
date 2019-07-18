#include "sintacticanalyser.h"

SintacticAnalyser::SintacticAnalyser(const QString &fileName,
    const QString &outFileName, OperationType operation) :
    IN_FILE(fileName), OUT_FILE(outFileName), OPERATION(operation)
{
    currentBlock = nullptr;
    mainProgram = nullptr;

    programStarted = false;
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
            lineCounter++;
            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            bool hasCommand;
            int tabulation, i;
            Token currentToken;

            //Removes tabulation
            for (i = tabulation = 0; i < lLine.tokenCount() && !hasCommand; ++i){
                currentToken = lLine.nextToken();
                if (currentToken.type() == Token::TABULATION) ++tabulation;
                else hasCommand = true;
            }

            if (hasCommand){
                Token oldToken;
                switch (currentToken.type()) {
                    case Token::IDENTIFIER:
                        if (!programStarted) {
                            qCritical() << "Token inesperado antes do início do programa!";
                            //TODO: Return what?
                            return 0;
                        } else if (programFinished){
                            qCritical() << "Token inesperado após o término do programa!";
                            //TODO: Return what?
                            return 0;
                        }

                        if (tabulation != indentFactor) qWarning() << "Erro no fator de tabulação!";

                        oldToken = currentToken;
                        currentToken = lLine.nextToken();
                        i++;

                        switch (currentToken.type()) {
                            case Token::CONTROL:
                            {
                                ControlParser controlParser;
                                controlParser.addToken(oldToken);
                                controlParser.addToken(currentToken);
                                while(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    controlParser.addToken(currentToken);
                                    i++;
                                }
                                currentBlock->addProgramItem(controlParser);
                                break;
                            }
                            case Token::ARITHIMETIC_OP:
                            case Token::BOOLEAN_OP:
                            {
                                ExpressionParser expressionParser;
                                expressionParser.addToken(oldToken);
                                expressionParser.addToken(currentToken);
                                while(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    expressionParser.addToken(currentToken);
                                    i++;
                                }
                                currentBlock->addProgramItem(expressionParser);
                                break;
                            }
                            default:
                                qCritical() << "Token inesperado.";
                                //TODO: Return what?
                                return 0;
                        }

                        break;

                    case Token::RESERVED:
                        if (!programStarted){
                            if (currentToken.word() == "programa"){
                                if (tabulation != indentFactor) qWarning() << "Erro no fator de tabulação!";

                                if(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    if (currentToken.type() == Token::IDENTIFIER){
                                        currentBlock = mainProgram = new MainProgramParser();
                                        while(i < lLine.tokenCount()){
                                            currentToken = lLine.nextToken();
                                            if (currentToken.type() != Token::TABULATION){
                                                qCritical() << "Token inválido!";
                                                //TODO: Return what?
                                                return 0;
                                            }
                                            i++;
                                        }
                                    }
                                    else {
                                        qCritical() << "Token inesperado.";
                                        //TODO: Return what?
                                        return 0;
                                    }
                                }
                                else currentBlock = mainProgram = new MainProgramParser();
                                indentFactor++;
                            }
                            else {
                                qCritical() << "Token inesperado antes do início do programa!";
                                //TODO: Return what?
                                return 0;
                            }
                        }
                        else {
                            if (currentToken.word().startsWith("fim-")){
                                if (tabulation != indentFactor - 1) qWarning() << "Erro no fator de tabulação!";

                                QString fimBloco;
                                switch (currentBlock->blockType()) {
                                    case BlockParser::IF:
                                    case BlockParser::ELSE_IF:
                                    case BlockParser::ELSE:
                                        fimBloco = "fim-se";
                                        break;
                                    case BlockParser::WHILE:
                                        fimBloco = "fim-enquanto";
                                        break;
                                    case BlockParser::MAIN:
                                        fimBloco = "fim-programa";
                                        programFinished = true;
                                        break;
                                    default:
                                        qCritical() << "Token inesperado.";
                                        //TODO: Return what?
                                        return 0;
                                }
                                if (currentToken.word() == fimBloco){
                                    currentBlock->setSelfClosed(true);
                                    currentBlock = currentBlock->parent();
                                    indentFactor--;

                                    while(i < lLine.tokenCount()){
                                        currentToken = lLine.nextToken();
                                        if (currentToken.type() != Token::TABULATION){
                                            qCritical() << "Token inválido!";
                                            //TODO: Return what?
                                            return 0;
                                        }
                                        i++;
                                    }
                                }
                                else {
                                    qCritical() << "Token inesperado.";
                                    //TODO: Return what?
                                    return 0;
                                }
                                break;
                            }
                            else if (currentToken.word() == "enquanto"){
                                if (currentBlock->blockType() == BlockParser::DO_WHILE){
                                    if (tabulation != indentFactor - 1) qWarning() << "Erro no fator de tabulação!";
                                    //TODO: Fecha bloco
                                }
                                else {
                                    if (tabulation != indentFactor) qWarning() << "Erro no fator de tabulação!";
                                    //TODO: Aninha bloco
                                }
                            }
                            else if (currentToken.word() == "se"){
                                if (tabulation != indentFactor) qWarning() << "Erro no fator de tabulação!";

                                ExpressionParser expression;
                                while(i < lLine.tokenCount()){
                                    expression.addToken(lLine.nextToken());
                                    i++;
                                }

                                //TODO: Check expression validity
                                BlockParser blockParser(currentBlock, BlockParser::IF, expression);
                                currentBlock->addProgramItem(blockParser);
                                currentBlock = &blockParser;
                                indentFactor++;
                            }
                            else if (currentToken.word() == "senão"){
                                if (tabulation != indentFactor - 1) qWarning() << "Erro no fator de tabulação!";

                                if (currentBlock->blockType() != BlockParser::IF &&
                                        currentBlock->blockType() != BlockParser::ELSE_IF){
                                    qCritical() << "Esperado estrutura condicional!";
                                    //TODO: Return what?
                                    return 0;
                                }

                                if (i < lLine.tokenCount()){
                                    Token oldToken = currentToken;
                                    currentToken = lLine.nextToken();
                                    ++i;

                                    if (currentToken.word() == "se"){
                                        ExpressionParser expressao;
                                        while(i < lLine.tokenCount()){
                                            expressao.addToken(lLine.nextToken());
                                            ++i;
                                        }
                                        currentBlock = currentBlock->parent();
                                        BlockParser block(currentBlock, BlockParser::ELSE_IF, expressao);
                                        currentBlock = &block;
                                    }
                                    else {
                                        qCritical() << "Esperado estrutura \"se\"!";
                                        //TODO: Return what?
                                        return 0;
                                    }
                                }
                                else {
                                    currentBlock = currentBlock->parent();
                                    BlockParser block(currentBlock, BlockParser::ELSE);
                                    block.setSelfClosed(true);
                                    currentBlock = &block;
                                }
                            }
                            else if (currentToken.word() == "leia"){
                                if (tabulation != indentFactor) qWarning() << "Erro no fator de tabulação!";
                                //TODO: Implementar
                            }
                            else if (currentToken.word() == "escreva"){
                                if (tabulation != indentFactor) qWarning() << "Erro no fator de tabulação!";
                                //TODO: Implementar
                            }
                            else{
                                qCritical() << "Token inesperado.";
                                //TODO: Return what?
                                return 0;
                            }
                        }
                        break;

                    default:
                        qCritical() << "Token inesperado.";
                        //TODO: Return what?
                        return 0;
                }
            }
        }
        sourceFile.close();

        //TODO: Convert file?

        return 0;
    }

    qCritical() << "Não foi possível abrir o código fonte. Você tem as permissões necessárias?";
    return 0; //TODO: Return what?
}
