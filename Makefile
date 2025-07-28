# CAPL 编译器 Makefile

# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -Iinclude
LDFLAGS = -ldl

# 目录设置
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# 源文件
SOURCES = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJ = $(BUILD_DIR)/main.o

# 目标文件
TARGET = $(BIN_DIR)/capl_compiler

# 默认目标
all: directories $(TARGET)

# 创建必要的目录
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# 链接目标文件
$(TARGET): $(OBJECTS) $(MAIN_OBJ)
	@echo "链接 $@"
	@$(CXX) $(OBJECTS) $(MAIN_OBJ) -o $@ $(LDFLAGS)
	@echo "构建完成: $@"

# 编译源文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "编译 $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 编译主程序
$(MAIN_OBJ): $(SRC_DIR)/main.cpp
	@echo "编译 $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 清理构建文件
clean:
	@echo "清理构建文件..."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
	@rm -f example.capl
	@rm -f *.cpp.bak
	@echo "清理完成"

# 运行测试
test: $(TARGET)
	@echo "=========================================="
	@echo "           CAPL 编译器测试套件"
	@echo "=========================================="
	@echo ""
	
	@echo "1. 基本功能测试"
	@echo "----------------------------------------"
	@echo "测试帮助信息..."
	@./$(TARGET) --help > /dev/null 2>&1; if [ $$? -eq 1 ]; then echo "✓ 帮助信息显示正常"; else echo "✗ 帮助信息显示异常"; fi
	
	@echo "测试版本信息..."
	@./$(TARGET) --version > /dev/null 2>&1; if [ $$? -eq 1 ]; then echo "✓ 版本信息显示正常"; else echo "✗ 版本信息显示异常"; fi
	@echo ""
	
	@echo "2. 语法检查测试"
	@echo "----------------------------------------"
	@echo "测试 test.can 语法检查..."
	@./$(TARGET) -S ./examples/test.can > /dev/null 2>&1 && echo "✓ test.can 语法检查通过" || echo "✗ test.can 语法检查失败"
	
	@echo "测试 example.capl 语法检查..."
	@./$(TARGET) -S ./examples/example.capl > /dev/null 2>&1 && echo "✓ example.capl 语法检查通过" || echo "✗ example.capl 语法检查失败"
	
	@echo "测试 error_test.capl 语法检查..."
	@./$(TARGET) -S ./examples/error_test.capl > /dev/null 2>&1 && echo "✓ error_test.capl 语法检查通过" || echo "✗ error_test.capl 语法检查失败"
	
	@echo "测试 complex_test.capl 语法检查..."
	@./$(TARGET) -S ./examples/complex_test.capl > /dev/null 2>&1 && echo "✓ complex_test.capl 语法检查通过" || echo "✗ complex_test.capl 语法检查失败"
	
	@echo "测试 performance_test.capl 语法检查..."
	@./$(TARGET) -S ./examples/performance_test.capl > /dev/null 2>&1 && echo "✓ performance_test.capl 语法检查通过" || echo "✗ performance_test.capl 语法检查失败"
	
	@echo "测试语法错误文件（应该失败）..."
	@./$(TARGET) -S ./examples/syntax_error_test.capl > /dev/null 2>&1 && echo "✗ syntax_error_test.capl 应该报告语法错误" || echo "✓ syntax_error_test.capl 正确报告语法错误"
	@echo ""
	
	@echo "3. 编译测试"
	@echo "----------------------------------------"
	@echo "测试 test.can 编译..."
	@./$(TARGET) ./examples/test.can -o test_output.cbf > /dev/null 2>&1 && echo "✓ test.can 编译成功" || echo "✗ test.can 编译失败"
	@test -f test_output.cbf && echo "✓ 输出文件 test_output.cbf 生成成功" || echo "✗ 输出文件 test_output.cbf 生成失败"
	
	@echo "测试 example.capl 编译..."
	@./$(TARGET) ./examples/example.capl -o example_output.cbf > /dev/null 2>&1 && echo "✓ example.capl 编译成功" || echo "✗ example.capl 编译失败"
	@test -f example_output.cbf && echo "✓ 输出文件 example_output.cbf 生成成功" || echo "✗ 输出文件 example_output.cbf 生成失败"
	
	@echo "测试 complex_test.capl 编译..."
	@./$(TARGET) ./examples/complex_test.capl -o complex_output.cbf > /dev/null 2>&1 && echo "✓ complex_test.capl 编译成功" || echo "✗ complex_test.capl 编译失败"
	@test -f complex_output.cbf && echo "✓ 输出文件 complex_output.cbf 生成成功" || echo "✗ 输出文件 complex_output.cbf 生成失败"
	@echo ""
	
	@echo "4. 高级选项测试"
	@echo "----------------------------------------"
	@echo "测试 AST 输出..."
	@./$(TARGET) --ast ./examples/test.can > test_ast.txt 2>&1 && echo "✓ AST 输出功能正常" || echo "✗ AST 输出功能异常"
	
	@echo "测试词法分析输出..."
	@./$(TARGET) --tokens ./examples/test.can > test_tokens.txt 2>&1 && echo "✓ 词法分析输出功能正常" || echo "✗ 词法分析输出功能异常"
	@echo ""
	
	@echo "5. 错误处理测试"
	@echo "----------------------------------------"
	@echo "测试不存在的文件..."
	@./$(TARGET) ./examples/nonexistent.capl > /dev/null 2>&1 && echo "✗ 应该报告文件不存在错误" || echo "✓ 正确处理文件不存在错误"
	
	@echo "测试无效选项..."
	@./$(TARGET) --invalid-option > /dev/null 2>&1 && echo "✗ 应该报告无效选项错误" || echo "✓ 正确处理无效选项错误"
	@echo ""
	
	@echo "6. 清理测试文件"
	@echo "----------------------------------------"
	@rm -f test_output.cbf example_output.cbf complex_output.cbf
	@rm -f test_ast.txt test_tokens.txt
	@echo "✓ 测试文件清理完成"
	@echo ""
	
