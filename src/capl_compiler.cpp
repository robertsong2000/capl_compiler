/**
 * CAPL 编译器主类实现
 */

#include "../include/capl_compiler.h"
#include "../include/ast.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace capl {

/**
 * 构造函数
 */
CAPLCompiler::CAPLCompiler() {
    // 初始化各个组件
    semantic_analyzer_ = std::make_unique<SemanticAnalyzer>();
    code_generator_ = std::make_unique<CodeGenerator>();
}

/**
 * 仅进行语法检查，不生成代码
 * @param source_file CAPL 源文件路径
 * @return 语法检查是否通过
 */
bool CAPLCompiler::syntaxCheck(const std::string& source_file) {
    // 清空之前的错误和警告
    errors_.clear();
    warnings_.clear();
    
    try {
        // 读取源文件
        std::ifstream file(source_file);
        if (!file.is_open()) {
            errors_.push_back("无法打开源文件: " + source_file);
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source_code = buffer.str();
        file.close();
        
        // 进行语法检查
        return syntaxCheckFromString(source_code);
        
    } catch (const std::exception& e) {
        errors_.push_back("语法检查过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

/**
 * 仅进行语法检查，不生成代码（从字符串）
 * @param source_code CAPL 源代码
 * @return 语法检查是否通过
 */
bool CAPLCompiler::syntaxCheckFromString(const std::string& source_code) {
    // 清空之前的错误和警告
    errors_.clear();
    warnings_.clear();
    
    try {
        std::cout << "开始语法检查..." << std::endl;
        
        // 1. 词法分析
        std::cout << "1. 词法分析..." << std::endl;
        lexer_ = std::make_unique<Lexer>(source_code);
        
        // 2. 语法分析
        std::cout << "2. 语法分析..." << std::endl;
        parser_ = std::make_unique<Parser>(std::move(lexer_));
        auto ast = parser_->parse();
        
        // 检查解析器是否有错误
        const auto& parser_errors = parser_->getErrors();
        if (!parser_errors.empty()) {
            // 将解析器的错误添加到编译器的错误列表中
            for (const auto& error : parser_errors) {
                errors_.push_back(error);
            }
            return false;
        }
        
        if (!ast) {
            errors_.push_back("语法分析失败");
            return false;
        }
        
        // 3. 语义分析
        std::cout << "3. 语义分析..." << std::endl;
        if (!semantic_analyzer_->analyze(ast)) {
            errors_.push_back("语义分析失败");
            return false;
        }
        
        // 语法检查模式不进行代码生成
        std::cout << "语法检查完成!" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        errors_.push_back("语法检查过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

/**
 * 析构函数
 */
CAPLCompiler::~CAPLCompiler() = default;

/**
 * 编译 CAPL 源文件
 * @param source_file CAPL 源文件路径
 * @param output_file 输出文件路径
 * @return 编译是否成功
 */
bool CAPLCompiler::compile(const std::string& source_file, const std::string& output_file) {
    // 清空之前的错误和警告
    errors_.clear();
    warnings_.clear();
    
    try {
        // 读取源文件
        std::ifstream file(source_file);
        if (!file.is_open()) {
            errors_.push_back("无法打开源文件: " + source_file);
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source_code = buffer.str();
        file.close();
        
        // 编译源代码
        return compileFromString(source_code, output_file);
        
    } catch (const std::exception& e) {
        errors_.push_back("编译过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

/**
 * 编译 CAPL 源代码字符串
 * @param source_code CAPL 源代码
 * @param output_file 输出文件路径
 * @return 编译是否成功
 */
bool CAPLCompiler::compileFromString(const std::string& source_code, const std::string& output_file) {
    // 清空之前的错误和警告
    errors_.clear();
    warnings_.clear();
    
    try {
        std::cout << "开始编译 CAPL 代码..." << std::endl;
        
        // 1. 词法分析
        std::cout << "1. 词法分析..." << std::endl;
        lexer_ = std::make_unique<Lexer>(source_code);
        
        // 2. 语法分析
        std::cout << "2. 语法分析..." << std::endl;
        parser_ = std::make_unique<Parser>(std::move(lexer_));
        auto ast = parser_->parse();
        
        if (!ast) {
            errors_.push_back("语法分析失败");
            return false;
        }
        
        // 3. 语义分析
        std::cout << "3. 语义分析..." << std::endl;
        if (!semantic_analyzer_->analyze(ast)) {
            errors_.push_back("语义分析失败");
            return false;
        }
        
        // 4. 代码生成
        std::cout << "4. 代码生成..." << std::endl;
        if (!code_generator_->generate(ast, semantic_analyzer_->getSymbolTable(), output_file)) {
            errors_.push_back("代码生成失败");
            return false;
        }
        
        std::cout << "编译成功!" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        errors_.push_back("编译过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

/**
 * 获取编译错误信息
 * @return 错误信息列表
 */
const std::vector<std::string>& CAPLCompiler::getErrors() const {
    return errors_;
}

/**
 * 获取编译警告信息
 * @return 警告信息列表
 */
const std::vector<std::string>& CAPLCompiler::getWarnings() const {
    return warnings_;
}

} // namespace capl