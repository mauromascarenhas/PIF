#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent, BlockType blocktype,
    const ExpressionParser &expression, const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem(), BLOCKTYPE(blocktype)
{
    selfClosed = true;

    parentBlock = parent;
    declaredVars = globalVars;
    blockExpression = expression;
}

BlockParser::~BlockParser(){

}
