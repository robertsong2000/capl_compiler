#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

namespace capl {

/**
 * 符号类型枚举
 */
enum class SymbolType {
    VARIABLE,
    FUNCTION,
    PARAMETER,
    UNKNOWN
};

/**
 * 符号信息结构
 */
struct Symbol {
    std::string name;
    SymbolType type;
    std::string data_type;  // 数据类型 (int, float, string, etc.)
    int line;               // 定义行号
    int column;             // 定义列号
    
    // 默认构造函数
    Symbol() : type(SymbolType::UNKNOWN), line(0), column(0) {}
    
    Symbol(const std::string& n, SymbolType t, const std::string& dt = "", int l = 0, int c = 0)
        : name(n), type(t), data_type(dt), line(l), column(c) {}
};

/**
 * 符号表类
 * 管理程序中的符号信息
 */
class SymbolTable {
public:
    /**
     * 构造函数
     */
    SymbolTable();
    
    /**
     * 析构函数
     */
    ~SymbolTable();
    
    /**
     * 添加符号
     * @param symbol 符号信息
     * @return 添加是否成功
     */
    bool addSymbol(const Symbol& symbol);
    
    /**
     * 查找符号
     * @param name 符号名称
     * @return 符号指针，未找到返回 nullptr
     */
    const Symbol* findSymbol(const std::string& name) const;
    
    /**
     * 检查符号是否存在
     * @param name 符号名称
     * @return 是否存在
     */
    bool hasSymbol(const std::string& name) const;
    
    /**
     * 获取所有符号
     * @return 符号列表
     */
    std::vector<Symbol> getAllSymbols() const;
    
    /**
     * 清空符号表
     */
    void clear();
    
    /**
     * 获取符号数量
     * @return 符号数量
     */
    size_t size() const;

private:
    std::unordered_map<std::string, Symbol> symbols_;
};

} // namespace capl

#endif // SYMBOL_TABLE_H