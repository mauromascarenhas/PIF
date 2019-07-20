#include "mainprogramparser.h"

MainProgramParser::MainProgramParser(const QString &programName)
    : BlockParser(nullptr, BlockType::MAIN), PROGRAM_NAME(programName)
{
    this->closedProgramBlock = false;
}

MainProgramParser::~MainProgramParser(){

}
