#ifndef BLANKITEM_H
#define BLANKITEM_H

#include "syntacticanalyser/programitem.h"

class BlankItem : public ProgramItem
{
public:
    explicit BlankItem();
    ~BlankItem();

    inline void toOutFile(int indentFactor, QTextStream &stream, ConvLang){
        QString out = "";
        for (int i = 0; i < indentFactor; ++i) out += "\t";
        out += "\n";

        stream << out;
    }
};

#endif // BLANKITEM_H
