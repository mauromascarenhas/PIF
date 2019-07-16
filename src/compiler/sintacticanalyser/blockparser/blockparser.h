#ifndef BLOCKPARSER_H
#define BLOCKPARSER_H

#include <QList>

#include "../programitem.h"


class BlockParser: public ProgramItem
{
public:
    BlockParser(BlockParser *parent = nullptr);
    ~BlockParser();

    BlockParser * parent();

private:
    BlockParser *parentBlock;

protected:
    QList<ProgramItem> programItems;
};

#endif // BLOCKPARSER_H
