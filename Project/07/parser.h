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
 * @brief Sets the currentCommand field for the parser.
 *
 * This function sets the command for the given parser object.
 *
 * @param parser A pointer to the Parser object.
 * @param commandStr The command to be set.
 */
void setCommand(Parser *parser, char *commandStr);

SegmentType parseSegmentType(const char *segment); 

ArithmeticType parseArithmeticType(const char *command);

/**
 * Retrieves the command type of the current command.
 *
 * @param parser The parser object.
 * @return The command type as a readable string.
 */
const char *getCommandType(Parser *parser);

/**
 * Retrieves the segment type from the given parser object. The segment type is only valid for push and pop commands.
 *
 * @param parser The parser object.
 * @return The segment type as a readable string.
 */
const char *getArg1(Parser *parser);

/**
 * Retrieves the offset from the given parser object. The offset is only valid for push and pop commands.
 *
 * @param parser The parser object.
 * @return The offset as an integer.
 */
int getArg2(Parser *parser);

#endif // PARSE_H