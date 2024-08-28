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

typedef struct {
    FILE *filestream;
    char filename[MAX_PATH_LENGTH];
    int labelCounter;
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

void writePush(CodeWriter *writer, const char *segment, int offset);

void writePop(CodeWriter *writer, const char *segment, int offset);

void writeLabel(CodeWriter *writer, const char *label);

void writeGoto(CodeWriter *writer, const char *label);

void writeIfGoto(CodeWriter *writer, const char *label);

void writeFunction(CodeWriter *writer, const char *functionName, int numLocals);

void writeCall(CodeWriter *writer, const char *functionName, int numArgs);

void writeReturn(CodeWriter *writer);

void incrementSP(CodeWriter *writer);

void decrementSP(CodeWriter *writer);

#endif // CODE_WRITER_H