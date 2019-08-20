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

#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <QList>

#include "../programitem.h"
#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/lexicalanalyser/token.h"
#include "syntacticanalyser/controlparser/controlparser.h"

class ExpressionParser : public ProgramItem
{
public:
    enum Validity{
        INVALID,
        INVALID_SEM,
        VALID_NO_ATTRIB,
        VALID_ATTRIB
    };

    ExpressionParser(const QHash<QString, ControlParser::IdentifierType> &globalVars = QHash<QString, ControlParser::IdentifierType>());
    ~ExpressionParser();

    inline void addToken(const Token &token){ this->tokens.append(token); }
    inline ControlParser::IdentifierType operationType(){ return this->cOperationType; }

    Validity validity();

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    int parenthesesCount;

    QList<Token> tokens;
    ControlParser::IdentifierType cOperationType;
    QHash<QString, ControlParser::IdentifierType> declaredVars;

    void toOutFileC(int indentFactor, QTextStream &stream);
    void toOutFileCPP(int indentFactor, QTextStream &stream);
    void toOutFileJAVA(int indentFactor, QTextStream &stream);
};

#endif // EXPRESSIONPARSER_H
