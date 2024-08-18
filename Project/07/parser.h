#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

#define MAX_COMMAND_LENGTH 256

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

typedef enum {
    SEG_LOCAL,
    SEG_ARGUMENT,
    SEG_THIS,
    SEG_THAT,
    SEG_CONSTANT,
    SEG_STATIC,
    SEG_TEMP,
    SEG_POINTER,
    INVALID_SEGMENT
} SegmentType;

typedef enum {
    ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT, INVALID_ARITHMETIC
} ArithmeticType;

typedef struct {
    CommandType type;
    SegmentType segment; 
    int offset;
    ArithmeticType arithmetic;
} Command;

typedef struct {
    FILE *filestream; 
    Command currentCommand;
} Parser;   

void parserInit(Parser *parser, const char *filename);

int advance(Parser *parser);

void setCommand(Parser *parser, char *command);

SegmentType parseSegmentType(const char *segment); 

ArithmeticType parseArithmeticType(const char *command);

const char *getCommandType(Parser *parser);

const char *getArg1(Parser *parser);

int getArg2(Parser *parser);

#endif // PARSE_H