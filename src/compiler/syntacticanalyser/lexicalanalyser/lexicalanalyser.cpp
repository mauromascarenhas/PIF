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

#include "lexicalanalyser.h"

LexicalAnalyser::LexicalAnalyser(const QString &line) :
    LANGUAGE_TEMPLATE("((\\b\\d+(?:\\.\\d+)?\\b)"
                      "|(\\b(numérico|literal|booleano|programa|senão|se|então|fim-se|faça|enquanto|fim-enquanto|fim-programa|leia|escreva)\\b)"
                      "|(\\t| {4})|([:,])"
                      "|((\\<\\=|\\>\\=|\\<|\\>|\\!\\=|\\=\\=|\\!)|\\b(e|ou)\\b)"
                      "|(\\b(verdadeiro|falso)\\b)|([\\+\\-\\*\\/])|(\\=)"
                      "|(\\b[a-zA-Z][\\w]*\\b)"
                      "|(\\()|(\\))"
                      "|(\"(?:[^\"\\\\]++|\\\\.)*+\")"
                      "|(#))|([^\\s\\1])")
{
    this->count = 0;
    this->line = line;
    getMatches();
}

LexicalAnalyser::~LexicalAnalyser(){
    tokens.clear();
}

Token LexicalAnalyser::nextToken(){
    if (tokens.isEmpty()) return Token();

    Token current = tokens.first();
    tokens.removeFirst();
    return current;
}

void LexicalAnalyser::getMatches(){
    QRegularExpressionMatchIterator i = LANGUAGE_TEMPLATE.globalMatch(this->line);

    while(i.hasNext()){
        QRegularExpressionMatch match = i.next();
        if (!match.captured(3).isEmpty()) tokens.append(Token(match.captured(3), Token::RESERVED, match.capturedStart(3)));
        else if (!match.captured(2).isEmpty()) tokens.append(Token(match.captured(2), Token::NUMERIC, match.capturedStart(2)));
        else if (!match.captured(5).isEmpty()) tokens.append(Token(match.captured(5), Token::TABULATION, match.capturedStart(5)));
        else if (!match.captured(6).isEmpty()) tokens.append(Token(match.captured(6), Token::CONTROL, match.capturedStart(6)));
        else if (!match.captured(7).isEmpty()) tokens.append(Token(match.captured(7), Token::BOOLEAN_OP, match.capturedStart(7)));
        else if (!match.captured(10).isEmpty()) tokens.append(Token(match.captured(10), Token::BOOLEAN_VAL, match.capturedStart(10)));
        else if (!match.captured(12).isEmpty()) tokens.append(Token(match.captured(12), Token::ARITHMETIC_OP, match.capturedStart(12)));
        else if (!match.captured(13).isEmpty()) tokens.append(Token(match.captured(13), Token::ASSIGNMENT, match.capturedStart(13)));
        else if (!match.captured(14).isEmpty()) tokens.append(Token(match.captured(14), Token::IDENTIFIER, match.capturedStart(14)));
        else if (!match.captured(15).isEmpty()) tokens.append(Token(match.captured(15), Token::PRIORITY_O, match.capturedStart(15)));
        else if (!match.captured(16).isEmpty()) tokens.append(Token(match.captured(16), Token::PRIORITY_C, match.capturedStart(16)));
        else if (!match.captured(17).isEmpty()) tokens.append(Token(match.captured(17), Token::LITERAL, match.capturedStart(17)));
        else if (!match.captured(19).isEmpty()) tokens.append(Token(match.captured(19), Token::INVALID, match.capturedStart(19)));
        else if (!match.captured(18).isEmpty()) break;
        else tokens.append(Token(match.captured(0), Token::UNDEFINED, match.capturedStart(0)));
    }

    std::sort(tokens.begin(), tokens.end());
    this->count = tokens.size();
}
