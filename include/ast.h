/**
 * CAPL 抽象语法树 (AST) 节点定义
 * 
 * 定义了 CAPL 语言的抽象语法树节点类型和结构
 */

#ifndef CAPL_AST_H
#define CAPL_AST_H

#include <memory>
#include <vector>
#include <string>
#include "token.h"

namespace capl {

/**
 * AST 节点类型枚举
 */
enum class ASTNodeType {
    // 程序结构
    PROGRAM,            // 程序根节点
    FUNCTION,           // 函数定义
    VARIABLE_DECL,      // 变量声明
    
    // 语句
    EXPRESSION_STMT,    // 表达式语句
    IF_STMT,            // if 语句
    WHILE_STMT,         // while 语句
    FOR_STMT,           // for 语句
    SWITCH_STMT,        // switch 语句
    CASE_STMT,          // case 语句
    BREAK_STMT,         // break 语句
    CONTINUE_STMT,      // continue 语句
    RETURN_STMT,        // return 语句
    BLOCK_STMT,         // 代码块语句
    
    // 表达式
    BINARY_EXPR,        // 二元表达式
    UNARY_EXPR,         // 一元表达式
    ASSIGNMENT_EXPR,    // 赋值表达式
    CALL_EXPR,          // 函数调用表达式
    MEMBER_EXPR,        // 成员访问表达式
    INDEX_EXPR,         // 数组索引表达式
    CONDITIONAL_EXPR,   // 条件表达式 (三元操作符)
    
    // 字面量
    INTEGER_LITERAL,    // 整数字面量
    FLOAT_LITERAL,      // 浮点数字面量
    STRING_LITERAL,     // 字符串字面量
    CHAR_LITERAL,       // 字符字面量
    BOOLEAN_LITERAL,    // 布尔字面量
    
    // 标识符
    IDENTIFIER,         // 标识符
    
    // CAPL 特有节点
    ON_MESSAGE,         // on message 事件处理
    ON_TIMER,           // on timer 事件处理
    ON_KEY,             // on key 事件处理
    ON_START,           // on start 事件处理
    ON_STOP,            // on stop 事件处理
    SIGNAL_ACCESS,      // 信号访问
    ENVVAR_ACCESS,      // 环境变量访问
    SYSVAR_ACCESS,      // 系统变量访问
    MESSAGE_SEND,       // 消息发送
    TIMER_SET,          // 定时器设置
};

/**
 * AST 节点基类
 */
class ASTNode {
public:
    /**
     * 构造函数
     * @param type 节点类型
     */
    explicit ASTNode(ASTNodeType type);
    
    /**
     * 虚析构函数
     */
    virtual ~ASTNode() = default;
    
    /**
     * 获取节点类型
     * @return 节点类型
     */
    ASTNodeType getType() const { return type_; }
    
    /**
     * 添加子节点
     * @param child 子节点
     */
    void addChild(std::unique_ptr<ASTNode> child);
    
    /**
     * 获取子节点列表
     * @return 子节点列表
     */
    const std::vector<std::unique_ptr<ASTNode>>& getChildren() const { return children_; }
    
    /**
     * 获取子节点数量
     * @return 子节点数量
     */
    size_t getChildCount() const { return children_.size(); }
    
    /**
     * 获取指定索引的子节点
     * @param index 索引
     * @return 子节点指针
     */
    ASTNode* getChild(size_t index) const;
    
    /**
     * 设置行号
     * @param line 行号
     */
    void setLine(int line) { line_ = line; }
    
    /**
     * 获取行号
     * @return 行号
     */
    int getLine() const { return line_; }
    
    /**
     * 设置列号
     * @param column 列号
     */
    void setColumn(int column) { column_ = column; }
    
    /**
     * 获取列号
     * @return 列号
     */
    int getColumn() const { return column_; }
    
    /**
     * 转换为字符串表示（用于调试）
     * @param indent 缩进级别
     * @return 字符串表示
     */
    virtual std::string toString(int indent = 0) const;

protected:
    ASTNodeType type_;                                  // 节点类型
    std::vector<std::unique_ptr<ASTNode>> children_;    // 子节点列表
    int line_ = 0;                                      // 行号
    int column_ = 0;                                    // 列号
};

/**
 * 程序节点
 * 表示整个 CAPL 程序
 */
class ProgramNode : public ASTNode {
public:
    ProgramNode();
    std::string toString(int indent = 0) const override;
};

/**
 * 函数节点
 * 表示函数定义
 */
class FunctionNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param name 函数名
     * @param return_type 返回类型
     */
    FunctionNode(const std::string& name, const std::string& return_type);
    
    const std::string& getName() const { return name_; }
    const std::string& getReturnType() const { return return_type_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string name_;          // 函数名
    std::string return_type_;   // 返回类型
};

/**
 * 变量声明节点
 */
class VariableDeclNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param name 变量名
     * @param type 变量类型
     */
    VariableDeclNode(const std::string& name, const std::string& type);
    
    const std::string& getName() const { return name_; }
    const std::string& getVarType() const { return var_type_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string name_;      // 变量名
    std::string var_type_;  // 变量类型
};

/**
 * 二元表达式节点
 */
class BinaryExprNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param op 操作符
     */
    explicit BinaryExprNode(const std::string& op);
    
    const std::string& getOperator() const { return operator_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string operator_;  // 操作符
};

/**
 * 一元表达式节点
 */
class UnaryExprNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param op 操作符
     */
    explicit UnaryExprNode(const std::string& op);
    
    const std::string& getOperator() const { return operator_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string operator_;  // 操作符
};

/**
 * 字面量节点
 */
class LiteralNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param type 节点类型
     * @param value 字面量值
     */
    LiteralNode(ASTNodeType type, const std::string& value);
    
    const std::string& getValue() const { return value_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string value_;     // 字面量值
};

/**
 * 标识符节点
 */
class IdentifierNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param name 标识符名称
     */
    explicit IdentifierNode(const std::string& name);
    
    const std::string& getName() const { return name_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string name_;      // 标识符名称
};

/**
 * CAPL 事件处理节点
 */
class OnEventNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param event_type 事件类型
     * @param event_name 事件名称
     */
    OnEventNode(ASTNodeType event_type, const std::string& event_name);
    
    const std::string& getEventName() const { return event_name_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string event_name_;    // 事件名称
};

/**
 * 函数调用节点
 */
class CallExprNode : public ASTNode {
public:
    /**
     * 构造函数
     * @param function_name 函数名
     */
    explicit CallExprNode(const std::string& function_name);
    
    const std::string& getFunctionName() const { return function_name_; }
    
    std::string toString(int indent = 0) const override;

private:
    std::string function_name_; // 函数名
};

/**
 * AST 访问者接口
 * 用于遍历和处理 AST
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(ASTNode* node) = 0;
    virtual void visitProgram(ProgramNode* node) = 0;
    virtual void visitFunction(FunctionNode* node) = 0;
    virtual void visitVariableDecl(VariableDeclNode* node) = 0;
    virtual void visitBinaryExpr(BinaryExprNode* node) = 0;
    virtual void visitUnaryExpr(UnaryExprNode* node) = 0;
    virtual void visitLiteral(LiteralNode* node) = 0;
    virtual void visitIdentifier(IdentifierNode* node) = 0;
    virtual void visitOnEvent(OnEventNode* node) = 0;
    virtual void visitCallExpr(CallExprNode* node) = 0;
};

} // namespace capl

#endif // CAPL_AST_H