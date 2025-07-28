# CAPL 编译器

一个用 C++ 实现的开源 CAPL (CAN Access Programming Language) 编译器，用于汽车电子和 CAN 总线应用开发。

## 项目概述

CAPL 是 Vector 公司开发的专门用于 CAN 网络编程的脚本语言。本项目实现了一个功能完整的 CAPL 编译器，包含以下核心组件：

- **词法分析器 (Lexer)**: 将 CAPL 源代码转换为 Token 流
- **语法分析器 (Parser)**: 将 Token 流转换为抽象语法树 (AST)
- **语义分析器 (Semantic Analyzer)**: 进行类型检查和语义验证
- **代码生成器 (Code Generator)**: 将 AST 转换为 C++ 代码
- **运行时系统 (Runtime)**: 提供 CAPL 程序运行时环境
- **符号表管理**: 管理变量、函数和消息符号

## 项目特性

### ✅ 已实现功能
- 完整的编译器工具链
- 支持基本 CAPL 语法解析
- 抽象语法树生成和遍历
- 符号表管理系统
- C++ 代码生成
- 命令行工具接口
- 多种输出格式支持

### 🎯 支持的 CAPL 语言特性
- 变量声明和初始化
- 消息定义和处理
- 事件处理 (on start, on message, on timer, on key, on stop)
- 函数定义和调用
- 基本数据类型 (int, float, string)
- 控制流语句 (if, for, while)
- 表达式计算

## 编译器架构

```
CAPL 源码 (.capl/.can) 
    ↓
词法分析器 (Lexer)
    ↓
语法分析器 (Parser)
    ↓
语义分析器 (Semantic Analyzer)
    ↓
代码生成器 (Code Generator)
    ↓
C++ 代码输出 (.cbf)
```

## 项目结构

```
capl_compiler/
├── include/              # 头文件
│   ├── ast.h            # 抽象语法树定义
│   ├── capl_compiler.h  # 编译器主类
│   ├── symbol_table.h   # 符号表管理
│   └── token.h          # Token 定义
├── src/                 # 源代码文件
│   ├── ast.cpp          # AST 实现
│   ├── capl_compiler.cpp # 编译器实现
│   ├── capl_runtime.cpp # 运行时支持
│   ├── code_generator.cpp # 代码生成器
│   ├── lexer.cpp        # 词法分析器
│   ├── main.cpp         # 主程序入口
│   ├── parser.cpp       # 语法分析器
│   ├── semantic_analyzer.cpp # 语义分析器
│   ├── symbol_table.cpp # 符号表实现
│   └── token.cpp        # Token 实现
├── examples/            # 示例和测试文件
│   ├── README.md        # 示例说明
│   ├── test.can         # 基础测试程序
│   ├── example.capl     # 完整示例程序
│   └── error_test.capl  # 错误测试文件
├── bin/                 # 可执行文件
├── build/               # 构建文件
├── Makefile            # Make 构建配置
└── README.md           # 项目文档
```

## 构建说明

### 系统要求
- C++17 或更高版本的编译器 (GCC 7+, Clang 5+, MSVC 2017+)
- Make 构建工具
- 支持的操作系统: Linux, macOS, Windows (使用 MinGW 或 MSYS2)

### 构建步骤
```bash
# 克隆项目
git clone <repository-url>
cd capl_compiler

# 编译项目
make

# 清理构建文件
make clean

# 运行测试
make test

# 运行演示
make demo
```

### 其他 Make 目标
```bash
# 显示帮助信息
make help

# 显示调试信息
make debug

# 安装到系统 (需要 sudo)
make install

# 从系统卸载 (需要 sudo)
make uninstall
```

## 使用说明

### 基本用法
```bash
# 编译 CAPL 文件
./bin/capl_compiler input.capl

# 指定输出文件
./bin/capl_compiler -o output.cbf input.capl

# 仅进行语法检查
./bin/capl_compiler -S input.capl

# 显示帮助信息
./bin/capl_compiler --help

# 显示版本信息
./bin/capl_compiler --version
```

### 高级选项
```bash
# 输出抽象语法树
./bin/capl_compiler --ast-dump input.capl

# 输出词法分析结果
./bin/capl_compiler --tokens-dump input.capl

# 启用调试信息
./bin/capl_compiler -g input.capl

# 设置优化级别
./bin/capl_compiler -O2 input.capl

# 添加包含目录
./bin/capl_compiler -I./include input.capl
```

### 示例程序
项目提供了多个示例程序，位于 `examples/` 目录：

```bash
# 编译基础示例
./bin/capl_compiler examples/test.can

# 编译完整示例
./bin/capl_compiler examples/example.capl

# 测试错误处理
./bin/capl_compiler examples/error_test.capl
```

## 支持的 CAPL 语法示例

### 变量声明
```capl
variables {
    int counter = 0;
    float temperature = 25.5;
    message 0x123 TestMessage;
}
```

### 事件处理
```capl
on start {
    write("程序启动");
}

on message 0x123 {
    write("收到消息: ID = 0x%X", this.id);
}

on timer myTimer {
    write("定时器触发");
}

on key 'a' {
    write("按键 'a' 被按下");
}
```

### 函数定义
```capl
void sendMessage(int id, int data) {
    TestMessage.id = id;
    TestMessage.byte(0) = data;
    output(TestMessage);
}
```

## 开发说明

### 添加新功能
1. 在相应的头文件中声明接口
2. 在对应的源文件中实现功能
3. 添加单元测试
4. 更新文档

### 代码风格
- 使用 4 个空格缩进
- 函数和变量使用 camelCase 命名
- 类名使用 PascalCase 命名
- 常量使用 UPPER_CASE 命名

### 调试技巧
```bash
# 使用调试模式编译
make DEBUG=1

# 查看详细输出
./bin/capl_compiler -v input.capl

# 使用 GDB 调试
gdb ./bin/capl_compiler
```

## 限制和已知问题

### 当前限制
- 部分高级 CAPL 特性尚未实现
- 错误报告可能不够详细
- 性能优化有待改进

### 已知问题
- 复杂表达式解析可能存在问题
- 某些边界情况处理不完善
- 内存管理需要进一步优化

## 技术挑战

1. **语法复杂性**: CAPL 语法相对复杂，需要仔细分析
2. **运行时环境**: 需要实现事件驱动的运行时环境
3. **硬件接口**: 需要支持多种 CAN 硬件接口
4. **兼容性**: 确保与现有 CAPL 代码的兼容性

## 贡献指南

欢迎社区贡献！请参考以下方式参与：

1. **代码贡献**: 提交 Pull Request
2. **问题报告**: 在 Issues 中报告 bug
3. **功能建议**: 提出新功能需求
4. **文档改进**: 完善项目文档

## 许可证

本项目采用 MIT 许可证，确保开源社区可以自由使用和修改。

## 联系方式

- 项目维护者: [您的姓名]
- 邮箱: [您的邮箱]
- 讨论群组: [创建讨论群组]

---

**注意**: 本项目仍在规划阶段，欢迎有兴趣的开发者加入讨论和贡献。