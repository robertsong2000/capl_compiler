/**
 * CAPL 编译器主程序入口
 * 
 * 提供命令行接口来编译 CAPL 源文件
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#ifdef __APPLE__
    #include <getopt.h>
#else
    #include <unistd.h>
    #include <getopt.h>
#endif
#include "../include/capl_compiler.h"

using namespace capl;

/**
 * 显示帮助信息
 * @param program_name 程序名称
 */
void showHelp(const char* program_name) {
    std::cout << "CAPL 编译器 v1.0.0\n";
    std::cout << "用法: " << program_name << " [选项] <输入文件>\n\n";
    std::cout << "选项:\n";
    std::cout << "  -h, --help              显示此帮助信息\n";
    std::cout << "  -v, --version           显示版本信息\n";
    std::cout << "  -o, --output <文件>     指定输出文件\n";
    std::cout << "  -I, --include <目录>    添加包含目录\n";
    std::cout << "  -D, --define <宏>       定义预处理宏\n";
    std::cout << "  -O, --optimize <级别>   设置优化级别 (0-3)\n";
    std::cout << "  -g, --debug             生成调试信息\n";
    std::cout << "  -w, --warnings          显示警告 (默认)\n";
    std::cout << "  -W, --no-warnings       不显示警告\n";
    std::cout << "  -E, --preprocess-only   仅进行预处理\n";
    std::cout << "  -S, --syntax-only       仅进行语法检查\n";
    std::cout << "      --ast-dump          输出抽象语法树\n";
    std::cout << "      --tokens-dump       输出词法分析结果\n";
    std::cout << "\n";
    std::cout << "示例:\n";
    std::cout << "  " << program_name << " test.can\n";
    std::cout << "  " << program_name << " -o output.cbf input.can\n";
    std::cout << "  " << program_name << " -S input.can  # 仅语法检查\n";
}

/**
 * 显示版本信息
 */
void showVersion() {
    std::cout << "CAPL 编译器 v1.0.0\n";
    std::cout << "开源 CAPL (CAN Access Programming Language) 编译器\n";
    std::cout << "构建日期: " << __DATE__ << " " << __TIME__ << "\n";
}

/**
 * 编译器选项结构
 */
struct CompilerOptions {
    std::string input_file;                 // 输入文件
    std::string output_file;                // 输出文件
    std::vector<std::string> include_dirs;  // 包含目录
    std::vector<std::string> defines;       // 预处理宏定义
    int optimize_level = 0;                 // 优化级别
    bool debug = false;                     // 生成调试信息
    bool show_warnings = true;              // 显示警告
    bool preprocess_only = false;           // 仅预处理
    bool syntax_only = false;               // 仅语法检查
    bool dump_ast = false;                  // 输出 AST
    bool dump_tokens = false;               // 输出 Token
};

/**
 * 获取文件名（不含扩展名）
 */
std::string getBaseName(const std::string& filepath) {
    size_t lastSlash = filepath.find_last_of("/\\");
    size_t lastDot = filepath.find_last_of('.');
    
    std::string filename = (lastSlash == std::string::npos) ? filepath : filepath.substr(lastSlash + 1);
    
    if (lastDot != std::string::npos && lastDot > lastSlash) {
        return filename.substr(0, lastDot - (lastSlash == std::string::npos ? 0 : lastSlash + 1));
    }
    
    return filename;
}

/**
 * 检查文件是否存在
 */
bool fileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

/**
 * 解析命令行参数
 * @param argc 参数数量
 * @param argv 参数数组
 * @param options 输出的选项结构
 * @return 解析是否成功
 */
