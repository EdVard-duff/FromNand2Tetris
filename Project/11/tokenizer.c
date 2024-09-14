#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "tokenizer.h"
#include "utility.h"

static const char symbolTable[] = {
    '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
};

static const char *keywords[] = {
    "class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean",
    "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"
};

int tokenizerInit(Tokenizer *tokenizer, const char *file_path)
{
    tokenizer->file = fopen(file_path, "r");
    if (tokenizer->file == NULL)
    {
        perror("Failed to open input file");
        return -1;
    }
    tokenizer->curLine[0] = '\0';
    tokenizer->lineptr = tokenizer->curLine;    
    return 0;
}

void freeTokenizer(Tokenizer *tokenizer)
{
    if (tokenizer->file != NULL)
    {
        fclose(tokenizer->file);
    }
}

int advance(Tokenizer* tokenizer)
{
    if (tokenizer->lineptr[0] == '\0')
    {
        if (read_valid_line(tokenizer->file, tokenizer->curLine, MAX_LINE_LENGTH) == -1)
        {
            return -1;
        }
        tokenizer->lineptr = tokenizer->curLine;
    }
    char* p = tokenizer->lineptr;
    while (isspace(*p))
    {
        p++;
    }
    tokenizer->lineptr = p;

    if (*p == '\"') {
        p++;
        while (*p != '\"') {
            p++;
        }
        tokenizer->tokenType = STRING_CONST;
        strncpy(tokenizer->curToken, tokenizer->lineptr + 1, p - tokenizer->lineptr - 1);
        tokenizer->curToken[p - tokenizer->lineptr - 1] = '\0';
        tokenizer->lineptr = p + 1;
    } 
    else if (isdigit(*p)) {
        while (isdigit(*p)) {
            p++;
        }
        tokenizer->tokenType = INT_CONST;
        strncpy(tokenizer->curToken, tokenizer->lineptr, p - tokenizer->lineptr);
        tokenizer->curToken[p - tokenizer->lineptr] = '\0';
        tokenizer->lineptr = p;
    }
    else if (isSymbol(*p)) {
        tokenizer->tokenType = SYMBOL;
        tokenizer->curToken[0] = *p;
        tokenizer->curToken[1] = '\0';
        tokenizer->lineptr = p + 1;
    }
    else {
        while (isalnum(*p) || *p == '_') {
            p++;
        }
        if (isKeyword(tokenizer->lineptr, p - tokenizer->lineptr)) {
            tokenizer->tokenType = KEYWORD;
        }
        else {
            tokenizer->tokenType = IDENTIFIER;
        }
        strncpy(tokenizer->curToken, tokenizer->lineptr, p - tokenizer->lineptr);
        tokenizer->curToken[p - tokenizer->lineptr] = '\0';
        tokenizer->lineptr = p;
    }

    return 0;
}

int isSymbol(char c)
{
    for (int i = 0; i < sizeof(symbolTable) / sizeof(symbolTable[0]); i++)
    {
        if (c == symbolTable[i])
        {
            return 1;
        }
    }
    return 0;
}

int isKeyword(const char *str, int len)
{
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strlen(keywords[i]) == len && strncmp(str, keywords[i], len) == 0)
        {
            return 1;
        }
    }
    return 0;
}