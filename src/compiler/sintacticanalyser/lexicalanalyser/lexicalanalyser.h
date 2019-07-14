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

private:
    const QRegularExpression LANGUAGE_TEMPLATE;

    int count;
    QString line;
    QList<Token> tokens;

    void getMatches();

};

#endif // LEXICALANALYSER_H