bool parseCommandLine(int argc, char* argv[], CompilerOptions& options) {
    static struct option long_options[] = {
        {"help",            no_argument,       0, 'h'},
        {"version",         no_argument,       0, 'v'},
        {"output",          required_argument, 0, 'o'},
        {"include",         required_argument, 0, 'I'},
        {"define",          required_argument, 0, 'D'},
        {"optimize",        required_argument, 0, 'O'},
        {"debug",           no_argument,       0, 'g'},
        {"warnings",        no_argument,       0, 'w'},
        {"no-warnings",     no_argument,       0, 'W'},
        {"preprocess-only", no_argument,       0, 'E'},
        {"syntax-only",     no_argument,       0, 'S'},
        {"ast-dump",        no_argument,       0, 1000},
        {"tokens-dump",     no_argument,       0, 1001},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "hvo:I:D:O:gwWES", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                showHelp(argv[0]);
                return false;
                
            case 'v':
                showVersion();
                return false;
                
            case 'o':
                options.output_file = optarg;
                break;
                
            case 'I':
                options.include_dirs.push_back(optarg);
                break;
                
            case 'D':
                options.defines.push_back(optarg);
                break;
                
            case 'O':
                options.optimize_level = std::stoi(optarg);
                if (options.optimize_level < 0 || options.optimize_level > 3) {
                    std::cerr << "错误: 优化级别必须在 0-3 之间\n";
                    return false;
                }
                break;
                
            case 'g':
                options.debug = true;
                break;
                
            case 'w':
                options.show_warnings = true;
                break;
                
            case 'W':
                options.show_warnings = false;
                break;
                
            case 'E':
                options.preprocess_only = true;
                break;
                
            case 'S':
                options.syntax_only = true;
                break;
                
            case 1000:  // --ast-dump
                options.dump_ast = true;
                break;
                
            case 1001:  // --tokens-dump
                options.dump_tokens = true;
                break;
                
            case '?':
                return false;
                
            default:
                std::cerr << "未知选项: " << c << "\n";
                return false;
        }
    }
    
    // 检查是否提供了输入文件
    if (optind >= argc) {
        std::cerr << "错误: 未指定输入文件\n";
        std::cerr << "使用 '" << argv[0] << " --help' 查看帮助信息\n";
        return false;
    }
    
    options.input_file = argv[optind];
    
    // 如果没有指定输出文件，自动生成
    if (options.output_file.empty()) {
        std::string basename = getBaseName(options.input_file);
        if (options.preprocess_only) {
            options.output_file = basename + ".i";
        } else if (options.syntax_only) {
            // 语法检查不需要输出文件
        } else if (options.dump_ast) {
            options.output_file = basename + "_ast.txt";
        } else if (options.dump_tokens) {
            options.output_file = basename + "_tokens.txt";
        } else {
            options.output_file = basename + ".cbf";
        }
    }
    
    return true;
}

/**
 * 主函数
 * @param argc 参数数量
 * @param argv 参数数组
 * @return 程序退出码
 */
