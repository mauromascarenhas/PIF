#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent, BlockType blocktype,
    const ExpressionParser &expression) : ProgramItem(),
    BLOCKTYPE(blocktype)
{
    parentBlock = parent;
    blockExpression = expression;
}

BlockParser::~BlockParser(){

}
