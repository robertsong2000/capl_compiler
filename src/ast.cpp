/**
 * CAPL AST 节点实现
 */

#include "../include/ast.h"

namespace capl {

// ASTNode 基类实现
ASTNode::ASTNode(ASTNodeType type) : type_(type) {
}

void ASTNode::addChild(std::unique_ptr<ASTNode> child) {
    children_.push_back(std::move(child));
}

ASTNode* ASTNode::getChild(size_t index) const {
    if (index < children_.size()) {
        return children_[index].get();
    }
    return nullptr;
}

std::string ASTNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "ASTNode(type=" + std::to_string(static_cast<int>(type_)) + ")\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// ProgramNode 实现
ProgramNode::ProgramNode() : ASTNode(ASTNodeType::PROGRAM) {
}

std::string ProgramNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "Program\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// FunctionNode 实现
FunctionNode::FunctionNode(const std::string& name, const std::string& return_type)
    : ASTNode(ASTNodeType::FUNCTION), name_(name), return_type_(return_type) {
}

std::string FunctionNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "Function: " + return_type_ + " " + name_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// VariableDeclNode 实现
VariableDeclNode::VariableDeclNode(const std::string& name, const std::string& type)
    : ASTNode(ASTNodeType::VARIABLE_DECL), name_(name), var_type_(type) {
}

std::string VariableDeclNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "VariableDecl: " + var_type_ + " " + name_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// BinaryExprNode 实现
BinaryExprNode::BinaryExprNode(const std::string& op)
    : ASTNode(ASTNodeType::BINARY_EXPR), operator_(op) {
}

std::string BinaryExprNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "BinaryExpr: " + operator_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// UnaryExprNode 实现
UnaryExprNode::UnaryExprNode(const std::string& op)
    : ASTNode(ASTNodeType::UNARY_EXPR), operator_(op) {
}

std::string UnaryExprNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "UnaryExpr: " + operator_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// LiteralNode 实现
LiteralNode::LiteralNode(ASTNodeType type, const std::string& value)
    : ASTNode(type), value_(value) {
}

std::string LiteralNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    
    std::string type_name;
    switch (type_) {
        case ASTNodeType::INTEGER_LITERAL:
            type_name = "IntegerLiteral";
            break;
        case ASTNodeType::FLOAT_LITERAL:
            type_name = "FloatLiteral";
            break;
        case ASTNodeType::STRING_LITERAL:
            type_name = "StringLiteral";
            break;
        case ASTNodeType::CHAR_LITERAL:
            type_name = "CharLiteral";
            break;
        case ASTNodeType::BOOLEAN_LITERAL:
            type_name = "BooleanLiteral";
            break;
        default:
            type_name = "Literal";
            break;
    }
    
    result += type_name + ": " + value_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// IdentifierNode 实现
IdentifierNode::IdentifierNode(const std::string& name)
    : ASTNode(ASTNodeType::IDENTIFIER), name_(name) {
}

std::string IdentifierNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "Identifier: " + name_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// OnEventNode 实现
OnEventNode::OnEventNode(ASTNodeType event_type, const std::string& event_name)
    : ASTNode(event_type), event_name_(event_name) {
}

std::string OnEventNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    
    std::string event_type_name;
    switch (type_) {
        case ASTNodeType::ON_MESSAGE:
            event_type_name = "OnMessage";
            break;
        case ASTNodeType::ON_TIMER:
            event_type_name = "OnTimer";
            break;
        case ASTNodeType::ON_KEY:
            event_type_name = "OnKey";
            break;
        case ASTNodeType::ON_START:
            event_type_name = "OnStart";
            break;
        case ASTNodeType::ON_STOP:
            event_type_name = "OnStop";
            break;
        default:
            event_type_name = "OnEvent";
            break;
    }
    
    result += event_type_name + ": " + event_name_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

// CallExprNode 实现
CallExprNode::CallExprNode(const std::string& function_name)
    : ASTNode(ASTNodeType::CALL_EXPR), function_name_(function_name) {
}

std::string CallExprNode::toString(int indent) const {
    std::string result;
    for (int i = 0; i < indent; ++i) {
        result += "  ";
    }
    result += "CallExpr: " + function_name_ + "\n";
    
    for (const auto& child : children_) {
        result += child->toString(indent + 1);
    }
    
    return result;
}

} // namespace capl