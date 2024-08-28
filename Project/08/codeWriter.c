
#include "codeWriter.h"
#include "parser.h"

#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <assert.h>

static int getSegmentBase(const char *segment)
{
    if (strcmp(segment, "local") == 0)
    {
        return LCL;
    }
    else if (strcmp(segment, "argument") == 0)
    {
        return ARG;
    }
    else if (strcmp(segment, "this") == 0)
    {
        return THIS;
    }
    else if (strcmp(segment, "that") == 0)
    {
        return THAT;
    }
    else if (strcmp(segment, "temp") == 0)
    {
        return TEMP;
    }
    else if (strcmp(segment, "pointer") == 0)
    {
        return POINTER;
    }

    return -1; // Invalid segment
}

int codeWriterInit(CodeWriter *writer, const char *filename)
{
    writer->labelCounter = 0;
    char filenameCopy[MAX_PATH_LENGTH];
    strcpy(filenameCopy, filename);

    char *dot = strrchr(filenameCopy, '.');
    if (dot)
    {
        *dot = '\0'; // Remove the extension
    }
    strcpy(writer->filename, basename(filenameCopy));
    writer->filestream = fopen(filename, "w");
    if (writer->filestream == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return -1; 
    }
    return 0;
}

void codeWriterFree(CodeWriter *writer)
{
    if (writer->filestream != NULL)
    {
        fclose(writer->filestream);
    }
}

int writeCommand(CodeWriter *writer, Parser *parser)
{
    CommandType type = parser->type;

    #ifdef PRINT_VMCODE
        fprintf(writer->filestream, "// %s\n", parser->curCommand);
    #endif

    switch (type)
    {
    case C_ARITHMETIC:
        writeArithmetic(writer, parser->arg1);
        break;
    case C_PUSH:
        writePush(writer, parser->arg1, parser->arg2);
        break;
    case C_POP:
        writePop(writer, parser->arg1, parser->arg2);
        break;
    case C_LABEL:
        writeLabel(writer, parser->arg1);
        break;
    case C_GOTO:
        writeGoto(writer, parser->arg1);
        break;
    case C_IF:
        writeIfGoto(writer, parser->arg1);
        break;
    case C_FUNCTION:
        writeFunction(writer, parser->arg1, parser->arg2);
        break;
    case C_CALL:
        writeCall(writer, parser->arg1, parser->arg2);
        break;
    case C_RETURN:
        writeReturn(writer);
        break;
    default:
        fprintf(stderr, "Invalid command type\n");
        return -1; 
    }

    #ifdef PRINT_VMCODE
        fprintf(writer->filestream, "\n");
    #endif

    return 0;
}

void writeArithmetic(CodeWriter *writer, const char *arithmetic)
{
    decrementSP(writer);
    if (strcmp(arithmetic, "add") == 0 || strcmp(arithmetic, "sub") == 0 || strcmp(arithmetic, "and") == 0 || strcmp(arithmetic, "or") == 0)
    {
        fprintf(writer->filestream, "D=M\n");
        decrementSP(writer);
        if (strcmp(arithmetic, "add") == 0)
        {
            fprintf(writer->filestream, "M=D+M\n");
        }
        else if (strcmp(arithmetic, "sub") == 0)
        {
            fprintf(writer->filestream, "M=M-D\n");
        }
        else if (strcmp(arithmetic, "and") == 0)
        {
            fprintf(writer->filestream, "M=D&M\n");
        }
        else if (strcmp(arithmetic, "or") == 0)
        {
            fprintf(writer->filestream, "M=D|M\n");
        }
    }
    else if (strcmp(arithmetic, "neg") == 0)
    {
        fprintf(writer->filestream, "M=-M\n");
    }
    else if (strcmp(arithmetic, "not") == 0)
    {
        fprintf(writer->filestream, "M=!M\n");
    }
    else if (strcmp(arithmetic, "eq") == 0 || strcmp(arithmetic, "gt") == 0 || strcmp(arithmetic, "lt") == 0)
    {
        char symbol_1[16], symbol_2[16], jump[4];
        if (strcmp(arithmetic, "eq") == 0)
        {
            sprintf(symbol_1, "EQ_TRUE_%d", writer->labelCounter);
            sprintf(symbol_2, "EQ_FALSE_%d", writer->labelCounter);
            strcpy(jump, "JEQ");
        }
        else if (strcmp(arithmetic, "gt") == 0)
        {
            sprintf(symbol_1, "GT_TRUE_%d", writer->labelCounter);
            sprintf(symbol_2, "GT_FALSE_%d", writer->labelCounter);
            strcpy(jump, "JGT");
        }
        else if (strcmp(arithmetic, "lt") == 0)
        {
            sprintf(symbol_1, "LT_TRUE_%d", writer->labelCounter);
            sprintf(symbol_2, "LT_FALSE_%d", writer->labelCounter);
            strcpy(jump, "JLT");
        }
        fprintf(writer->filestream, "D=M\n");
        decrementSP(writer);
        fprintf(writer->filestream, "D=M-D\n");
        fprintf(writer->filestream, "@%s\n", symbol_1);
        fprintf(writer->filestream, "D;%s\n", jump);
        fprintf(writer->filestream, "D=0\n");
        fprintf(writer->filestream, "@%s\n", symbol_2);
        fprintf(writer->filestream, "0;JMP\n");
        fprintf(writer->filestream, "(%s)\n", symbol_1);
        fprintf(writer->filestream, "D=-1\n");
        fprintf(writer->filestream, "(%s)\n", symbol_2);
        fprintf(writer->filestream, "@SP\n");
        fprintf(writer->filestream, "A=M\n");
        fprintf(writer->filestream, "M=D\n");

        writer->labelCounter++;
    }
    else
    {
        fprintf(stderr, "Invalid arithmetic command\n");
        exit(1);
    }
    incrementSP(writer);
}

