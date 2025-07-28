/**
 * CAPL 词法分析器实现
 */

#include "../include/capl_compiler.h"
#include "../include/token.h"
#include <cctype>
#include <sstream>

namespace capl {

Lexer::Lexer(const std::string& source) 
    : source_(source), position_(0), line_(1), column_(1) {
}

Token Lexer::nextToken() {
    // 跳过空白字符
    while (position_ < source_.length() && std::isspace(source_[position_])) {
        if (source_[position_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        position_++;
    }
    
    // 检查是否到达文件末尾
    if (position_ >= source_.length()) {
        return Token(TokenType::EOF_TOKEN, "", line_, column_);
    }
    
    char current = source_[position_];
    size_t start_pos = position_;
    size_t start_column = column_;
    
    // 处理注释
    if (current == '/' && position_ + 1 < source_.length()) {
        if (source_[position_ + 1] == '/') {
            // 单行注释
            while (position_ < source_.length() && source_[position_] != '\n') {
                position_++;
                column_++;
            }
            return nextToken(); // 递归调用获取下一个 token
        } else if (source_[position_ + 1] == '*') {
            // 多行注释
            position_ += 2;
            column_ += 2;
            while (position_ + 1 < source_.length()) {
                if (source_[position_] == '*' && source_[position_ + 1] == '/') {
                    position_ += 2;
                    column_ += 2;
                    break;
                }
                if (source_[position_] == '\n') {
                    line_++;
                    column_ = 1;
                } else {
                    column_++;
                }
                position_++;
            }
            return nextToken(); // 递归调用获取下一个 token
        }
    }
    
    // 处理数字
    if (std::isdigit(current)) {
        std::string number;
        bool isFloat = false;
        bool isHex = false;
        
        // 检查是否是十六进制数字 (0x 或 0X)
        if (current == '0' && position_ + 1 < source_.length() && 
            (source_[position_ + 1] == 'x' || source_[position_ + 1] == 'X')) {
            isHex = true;
            number += source_[position_]; // '0'
            position_++;
            column_++;
            number += source_[position_]; // 'x' 或 'X'
            position_++;
            column_++;
            
            // 读取十六进制数字
            while (position_ < source_.length() && 
                   std::isxdigit(source_[position_])) {
                number += source_[position_];
                position_++;
                column_++;
            }
        } else {
            // 处理十进制数字
            while (position_ < source_.length() && 
                   (std::isdigit(source_[position_]) || source_[position_] == '.')) {
                if (source_[position_] == '.') {
                    if (isFloat) break; // 第二个小数点，停止
                    isFloat = true;
                }
                number += source_[position_];
                position_++;
                column_++;
            }
        }
        
        return Token(isFloat ? TokenType::FLOAT : TokenType::INTEGER, 
                    number, line_, start_column);
    }
    
    // 处理标识符和关键字
    if (std::isalpha(current) || current == '_') {
        std::string identifier;
        
        while (position_ < source_.length() && 
               (std::isalnum(source_[position_]) || source_[position_] == '_')) {
            identifier += source_[position_];
            position_++;
            column_++;
        }
        
        // 检查是否是关键字
        KeywordMap& keywordMap = KeywordMap::getInstance();
        if (keywordMap.isKeyword(identifier)) {
            TokenType type = keywordMap.getKeywordType(identifier);
            return Token(type, identifier, line_, start_column);
        }
        
        return Token(TokenType::IDENTIFIER, identifier, line_, start_column);
    }
    
    // 处理字符串字面量
    if (current == '"') {
        std::string str;
        position_++; // 跳过开始的引号
        column_++;
        
        while (position_ < source_.length() && source_[position_] != '"') {
            if (source_[position_] == '\\' && position_ + 1 < source_.length()) {
                // 处理转义字符
                position_++;
                column_++;
                switch (source_[position_]) {
                    case 'n': str += '\n'; break;
                    case 't': str += '\t'; break;
                    case 'r': str += '\r'; break;
                    case '\\': str += '\\'; break;
                    case '"': str += '"'; break;
                    default: str += source_[position_]; break;
                }
            } else {
                str += source_[position_];
            }
            position_++;
            column_++;
        }
        
        if (position_ < source_.length()) {
            position_++; // 跳过结束的引号
            column_++;
        }
        
        return Token(TokenType::STRING, str, line_, start_column);
    }
    
    // 处理字符字面量
    if (current == '\'') {
        std::string charLiteral;
        position_++; // 跳过开始的单引号
        column_++;
        
        if (position_ < source_.length()) {
            if (source_[position_] == '\\' && position_ + 1 < source_.length()) {
                // 处理转义字符
                position_++;
                column_++;
                switch (source_[position_]) {
                    case 'n': charLiteral += '\n'; break;
                    case 't': charLiteral += '\t'; break;
                    case 'r': charLiteral += '\r'; break;
                    case '\\': charLiteral += '\\'; break;
                    case '\'': charLiteral += '\''; break;
                    default: charLiteral += source_[position_]; break;
                }
            } else {
                charLiteral += source_[position_];
            }
            position_++;
            column_++;
        }
        
        if (position_ < source_.length() && source_[position_] == '\'') {
            position_++; // 跳过结束的单引号
            column_++;
        }
        
        return Token(TokenType::CHAR, charLiteral, line_, start_column);
    }
    
    // 处理单字符 token
    position_++;
    column_++;
    
    switch (current) {
        case '+': return Token(TokenType::PLUS, "+", line_, start_column);
        case '-': return Token(TokenType::MINUS, "-", line_, start_column);
        case '*': return Token(TokenType::MULTIPLY, "*", line_, start_column);
        case '/': return Token(TokenType::DIVIDE, "/", line_, start_column);
        case '=': 
            if (position_ < source_.length() && source_[position_] == '=') {
                position_++;
                column_++;
                return Token(TokenType::EQUAL, "==", line_, start_column);
            }
            return Token(TokenType::ASSIGN, "=", line_, start_column);
        case '!':
            if (position_ < source_.length() && source_[position_] == '=') {
                position_++;
                column_++;
                return Token(TokenType::NOT_EQUAL, "!=", line_, start_column);
            }
            return Token(TokenType::LOGICAL_NOT, "!", line_, start_column);
        case '<':
            if (position_ < source_.length() && source_[position_] == '=') {
                position_++;
                column_++;
                return Token(TokenType::LESS_EQUAL, "<=", line_, start_column);
            }
            return Token(TokenType::LESS, "<", line_, start_column);
        case '>':
            if (position_ < source_.length() && source_[position_] == '=') {
                position_++;
                column_++;
                return Token(TokenType::GREATER_EQUAL, ">=", line_, start_column);
            }
            return Token(TokenType::GREATER, ">", line_, start_column);
        case '&':
            if (position_ < source_.length() && source_[position_] == '&') {
                position_++;
                column_++;
                return Token(TokenType::LOGICAL_AND, "&&", line_, start_column);
            }
            return Token(TokenType::BITWISE_AND, "&", line_, start_column);
        case '|':
            if (position_ < source_.length() && source_[position_] == '|') {
                position_++;
                column_++;
                return Token(TokenType::LOGICAL_OR, "||", line_, start_column);
            }
            return Token(TokenType::BITWISE_OR, "|", line_, start_column);
        case '(': return Token(TokenType::LEFT_PAREN, "(", line_, start_column);
        case ')': return Token(TokenType::RIGHT_PAREN, ")", line_, start_column);
        case '{': return Token(TokenType::LEFT_BRACE, "{", line_, start_column);
        case '}': return Token(TokenType::RIGHT_BRACE, "}", line_, start_column);
        case '[': return Token(TokenType::LEFT_BRACKET, "[", line_, start_column);
        case ']': return Token(TokenType::RIGHT_BRACKET, "]", line_, start_column);
        case ';': return Token(TokenType::SEMICOLON, ";", line_, start_column);
        case ',': return Token(TokenType::COMMA, ",", line_, start_column);
        case '.': return Token(TokenType::DOT, ".", line_, start_column);
        default:
            return Token(TokenType::UNKNOWN, std::string(1, current), line_, start_column);
    }
}

bool Lexer::hasMoreTokens() const {
    return position_ < source_.length();
}

} // namespace capl