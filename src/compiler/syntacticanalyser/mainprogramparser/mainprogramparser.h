/*
 *     This file is part of PIFC (Compiler).
 *
 *    PIFC (Compiler) is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Foobar is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MAINPROGRAMPARSER_H
#define MAINPROGRAMPARSER_H

#include <QList>
#include <QString>

#include "../blockparser/blockparser.h"
#include "syntacticanalyser/programitem.h"

class MainProgramParser : public BlockParser
{
public:
    MainProgramParser(const QString &programName = "");
    ~MainProgramParser();

    inline void closeProgramBlock() { this->closedProgramBlock = true; }
    inline bool isProgramBlockClosed() { return this->closedProgramBlock; }

    inline QString programName() { return this->PROGRAM_NAME; }

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    using BlockParser::getBlockExpression;
    using BlockParser::setBlockExpression;

    bool closedProgramBlock;

    const QString PROGRAM_NAME;

    void toOutFileC(int indentFactor, QTextStream &stream);
    void toOutFileCPP(int indentFactor, QTextStream &stream);
    void toOutFileJAVA(int indentFactor, QTextStream &stream);
};

#endif // MAINPROGRAMPARSER_H
