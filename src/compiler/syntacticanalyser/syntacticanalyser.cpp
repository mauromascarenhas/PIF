/*
 *     This file is part of PIFC (Compiler).
 *
 *    PIFC (Compiler) is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Foobar is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "syntacticanalyser.h"

SyntacticAnalyser::SyntacticAnalyser(const QString &fileName,
    const QString &outFileName, OperationType operation, bool forceOverwrite) :
    OVERWRITE(forceOverwrite), IN_FILE(fileName), OUT_FILE(outFileName),
    OPERATION(operation)
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
            LexicalAnalyser lLine(line);

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
                            if (tabulation != indentFactor){
                                int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                            QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                            lineCounter);
                                if (ret) return ret;
                            }

                            if(i < lLine.tokenCount()){
                                currentToken = lLine.nextToken();
                                i++;

                                if (currentToken.type() == Token::IDENTIFIER){
                                    programStarted = true;
                                    currentBlock = mainProgram = new MainProgramParser(currentToken.word());
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
                }
            }
        }

        if (stream.atEnd() && !programStarted)
            return MessageLogger::getInstance().log(MessageLogger::E_NO_MAIN_ROUTINE);

        bool canSkipLine = false;
        while(!stream.atEnd() && !programFinished){
            lineCounter++;
            QString line = stream.readLine();
            LexicalAnalyser lLine(line);

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
                canSkipLine = false;

                Token oldToken;
                switch (currentToken.type()) {
                    case Token::IDENTIFIER:
                        if (tabulation != indentFactor){
                            int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                        QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                        lineCounter);
                            if (ret) return ret;
                        }

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
                                if (!controlParser->insertNewVars(availableVars))
                                    return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED, "", lineCounter);
                                currentBlock->addProgramItem(controlParser);
                                break;
                            }
                            case Token::ASSIGNMENT:
                            {
                                ExpressionParser *expression = new ExpressionParser(availableVars);
                                expression->addToken(oldToken);
                                expression->addToken(currentToken);
                                while(i < lLine.tokenCount()){
                                    currentToken = lLine.nextToken();
                                    expression->addToken(currentToken);
                                    i++;
                                }

                                ExpressionParser::Validity eVal = expression->validity();
                                if (eVal != ExpressionParser::VALID_ATTRIB){
                                    if (eVal == ExpressionParser::VALID_NO_ATTRIB)
                                        return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED,
                                                                                "Expressões de instrução (booleanas, numéricas ou literais) devem realizar atribuições.",
                                                                                lineCounter);
                                    return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED,
                                                                            "", lineCounter);
                                }


                                currentBlock->addProgramItem(expression);
                                break;
                            }
                            default:
                                return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                            "", lineCounter, currentToken);
                        }

                        canSkipLine = true;
                        break;

                    case Token::RESERVED:
                        if (currentToken.word().startsWith("fim-")){
                            if (tabulation != indentFactor - 1){
                                int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                            QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                            lineCounter);
                                if (ret) return ret;
                            }

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

                            canSkipLine = true;
                            break;
                        }
                        else if (currentToken.word() == "enquanto"){
                            if (currentBlock->blockType() == BlockParser::DO_WHILE){
                                if (tabulation != indentFactor - 1){
                                    int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                                QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                                lineCounter);
                                    if (ret) return ret;
                                }

                                if (i < lLine.tokenCount()){
                                    ExpressionParser *expression = new ExpressionParser(availableVars);
                                    while(i < lLine.tokenCount()){
                                        expression->addToken(lLine.nextToken());
                                        i++;
                                    }

                                    ExpressionParser::Validity eVal = expression->validity();
                                    if (eVal != ExpressionParser::VALID_NO_ATTRIB){
                                        if (eVal == ExpressionParser::VALID_ATTRIB)
                                            return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                                                    "Expressões de bloco não devem fazer atribuições.",
                                                                                    lineCounter, Token("="));
                                        return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED,
                                                                                "", lineCounter);
                                    }

                                    currentBlock->setBlockExpression(expression);
                                    currentBlock = currentBlock->parent();
                                    indentFactor--;
                                }
                                else return MessageLogger::getInstance().log(MessageLogger::E_EXPECTED_COND,
                                                "Esperado identificador ou expressão.", lineCounter);
                                canSkipLine = true;
                            }
                            else {
                                if (tabulation != indentFactor){
                                    int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                                QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                                lineCounter);
                                    if (ret) return ret;
                                }

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
                                    i++;
                                }

                                ExpressionParser::Validity eVal = expression->validity();
                                if (eVal != ExpressionParser::VALID_NO_ATTRIB){
                                    if (eVal == ExpressionParser::VALID_ATTRIB)
                                        return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                                                "Expressões de bloco não devem fazer atribuições.",
                                                                                lineCounter, Token("="));
                                    return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED,
                                                                            "", lineCounter);
                                }

                                BlockParser *block = new BlockParser(currentBlock, BlockParser::WHILE, expression);
                                currentBlock->addProgramItem(block);
                                currentBlock = block;
                                indentFactor++;
                            }
                        }
                        else if (currentToken.word() == "se"){
                            if (tabulation != indentFactor){
                                int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                            QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                            lineCounter);
                                if (ret) return ret;
                            }

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
                                i++;
                            }

                            ExpressionParser::Validity eVal = expression->validity();
                            if (eVal != ExpressionParser::VALID_NO_ATTRIB){
                                if (eVal == ExpressionParser::VALID_ATTRIB)
                                    return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                                            "Expressões de bloco não devem fazer atribuições.",
                                                                            lineCounter, Token("="));
                                return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED,
                                                                        "", lineCounter);
                            }

                            BlockParser *block = new BlockParser(currentBlock, BlockParser::IF, expression);
                            currentBlock->addProgramItem(block);
                            currentBlock = block;
                            indentFactor++;
                        }
                        else if (currentToken.word() == "senão"){
                            if (tabulation != indentFactor - 1){
                                int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                            QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                            lineCounter);
                                if (ret) return ret;
                            }

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
                                        if (tCurrent.word() == "então") hasNFinished = false;
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
                                        i++;
                                    }

                                    currentBlock = currentBlock->parent();

                                    ExpressionParser::Validity eVal = expression->validity();
                                    if (eVal != ExpressionParser::VALID_NO_ATTRIB){
                                        if (eVal == ExpressionParser::VALID_ATTRIB)
                                            return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T_EXP,
                                                                                    "Expressões de bloco não devem fazer atribuições.",
                                                                                    lineCounter, Token("="));
                                        return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED,
                                                                                "", lineCounter);
                                    }

                                    BlockParser *block = new BlockParser(currentBlock, BlockParser::ELSE_IF, expression);
                                    currentBlock->addProgramItem(block);
                                    currentBlock = block;
                                }
                                else return MessageLogger::getInstance().log(MessageLogger::E_STRUCTURE_EXPECTED,
                                                "Estrutura esperada: \"se\"", lineCounter);
                            }
                            else {
                                currentBlock = currentBlock->parent();
                                BlockParser *block = new BlockParser(currentBlock, BlockParser::ELSE);
                                currentBlock->addProgramItem(block);
                                currentBlock = block;
                            }
                        }
                        else if (currentToken.word() == "faça"){
                            BlockParser *block = new BlockParser(currentBlock, BlockParser::DO_WHILE);
                            currentBlock->addProgramItem(block);
                            currentBlock = block;
                            indentFactor++;
                        }
                        else if (currentToken.word() == "leia"){
                            if (tabulation != indentFactor){
                                int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                            QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                            lineCounter);
                                if (ret) return ret;
                            }

                            IOParser *inExpression = new IOParser(IOParser::INPUT, availableVars);
                            while(i < lLine.tokenCount()){
                                inExpression->addToken(lLine.nextToken());
                                i++;
                            }
                            if (!inExpression->isValid())
                                return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED, "", lineCounter);
                            currentBlock->addProgramItem(inExpression);
                            canSkipLine = true;
                        }
                        else if (currentToken.word() == "escreva"){
                            if (tabulation != indentFactor){
                                int ret = MessageLogger::getInstance().log(MessageLogger::W_INDENT_FACTOR,
                                            QString("Esperado %1, obtido %2").arg(indentFactor, tabulation),
                                            lineCounter);
                                if (ret) return ret;
                            }

                            IOParser *outExpression = new IOParser(IOParser::OUTPUT, availableVars);
                            while(i < lLine.tokenCount()){
                                outExpression->addToken(lLine.nextToken());
                                i++;
                            }
                            if (!outExpression->isValid())
                                return MessageLogger::getInstance().log(MessageLogger::E_UNDEFINED, "", lineCounter);
                            currentBlock->addProgramItem(outExpression);
                            canSkipLine = true;
                        }
                        else return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                        "", lineCounter, currentToken);
                        break;

                    default:
                        return MessageLogger::getInstance().log(MessageLogger::E_UNEXPECTED_T,
                                    "", lineCounter, currentToken);
                }
            }
            else if (canSkipLine){
                currentBlock->addProgramItem(new BlankItem());
                canSkipLine = false;
            }
        }

        while(!stream.atEnd()){
            lineCounter++;
            QString line = stream.readLine();
            LexicalAnalyser lLine(line);

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

        QString outExt;
        switch (OPERATION) {
            case CONVERT_CPP:
                outExt = ".cpp";
                break;
            case CONVERT_JAVA:
                outExt = ".java";
                break;
            default:
                outExt = ".c";
                break;
        }

        QString outFileName;
        if (OUT_FILE.isEmpty()){
            if (mainProgram->programName().isEmpty())
                outFileName = QFileInfo(IN_FILE).absolutePath() + QDir::separator()
                                + QFileInfo(IN_FILE).baseName()
                                + outExt;
            else outFileName = QFileInfo(IN_FILE).absolutePath() + QDir::separator()
                                + mainProgram->programName()
                                + outExt;
        }
        else if (QFileInfo(OUT_FILE).suffix() == outExt) outFileName = OUT_FILE;
        else {
            outFileName = QFileInfo(OUT_FILE).absolutePath() + QDir::separator()
                            + QFileInfo(OUT_FILE).baseName()
                            + outExt;
            MessageLogger::getInstance().log(MessageLogger::INFO,
                                             QString("Arquivo fornecido como saída (%1) renomeado para : %2")
                                                .arg(OUT_FILE, outFileName));
        }

        if (OPERATION == CONVERT_JAVA){
            if (mainProgram->programName().isEmpty())
                return MessageLogger::getInstance().log(MessageLogger::E_EMPTY_JAVA_CLASS);
            else if (QFileInfo(outFileName).baseName() != mainProgram->programName())
                return MessageLogger::getInstance().log(MessageLogger::E_INVALID_JAVA_CLASS,
                                                        QString("Nome da classe : '%1'. Nome base do arquivo : '%2'")
                                                            .arg(mainProgram->programName(), QFileInfo(OUT_FILE).baseName()));
        }

        if (QFile::exists(outFileName)){
            if (OVERWRITE){
                if (QFile::remove(outFileName)){
                    outFileName = QFileInfo(outFileName).absolutePath() + QDir::separator()
                                    + QFileInfo(outFileName).baseName()
                                    + outExt;
                    MessageLogger::getInstance().log(MessageLogger::WARNING,
                                                     QString("O arquivo fornecido como saída (%1) foi sobrescrito.")
                                                        .arg(outFileName));
                }
                else return MessageLogger::getInstance().log(MessageLogger::E_OUT_CANT_REMOVE, OUT_FILE);
            }
            else return MessageLogger::getInstance().log(MessageLogger::E_OUT_EXISTS, OUT_FILE);
        }

        QFile outFile(outFileName);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream stream(&outFile);
            stream.setCodec("UTF-8"); //ISO 8859-1
            switch (OPERATION) {
                case CONVERT_CPP:
                    mainProgram->toOutFile(0, stream, ProgramItem::CPP);
                    break;
                case CONVERT_JAVA:
                    mainProgram->toOutFile(0, stream, ProgramItem::JAVA);
                    break;
                default:
                    mainProgram->toOutFile(0, stream);
                    break;
            }
        }
        else return MessageLogger::getInstance().log(MessageLogger::E_CANT_OPEN_FILE);

        return MessageLogger::getInstance().log(MessageLogger::SUCCESS);
    }

    return MessageLogger::getInstance().log(MessageLogger::E_CANT_OPEN_FILE);
}
