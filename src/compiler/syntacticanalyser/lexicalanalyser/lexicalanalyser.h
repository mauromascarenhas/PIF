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

#ifndef LEXICALANALYSER_H
#define LEXICALANALYSER_H

#include <QList>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "token.h"

class LexicalAnalyser
{
public:
    explicit LexicalAnalyser(const QString &line = "");
    ~LexicalAnalyser();

    Token nextToken();
    inline int tokenCount() { return this->count; }
    inline bool hasNextToken() { return this->tokens.size(); }

private:
    const QRegularExpression LANGUAGE_TEMPLATE;

    int count;
    QString line;
    QList<Token> tokens;

    void getMatches();

};

#endif // LEXICALANALYSER_H
