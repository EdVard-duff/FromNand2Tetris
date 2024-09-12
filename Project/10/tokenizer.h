#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define MAX_TOKEN_LENGTH 64

typedef enum
{
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST,
    INVALID_TOKEN
} TokenType;

typedef struct {
    FILE *file;
    TokenType tokenType;
    char curLine[MAX_LINE_LENGTH];
    char curToken[MAX_TOKEN_LENGTH];
    char *lineptr; 
} Tokenizer; 

int tokenizerInit(Tokenizer *tokenizer, const char *file_path);

void freeTokenizer(Tokenizer *tokenizer);

/**
 * Advances the tokenizer to the next token.
 *
 * @param tokenizer The tokenizer object.
 * @return -1 if the end of the file is reached, 0 otherwise.
 */
int advance(Tokenizer* tokenizer);

int isSymbol(char c);

int isKeyword(const char *str, int len);

#endif // TOKEN