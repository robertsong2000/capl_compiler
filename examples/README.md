# CAPL 编译器示例文件

本目录包含了用于测试和演示 CAPL 编译器功能的示例文件。

## 📁 文件说明

### `test.can`
- **描述**: 基础 CAPL 测试程序
- **功能**: 演示基本的变量声明、消息处理和事件响应
- **特性**:
  - 变量声明和初始化
  - CAN 消息处理 (引擎数据、车速数据)
  - 定时器使用
  - 按键事件处理
  - 数据解析和格式化输出

### `example.capl`
- **描述**: 完整的 CAPL 程序示例
- **功能**: 展示更复杂的 CAPL 语言特性
- **特性**:
  - 复杂的消息处理逻辑
  - 函数定义和调用
  - 条件判断和循环
  - 多种事件类型处理

### `error_test.capl`
- **描述**: 包含语法和语义错误的测试文件
- **用途**: 测试编译器的错误检测和报告功能
- **包含错误**:
  - 语法错误 (缺少分号、括号不匹配等)
  - 语义错误 (未定义函数调用等)

## 🚀 使用方法

### 编译示例文件
```bash
# 编译基础测试文件
./bin/capl_compiler examples/test.can

# 编译完整示例
./bin/capl_compiler examples/example.capl

# 测试错误处理
./bin/capl_compiler examples/error_test.capl
```

### 查看编译器输出
```bash
# 查看 AST 输出
./bin/capl_compiler --ast-dump examples/test.can

# 查看词法分析结果
./bin/capl_compiler --tokens-dump examples/test.can

# 仅进行语法检查
./bin/capl_compiler -S examples/test.can
```

### 指定输出文件
```bash
# 指定输出文件名
./bin/capl_compiler -o my_output.cbf examples/test.can
```

## 📝 添加新的示例

如果你想添加新的示例文件：

1. 在 `examples/` 目录下创建 `.capl` 或 `.can` 文件
2. 确保文件包含有效的 CAPL 语法
3. 添加适当的注释说明功能
4. 更新本 README 文件

## 🔍 示例文件特性对比

| 文件 | 复杂度 | 主要特性 | 适用场景 |
|------|--------|----------|----------|
| `test.can` | 简单 | 基础语法、消息处理 | 入门学习 |
| `example.capl` | 中等 | 完整功能、函数定义 | 功能演示 |
| `error_test.capl` | - | 错误示例 | 错误测试 |

## 💡 提示

- 所有示例文件都可以作为学习 CAPL 语言的参考
- 编译器会在当前目录生成对应的 `.cbf` 输出文件
- 使用 `--help` 参数查看编译器的完整选项列表