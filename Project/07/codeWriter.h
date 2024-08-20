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
void codeWriterInit(CodeWriter *writer, const char *filename);

// 析构函数
void codeWriterFree(CodeWriter *writer);

/**
 * Writes the assembly code for a command.
 *  
 * This function is a wrapper for writing the assembly code for certain types of commands. 
 * 
 * @param writer The CodeWriter object responsible for writing the assembly code.
 * @param command The Command object representing the command to be written.
 */
void writeCommand(CodeWriter *writer, Command *command);

/**
 * Writes the assembly code for an arithmetic command.
 *
 * @param writer The CodeWriter object responsible for writing the assembly code.
 * @param command The Command object representing the arithmetic command to be written.
 */
void writeArithmetic(CodeWriter *writer, Command *command);

/**
 * Writes the assembly code for the push command.
 *
 * @param writer The CodeWriter object responsible for writing the assembly code.
 * @param command The Command object representing the push command.
 */
void writePush(CodeWriter *writer, Command *command);

/**
 * Writes the assembly code for the pop command.
 *
 * @param writer The CodeWriter object responsible for writing the assembly code.
 * @param command The Command object representing the pop command.
 */
void writePop(CodeWriter *writer, Command *command);

/**
 * @brief Writes the assembly code for incrementing the stack pointer.
 *
 * @param writer The CodeWriter object responsible for writing the assembly code.
 */
void incrementSP(CodeWriter *writer);

/**
 * @brief Writes the assembly code for decrementing the stack pointer.
 *
 * @param writer The CodeWriter object responsible for writing the assembly code.
 */
void decrementSP(CodeWriter *writer);

#endif // CODE_WRITER_H