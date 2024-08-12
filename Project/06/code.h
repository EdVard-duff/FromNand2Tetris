#ifndef CODE_H
#define CODE_H

#include "parser.h"

#define A_BIT 0
#define D_BIT 1
#define M_BIT 2

#define COMP_SIZE 28
#define JUMP_SIZE 8

char* dest(const char* mnemonic, char* dest); 

char* comp(const char* mnemonic, char* dest);

char* jump(const char* mnemonic, char* dest);

char* getBinaryCode(Parser *parser, char *binaryCode);
#endif // CODE_H