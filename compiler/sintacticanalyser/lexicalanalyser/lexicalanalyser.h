#ifndef LEXICALANALYSER_H
#define LEXICALANALYSER_H

#include <list>
#include <regex>

#include "token/token.h"

class LexicalAnaliser{
    public:
        explicit LexicalAnaliser();
        ~LexicalAnaliser();

        Token nextToken();
    private:
        const std::regex LANGUAGE_TEMPLATE;
		std::list<Token> tokens;
};

#endif // LEXICALANALYSER_H
