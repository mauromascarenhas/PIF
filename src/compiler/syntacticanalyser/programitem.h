#ifndef PROGRAMITEM_H
#define PROGRAMITEM_H

#include <QString>

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
    //TODO: Implement (should request open stream as well/instead?)
    //virtual QString toString(int indentFactor, ConvLang conv = C) = 0;
};

#endif // PROGRAMITEM_H
