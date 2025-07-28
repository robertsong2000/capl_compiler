#include "../include/symbol_table.h"

namespace capl {

SymbolTable::SymbolTable() {
    // 构造函数实现
}

SymbolTable::~SymbolTable() {
    // 析构函数实现
}

bool SymbolTable::addSymbol(const Symbol& symbol) {
    // 检查符号是否已存在
    if (symbols_.find(symbol.name) != symbols_.end()) {
        return false; // 符号已存在
    }
    
    // 添加符号
    symbols_[symbol.name] = symbol;
    return true;
}

const Symbol* SymbolTable::findSymbol(const std::string& name) const {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool SymbolTable::hasSymbol(const std::string& name) const {
    return symbols_.find(name) != symbols_.end();
}

std::vector<Symbol> SymbolTable::getAllSymbols() const {
    std::vector<Symbol> result;
    result.reserve(symbols_.size());
    
    for (const auto& pair : symbols_) {
        result.push_back(pair.second);
    }
    
    return result;
}

void SymbolTable::clear() {
    symbols_.clear();
}

size_t SymbolTable::size() const {
    return symbols_.size();
}

} // namespace capl