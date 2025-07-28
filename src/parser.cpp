/**
 * CAPL 语法分析器实现
 */

#include "../include/capl_compiler.h"
#include "../include/ast.h"
#include <iostream>
#include <stdexcept>

namespace capl {

Parser::Parser(std::unique_ptr<Lexer> lexer) 
    : lexer_(std::move(lexer)), has_errors_(false) {
    // 获取第一个 token
    advance();
}

/**
 * 报告语法错误
 * @param message 错误消息
 */
void Parser::reportError(const std::string& message) {
    has_errors_ = true;
    std::string error_msg = "语法错误 (行 " + std::to_string(current_token_.getLine()) + 
                           ", 列 " + std::to_string(current_token_.getColumn()) + "): " + message;
    errors_.push_back(error_msg);
    std::cerr << error_msg << std::endl;
}

/**
 * 期望特定类型的 token
 * @param expected_type 期望的 token 类型
 * @return 是否匹配成功
 */
bool Parser::expect(TokenType expected_type) {
    if (current_token_.getType() == expected_type) {
        advance();
        return true;
    } else {
        reportError("期望 '" + tokenTypeToString(expected_type) + 
                   "', 但得到 '" + current_token_.getValue() + "'");
        return false;
    }
}

/**
 * 前进到下一个 token
 */
void Parser::advance() {
    current_token_ = lexer_->nextToken();
}

/**
 * 将 TokenType 转换为字符串
 */
std::string Parser::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::SEMICOLON: return ";";
        case TokenType::LEFT_BRACE: return "{";
        case TokenType::RIGHT_BRACE: return "}";
        case TokenType::LEFT_PAREN: return "(";
        case TokenType::RIGHT_PAREN: return ")";
        case TokenType::IDENTIFIER: return "标识符";
        case TokenType::INTEGER: return "整数";
        case TokenType::ASSIGN: return "=";
        default: return "未知";
    }
}

std::unique_ptr<ASTNode> Parser::parse() {
    auto result = parseProgram();
    if (has_errors_) {
        return nullptr;
    }
    return result;
}

std::unique_ptr<ASTNode> Parser::parseProgram() {
    auto program = std::make_unique<ASTNode>(ASTNodeType::PROGRAM);
    
    while (current_token_.getType() != TokenType::EOF_TOKEN && !has_errors_) {
        try {
            auto stmt = parseTopLevelDeclaration();
            if (stmt) {
                program->addChild(std::move(stmt));
            } else {
                // 如果parseTopLevelDeclaration返回nullptr，说明遇到了意外的token
                // 需要跳过它以避免无限循环
                if (current_token_.getType() != TokenType::EOF_TOKEN) {
                    advance();
                }
            }
        } catch (const std::exception& e) {
            reportError("解析顶级声明时出错: " + std::string(e.what()));
            // 尝试恢复：跳到下一个可能的声明开始
            while (current_token_.getType() != TokenType::EOF_TOKEN &&
                   current_token_.getType() != TokenType::VARIABLES &&
                   current_token_.getType() != TokenType::ON) {
                advance();
            }
        }
    }
    
    return program;
}

/**
 * 解析顶级声明（variables 块、事件处理器等）
 */
std::unique_ptr<ASTNode> Parser::parseTopLevelDeclaration() {
    switch (current_token_.getType()) {
        case TokenType::VARIABLES:
            return parseVariablesBlock();
        case TokenType::ON:
            return parseEventHandler();
        default:
            reportError("意外的顶级声明: " + current_token_.getValue());
            // 不要在这里跳过token，让parseProgram来处理
            return nullptr;
    }
}

/**
 * 解析 variables 块
 */
