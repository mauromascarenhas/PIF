#include "blockparser.h"

BlockParser::BlockParser(BlockParser *parent, BlockType blocktype,
    ExpressionParser *expression, const QHash<QString, ControlParser::IdentifierType> &globalVars)
    : ProgramItem(), BLOCKTYPE(blocktype)
{
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

void BlockParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang conv){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";
    indentFactor++;

    switch (BLOCKTYPE) {
        case WHILE:
            stream << QString("%1while (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString("){\n").toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        case DO_WHILE:
            stream << QString("%1do {\n").arg(tabs).toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1} while (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString(");\n").toUtf8();
            break;
        case IF:
            stream << QString("%1if (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString("){\n").toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        case ELSE_IF:
            stream << QString("%1else if (").arg(tabs).toUtf8();
            blockExpression->toOutFile(0, stream, conv);
            stream << QString("){\n").toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        case ELSE:
            stream << QString("%1else {\n").arg(tabs).toUtf8();
            if (programItemsC.size() > 1 && dynamic_cast<BlankItem*>(programItemsC.last())) programItemsC.removeLast();
            for (int i = 0; i < programItemsC.size(); ++i)
                programItemsC[i]->toOutFile(indentFactor, stream, conv);
            stream << QString("%1}\n").arg(tabs).toUtf8();
            break;
        default:
            break;
    }
}
