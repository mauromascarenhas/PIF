#include "mainprogramparser.h"

MainProgramParser::MainProgramParser(const QString &programName)
    : BlockParser(nullptr, BlockType::MAIN), PROGRAM_NAME(programName)
{
    this->closedProgramBlock = false;
}

MainProgramParser::~MainProgramParser(){

}

void MainProgramParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang conv){
    switch (conv) {
        case CPP:
            toOutFileCPP(indentFactor, stream);
            break;
        case JAVA:
            toOutFileJAVA(indentFactor, stream);
            break;
        default:
            toOutFileC(indentFactor, stream);
            break;
    }
}

void MainProgramParser::toOutFileC(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    indentFactor++;

    stream << QString("// Programa %1 gerado automaticamente por PIFC\n\n").arg(PROGRAM_NAME).toUtf8();
    stream << QString("%1#include <stdio.h>\n"
                      "%1#include <string.h>\n"
                      "%1#include <stdlib.h>\n"
                      "\n"
                      "%1typedef unsigned char string[1024];\n"
                      "%1typedef int bool;\n"
                      "%1enum {false, true};\n"
                      "\n"
                      "%1int main(int argc, char *argv[]){\n"
                      "%1\tsetbuf(stdout, NULL);\n\n").arg(tabs).toUtf8();

    for (int i = 0; i < programItemsC.size(); ++i)
        programItemsC[i]->toOutFile(indentFactor, stream);

    stream << QString("%1\n"
                      "%1\treturn EXIT_SUCCESS;\n"
                      "%1}\n").arg(tabs).toUtf8();

}

void MainProgramParser::toOutFileCPP(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    indentFactor++;

    stream << QString("// Programa %1 gerado automaticamente por PIFC\n\n").arg(PROGRAM_NAME).toUtf8();
    stream << QString("%1#include <string>\n"
                      "%1#include <cstdlib>\n"
                      "%1#include <sstream>\n"
                      "%1#include <iostream>\n"
                      "\n"
                      "%1using namespace std;\n"
                      "\n"
                      "%1stringstream __sstrm__;\n"
                      "\n"
                      "%1int main(int argc, char *argv[]){\n"
                      "%1\tsetbuf(stdout, NULL);\n\n").arg(tabs).toUtf8();

    for (int i = 0; i < programItemsC.size(); ++i)
        programItemsC[i]->toOutFile(indentFactor, stream, ProgramItem::CPP);

    stream << QString("%1\n"
                      "%1\treturn EXIT_SUCCESS;\n"
                      "%1}\n").arg(tabs).toUtf8();
}

void MainProgramParser::toOutFileJAVA(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    indentFactor+=2;

    stream << QString("// Programa %1 gerado automaticamente por PIFC\n\n").arg(PROGRAM_NAME).toUtf8();
    stream << QString("%1import java.util.Scanner;\n"
                      "\n"
                      "%1public class %2 {\n"
                      "\n"
                      "%1\tpublic static void main(String[] args) {\n"
                      "%1\t\tScanner scanner = new Scanner(System.in);\n"
                      "\n").arg(tabs, PROGRAM_NAME).toUtf8();

    for (int i = 0; i < programItemsC.size(); ++i)
        programItemsC[i]->toOutFile(indentFactor, stream, ProgramItem::JAVA);

    stream << QString("%1\t}\n\n"
                      "%1}\n").arg(tabs).toUtf8();
}
