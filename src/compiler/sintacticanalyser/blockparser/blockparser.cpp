#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent)
    : ProgramItem()
{
    parentBlock = parent;
}

BlockParser::~BlockParser(){

}
