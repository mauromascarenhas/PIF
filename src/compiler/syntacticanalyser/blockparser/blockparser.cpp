#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent, BlockType blocktype,
    ExpressionParser *expression, const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem(), BLOCKTYPE(blocktype)
{
    selfClosed = true;

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
