#include "mainprogramparser.h"

MainProgramParser::MainProgramParser(const QString &programName)
    : BlockParser(), PROGRAM_NAME(programName)
{
    this->closedProgramBlock = false;
}

MainProgramParser::~MainProgramParser(){

}
