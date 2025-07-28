/**
 * CAPL 语义分析器实现
 */

#include "../include/capl_compiler.h"
#include "../include/ast.h"
#include "../include/symbol_table.h"
#include <functional>
#include <iostream>

namespace capl {

SemanticAnalyzer::SemanticAnalyzer() 
    : symbol_table_(std::make_unique<SymbolTable>()) {
}

bool SemanticAnalyzer::analyze(const std::unique_ptr<ASTNode>& ast) {
    if (!ast) {
        return false;
    }
    
    // 使用 lambda 函数进行递归分析
    std::function<void(const ASTNode*)> analyzeNode = 
        [&](const ASTNode* node) {
            if (!node) return;
            
            switch (node->getType()) {
                case ASTNodeType::FUNCTION: {
                    // 对于函数节点，尝试转换为 FunctionNode
                    const FunctionNode* funcNode = dynamic_cast<const FunctionNode*>(node);
                    if (funcNode) {
                        Symbol func_symbol(funcNode->getName(), SymbolType::FUNCTION, funcNode->getReturnType());
                        symbol_table_->addSymbol(func_symbol);
                    }
                    break;
                }
                case ASTNodeType::VARIABLE_DECL: {
                    // 对于变量声明节点，尝试转换为 VariableDeclNode
                    const VariableDeclNode* varNode = dynamic_cast<const VariableDeclNode*>(node);
                    if (varNode) {
                        Symbol var_symbol(varNode->getName(), SymbolType::VARIABLE, varNode->getVarType());
                        symbol_table_->addSymbol(var_symbol);
                    }
                    break;
                }
                case ASTNodeType::IDENTIFIER: {
                    // 对于标识符节点，需要检查是否已定义
                    const IdentifierNode* idNode = dynamic_cast<const IdentifierNode*>(node);
                    if (idNode && !symbol_table_->hasSymbol(idNode->getName())) {
                        std::cerr << "Error: Undefined identifier '" << idNode->getName() << "'" << std::endl;
                    }
                    break;
                }
                case ASTNodeType::CALL_EXPR: {
                    // 对于函数调用节点，检查函数是否已定义
                    const CallExprNode* callNode = dynamic_cast<const CallExprNode*>(node);
                    if (callNode && !symbol_table_->hasSymbol(callNode->getFunctionName())) {
                        std::cerr << "Error: Undefined function '" << callNode->getFunctionName() << "'" << std::endl;
                    }
                    break;
                }
                default:
                    break;
            }
            
            // 递归分析子节点
            for (const auto& child : node->getChildren()) {
                analyzeNode(child.get());
            }
        };
    
    // 开始分析
    analyzeNode(ast.get());
    return true;
}

const SymbolTable& SemanticAnalyzer::getSymbolTable() const {
    return *symbol_table_;
}

} // namespace capl