# 自动化测试脚本
test-auto: $(TARGET)
	@echo "运行自动化测试脚本..."
	@./test_runner.sh
	
# 运行详细测试（显示完整输出）
test-verbose: $(TARGET)
	@echo "=========================================="
	@echo "        CAPL 编译器详细测试套件"
	@echo "=========================================="
	@echo ""
	
	@echo "1. 基本功能测试"
	@echo "----------------------------------------"
	@echo "测试帮助信息..."
	-@./$(TARGET) --help
	@echo ""
	@echo "测试版本信息..."
	-@./$(TARGET) --version
	@echo ""
	
	@echo "2. 语法检查测试"
	@echo "----------------------------------------"
	@echo "测试 test.can 语法检查..."
	@./$(TARGET) -S ./examples/test.can
	@echo ""
	@echo "测试 example.capl 语法检查..."
	@./$(TARGET) -S ./examples/example.capl
	@echo ""
	@echo "测试 error_test.capl 语法检查..."
	@./$(TARGET) -S ./examples/error_test.capl
	@echo ""
	@echo "测试 complex_test.capl 语法检查..."
	@./$(TARGET) -S ./examples/complex_test.capl
	@echo ""
	@echo "测试 performance_test.capl 语法检查..."
	@./$(TARGET) -S ./examples/performance_test.capl
	@echo ""
	@echo "测试语法错误文件（应该失败）..."
	-@./$(TARGET) -S ./examples/syntax_error_test.capl
	@echo ""
	
	@echo "3. 编译测试"
	@echo "----------------------------------------"
	@echo "测试 test.can 编译..."
	@./$(TARGET) ./examples/test.can -o test_output_verbose.cbf
	@echo ""
	@echo "测试 example.capl 编译..."
	@./$(TARGET) ./examples/example.capl -o example_output_verbose.cbf
	@echo ""
	@echo "测试 complex_test.capl 编译..."
	@./$(TARGET) ./examples/complex_test.capl -o complex_output_verbose.cbf
	@echo ""
	
	@echo "4. 高级选项测试"
	@echo "----------------------------------------"
	@echo "测试 AST 输出..."
	@./$(TARGET) --ast-dump ./examples/test.can
	@echo ""
	@echo "测试词法分析输出..."
	@./$(TARGET) --tokens-dump ./examples/test.can
	@echo ""
	
	@echo "5. 清理测试文件"
	@echo "----------------------------------------"
	@rm -f test_output_verbose.cbf example_output_verbose.cbf complex_output_verbose.cbf
	@rm -f test_ast.txt test_tokens.txt
	@echo "✓ 测试文件清理完成"
	@echo ""
	
	@echo "=========================================="
	@echo "           详细测试完成"
	@echo "=========================================="

demo: $(TARGET)
	@echo "创建并测试示例程序..."
	@echo "y" | ./$(TARGET)
	@./$(TARGET) -v example.capl
	@echo "示例程序测试完成"

# 安装（可选）
install: $(TARGET)
	@echo "安装 CAPL 编译器..."
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "安装完成"

# 卸载（可选）
uninstall:
	@echo "卸载 CAPL 编译器..."
	@sudo rm -f /usr/local/bin/capl_compiler
	@echo "卸载完成"

# 显示帮助信息
help:
	@echo "CAPL 编译器构建系统"
	@echo "===================="
	@echo ""
	@echo "可用目标:"
	@echo "  all          - 构建编译器（默认目标）"
	@echo "  clean        - 清理构建文件"
	@echo "  test         - 运行基本测试（简洁输出）"
	@echo "  test-verbose - 运行详细测试（完整输出）"
	@echo "  test-auto    - 运行自动化测试脚本（带颜色和统计）"
	@echo "  debug        - 构建调试版本"
	@echo "  demo         - 运行演示"
	@echo "  install      - 安装到系统"
	@echo "  uninstall    - 从系统卸载"
	@echo "  help         - 显示此帮助信息"
	@echo ""
	@echo "示例用法:"
	@echo "  make              # 构建编译器"
	@echo "  make test         # 运行基本测试"
	@echo "  make test-verbose # 运行详细测试"
	@echo "  make test-auto    # 运行自动化测试"
	@echo "  make clean        # 清理构建文件"
	@echo ""

# 调试信息
debug:
	@echo "源文件: $(SOURCES)"
	@echo "目标文件: $(OBJECTS)"
	@echo "主目标文件: $(MAIN_OBJ)"
	@echo "最终目标: $(TARGET)"

# 声明伪目标
.PHONY: all clean test test-verbose test-auto demo install uninstall help debug directories

# 依赖关系
$(OBJECTS): $(wildcard $(INCLUDE_DIR)/*.h)
$(MAIN_OBJ): $(wildcard $(INCLUDE_DIR)/*.h)