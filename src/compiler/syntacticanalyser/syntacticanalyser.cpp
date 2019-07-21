#include "syntacticanalyser.h"

SyntacticAnalyser::SyntacticAnalyser(const QString &fileName,
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

SyntacticAnalyser::~SyntacticAnalyser(){
    if (mainProgram) delete mainProgram;
}

int SyntacticAnalyser::execute(){
    QFile sourceFile(IN_FILE);
    if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&sourceFile);
        stream.setCodec("UTF-8"); //ISO 8859-1

        while(!stream.atEnd() && !programStarted){
            lineCounter++;
            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            int tabulation, i;
            Token currentToken;
            bool hasCommand = false;

            //Removes tabulation (but counts it)
            for (i = tabulation = 0; i < lLine.tokenCount() && !hasCommand; ++i){
                currentToken = lLine.nextToken();
                if (currentToken.type() == Token::TABULATION) ++tabulation;
                else hasCommand = true;
            }

            if (hasCommand){
                switch (currentToken.type()) {
                    case Token::RESERVED:
                        if (currentToken.word() == "programa"){
                            if (tabulation != indentFactor)
                                MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                    QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                    lineCounter);

                            if(i < lLine.tokenCount()){
                                currentToken = lLine.nextToken();
                                i++;

                                if (currentToken.type() == Token::IDENTIFIER){
                                    programStarted = true;
                                    currentBlock = mainProgram = new MainProgramParser();
                                    while(i < lLine.tokenCount()){
                                        currentToken = lLine.nextToken();
                                        if (currentToken.type() != Token::TABULATION)
                                            return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                                        "Não é possível inserir mais de um argumento após a declaração do programa.",
                                                        lineCounter, currentToken);
                                        i++;
                                    }
                                }
                                else return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                                "", lineCounter, currentToken);
                            }
                            else currentBlock = mainProgram = new MainProgramParser();
                            indentFactor++;
                            break;
                        }
                    default:
                        return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                    "Não pode haver qualquer texto antes da declaração do programa.", lineCounter,
                                    currentToken);
                        break;
                }
            }

            qDebug() << "Indent factor : " << indentFactor;
        }

        if (stream.atEnd() && !programStarted)
            return MessageLogger::getInstance().log(MessageLogger::E_NO_MAIN_ROUTINE);

        while(!stream.atEnd() && !programFinished){
            lineCounter++;
            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            //TODO: define this block as a method?
            int tabulation, i;
            Token currentToken;
            bool hasCommand = false;

            //Removes tabulation (but counts it)
            for (i = tabulation = 0; i < lLine.tokenCount() && !hasCommand; ++i){
                currentToken = lLine.nextToken();
                if (currentToken.type() == Token::TABULATION) ++tabulation;
                else hasCommand = true;
            }

            if (hasCommand){
                Token oldToken;
                switch (currentToken.type()) {
                    case Token::IDENTIFIER:
                        //WARNING: Return if critical!
                        if (tabulation != indentFactor)
                            MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                lineCounter);

                        oldToken = currentToken;
                        currentToken = lLine.nextToken();
                        i++;

                        switch (currentToken.type()) {
                            case Token::CONTROL:
                            {
                                ControlParser *controlParser = new ControlParser();
                                controlParser->addToken(oldToken);
                                controlParser->addToken(currentToken);
                                while(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    controlParser->addToken(currentToken);
                                    i++;
                                }
                                qDebug() << controlParser->insertNewVars(availableVars);
                                currentBlock->addProgramItem(controlParser);
                                break;
                            }
                            case Token::ASSIGNMENT:
                            {
                                ExpressionParser *expressionParser = new ExpressionParser(availableVars);
                                expressionParser->addToken(oldToken);
                                expressionParser->addToken(currentToken);
                                while(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    expressionParser->addToken(currentToken);
                                    i++;
                                }
                                qDebug() << expressionParser->validity();
                                currentBlock->addProgramItem(expressionParser);
                                break;
                            }
                            default:
                                return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                            "", lineCounter, currentToken);
                        }

                        break;

                    case Token::RESERVED:
                        if (currentToken.word().startsWith("fim-")){
                            if (tabulation != indentFactor - 1)
                                MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                    QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                    lineCounter);

                            QString blockEnd;
                            switch (currentBlock->blockType()) {
                                case BlockParser::IF:
                                case BlockParser::ELSE_IF:
                                case BlockParser::ELSE:
                                    blockEnd = "fim-se";
                                    break;
                                case BlockParser::WHILE:
                                    blockEnd = "fim-enquanto";
                                    break;
                                case BlockParser::MAIN:
                                    blockEnd = "fim-programa";
                                    programFinished = true;
                                    break;
                                default:
                                    return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                                "", lineCounter, currentToken);
                            }
                            if (currentToken.word() == blockEnd){
                                currentBlock->setSelfClosed(true);
                                currentBlock = currentBlock->parent();
                                indentFactor--;

                                while(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    if (currentToken.type() != Token::TABULATION)
                                        return MessageLogger::getInstance().log(MessageLogger::E_INVALID_T,
                                                    "", lineCounter, currentToken);
                                    i++;
                                }
                            }
                            else return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                            "", lineCounter, currentToken);
                            break;
                        }
                        else if (currentToken.word() == "enquanto"){
                            if (currentBlock->blockType() == BlockParser::DO_WHILE){
                                if (tabulation != indentFactor - 1)
                                    MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                        QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                        lineCounter);

                                if (i < lLine.tokenCount()){
                                    ExpressionParser *expression = new ExpressionParser(availableVars);
                                    while(i < lLine.tokenCount()){
                                        expression->addToken(lLine.nextToken());
                                        i++;
                                    }
                                    qDebug() << expression->validity();
                                    currentBlock->setBlockExpression(expression);
                                    currentBlock = currentBlock->parent();
                                    indentFactor--;
                                }
                                else return MessageLogger::getInstance().log(MessageLogger::E_EXPECTED_COND,
                                                "Esperado identificador ou expressão.", lineCounter);
                            }
                            else {
                                if (tabulation != indentFactor)
                                    MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                        QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                        lineCounter);

                                ExpressionParser *expression = new ExpressionParser(availableVars);

                                bool hasNFinished = true;
                                while(i < lLine.tokenCount() && hasNFinished){
                                    Token tCurrent = lLine.nextToken();
                                    if (tCurrent.word() == "faça") hasNFinished = false;
                                    else expression->addToken(tCurrent);
                                    i++;
                                }
                                if (hasNFinished) return MessageLogger::getInstance().log(MessageLogger::E_NO_END_OF_EXPR,
                                                            "", lineCounter);

                                while (i < lLine.tokenCount()){
                                    Token current = lLine.nextToken();
                                    if (current.type() != Token::TABULATION)
                                        return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                                    "Não pode haver qualquer declaração após a construção da estrutura.",
                                                    lineCounter, current);
                                }

                                qDebug() << expression->validity();
                                BlockParser *block = new BlockParser(currentBlock, BlockParser::WHILE, expression);
                                currentBlock->addProgramItem(block);
                                currentBlock = block;
                                indentFactor++;
                            }
                        }
                        else if (currentToken.word() == "se"){
                            if (tabulation != indentFactor)
                                MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                    QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                    lineCounter);

                            ExpressionParser *expression = new ExpressionParser(availableVars);

                            bool hasNFinished = true;
                            while(i < lLine.tokenCount() && hasNFinished){
                                Token tCurrent = lLine.nextToken();
                                if (tCurrent.word() == "então") hasNFinished = false;
                                else expression->addToken(tCurrent);
                                i++;
                            }
                            if (hasNFinished) return MessageLogger::getInstance().log(MessageLogger::E_NO_END_OF_EXPR,
                                                        "", lineCounter);

                            while (i < lLine.tokenCount()){
                                Token current = lLine.nextToken();
                                if (current.type() != Token::TABULATION)
                                    return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                                "Não pode haver qualquer declaração após a construção da estrutura.",
                                                lineCounter, current);
                            }

                            //TODO: Check expression validity
                            qDebug() << expression->validity();
                            BlockParser *block = new BlockParser(currentBlock, BlockParser::IF, expression);
                            currentBlock->addProgramItem(block);
                            currentBlock = block;
                            indentFactor++;
                        }
                        else if (currentToken.word() == "senão"){
                            if (tabulation != indentFactor - 1)
                                MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                    QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                    lineCounter);

                            if (currentBlock->blockType() != BlockParser::IF &&
                                    currentBlock->blockType() != BlockParser::ELSE_IF)
                                return MessageLogger::getInstance().log(MessageLogger::E_STRUCTURE_EXPECTED,
                                            "Esperava-se estrutura condicional.", lineCounter);

                            if (i < lLine.tokenCount()){
                                Token oldToken = currentToken;
                                currentToken = lLine.nextToken();
                                ++i;

                                if (currentToken.word() == "se"){
                                    ExpressionParser *expression = new ExpressionParser(availableVars);

                                    bool hasNFinished = true;
                                    while(i < lLine.tokenCount() && hasNFinished){
                                        Token tCurrent = lLine.nextToken();
                                        if (tCurrent.word() == "faça") hasNFinished = false;
                                        else expression->addToken(tCurrent);
                                        i++;
                                    }
                                    if (hasNFinished)
                                        return MessageLogger::getInstance().log(MessageLogger::E_NO_END_OF_EXPR,
                                                    "", lineCounter);

                                    while (i < lLine.tokenCount()){
                                        Token current = lLine.nextToken();
                                        if (current.type() != Token::TABULATION)
                                            return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                                        "Token inesperado após término de expressão.", lineCounter, current);
                                    }

                                    currentBlock = currentBlock->parent();
                                    qDebug() << expression->validity();
                                    BlockParser *block = new BlockParser(currentBlock, BlockParser::ELSE_IF, expression);
                                    currentBlock->addProgramItem(block);
                                    currentBlock = block;
                                }
                                else return MessageLogger::getInstance().log(MessageLogger::E_STRUCTURE_EXPECTED,
                                                "Estrutura esperada: \"se\"", lineCounter);
                            }
                            else {
                                currentBlock = currentBlock->parent();
                                BlockParser block(currentBlock, BlockParser::ELSE);
                                block.setSelfClosed(true);
                                currentBlock = &block;
                            }
                        }
                        else if (currentToken.word() == "leia"){
                            if (tabulation != indentFactor)
                                MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                    QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                    lineCounter);

                            IOParser *inExpression = new IOParser(IOParser::INPUT);
                            while(i < lLine.tokenCount()){
                                inExpression->addToken(lLine.nextToken());
                                i++;
                            }
                            qDebug() << inExpression->isValid();
                            currentBlock->addProgramItem(inExpression);
                        }
                        else if (currentToken.word() == "escreva"){
                            if (tabulation != indentFactor)
                                MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                    QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                    lineCounter);

                            IOParser *outExpression = new IOParser(IOParser::INPUT);
                            while(i < lLine.tokenCount()){
                                outExpression->addToken(lLine.nextToken());
                                i++;
                            }
                            qDebug() << outExpression->isValid();
                            currentBlock->addProgramItem(outExpression);
                        }
                        else return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                        "", lineCounter, currentToken);
                        break;

                    default:
                        return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                    "", lineCounter, currentToken);
                        break;
                }
            }
        }

        while(!stream.atEnd()){
            lineCounter++;
            QString line = stream.readLine();
            qInfo() << "Line : " << line;
            LexicalAnalyser lLine(line);
            qInfo() << "Token count : " << lLine.tokenCount() << "\n";

            //Detects invalid tokens
            while(lLine.hasNextToken()){
                Token currentToken = lLine.nextToken();
                if (currentToken.type() != Token::TABULATION)
                    return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                "Não pode haver qualquer declaração após o término do programa principal.",
                                lineCounter, currentToken);
            }
        }
        sourceFile.close();

        //TODO: Convert file

        return MessageLogger::getInstance().log(MessageLogger::SUCCESS);
    }

    return MessageLogger::getInstance().log(MessageLogger::E_CANT_OPEN_FILE);
}
