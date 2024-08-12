#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define HEAP_START 16 // 堆地址起始位置

// 预先分配空间避免 malloc 带来的麻烦
#define MAX_FILENAME_LENGTH 256 // 文件名最大长度
#define MAX_COMMAND_LENGTH 256  // 指令最大长度
#define MAX_SYMBOL_LENGTH 256   // 符号最大长度

// 指令类型
typedef enum
{
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
    INVALID_COMMAND
} CommandType;

// 解析器结构体
typedef struct
{
    FILE *file;                              // 文件指针
    char filename[MAX_FILENAME_LENGTH];      // 文件名
    char currentCommand[MAX_COMMAND_LENGTH]; // 当前指令
    CommandType commandType;                 // 当前指令类型
    int heapAddress;                         // 堆地址
    int lineNumber;                          // 当前行数
    int address;                             // 当前指令地址
    char symbol[MAX_SYMBOL_LENGTH];          // 当前指令中符号
    char dest[4];                            // 当前指令中 dest
    char comp[4];                            // 当前指令中 comp
    char jump[4];                            // 当前指令中 jump
} Parser;

// 构造函数
Parser *parserInit(const char *filename);

// 析构函数
void parserFree(Parser *parser);

// 重置解析器
void resetParser(Parser *parser);

// 判断是否还有下一条指令
int hasMoreCommands(Parser *parser);

// 读取下一条指令
int advance(Parser *parser);

// 设置当前指令类型
void setCommandType(Parser *parser);

#endif // PARSER_H