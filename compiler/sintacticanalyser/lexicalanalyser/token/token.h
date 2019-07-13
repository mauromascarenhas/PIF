#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token{
    public:
        enum TokenType{
            ARITHIMETIC_OP,
            BOOLEAN_OP,
            CONTROL,
            IDENTIFIER,
            INVALID,
            NUMERIC,
            RESERVED,
            TABULATION,
            UNDEFINED
        };

        explicit Token(const std::string &token = "", TokenType type = UNDEFINED);
        ~Token();

        void setToken(const std::string &token);
        void setType(TokenType type);

        std::string token();
        TokenType type();

    private:
        std::string word;
        TokenType currentType;
};

#endif // TOKEN_H