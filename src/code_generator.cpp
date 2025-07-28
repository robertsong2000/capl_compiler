/**
 * CAPL 代码生成器实现
 */

#include "../include/capl_compiler.h"
#include "../include/ast.h"
#include <iostream>
#include <fstream>
#include <functional>

namespace capl {

/**
 * 构造函数
 */
CodeGenerator::CodeGenerator() {
}

/**
 * 生成目标代码
 * @param ast AST 根节点
 * @param symbol_table 符号表
 * @param output_file 输出文件路径
 * @return 生成是否成功
 */
bool CodeGenerator::generate(const std::unique_ptr<ASTNode>& ast, 
                            const SymbolTable& symbol_table,
                            const std::string& output_file) {
    if (!ast) {
        std::cerr << "错误: AST 为空" << std::endl;
        return false;
    }
    
    try {
        std::ofstream output(output_file);
        if (!output.is_open()) {
            std::cerr << "错误: 无法打开输出文件: " << output_file << std::endl;
            return false;
        }
        
        // 生成 C++ 代码头部
        output << "// 由 CAPL 编译器生成的 C++ 代码\n";
        output << "#include <iostream>\n";
        output << "#include <string>\n";
        output << "#include <vector>\n";
        output << "#include <map>\n\n";
        
        // 生成 CAPL 运行时支持代码
        output << "// CAPL 运行时支持函数\n";
        output << "namespace capl_runtime {\n";
        output << "    void write(const std::string& msg) {\n";
        output << "        std::cout << msg << std::endl;\n";
        output << "    }\n";
        output << "    \n";
        output << "    void setTimer(const std::string& name, int ms) {\n";
        output << "        // 定时器实现占位符\n";
        output << "    }\n";
        output << "    \n";
        output << "    void output(int value) {\n";
        output << "        std::cout << \"输出: \" << value << std::endl;\n";
        output << "    }\n";
        output << "}\n\n";
        output << "using namespace capl_runtime;\n\n";
        
        // 使用 lambda 函数递归生成代码
        std::function<void(ASTNode*, std::ofstream&, int)> generateNode = 
            [&](ASTNode* node, std::ofstream& out, int indent) {
                if (!node) {
                    return;
                }
                
                std::string indentStr(indent * 4, ' ');
                
                switch (node->getType()) {
                    case ASTNodeType::PROGRAM: {
                        out << "int main() {\n";
                        out << "    // CAPL 程序开始\n";
                        for (const auto& child : node->getChildren()) {
                            generateNode(child.get(), out, indent + 1);
                        }
                        out << "    return 0;\n";
                        out << "}\n";
                        break;
                    }
                    case ASTNodeType::FUNCTION: {
                        FunctionNode* funcNode = static_cast<FunctionNode*>(node);
                        out << indentStr << funcNode->getReturnType() << " " 
                            << funcNode->getName() << "() {\n";
                        for (const auto& child : node->getChildren()) {
                            generateNode(child.get(), out, indent + 1);
                        }
                        out << indentStr << "}\n\n";
                        break;
                    }
                    case ASTNodeType::VARIABLE_DECL: {
                        VariableDeclNode* varNode = static_cast<VariableDeclNode*>(node);
                        out << indentStr << varNode->getVarType() << " " 
                            << varNode->getName() << ";\n";
                        break;
                    }
                    case ASTNodeType::ON_START: {
                        out << indentStr << "// on start 事件处理\n";
                        out << indentStr << "void onStart() {\n";
                        for (const auto& child : node->getChildren()) {
                            generateNode(child.get(), out, indent + 1);
                        }
                        out << indentStr << "}\n\n";
                        break;
                    }
                    case ASTNodeType::ON_MESSAGE: {
                        out << indentStr << "// on message 事件处理\n";
                        out << indentStr << "void onMessage() {\n";
                        for (const auto& child : node->getChildren()) {
                            generateNode(child.get(), out, indent + 1);
                        }
                        out << indentStr << "}\n\n";
                        break;
                    }
                    case ASTNodeType::CALL_EXPR: {
                        CallExprNode* callNode = static_cast<CallExprNode*>(node);
                        out << indentStr << callNode->getFunctionName() << "();\n";
                        break;
                    }
                    case ASTNodeType::INTEGER_LITERAL:
                    case ASTNodeType::FLOAT_LITERAL:
                    case ASTNodeType::STRING_LITERAL: {
                        LiteralNode* litNode = static_cast<LiteralNode*>(node);
                        out << litNode->getValue();
                        break;
                    }
                    case ASTNodeType::IDENTIFIER: {
                        IdentifierNode* idNode = static_cast<IdentifierNode*>(node);
                        out << idNode->getName();
                        break;
                    }
                    default:
                        // 对于其他类型的节点，递归处理子节点
                        for (const auto& child : node->getChildren()) {
                            generateNode(child.get(), out, indent);
                        }
                        break;
                }
            };
        
        // 开始生成代码
        generateNode(ast.get(), output, 0);
        
        output.close();
        std::cout << "代码生成成功: " << output_file << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "代码生成错误: " << e.what() << std::endl;
        return false;
    }
}

} // namespace capl