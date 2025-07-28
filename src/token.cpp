/**
 * CAPL Token 实现
 */

#include "../include/token.h"

namespace capl {

// Token 类实现
Token::Token(TokenType type, const std::string& value, int line, int column)
    : type_(type), value_(value), line_(line), column_(column) {
}

Token::Token() : type_(TokenType::UNKNOWN), value_(""), line_(0), column_(0) {
}

bool Token::isKeyword() const {
    return type_ >= TokenType::VARIABLES && type_ <= TokenType::SYSVAR;
}

bool Token::isOperator() const {
    return type_ >= TokenType::ASSIGN && type_ <= TokenType::RIGHT_SHIFT;
}

bool Token::isLiteral() const {
    return type_ >= TokenType::INTEGER && type_ <= TokenType::CHAR;
}

std::string Token::toString() const {
    return tokenTypeToString(type_) + "(" + value_ + ")";
}

// KeywordMap 类实现
KeywordMap& KeywordMap::getInstance() {
    static KeywordMap instance;
    return instance;
}

KeywordMap::KeywordMap() {
    initializeKeywords();
}

void KeywordMap::initializeKeywords() {
    keywords_["variables"] = TokenType::VARIABLES;
    keywords_["on"] = TokenType::ON;
    keywords_["message"] = TokenType::MESSAGE;
    keywords_["timer"] = TokenType::TIMER;
    keywords_["key"] = TokenType::KEY;
    keywords_["start"] = TokenType::START;
    keywords_["stop"] = TokenType::STOP;
    keywords_["if"] = TokenType::IF;
    keywords_["else"] = TokenType::ELSE;
    keywords_["while"] = TokenType::WHILE;
    keywords_["for"] = TokenType::FOR;
    keywords_["switch"] = TokenType::SWITCH;
    keywords_["case"] = TokenType::CASE;
    keywords_["default"] = TokenType::DEFAULT;
    keywords_["break"] = TokenType::BREAK;
    keywords_["continue"] = TokenType::CONTINUE;
    keywords_["return"] = TokenType::RETURN;
    keywords_["void"] = TokenType::VOID;
    keywords_["int"] = TokenType::INT;
    keywords_["float"] = TokenType::FLOAT_KW;
    keywords_["char"] = TokenType::CHAR_KW;
    keywords_["byte"] = TokenType::BYTE;
    keywords_["word"] = TokenType::WORD;
    keywords_["dword"] = TokenType::DWORD;
    keywords_["long"] = TokenType::LONG;
    keywords_["can"] = TokenType::CAN;
    keywords_["candb"] = TokenType::CANDB;
    keywords_["signal"] = TokenType::SIGNAL;
    keywords_["envvar"] = TokenType::ENVVAR;
    keywords_["sysvar"] = TokenType::SYSVAR;
}

bool KeywordMap::isKeyword(const std::string& word) const {
    return keywords_.find(word) != keywords_.end();
}

TokenType KeywordMap::getKeywordType(const std::string& word) const {
    auto it = keywords_.find(word);
    if (it != keywords_.end()) {
        return it->second;
    }
    return TokenType::UNKNOWN;
}

// Token 类型转字符串函数
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::VARIABLES: return "VARIABLES";
        case TokenType::ON: return "ON";
        case TokenType::MESSAGE: return "MESSAGE";
        case TokenType::TIMER: return "TIMER";
        case TokenType::KEY: return "KEY";
        case TokenType::START: return "START";
        case TokenType::STOP: return "STOP";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::SWITCH: return "SWITCH";
        case TokenType::CASE: return "CASE";
        case TokenType::DEFAULT: return "DEFAULT";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::VOID: return "VOID";
        case TokenType::INT: return "INT";
        case TokenType::FLOAT_KW: return "FLOAT_KW";
        case TokenType::CHAR_KW: return "CHAR_KW";
        case TokenType::BYTE: return "BYTE";
        case TokenType::WORD: return "WORD";
        case TokenType::DWORD: return "DWORD";
        case TokenType::LONG: return "LONG";
        case TokenType::CAN: return "CAN";
        case TokenType::CANDB: return "CANDB";
        case TokenType::SIGNAL: return "SIGNAL";
        case TokenType::ENVVAR: return "ENVVAR";
        case TokenType::SYSVAR: return "SYSVAR";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::INCREMENT: return "INCREMENT";
        case TokenType::DECREMENT: return "DECREMENT";
        case TokenType::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LOGICAL_AND: return "LOGICAL_AND";
        case TokenType::LOGICAL_OR: return "LOGICAL_OR";
        case TokenType::LOGICAL_NOT: return "LOGICAL_NOT";
        case TokenType::BITWISE_AND: return "BITWISE_AND";
        case TokenType::BITWISE_OR: return "BITWISE_OR";
        case TokenType::BITWISE_XOR: return "BITWISE_XOR";
        case TokenType::BITWISE_NOT: return "BITWISE_NOT";
        case TokenType::LEFT_SHIFT: return "LEFT_SHIFT";
        case TokenType::RIGHT_SHIFT: return "RIGHT_SHIFT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::COLON: return "COLON";
        case TokenType::QUESTION: return "QUESTION";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::EOF_TOKEN: return "EOF_TOKEN";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::INCLUDE: return "INCLUDE";
        case TokenType::PRAGMA: return "PRAGMA";
        default: return "UNKNOWN";
    }
}

} // namespace capl