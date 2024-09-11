#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include <stdio.h>
#include "parser.h"
#include "utility.h"

// Segment Base Addresses
#define LCL 1
#define ARG 2
#define THIS 3
#define THAT 4
#define TEMP 5
#define POINTER 3
#define STATIC 16

#define POP_TEMP "R15" // Temp register for pop operations
#define FRAME_END "R14" // Temp register for frame end
#define RET_ADDR "R13" // Temp register for return address
typedef struct {
    FILE *filestream;
    char filename[MAX_PATH_LENGTH];
    int labelCounter;
    int returnCounter;
} CodeWriter;

// 构造函数
int codeWriterInit(CodeWriter *writer, const char *filename);

// 析构函数
void codeWriterFree(CodeWriter *writer);

/**
 * @brief A warpper function that writes specific command based on the command type.
 *
 * @param writer The CodeWriter object used to write the command.
 * @param parser The Parser object used to parse the command.
 * @return Returns 0 if the command is written successfully, otherwise returns -1.
 */
int writeCommand(CodeWriter *writer, Parser *parser);

void writeArithmetic(CodeWriter *writer, const char *arithmetic);

void writePush(CodeWriter *writer, const char *segment, int offset, const char *filename);

void writePop(CodeWriter *writer, const char *segment, int offset, const char *filename);

void writeLabel(CodeWriter *writer, const char *label);

void writeGoto(CodeWriter *writer, const char *label);

void writeIfGoto(CodeWriter *writer, const char *label);

void writeFunction(CodeWriter *writer, const char *functionName, int numLocals);

void writeCall(CodeWriter *writer, const char *functionName, int numArgs);

void writeReturn(CodeWriter *writer);

void writeBootstrap(CodeWriter *writer);

void incrementSP(CodeWriter *writer);

void decrementSP(CodeWriter *writer);

void setStackTop2D(CodeWriter *writer);

#endif // CODE_WRITER_H