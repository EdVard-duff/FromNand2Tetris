
#include "codeWriter.h"
#include "parser.h"

#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <assert.h>

void codeWriterInit(CodeWriter *writer, const char *filename) {
    writer->labelCounter = 0;
    char filenameCopy[MAX_PATH_LENGTH];
    strcpy(filenameCopy, filename);
    strcpy(writer->filename, basename(filenameCopy));
    writer->filestream = fopen(filename, "w");
    if (writer->filestream == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }
}

void codeWriterFree(CodeWriter *writer) {
    if (writer->filestream != NULL) {
        fclose(writer->filestream);
    }
}

void writeCommand(CodeWriter *writer, Command *command) {
    CommandType type = command->type;
    switch (type) {
        case C_ARITHMETIC:
            writeArithmetic(writer, command);
            break;
        case C_PUSH:
            writePush(writer, command);
            break;
        case C_POP:
            writePop(writer, command);
            break;
        default:
            fprintf(stderr, "Invalid command type\n");
            exit(1);
    }
}

void writeArithmetic(CodeWriter *writer, Command *command) {
    assert(command->type == C_ARITHMETIC);
    ArithmeticType arithmetic = command->arithmetic;
    // The following logic could be optimized based on the arithmetic type
    switch (arithmetic) {
        case ADD:
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "M=D+M\n");
            incrementSP(writer);
            break;
        case SUB:
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "M=M-D\n");
            incrementSP(writer);
            break;
        case NEG:
            decrementSP(writer);
            fprintf(writer->filestream, "M=-M\n");
            incrementSP(writer);
            break;
        case EQ: // Reference code could be found under the SimpleEQ.asm file
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "D=M-D\n");
            fprintf(writer->filestream, "@EQ_TRUE_%d\n", writer->labelCounter);
            fprintf(writer->filestream, "D;JEQ\n");
            fprintf(writer->filestream, "D=0\n");
            fprintf(writer->filestream, "@EQ_CONT_%d\n", writer->labelCounter);
            fprintf(writer->filestream, "0;JMP\n");
            fprintf(writer->filestream, "(EQ_TRUE_%d)\n", writer->labelCounter);
            fprintf(writer->filestream, "D=-1\n");
            fprintf(writer->filestream, "(EQ_CONT_%d)\n", writer->labelCounter);
            fprintf(writer->filestream, "@SP\n");
            fprintf(writer->filestream, "A=M\n");
            fprintf(writer->filestream, "M=D\n");
            incrementSP(writer);

            writer->labelCounter++;
            break;
        case GT:
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "D=M-D\n");
            fprintf(writer->filestream, "@GT_TRUE_%d\n", writer->labelCounter);
            fprintf(writer->filestream, "D;JGT\n");
            fprintf(writer->filestream, "D=0\n");
            fprintf(writer->filestream, "@GT_CONT_%d\n", writer->labelCounter);
            fprintf(writer->filestream, "0;JMP\n");
            fprintf(writer->filestream, "(GT_TRUE_%d)\n", writer->labelCounter);
            fprintf(writer->filestream, "D=-1\n");
            fprintf(writer->filestream, "(GT_CONT_%d)\n", writer->labelCounter);
            fprintf(writer->filestream, "@SP\n");
            fprintf(writer->filestream, "A=M\n");
            fprintf(writer->filestream, "M=D\n");
            incrementSP(writer);

            writer->labelCounter++;
            break;
        case LT:
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "D=M-D\n");
            fprintf(writer->filestream, "@LT_TRUE_%d\n", writer->labelCounter);
            fprintf(writer->filestream, "D;JLT\n");
            fprintf(writer->filestream, "D=0\n");
            fprintf(writer->filestream, "@LT_CONT_%d\n", writer->labelCounter);
            fprintf(writer->filestream, "0;JMP\n");
            fprintf(writer->filestream, "(LT_TRUE_%d)\n", writer->labelCounter);
            fprintf(writer->filestream, "D=-1\n");
            fprintf(writer->filestream, "(LT_CONT_%d)\n", writer->labelCounter);
            fprintf(writer->filestream, "@SP\n");
            fprintf(writer->filestream, "A=M\n");
            fprintf(writer->filestream, "M=D\n");
            incrementSP(writer);

            writer->labelCounter++;
            break;
        case AND:
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "M=D&M\n");
            incrementSP(writer);
            break;
        case OR:
            decrementSP(writer);
            fprintf(writer->filestream, "D=M\n");
            decrementSP(writer);
            fprintf(writer->filestream, "M=D|M\n");
            incrementSP(writer);
            break;
        case NOT:
            decrementSP(writer);
            fprintf(writer->filestream, "M=!M\n");
            incrementSP(writer);
            break;
        default:
            fprintf(stderr, "Invalid arithmetic type\n");
            exit(1);
    }
}

