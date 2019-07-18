#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent, BlockType blocktype,
    const ExpressionParser &expression) : ProgramItem(),
    BLOCKTYPE(blocktype)
{
    selfClosed = true;

    parentBlock = parent;
    blockExpression = expression;
}

BlockParser::~BlockParser(){

}
