#ifndef BLOCKPARSER_H
#define BLOCKPARSER_H

#include <QList>

#include "../programitem.h"
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
        UNDEFINED
    };

    BlockParser(BlockParser *parent = nullptr,
                BlockType blocktype = UNDEFINED,
                const ExpressionParser &expression = ExpressionParser());
    ~BlockParser();

    BlockParser * parent();

    inline ExpressionParser getBlockExpression() { return this->blockExpression; }
    inline void setBlockExpression(const ExpressionParser &expression) { this->blockExpression = expression; }

    inline BlockType blockType(){ return this->BLOCKTYPE; }

private:
    BlockParser *parentBlock;

protected:
    const BlockType BLOCKTYPE;

    QList<ProgramItem> programItems;
    ExpressionParser blockExpression;
};

#endif // BLOCKPARSER_H
