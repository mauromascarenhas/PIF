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

#ifndef CONTROLPARSER_H
#define CONTROLPARSER_H

#include <QList>
#include <QHash>
#include <QString>

#include "../programitem.h"
#include "messagelogger/messagelogger.h"
#include "syntacticanalyser/lexicalanalyser/token.h"

class ControlParser : public ProgramItem
{
public:
    ControlParser();
    ~ControlParser();

    enum IdentifierType {
        NUMERIC,
        LITERAL,
        BOOLEAN
    };
    Q_ENUM(IdentifierType)

    inline void addToken(const Token &token){ this->tokens.append(token); }

    bool isValid();
    bool insertNewVars(QHash<QString, IdentifierType> &globalVars);

    inline QStringList getDeclaredIDs() { return this->declaredIDs; }

    void toOutFile(int indentFactor, QTextStream &stream, ConvLang conv = C);

private:
    QList<Token> tokens;
    QStringList declaredIDs;
    IdentifierType currentType;

    void toOutFileC(int indentFactor, QTextStream &stream);
    void toOutFileCPP(int indentFactor, QTextStream &stream);
    void toOutFileJAVA(int indentFactor, QTextStream &stream);
};

#endif // CONTROLPARSER_H
