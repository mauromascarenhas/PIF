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

#include "controlparser.h"

ControlParser::ControlParser()
    : ProgramItem()
{

}

ControlParser::~ControlParser(){

}

bool ControlParser::isValid(){
    if (!tokens.length()) return false;

    bool hasFinished = false;
    Token current = tokens.first();
    Token oldToken = current;
    if (oldToken.type() != Token::IDENTIFIER) return false;

    for (int i = 1; i < tokens.length(); ++i) {
        current = tokens[i];

        if (hasFinished){
            if (current.type() == Token::TABULATION) continue;
            else return 0;
        }

        switch (current.type()) {
            case Token::IDENTIFIER:
                if (oldToken.word() != ",") return false;
                oldToken = current;
                break;
            case Token::CONTROL:
                if (oldToken.type() != Token::IDENTIFIER) return false;
                oldToken = current;
                break;
            case Token::RESERVED:
                if ((current.word() == "numérico" || current.word() == "literal"
                        || current.word() == "booleano") && oldToken.word() == ":")
                    hasFinished = true;
                else return false;
                break;
            default:
                return false;
        }
    }
    return true;
}

bool ControlParser::insertNewVars(QHash<QString, IdentifierType> &globalVars){
    if (!this->isValid()) return false;

    for (int i = 0; i < tokens.size(); ++i){
        Token current = tokens[i];
        switch (current.type()) {
            case Token::IDENTIFIER:
                declaredIDs.append(current.word());
                break;
            case Token::RESERVED:
                if (current.word() == "numérico") currentType = NUMERIC;
                else if (current.word() == "literal") currentType = LITERAL;
                else currentType = BOOLEAN;
                break;
            default:
                break;
        }
    }

    bool hasDuplicate = false;
    for (int i = 0; i < declaredIDs.length(); ++i){
        if (globalVars.contains(declaredIDs[i])){
            MessageLogger::getInstance().log(MessageLogger::ERROR,
                                             QString("O identificador %1 já foi previamente declarado.")
                                                .arg(declaredIDs[i]));
            hasDuplicate = true;
        }
        else globalVars.insert(declaredIDs[i], currentType);
    }

    return !hasDuplicate;
}

void ControlParser::toOutFile(int indentFactor, QTextStream &stream, ConvLang conv){
    switch (conv) {
        case CPP:
            toOutFileCPP(indentFactor, stream);
            break;
        case JAVA:
            toOutFileJAVA(indentFactor, stream);
            break;
        default:
            toOutFileC(indentFactor, stream);
            break;
    }
}

void ControlParser::toOutFileC(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (currentType) {
        case NUMERIC:
            stream << QString("%1double ").arg(tabs).toUtf8();
            break;
        case BOOLEAN:
            stream << QString("%1bool ").arg(tabs).toUtf8();
            break;
        default:
            stream << QString("%1string ").arg(tabs).toUtf8();
            break;
    }
    for (int i = 0; i < declaredIDs.size(); ++i)
        stream << QString("%1%2").arg(declaredIDs[i], i == declaredIDs.length() - 1 ? "" : ", ").toUtf8();
    stream << QString(";\n").toUtf8();
}

void ControlParser::toOutFileCPP(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (currentType) {
        case NUMERIC:
            stream << QString("%1double ").arg(tabs).toUtf8();
            break;
        case BOOLEAN:
            stream << QString("%1bool ").arg(tabs).toUtf8();
            break;
        default:
            stream << QString("%1string ").arg(tabs).toUtf8();
            break;
    }
    for (int i = 0; i < declaredIDs.size(); ++i)
        stream << QString("%1%2").arg(declaredIDs[i], i == declaredIDs.length() - 1 ? "" : ", ").toUtf8();
    stream << QString(";\n").toUtf8();
}

void ControlParser::toOutFileJAVA(int indentFactor, QTextStream &stream){
    QString tabs = "";
    for (int i = 0; i < indentFactor; ++i) tabs += "\t";

    switch (currentType) {
        case NUMERIC:
            stream << QString("%1double ").arg(tabs).toUtf8();
            break;
        case BOOLEAN:
            stream << QString("%1boolean ").arg(tabs).toUtf8();
            break;
        default:
            stream << QString("%1String ").arg(tabs).toUtf8();
            break;
    }
    for (int i = 0; i < declaredIDs.size(); ++i)
        stream << QString("%1%2").arg(declaredIDs[i], i == declaredIDs.length() - 1 ? "" : ", ").toUtf8();
    stream << QString(";\n").toUtf8();
}
