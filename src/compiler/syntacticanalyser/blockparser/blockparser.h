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
