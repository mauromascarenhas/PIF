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
