/**
 * CAPL 编译器项目 - 核心架构设计
 * 
 * 本文件定义了开源 CAPL 编译器的核心架构和接口
 */

#ifndef CAPL_COMPILER_H
#define CAPL_COMPILER_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "token.h"
#include "symbol_table.h"

namespace capl {

// 前向声明
class ASTNode;
class CodeGenerator;

/**
 * CAPL 编译器主类
 * 负责协调整个编译过程
 */
class CAPLCompiler {
public:
    /**
     * 构造函数
     */
    CAPLCompiler();
    
    /**
     * 析构函数
     */
    ~CAPLCompiler();
    
    /**
     * 编译 CAPL 源文件
     * @param source_file CAPL 源文件路径
     * @param output_file 输出文件路径
     * @return 编译是否成功
     */
    bool compile(const std::string& source_file, const std::string& output_file);
    
    /**
     * 编译 CAPL 源代码字符串
     * @param source_code CAPL 源代码
     * @param output_file 输出文件路径
     * @return 编译是否成功
     */
    bool compileFromString(const std::string& source_code, const std::string& output_file);
    
    /**
     * 仅进行语法检查，不生成代码
     * @param source_file CAPL 源文件路径
     * @return 语法检查是否通过
     */
    bool syntaxCheck(const std::string& source_file);
    
    /**
     * 仅进行语法检查，不生成代码（从字符串）
     * @param source_code CAPL 源代码
     * @return 语法检查是否通过
     */
    bool syntaxCheckFromString(const std::string& source_code);
    
    /**
     * 获取编译错误信息
     * @return 错误信息列表
     */
    const std::vector<std::string>& getErrors() const;
    
    /**
     * 获取编译警告信息
     * @return 警告信息列表
     */
    const std::vector<std::string>& getWarnings() const;

private:
    std::unique_ptr<class Lexer> lexer_;           // 词法分析器
    std::unique_ptr<class Parser> parser_;         // 语法分析器
    std::unique_ptr<class SemanticAnalyzer> semantic_analyzer_; // 语义分析器
    std::unique_ptr<CodeGenerator> code_generator_; // 代码生成器
    
    std::vector<std::string> errors_;              // 错误信息
    std::vector<std::string> warnings_;            // 警告信息
};

/**
 * CAPL 词法分析器
 * 将源代码转换为 Token 流
 */
class Lexer {
public:
    /**
     * 构造函数
     * @param source 源代码
     */
    explicit Lexer(const std::string& source);
    
    /**
     * 获取下一个 Token
     * @return Token 对象
     */
    Token nextToken();
    
    /**
     * 检查是否还有更多 Token
     * @return 是否有更多 Token
     */
    bool hasMoreTokens() const;

private:
    std::string source_;
    size_t position_;
    size_t line_;
    size_t column_;
};

/**
 * CAPL 语法分析器
 * 将 Token 流转换为抽象语法树 (AST)
 */
class Parser {
public:
    /**
     * 构造函数
     * @param lexer 词法分析器
     */
    explicit Parser(std::unique_ptr<Lexer> lexer);
    
    /**
     * 解析源代码生成 AST
     * @return AST 根节点
     */
    std::unique_ptr<ASTNode> parse();
    
    /**
     * 获取解析错误信息
     * @return 错误信息列表
     */
    const std::vector<std::string>& getErrors() const;

private:
    std::unique_ptr<Lexer> lexer_;
    Token current_token_;
    std::vector<std::string> errors_;
    bool has_errors_;
    
    // 错误处理方法
    void reportError(const std::string& message);
    bool expect(TokenType expected);
    void advance();
    std::string tokenTypeToString(TokenType type);
    
    // 各种语法规则的解析方法
    std::unique_ptr<ASTNode> parseProgram();
    std::unique_ptr<ASTNode> parseTopLevelDeclaration();
    std::unique_ptr<ASTNode> parseVariablesBlock();
    std::unique_ptr<ASTNode> parseVariableDeclaration();
    std::unique_ptr<ASTNode> parseEventHandler();
    std::unique_ptr<ASTNode> parseFunction();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseAssignmentOrCall();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseWhileStatement();
    std::unique_ptr<ASTNode> parseForStatement();
    std::unique_ptr<ASTNode> parseExpression();
};

/**
 * CAPL 语义分析器
 * 进行类型检查和语义验证
 */
class SemanticAnalyzer {
public:
    /**
     * 构造函数
     */
    SemanticAnalyzer();
    
    /**
     * 分析 AST 进行语义检查
     * @param ast AST 根节点
     * @return 分析是否成功
     */
    bool analyze(const std::unique_ptr<ASTNode>& ast);
    
    /**
     * 获取符号表
     * @return 符号表引用
     */
    const SymbolTable& getSymbolTable() const;

private:
    std::unique_ptr<SymbolTable> symbol_table_;
};

/**
 * CAPL 代码生成器
 * 将 AST 转换为目标代码
 */
class CodeGenerator {
public:
    /**
     * 构造函数
     */
    CodeGenerator();
    
    /**
     * 生成目标代码
     * @param ast AST 根节点
     * @param symbol_table 符号表
     * @param output_file 输出文件路径
     * @return 生成是否成功
     */
    bool generate(const std::unique_ptr<ASTNode>& ast, 
                  const SymbolTable& symbol_table,
                  const std::string& output_file);

private:
    // 代码生成的具体实现
};

/**
 * CAPL 运行时环境
 * 提供 CAPL 程序运行时所需的环境和服务
 */
class CAPLRuntime {
public:
    /**
     * 构造函数
     */
    CAPLRuntime();
    
    /**
     * 初始化运行时环境
     * @return 初始化是否成功
     */
    bool initialize();
    
    /**
     * 加载并执行编译后的 CAPL 程序
     * @param program_file 程序文件路径
     * @return 执行是否成功
     */
    bool loadAndRun(const std::string& program_file);
    
    /**
     * 停止运行时环境
     */
    void shutdown();

private:
    // 运行时环境的具体实现
};

} // namespace capl

#endif // CAPL_COMPILER_H