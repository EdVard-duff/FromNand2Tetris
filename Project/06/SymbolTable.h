#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "uthash.h"

#define MAX_SYMBOL_LENGTH 256

// https://troydhanson.github.io/uthash/userguide.html
// struct fields 作为 key, struct pointer 作为 value
// 插入之前需要检查 key 是否存在
typedef struct {
    char name[MAX_SYMBOL_LENGTH];
    int address;
    UT_hash_handle hh;
} Symbol;

extern Symbol *symbolTable;

void symbolTableInit(); 

void symbolTableFree(); 

void addEntry(const char *symbol, int address); 

int findSymbol(const char *symbol); 

void printTable(); 

#endif // SYMBOL_TABLE_H
