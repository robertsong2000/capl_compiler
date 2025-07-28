/**
 * CAPL Token 定义
 * 
 * 定义了 CAPL 语言的所有 Token 类型和相关数据结构
 */

#ifndef CAPL_TOKEN_H
#define CAPL_TOKEN_H

#include <string>
#include <unordered_map>

namespace capl {

/**
 * Token 类型枚举
 */
enum class TokenType {
    // 字面量
    INTEGER,        // 整数
    FLOAT,          // 浮点数
    STRING,         // 字符串
    CHAR,           // 字符
    
    // 标识符
    IDENTIFIER,     // 标识符
    
    // 关键字
    VARIABLES,      // variables
    ON,             // on
    MESSAGE,        // message
    TIMER,          // timer
    KEY,            // key
    START,          // start
    STOP,           // stop
    IF,             // if
    ELSE,           // else
    WHILE,          // while
    FOR,            // for
    SWITCH,         // switch
    CASE,           // case
    DEFAULT,        // default
    BREAK,          // break
    CONTINUE,       // continue
    RETURN,         // return
    VOID,           // void
    INT,            // int
    FLOAT_KW,       // float
    CHAR_KW,        // char
    BYTE,           // byte
    WORD,           // word
    DWORD,          // dword
    LONG,           // long
    
    // CAN 相关关键字
    CAN,            // can
    CANDB,          // candb
    SIGNAL,         // signal
    ENVVAR,         // envvar
    SYSVAR,         // sysvar
    
    // 操作符
    ASSIGN,         // =
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    MODULO,         // %
    INCREMENT,      // ++
    DECREMENT,      // --
    PLUS_ASSIGN,    // +=
    MINUS_ASSIGN,   // -=
    
    // 比较操作符
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    LESS,           // <
    LESS_EQUAL,     // <=
    GREATER,        // >
    GREATER_EQUAL,  // >=
    
    // 逻辑操作符
    LOGICAL_AND,    // &&
    LOGICAL_OR,     // ||
    LOGICAL_NOT,    // !
    
    // 位操作符
    BITWISE_AND,    // &
    BITWISE_OR,     // |
    BITWISE_XOR,    // ^
    BITWISE_NOT,    // ~
    LEFT_SHIFT,     // <<
    RIGHT_SHIFT,    // >>
    
    // 分隔符
    SEMICOLON,      // ;
    COMMA,          // ,
    DOT,            // .
    COLON,          // :
    QUESTION,       // ?
    
    // 括号
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    
    // 特殊
    NEWLINE,        // 换行
    EOF_TOKEN,      // 文件结束
    UNKNOWN,        // 未知 Token
    
    // 注释
    COMMENT,        // 注释
    
    // 预处理指令
    INCLUDE,        // #include
    PRAGMA,         // #pragma
};

/**
 * Token 类
 * 表示词法分析的基本单元
 */
class Token {
public:
    /**
     * 构造函数
     * @param type Token 类型
     * @param value Token 值
     * @param line 行号
     * @param column 列号
     */
    Token(TokenType type, const std::string& value, int line, int column);
    
    /**
     * 默认构造函数
     */
    Token();
    
    /**
     * 获取 Token 类型
     * @return Token 类型
     */
    TokenType getType() const { return type_; }
    
    /**
     * 获取 Token 值
     * @return Token 值
     */
    const std::string& getValue() const { return value_; }
    
    /**
     * 获取行号
     * @return 行号
     */
    int getLine() const { return line_; }
    
    /**
     * 获取列号
     * @return 列号
     */
    int getColumn() const { return column_; }
    
    /**
     * 检查是否为关键字
     * @return 是否为关键字
     */
    bool isKeyword() const;
    
    /**
     * 检查是否为操作符
     * @return 是否为操作符
     */
    bool isOperator() const;
    
    /**
     * 检查是否为字面量
     * @return 是否为字面量
     */
    bool isLiteral() const;
    
    /**
     * 转换为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    TokenType type_;        // Token 类型
    std::string value_;     // Token 值
    int line_;              // 行号
    int column_;            // 列号
};

/**
 * 关键字映射表
 * 用于快速查找关键字对应的 Token 类型
 */
class KeywordMap {
public:
    /**
     * 获取单例实例
     * @return 单例实例
     */
    static KeywordMap& getInstance();
    
    /**
     * 检查是否为关键字
     * @param word 单词
     * @return 是否为关键字
     */
    bool isKeyword(const std::string& word) const;
    
    /**
     * 获取关键字对应的 Token 类型
     * @param word 关键字
     * @return Token 类型
     */
    TokenType getKeywordType(const std::string& word) const;

private:
    /**
     * 构造函数（私有）
     */
    KeywordMap();
    
    /**
     * 初始化关键字映射
     */
    void initializeKeywords();
    
    std::unordered_map<std::string, TokenType> keywords_;
};

/**
 * Token 类型转字符串
 * @param type Token 类型
 * @return 字符串表示
 */
std::string tokenTypeToString(TokenType type);

} // namespace capl

#endif // CAPL_TOKEN_H