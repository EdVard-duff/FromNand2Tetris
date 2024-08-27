#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define MAX_COMMAND_LENGTH 64

typedef enum {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    INVALID_COMMAND
} CommandType;

typedef struct {
    FILE *filestream; 
    char curCommand[MAX_COMMAND_LENGTH];
    CommandType type;
    char arg1[MAX_COMMAND_LENGTH];
    int arg2;
} Parser;   

// 构造函数
void parserInit(Parser *parser, const char *filename);

// 析构函数
void parserFree(Parser *parser);

/**
 * Advances the parser to the next command in the input file.
 *
 * @param parser A pointer to the Parser struct.
 * @return Returns -1 if the end of the file is reached, otherwise returns 0.
 */
int advance(Parser *parser);

/**
 * @brief 设置 Parser 的各个字段. 
 * @param parser A pointer to the Parser object.
 * @param commandStr The command to be set.
 */
void setCommand(Parser *parser, char *commandStr);

const char *getArg1(Parser *parser);

int getArg2(Parser *parser);

#endif // PARSE_H