int main(int argc, char* argv[]) {
    CompilerOptions options;
    
    // 解析命令行参数
    if (!parseCommandLine(argc, argv, options)) {
        return 1;
    }
    
    // 检查输入文件是否存在
    if (!fileExists(options.input_file)) {
        std::cerr << "错误: 输入文件不存在: " << options.input_file << "\n";
        return 1;
    }
    
    // 创建编译器实例
    CAPLCompiler compiler;
    
    try {
        std::cout << "正在编译: " << options.input_file << "\n";
        
        // 执行编译
        bool success = false;
        if (options.dump_tokens) {
            // 仅输出词法分析结果
            std::cout << "进行词法分析...\n";
            
            // 读取源文件
            std::ifstream file(options.input_file);
            if (!file.is_open()) {
                std::cerr << "错误: 无法打开文件: " << options.input_file << "\n";
                return 1;
            }
            
            std::string source_code((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
            file.close();
            
            // 创建词法分析器
            capl::Lexer lexer(source_code);
            
            std::cout << "Token 序列:\n";
            std::cout << "行号\t列号\t类型\t\t值\n";
            std::cout << "----\t----\t----\t\t----\n";
            
            // 输出所有 tokens
             while (true) {
                 capl::Token token = lexer.nextToken();
                 std::cout << token.getLine() << "\t" << token.getColumn() << "\t";
                 
                 // 输出 token 类型名称
                 switch (token.getType()) {
                     case capl::TokenType::VARIABLES: std::cout << "VARIABLES"; break;
                     case capl::TokenType::ON: std::cout << "ON"; break;
                     case capl::TokenType::START: std::cout << "START"; break;
                     case capl::TokenType::MESSAGE: std::cout << "MESSAGE"; break;
                     case capl::TokenType::TIMER: std::cout << "TIMER"; break;
                     case capl::TokenType::KEY: std::cout << "KEY"; break;
                     case capl::TokenType::IF: std::cout << "IF"; break;
                     case capl::TokenType::ELSE: std::cout << "ELSE"; break;
                     case capl::TokenType::WHILE: std::cout << "WHILE"; break;
                     case capl::TokenType::FOR: std::cout << "FOR"; break;
                     case capl::TokenType::INT: std::cout << "INT"; break;
                     case capl::TokenType::FLOAT_KW: std::cout << "FLOAT_KW"; break;
                     case capl::TokenType::CHAR_KW: std::cout << "CHAR_KW"; break;
                     case capl::TokenType::IDENTIFIER: std::cout << "IDENTIFIER"; break;
                     case capl::TokenType::INTEGER: std::cout << "INTEGER"; break;
                     case capl::TokenType::FLOAT: std::cout << "FLOAT"; break;
                     case capl::TokenType::STRING: std::cout << "STRING"; break;
                     case capl::TokenType::CHAR: std::cout << "CHAR"; break;
                     case capl::TokenType::LEFT_BRACE: std::cout << "LEFT_BRACE"; break;
                     case capl::TokenType::RIGHT_BRACE: std::cout << "RIGHT_BRACE"; break;
                     case capl::TokenType::LEFT_PAREN: std::cout << "LEFT_PAREN"; break;
                     case capl::TokenType::RIGHT_PAREN: std::cout << "RIGHT_PAREN"; break;
                     case capl::TokenType::SEMICOLON: std::cout << "SEMICOLON"; break;
                     case capl::TokenType::ASSIGN: std::cout << "ASSIGN"; break;
                     case capl::TokenType::PLUS: std::cout << "PLUS"; break;
                     case capl::TokenType::MINUS: std::cout << "MINUS"; break;
                     case capl::TokenType::MULTIPLY: std::cout << "MULTIPLY"; break;
                     case capl::TokenType::DIVIDE: std::cout << "DIVIDE"; break;
                     case capl::TokenType::EQUAL: std::cout << "EQUAL"; break;
                     case capl::TokenType::NOT_EQUAL: std::cout << "NOT_EQUAL"; break;
                     case capl::TokenType::LESS: std::cout << "LESS"; break;
                     case capl::TokenType::GREATER: std::cout << "GREATER"; break;
                     case capl::TokenType::EOF_TOKEN: std::cout << "EOF_TOKEN"; break;
                     default: std::cout << "UNKNOWN"; break;
                 }
                 
                 std::cout << "\t\t\"" << token.getValue() << "\"\n";
                 
                 if (token.getType() == capl::TokenType::EOF_TOKEN) {
                     break;
                 }
             }
            
            success = true;
        } else if (options.syntax_only) {
            // 仅进行语法检查
            std::cout << "进行语法检查...\n";
            success = compiler.syntaxCheck(options.input_file);
        } else {
            success = compiler.compile(options.input_file, options.output_file);
        }
        
        // 显示警告信息
        if (options.show_warnings) {
            const auto& warnings = compiler.getWarnings();
            for (const auto& warning : warnings) {
                std::cout << "警告: " << warning << "\n";
            }
        }
        
        // 显示错误信息
        const auto& errors = compiler.getErrors();
        for (const auto& error : errors) {
            std::cerr << "错误: " << error << "\n";
        }
        
        if (success) {
            if (options.syntax_only) {
                std::cout << "语法检查通过\n";
            } else {
                std::cout << "编译成功: " << options.output_file << "\n";
            }
            return 0;
        } else {
            std::cerr << "编译失败\n";
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "编译器内部错误: " << e.what() << "\n";
        return 1;
    }
}