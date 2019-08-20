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

#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent, BlockType blocktype,
    ExpressionParser *expression, const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem(), BLOCKTYPE(blocktype)
{
    parentBlock = parent;
    declaredVars = globalVars;
    blockExpression = expression;
}

BlockParser::~BlockParser(){
    while (programItemsC.size()){
        delete programItemsC.first();
        programItemsC.removeFirst();
    }
}

void BlockParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang conv){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    indentFactor++;

    switch (BLOCKTYPE) {
        case WHILE:
            stream << QString("%1while (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString("){\n").toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        case DO_WHILE:
            stream << QString("%1do {\n").arg(tabs).toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1} while (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString(");\n").toUtf8();
            break;
        case IF:
            stream << QString("%1if (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString("){\n").toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        case ELSE_IF:
            stream << QString("%1else if (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString("){\n").toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        case ELSE:
            stream << QString("%1else {\n").arg(tabs).toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        default:
            break;
    }
}