void writePush(CodeWriter *writer, const char *segment, int offset)
{
    if (strcmp(segment, "local") == 0 || strcmp(segment, "argument") == 0 || strcmp(segment, "this") == 0 || strcmp(segment, "that") == 0)
    {
        fprintf(writer->filestream, "@%d\n", getSegmentBase(segment));
        fprintf(writer->filestream, "D=M\n");
        fprintf(writer->filestream, "@%d\n", offset);
        fprintf(writer->filestream, "A=D+A\n");
        fprintf(writer->filestream, "D=M\n");
    }
    else if (strcmp(segment, "temp") == 0 || strcmp(segment, "pointer") == 0)
    {
        fprintf(writer->filestream, "@%d\n", getSegmentBase(segment) + offset);
        fprintf(writer->filestream, "D=M\n");
    }
    else if (strcmp(segment, "constant") == 0)
    {
        fprintf(writer->filestream, "@%d\n", offset);
        fprintf(writer->filestream, "D=A\n");
    }
    else if (strcmp(segment, "static") == 0)
    {
        fprintf(writer->filestream, "@%s.%d\n", writer->filename, offset);
        fprintf(writer->filestream, "D=M\n");
    }
    else
    {
        fprintf(stderr, "Invalid segment type\n");
        exit(1);
    }
    fprintf(writer->filestream, "@SP\n");
    fprintf(writer->filestream, "A=M\n");
    fprintf(writer->filestream, "M=D\n");
    incrementSP(writer);
}

void writePop(CodeWriter *writer, const char *segment, int offset)
{
    if (strcmp(segment, "local") == 0 || strcmp(segment, "argument") == 0 || strcmp(segment, "this") == 0 || strcmp(segment, "that") == 0)
    {
        fprintf(writer->filestream, "@%d\n", getSegmentBase(segment));
        fprintf(writer->filestream, "D=M\n");
        fprintf(writer->filestream, "@%d\n", offset);
        fprintf(writer->filestream, "D=D+A\n");
    }
    else if (strcmp(segment, "temp") == 0 || strcmp(segment, "pointer") == 0)
    {
        fprintf(writer->filestream, "@%d\n", getSegmentBase(segment) + offset);
        fprintf(writer->filestream, "D=A\n");
    }
    else if (strcmp(segment, "static") == 0)
    {
        fprintf(writer->filestream, "@%s.%d\n", writer->filename, offset);
        fprintf(writer->filestream, "D=A\n");
    }
    else
    {
        fprintf(stderr, "Invalid segment type\n");
        exit(1);
    }
    fprintf(writer->filestream, "@R13\n"); // Store the address in R13
    fprintf(writer->filestream, "M=D\n");
    decrementSP(writer);
    fprintf(writer->filestream, "D=M\n");
    fprintf(writer->filestream, "@R13\n");
    fprintf(writer->filestream, "A=M\n");
    fprintf(writer->filestream, "M=D\n");
}

void writeLabel(CodeWriter *writer, const char *label)
{
    fprintf(writer->filestream, "(%s)\n", label);
}

void writeGoto(CodeWriter *writer, const char *label)
{
    fprintf(writer->filestream, "@%s\n", label);
    fprintf(writer->filestream, "0;JMP\n");
}

void writeIfGoto(CodeWriter *writer, const char *label)
{
    decrementSP(writer);
    fprintf(writer->filestream, "D=M\n");
    fprintf(writer->filestream, "@%s\n", label);
    fprintf(writer->filestream, "D;JNE\n");
}

void writeFunction(CodeWriter *writer, const char *functionName, int nLocals) 
{
    return; 
}

void writeCall(CodeWriter *writer, const char *functionName, int nArgs) 
{
    return; 
}

void writeReturn(CodeWriter *writer)
{
    return; 
}

void incrementSP(CodeWriter *writer)
{
    fprintf(writer->filestream, "@SP\n");
    fprintf(writer->filestream, "M=M+1\n");
}

void decrementSP(CodeWriter *writer)
{
    fprintf(writer->filestream, "@SP\n");
    fprintf(writer->filestream, "AM=M-1\n");
}
