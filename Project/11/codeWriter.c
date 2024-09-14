#include <stdio.h>
#include <config.h>
#include "codeWriter.h"
#include "utility.h"

extern const char *outDir;

FILE *outputVMFile, *outputXMLFile;

int codeWriterInit(const char *inputFilePath)
{
    char outputFile[MAX_PATH_LENGTH];

    generateOutputFilepath(outputFile, inputFilePath, outDir, "vm");
    outputVMFile = fopen(outputFile, "w");
    printf("Output file: %s\n", outputFile);

    generateOutputFilepath(outputFile, inputFilePath, outDir, "xml");
    outputXMLFile = fopen(outputFile, "w");
    printf("Output file: %s\n", outputFile);

    if (outputVMFile == NULL || outputXMLFile == NULL)
    {
        perror("Failed to open output file");
        FreeCodeWriter();
        return -1;
    }
    return 0;
}

void FreeCodeWriter()
{
    if (outputVMFile != NULL)
    {
        fclose(outputVMFile);
    }
    if (outputXMLFile != NULL)
    {
        fclose(outputXMLFile);
    }
}

void writePush(const char *segment, int index)
{
    fprintf(outputVMFile, "push %s %d\n", segment, index);
}

void writePop(const char *segment, int index)
{
    fprintf(outputVMFile, "pop %s %d\n", segment, index);
}

void writeArithmetic(const char *command)
{
    fprintf(outputVMFile, "%s\n", command);
}

void writeLabel(const char *label)
{
    fprintf(outputVMFile, "label %s\n", label);
}

void writeGoto(const char *label)
{
    fprintf(outputVMFile, "goto %s\n", label);
}

void writeIf(const char *label)
{
    fprintf(outputVMFile, "if-goto %s\n", label);
}

void writeCall(const char *name, int nArgs)
{
    fprintf(outputVMFile, "call %s %d\n", name, nArgs);
}

void writeFunction(const char *className, const char *subroutineName, int nLocals)
{
    fprintf(outputVMFile, "function %s.%s %d\n", className, subroutineName, nLocals);
}

void writeReturn()
{
    fprintf(outputVMFile, "return\n");
}
