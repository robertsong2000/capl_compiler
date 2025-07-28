#!/bin/bash

# CAPL 编译器自动化测试脚本
# 用于持续集成和自动化测试

echo "=========================================="
echo "        CAPL 编译器自动化测试"
echo "=========================================="

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 测试计数器
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# 测试函数
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="${3:-0}"
    
    echo -n "测试: $test_name ... "
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if eval "$command" > /dev/null 2>&1; then
        actual_exit_code=0
    else
        actual_exit_code=$?
    fi
    
    if [ $actual_exit_code -eq $expected_exit_code ]; then
        echo -e "${GREEN}通过${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}失败${NC} (期望退出码: $expected_exit_code, 实际: $actual_exit_code)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# 检查编译器是否存在
if [ ! -f "./bin/capl_compiler" ]; then
    echo -e "${RED}错误: 编译器可执行文件不存在，请先运行 make${NC}"
    exit 1
fi

echo "1. 基本功能测试"
echo "----------------------------------------"
run_test "帮助信息" "./bin/capl_compiler --help" 1
run_test "版本信息" "./bin/capl_compiler --version" 1

echo ""
echo "2. 语法检查测试"
echo "----------------------------------------"
run_test "test.can 语法检查" "./bin/capl_compiler -S ./examples/test.can" 0
run_test "example.capl 语法检查" "./bin/capl_compiler -S ./examples/example.capl" 0
run_test "error_test.capl 语法检查" "./bin/capl_compiler -S ./examples/error_test.capl" 0
run_test "complex_test.capl 语法检查" "./bin/capl_compiler -S ./examples/complex_test.capl" 0
run_test "performance_test.capl 语法检查" "./bin/capl_compiler -S ./examples/performance_test.capl" 0

echo ""
echo "3. 编译测试"
echo "----------------------------------------"
run_test "test.can 编译" "./bin/capl_compiler ./examples/test.can -o test_auto.cbf" 0
run_test "example.capl 编译" "./bin/capl_compiler ./examples/example.capl -o example_auto.cbf" 0
run_test "complex_test.capl 编译" "./bin/capl_compiler ./examples/complex_test.capl -o complex_auto.cbf" 0

echo ""
echo "4. 文件生成验证"
echo "----------------------------------------"
run_test "test_auto.cbf 文件生成" "test -f test_auto.cbf" 0
run_test "example_auto.cbf 文件生成" "test -f example_auto.cbf" 0
run_test "complex_auto.cbf 文件生成" "test -f complex_auto.cbf" 0

echo ""
echo "5. 高级选项测试"
echo "----------------------------------------"
run_test "AST 输出" "./bin/capl_compiler --ast ./examples/test.can > test_auto_ast.txt" 0
run_test "词法分析输出" "./bin/capl_compiler --tokens ./examples/test.can > test_auto_tokens.txt" 0

echo ""
echo "6. 错误处理测试"
echo "----------------------------------------"
run_test "不存在的文件" "./bin/capl_compiler ./examples/nonexistent.capl" 1
run_test "无效选项" "./bin/capl_compiler --invalid-option" 1

echo ""
echo "7. 性能测试"
echo "----------------------------------------"
start_time=$(date +%s.%N)
run_test "大文件编译性能" "./bin/capl_compiler ./examples/performance_test.capl -o perf_auto.cbf" 0
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "编译时间: ${duration}s"

echo ""
echo "8. 清理测试文件"
echo "----------------------------------------"
rm -f test_auto.cbf example_auto.cbf complex_auto.cbf perf_auto.cbf
rm -f test_auto_ast.txt test_auto_tokens.txt
echo "✓ 测试文件清理完成"

echo ""
echo "=========================================="
echo "           测试结果汇总"
echo "=========================================="
echo "总测试数: $TOTAL_TESTS"
echo -e "通过: ${GREEN}$PASSED_TESTS${NC}"
echo -e "失败: ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}所有测试通过！${NC}"
    exit 0
else
    echo -e "${RED}有 $FAILED_TESTS 个测试失败${NC}"
    exit 1
fi