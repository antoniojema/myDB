#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>
#include "common/typedef.h"

namespace SQL {

enum class Keyword {
    SELECT,
    INSERT,
    UPDATE,
    DELETE,
    FROM,
    WHERE,
    DROP
};
std::map<std::string, Keyword> keyword_map = {
    {"SELECT", Keyword::SELECT},
    {"INSERT", Keyword::INSERT},
    {"UPDATE", Keyword::UPDATE},
    {"DELETE", Keyword::DELETE},
    {"FROM", Keyword::FROM},
    {"WHERE", Keyword::WHERE},
    {"DROP", Keyword::DROP},
};

enum class BinaryOperator {
    EQUALS,
    NOT_EQUAL,
    LESS_THAN,
    LESS_OR_EQUAL,
    GREATER_THAN,
    GREATER_OR_EQUAL,
    PLUS,
    MINUS,
    MULT,
    DIV,
};

enum class Delimiter {
    BracketOpen,
    BracketClose,
};

struct Token {
    virtual ~Token() = 0;
};

struct KeywordToken : Token {
    Keyword key {};
    KeywordToken(Keyword _key) : key(_key) {}
};

struct BinaryOperatorToken : Token {
    BinaryOperator binop {};
    BinaryOperatorToken(BinaryOperator _binop) : binop(_binop) {}
    BinaryOperatorToken() = delete;
};

struct DelimiterToken : Token {
    Delimiter delim {};
    DelimiterToken(Delimiter _delim): delim(_delim) {}
    DelimiterToken() = delete;
};

struct IntegerToken : Token {
    ssize_t val {};
    IntegerToken(ssize_t _val): val(_val) {}
    IntegerToken() = delete;
};

struct FloatToken : Token {
    double val {};
    FloatToken(double _val): val(_val) {}
    FloatToken() = delete;
};

struct WordToken : Token {
    std::string word {};
    WordToken(std::string&& _word) noexcept: word(std::move(_word)) {}
    WordToken(const std::string& _word) : word(_word) {}
    WordToken() = delete;
};

struct StringToken : Token {
    std::string str {};
    StringToken(std::string&& _str) noexcept: str(std::move(_str)) {}
    StringToken(const std::string& _str) : str(_str) {}
    StringToken() = delete;
};

struct DateToken : Token {
    std::chrono::year_month_day date {};
    DateToken(const std::chrono::year_month_day& _date) : date(_date) {}
    DateToken() = delete;
};

struct WildcardToken : Token {};

struct CommaToken : Token {};

struct EOSToken : Token {};

std::vector<std::unique_ptr<Token>> get_tokens(std::istream& is);

} // namespace SQL
