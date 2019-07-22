#ifndef PROGRAMITEM_H
#define PROGRAMITEM_H

#include <QString>
#include <QTextStream>

class ProgramItem
{
public:
    ProgramItem();
    virtual ~ProgramItem();

    enum ConvLang{
        C,
        CPP,
        JAVA
    };

    virtual void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C) = 0;
};

#endif // PROGRAMITEM_H
