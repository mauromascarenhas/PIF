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

#ifndef IOPARSER_H
#define IOPARSER_H

#include <QList>
#include <QHash>
#include <QString>

#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/programitem.h"
#include "syntacticanalyser/lexicalanalyser/token.h"
#include "syntacticanalyser/controlparser/controlparser.h"

class IOParser : public ProgramItem
{
public:
    enum IOType{
        INPUT,
        OUTPUT
    };

    IOParser(IOType type = OUTPUT,
             const QHash<QString, ControlParser::IdentifierType> &globalVars = QHash<QString, ControlParser::IdentifierType>());
    ~IOParser();

    inline void addToken(const Token &token){ this->arguments.append(token); }
    inline bool isValid() { return ioType == OUTPUT ? outValidity() : inValidity(); }

    inline void setType(IOType type) { this->ioType = type; }
    inline IOType type(){ return this->ioType; }

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    IOType ioType;
    QList<Token> arguments;
    QHash<QString, ControlParser::IdentifierType> globalVars;

    bool inValidity();
    bool outValidity();

    QList<Token> validArgs();

    void toOutFileC(int indentFactor, QTextStream &stream);
    void toOutFileCPP(int indentFactor, QTextStream &stream);
    void toOutFileJAVA(int indentFactor, QTextStream &stream);
};

#endif // IOPARSER_H
