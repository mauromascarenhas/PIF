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

#ifndef BLOCKPARSER_H
#define BLOCKPARSER_H

#include <QList>
#include <QHash>

#include "../programitem.h"
#include "../blankitem/blankitem.h"
#include "../controlparser/controlparser.h"
#include "../expresionparser/expressionparser.h"

class BlockParser: public ProgramItem
{
public:
    enum BlockType{
        WHILE,
        DO_WHILE,
        IF,
        ELSE_IF,
        ELSE,
        MAIN,
        UNDEFINED
    };
    Q_ENUM(BlockType)

    BlockParser(BlockParser *parent = nullptr,
                BlockType blocktype = UNDEFINED,
                ExpressionParser *expression = nullptr,
                const QHash<QString, ControlParser::IdentifierType> &globalVars = QHash<QString, ControlParser::IdentifierType>());
    ~BlockParser();

    inline BlockParser * parent() { return this->parentBlock; }

    inline ExpressionParser * getBlockExpression() { return this->blockExpression; }
    inline void setBlockExpression(ExpressionParser *expression) { this->blockExpression = expression; }

    inline void addProgramItem(ProgramItem *programItem) { this->programItemsC.append(programItem); }
    inline QList<ProgramItem*> programItems() { return this->programItemsC; }

    inline BlockType blockType(){ return this->BLOCKTYPE; }

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    BlockParser *parentBlock;

protected:
    const BlockType BLOCKTYPE;

    QList<ProgramItem*> programItemsC;
    ExpressionParser* blockExpression;
    QHash<QString, ControlParser::IdentifierType> declaredVars;
};

#endif // BLOCKPARSER_H
