#include "parser.h"
#include "SymbolTable.h"
#include "code.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // For basename and dirname

// 生成输出文件路径
char *generateHackFilePath(const char *inputFilePath);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: Hackassembler.exe <filename>\n");
        return 1;
    }

    char *filename = argv[1];
    printf("Assembling %s\n", filename);
    Parser *parser = parserInit(filename);
    symbolTableInit();

    // First pass, add all labels to symbol table
    while (advance(parser) != -1)
    {
        if (parser->commandType == L_COMMAND)
        {
            addEntry(parser->symbol, parser->lineNumber);
        }
    }

    // Second pass, generate binary code
    char *outfile = generateHackFilePath(filename);
    FILE *out = fopen(outfile, "w");
    if (out == NULL) {
        perror("Failed to open output file");
        free(outfile);
        parserFree(parser);
        symbolTableFree();
        return EXIT_FAILURE;
    }

    char buffer[17];
    resetParser(parser);
    int firstLine = 1; 
    while (advance(parser) != -1)
    {
        if (getBinaryCode(parser, buffer) != NULL)
        {
            // printf("Current command: %s %s\n", parser->currentCommand, buffer);
            if (!firstLine)
            {
                fprintf(out, "\n");
            }
            fprintf(out, "%s", buffer);
            firstLine = 0;
        }
    }
    fclose(out);
    printf("Output written to %s\n", outfile);

    // Free memory
    free(outfile);
    parserFree(parser);
    symbolTableFree();

    return 0;
}

char *generateHackFilePath(const char *inputFilePath)
{
    // 复制输入文件路径，以避免修改原始路径
    char *pathCopy = strdup(inputFilePath);
    if (pathCopy == NULL)
    {
        perror("Failed to duplicate path");
        exit(EXIT_FAILURE);
    }

    // 提取文件名（不带扩展名）
    char *baseName = basename(pathCopy);
    char *dotPos = strrchr(baseName, '.');
    size_t baseNameLen = (dotPos != NULL) ? (dotPos - baseName) : strlen(baseName);

    // 计算新路径的总长度
    size_t newPathLen = baseNameLen + 5 + 1; // <baseName>.hack\0

    // 分配足够的内存来存储新文件路径
    char *outputFilePath = (char *)malloc(newPathLen);
    if (outputFilePath == NULL)
    {
        perror("Failed to allocate memory");
        free(pathCopy);
        exit(EXIT_FAILURE);
    }

    // 复制基础文件名并添加 ".hack" 后缀
    strncpy(outputFilePath, baseName, baseNameLen);
    strcpy(outputFilePath + baseNameLen, ".hack");

    // 释放内存
    free(pathCopy);

    return outputFilePath;
}