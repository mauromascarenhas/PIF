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
        MAIN,
        UNDEFINED
    };

    BlockParser(BlockParser *parent = nullptr,
                BlockType blocktype = UNDEFINED,
                const ExpressionParser &expression = ExpressionParser());
    ~BlockParser();

    inline BlockParser * parent() { return this->parentBlock; }

    inline ExpressionParser getBlockExpression() { return this->blockExpression; }
    inline void setBlockExpression(const ExpressionParser &expression) { this->blockExpression = expression; }

    inline bool isSelfClosed() { return this->selfClosed; }
    inline void setSelfClosed(bool selfClosed) { this->selfClosed = selfClosed; }

    inline void addProgramItem(const ProgramItem &programItem) { this->programItems.append(programItem); }

    inline BlockType blockType(){ return this->BLOCKTYPE; }

private:
    BlockParser *parentBlock;

protected:
    bool selfClosed;

    const BlockType BLOCKTYPE;

    QList<ProgramItem> programItems;
    ExpressionParser blockExpression;
};

#endif // BLOCKPARSER_H