std::unique_ptr<ASTNode> Parser::parseVariablesBlock() {
    auto block = std::make_unique<ASTNode>(ASTNodeType::BLOCK_STMT);
    
    // 期望 'variables' 关键字
    if (!expect(TokenType::VARIABLES)) {
        return nullptr;
    }
    
    // 期望左大括号
    if (!expect(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    // 解析变量声明
    while (current_token_.getType() != TokenType::RIGHT_BRACE && 
           current_token_.getType() != TokenType::EOF_TOKEN) {
        auto var_decl = parseVariableDeclaration();
        if (var_decl) {
            block->addChild(std::move(var_decl));
        } else {
            // 如果解析失败，跳过当前token直到找到分号或右大括号
            while (current_token_.getType() != TokenType::SEMICOLON && 
                   current_token_.getType() != TokenType::RIGHT_BRACE && 
                   current_token_.getType() != TokenType::EOF_TOKEN) {
                advance();
            }
            if (current_token_.getType() == TokenType::SEMICOLON) {
                advance(); // 跳过分号
            }
        }
    }
    
    // 期望右大括号
    if (!expect(TokenType::RIGHT_BRACE)) {
        return nullptr;
    }
    
    return block;
}

/**
 * 解析变量声明
 */
std::unique_ptr<ASTNode> Parser::parseVariableDeclaration() {
    auto var_decl = std::make_unique<ASTNode>(ASTNodeType::VARIABLE_DECL);
    
    // 期望类型（int, float, char 等）
    if (current_token_.getType() != TokenType::INT && 
        current_token_.getType() != TokenType::FLOAT_KW &&
        current_token_.getType() != TokenType::CHAR_KW) {
        reportError("期望变量类型 (int, float, char), 但得到 '" + current_token_.getValue() + "'");
        // 跳过错误的token，避免无限循环
        advance();
        return nullptr;
    }
    
    advance(); // 跳过类型
    
    // 期望变量名
    if (current_token_.getType() != TokenType::IDENTIFIER) {
        reportError("期望变量名");
        // 跳过错误的token，避免无限循环
        advance();
        return nullptr;
    }
    
    advance(); // 跳过变量名
    
    // 期望分号
    if (!expect(TokenType::SEMICOLON)) {
        return nullptr;
    }
    
    return var_decl;
}

/**
 * 解析事件处理器
 */
std::unique_ptr<ASTNode> Parser::parseEventHandler() {
    auto event_handler = std::make_unique<ASTNode>(ASTNodeType::FUNCTION);
    
    // 期望 'on' 关键字
    if (!expect(TokenType::ON)) {
        return nullptr;
    }
    
    // 解析事件类型
    if (current_token_.getType() == TokenType::START ||
        current_token_.getType() == TokenType::MESSAGE ||
        current_token_.getType() == TokenType::TIMER ||
        current_token_.getType() == TokenType::KEY) {
        advance();
    } else {
        reportError("期望事件类型 (start, message, timer, key)");
        return nullptr;
    }
    
    // 如果是 message 事件，可能有 ID
    if (current_token_.getType() == TokenType::INTEGER) {
        advance();
    }
    
    // 期望左大括号
    if (!expect(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    // 解析语句块
    while (current_token_.getType() != TokenType::RIGHT_BRACE && 
           current_token_.getType() != TokenType::EOF_TOKEN) {
        auto stmt = parseStatement();
        if (stmt) {
            event_handler->addChild(std::move(stmt));
        } else {
            // 如果parseStatement返回nullptr，但不是因为遇到右大括号或EOF，
            // 说明有错误，需要跳过当前token避免无限循环
            if (current_token_.getType() != TokenType::RIGHT_BRACE && 
                current_token_.getType() != TokenType::EOF_TOKEN) {
                advance();
            }
        }
    }
    
    // 期望右大括号
    if (!expect(TokenType::RIGHT_BRACE)) {
        return nullptr;
    }
    
    return event_handler;
}

std::unique_ptr<ASTNode> Parser::parseFunction() {
    // 简单的函数解析实现
    auto func = std::make_unique<ASTNode>(ASTNodeType::FUNCTION);
    return func;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    switch (current_token_.getType()) {
        case TokenType::IDENTIFIER:
            return parseAssignmentOrCall();
        case TokenType::IF:
            return parseIfStatement();
        case TokenType::WHILE:
            return parseWhileStatement();
        case TokenType::FOR:
            return parseForStatement();
        case TokenType::RIGHT_BRACE:
        case TokenType::EOF_TOKEN:
            // 这些不是语句，而是语句块的结束标志
            return nullptr;
        default:
            reportError("意外的语句: " + current_token_.getValue());
            advance(); // 跳过错误的 token
            return nullptr;
    }
}

/**
 * 解析赋值语句或函数调用
 */
std::unique_ptr<ASTNode> Parser::parseAssignmentOrCall() {
    auto stmt = std::make_unique<ASTNode>(ASTNodeType::EXPRESSION_STMT);
    
    // 期望标识符
    if (current_token_.getType() != TokenType::IDENTIFIER) {
        reportError("期望标识符");
        return nullptr;
    }
    
    advance(); // 跳过标识符
    
    if (current_token_.getType() == TokenType::ASSIGN) {
        // 赋值语句
        advance(); // 跳过 =
        
        // 解析表达式
        auto expr = parseExpression();
        if (!expr) {
            return nullptr;
        }
        
        // 期望分号
        if (!expect(TokenType::SEMICOLON)) {
            return nullptr;
        }
    } else if (current_token_.getType() == TokenType::LEFT_PAREN) {
        // 函数调用
        advance(); // 跳过 (
        
        // 解析参数（简化处理）
        while (current_token_.getType() != TokenType::RIGHT_PAREN && 
               current_token_.getType() != TokenType::EOF_TOKEN) {
            advance();
        }
        
        // 期望右括号
        if (!expect(TokenType::RIGHT_PAREN)) {
            return nullptr;
        }
        
        // 期望分号
        if (!expect(TokenType::SEMICOLON)) {
            return nullptr;
        }
    } else {
        reportError("期望 '=' 或 '(' 在标识符后");
        return nullptr;
    }
    
    return stmt;
}

/**
 * 解析 if 语句
 */
std::unique_ptr<ASTNode> Parser::parseIfStatement() {
    auto if_stmt = std::make_unique<ASTNode>(ASTNodeType::IF_STMT);
    
    // 期望 'if' 关键字
    if (!expect(TokenType::IF)) {
        return nullptr;
    }
    
    // 期望左括号
    if (!expect(TokenType::LEFT_PAREN)) {
        return nullptr;
    }
    
    // 解析条件表达式
    auto condition = parseExpression();
    if (!condition) {
        return nullptr;
    }
    
    // 期望右括号
    if (!expect(TokenType::RIGHT_PAREN)) {
        return nullptr;
    }
    
    // 期望左大括号
    if (!expect(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    // 解析语句块
    while (current_token_.getType() != TokenType::RIGHT_BRACE && 
           current_token_.getType() != TokenType::EOF_TOKEN) {
        auto stmt = parseStatement();
        if (stmt) {
            if_stmt->addChild(std::move(stmt));
        } else {
            // 如果parseStatement返回nullptr，但不是因为遇到右大括号或EOF，
            // 说明有错误，需要跳过当前token避免无限循环
            if (current_token_.getType() != TokenType::RIGHT_BRACE && 
                current_token_.getType() != TokenType::EOF_TOKEN) {
                advance();
            }
        }
    }
    
    // 期望右大括号
    if (!expect(TokenType::RIGHT_BRACE)) {
        return nullptr;
    }
    
    // 检查是否有 else 子句
    if (current_token_.getType() == TokenType::ELSE) {
        advance(); // 跳过 'else'
        
        // 期望左大括号
        if (!expect(TokenType::LEFT_BRACE)) {
            return nullptr;
        }
        
        // 解析 else 语句块
        while (current_token_.getType() != TokenType::RIGHT_BRACE && 
               current_token_.getType() != TokenType::EOF_TOKEN) {
            auto stmt = parseStatement();
            if (stmt) {
                if_stmt->addChild(std::move(stmt));
            } else {
                // 如果parseStatement返回nullptr，但不是因为遇到右大括号或EOF，
                // 说明有错误，需要跳过当前token避免无限循环
                if (current_token_.getType() != TokenType::RIGHT_BRACE && 
                    current_token_.getType() != TokenType::EOF_TOKEN) {
                    advance();
                }
            }
        }
        
        // 期望右大括号
        if (!expect(TokenType::RIGHT_BRACE)) {
            return nullptr;
        }
    }
    
    return if_stmt;
}

/**
 * 解析 while 语句
 */
std::unique_ptr<ASTNode> Parser::parseWhileStatement() {
    auto while_stmt = std::make_unique<ASTNode>(ASTNodeType::WHILE_STMT);
    
    // 期望 'while' 关键字
    if (!expect(TokenType::WHILE)) {
        return nullptr;
    }
    
    // 期望左括号
    if (!expect(TokenType::LEFT_PAREN)) {
        return nullptr;
    }
    
    // 解析条件表达式
    auto condition = parseExpression();
    if (!condition) {
        return nullptr;
    }
    
    // 期望右括号
    if (!expect(TokenType::RIGHT_PAREN)) {
        return nullptr;
    }
    
    // 期望左大括号
    if (!expect(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    // 解析语句块
    while (current_token_.getType() != TokenType::RIGHT_BRACE && 
           current_token_.getType() != TokenType::EOF_TOKEN) {
        auto stmt = parseStatement();
        if (stmt) {
            while_stmt->addChild(std::move(stmt));
        } else {
            // 如果parseStatement返回nullptr，但不是因为遇到右大括号或EOF，
            // 说明有错误，需要跳过当前token避免无限循环
            if (current_token_.getType() != TokenType::RIGHT_BRACE && 
                current_token_.getType() != TokenType::EOF_TOKEN) {
                advance();
            }
        }
    }
    
    // 期望右大括号
    if (!expect(TokenType::RIGHT_BRACE)) {
        return nullptr;
    }
    
    return while_stmt;
}

/**
 * 解析 for 语句
 */
std::unique_ptr<ASTNode> Parser::parseForStatement() {
    auto for_stmt = std::make_unique<ASTNode>(ASTNodeType::FOR_STMT);
    
    // 期望 'for' 关键字
    if (!expect(TokenType::FOR)) {
        return nullptr;
    }
    
    // 期望左括号
    if (!expect(TokenType::LEFT_PAREN)) {
        return nullptr;
    }
    
    // 简化处理：跳过 for 循环的三个部分
    int semicolon_count = 0;
    while (current_token_.getType() != TokenType::RIGHT_PAREN && 
           current_token_.getType() != TokenType::EOF_TOKEN) {
        if (current_token_.getType() == TokenType::SEMICOLON) {
            semicolon_count++;
        }
        advance();
    }
    
    if (semicolon_count != 2) {
        reportError("for 循环应该有两个分号");
        return nullptr;
    }
    
    // 期望右括号
    if (!expect(TokenType::RIGHT_PAREN)) {
        return nullptr;
    }
    
    // 期望左大括号
    if (!expect(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    // 解析语句块
    while (current_token_.getType() != TokenType::RIGHT_BRACE && 
           current_token_.getType() != TokenType::EOF_TOKEN) {
        auto stmt = parseStatement();
        if (stmt) {
            for_stmt->addChild(std::move(stmt));
        }
    }
    
    // 期望右大括号
    if (!expect(TokenType::RIGHT_BRACE)) {
        return nullptr;
    }
    
    return for_stmt;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    // 简单的表达式解析实现
    auto expr = std::make_unique<ASTNode>(ASTNodeType::BINARY_EXPR);
    
    // 至少需要一个操作数
    if (current_token_.getType() == TokenType::IDENTIFIER ||
        current_token_.getType() == TokenType::INTEGER ||
        current_token_.getType() == TokenType::FLOAT ||
        current_token_.getType() == TokenType::CHAR) {
        advance();
    } else {
        reportError("期望表达式");
        return nullptr;
    }
    
    // 可选的操作符和第二个操作数
    if (current_token_.getType() == TokenType::PLUS ||
        current_token_.getType() == TokenType::MINUS ||
        current_token_.getType() == TokenType::MULTIPLY ||
        current_token_.getType() == TokenType::DIVIDE ||
        current_token_.getType() == TokenType::EQUAL ||
        current_token_.getType() == TokenType::NOT_EQUAL ||
        current_token_.getType() == TokenType::LESS ||
        current_token_.getType() == TokenType::GREATER ||
        current_token_.getType() == TokenType::LESS_EQUAL ||
        current_token_.getType() == TokenType::GREATER_EQUAL) {
        advance(); // 跳过操作符
        
        // 期望第二个操作数
        if (current_token_.getType() == TokenType::IDENTIFIER ||
            current_token_.getType() == TokenType::INTEGER ||
            current_token_.getType() == TokenType::FLOAT ||
            current_token_.getType() == TokenType::CHAR) {
            advance();
        } else {
            reportError("期望表达式的右操作数");
            return nullptr;
        }
    }
    
    return expr;
}

/**
 * 获取解析错误
 */
const std::vector<std::string>& Parser::getErrors() const {
    return errors_;
}

} // namespace capl