void writePush(CodeWriter *writer, Command *command) {
    SegmentType segment = command->segment;
    switch (segment) {
        case SEG_CONSTANT:
            fprintf(writer->filestream, "@%d\n", command->offset);
            fprintf(writer->filestream, "D=A\n");
            break;
        case SEG_LOCAL:
            fprintf(writer->filestream, "@%d\n", LCL);
            fprintf(writer->filestream, "D=M\n");
            fprintf(writer->filestream, "@%d\n", command->offset);
            fprintf(writer->filestream, "A=D+A\n");
            fprintf(writer->filestream, "D=M\n");
            break;
        case SEG_ARGUMENT:
            fprintf(writer->filestream, "@%d\n", ARG);
            fprintf(writer->filestream, "D=M\n");
            fprintf(writer->filestream, "@%d\n", command->offset);
            fprintf(writer->filestream, "A=D+A\n");
            fprintf(writer->filestream, "D=M\n");
            break;
        case SEG_THIS:
            fprintf(writer->filestream, "@%d\n", THIS);
            fprintf(writer->filestream, "D=M\n");
            fprintf(writer->filestream, "@%d\n", command->offset);
            fprintf(writer->filestream, "A=D+A\n");
            fprintf(writer->filestream, "D=M\n");
            break;
        case SEG_THAT:
            fprintf(writer->filestream, "@%d\n", THAT);
            fprintf(writer->filestream, "D=M\n");
            fprintf(writer->filestream, "@%d\n", command->offset);
            fprintf(writer->filestream, "A=D+A\n");
            fprintf(writer->filestream, "D=M\n");
            break;
        case SEG_TEMP:  
            fprintf(writer->filestream, "@%d\n", TEMP + command->offset);
            fprintf(writer->filestream, "D=M\n");
            break;
        case SEG_POINTER:
            fprintf(writer->filestream, "@%d\n", POINTER + command->offset);
            fprintf(writer->filestream, "D=M\n");
            break;
        case SEG_STATIC:
            fprintf(writer->filestream, "@%s.%d\n", writer->filename, command->offset);
            fprintf(writer->filestream, "D=M\n");
            break;
        default:
            fprintf(stderr, "Invalid segment type\n");
            exit(1);
    }
    fprintf(writer->filestream, "@SP\n");
    fprintf(writer->filestream, "A=M\n");
    fprintf(writer->filestream, "M=D\n");
    incrementSP(writer);
}

void writePop(CodeWriter *writer, Command *command) {
    SegmentType segment = command->segment;

    switch (segment)
    {
    case SEG_LOCAL:
        fprintf(writer->filestream, "@%d\n", LCL);
        fprintf(writer->filestream, "D=M\n");
        fprintf(writer->filestream, "@%d\n", command->offset);
        fprintf(writer->filestream, "D=D+A\n");
        fprintf(writer->filestream, "@R13\n"); // 寄存器不够用了, Store the address in R13
        fprintf(writer->filestream, "M=D\n");
        break;
    case SEG_ARGUMENT:
        fprintf(writer->filestream, "@%d\n", ARG);
        fprintf(writer->filestream, "D=M\n");
        fprintf(writer->filestream, "@%d\n", command->offset);
        fprintf(writer->filestream, "D=D+A\n");
        fprintf(writer->filestream, "@R13\n");
        fprintf(writer->filestream, "M=D\n");
        break;
    case SEG_THIS:
        fprintf(writer->filestream, "@%d\n", THIS);
        fprintf(writer->filestream, "D=M\n");
        fprintf(writer->filestream, "@%d\n", command->offset);
        fprintf(writer->filestream, "D=D+A\n");
        fprintf(writer->filestream, "@R13\n");
        fprintf(writer->filestream, "M=D\n");
        break;
    case SEG_THAT:
        fprintf(writer->filestream, "@%d\n", THAT);
        fprintf(writer->filestream, "D=M\n");
        fprintf(writer->filestream, "@%d\n", command->offset);
        fprintf(writer->filestream, "D=D+A\n");
        fprintf(writer->filestream, "@R13\n");
        fprintf(writer->filestream, "M=D\n");
        break;
    case SEG_TEMP:
        fprintf(writer->filestream, "@%d\n", TEMP + command->offset);
        fprintf(writer->filestream, "D=A\n");
        fprintf(writer->filestream, "@R13\n");
        fprintf(writer->filestream, "M=D\n");
        break;
    case SEG_POINTER:
        fprintf(writer->filestream, "@%d\n", POINTER + command->offset);
        fprintf(writer->filestream, "D=A\n");
        fprintf(writer->filestream, "@R13\n");
        fprintf(writer->filestream, "M=D\n");
        break;
    case SEG_STATIC:
        fprintf(writer->filestream, "@%s.%d\n", writer->filename, command->offset);
        fprintf(writer->filestream, "D=A\n");
        fprintf(writer->filestream, "@R13\n");
        fprintf(writer->filestream, "M=D\n");
        break;
    default:
        fprintf(stderr, "Invalid segment type\n");
        exit(1);
    }
    decrementSP(writer);
    fprintf(writer->filestream, "D=M\n");
    fprintf(writer->filestream, "@R13\n");
    fprintf(writer->filestream, "A=M\n");
    fprintf(writer->filestream, "M=D\n");
}

void incrementSP(CodeWriter *writer) {
    fprintf(writer->filestream, "@SP\n");
    fprintf(writer->filestream, "AM=M+1\n");
}

void decrementSP(CodeWriter *writer) {
    fprintf(writer->filestream, "@SP\n");
    fprintf(writer->filestream, "AM=M-1\n");
}