#include "lexicalanalyser.h"

LexicalAnalyser::LexicalAnalyser(const QString &line) :
    LANGUAGE_TEMPLATE("((\\b\\d+(?:\\.\\d+)?)|((numérico|literal|programa|senão|se|então|fim-se|enquanto|fim-enquanto|fim-programa|leia|escreva)\\b)|(\\t))|([:,])|([a-zA-ZÇ-ÜÀ-ÁãÃÊ-ÏÌÓ-Ýá-ñ][\\wÇ-ÜÀ-ÁãÃÊ-ÏÌÓ-Ýá-ó]*)|((\\<\\=|\\>\\=|\\<|\\>|\\!\\=|\\=\\=|\\!)|(e|ou)\\b)|([\\+\\-\\*\\/\\=])|([^\\s\\1])")
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
        else if (!match.captured(7).isEmpty()) tokens.append(Token(match.captured(7), Token::IDENTIFIER, match.capturedStart(7)));
        else if (!match.captured(2).isEmpty()) tokens.append(Token(match.captured(2), Token::NUMERIC, match.capturedStart(2)));
        else if (!match.captured(5).isEmpty()) tokens.append(Token(match.captured(5), Token::TABULATION, match.capturedStart(5)));
        else if (!match.captured(6).isEmpty()) tokens.append(Token(match.captured(6), Token::CONTROL, match.capturedStart(6)));
        else if (!match.captured(8).isEmpty()) tokens.append(Token(match.captured(8), Token::BOOLEAN_OP, match.capturedStart(8)));
        else if (!match.captured(11).isEmpty()) tokens.append(Token(match.captured(11), Token::ARITHIMETIC_OP, match.capturedStart(11)));
        else if (!match.captured(12).isEmpty()) tokens.append(Token(match.captured(12), Token::INVALID, match.capturedStart(12)));
        else tokens.append(Token(match.captured(0), Token::UNDEFINED, match.capturedStart(0)));
    }

    std::sort(tokens.begin(), tokens.end());
    this->count = tokens.size();
}
