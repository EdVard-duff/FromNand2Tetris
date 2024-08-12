#include "symbolTable.h"
#include <stdio.h>

Symbol *symbolTable;

void symbolTableInit() {
    symbolTable = NULL;
    char symbol[4];
    for (int i = 0; i < 16; i++) {
        sprintf(symbol, "R%d", i);
        addEntry(symbol, i);
    }

    addEntry("SCREEN", 16384);
    addEntry("KBD", 24576);
    addEntry("SP", 0);
    addEntry("LCL", 1);
    addEntry("ARG", 2);
    addEntry("THIS", 3);
    addEntry("THAT", 4);
} 

void symbolTableFree() {
    Symbol *s, *tmp;
    HASH_ITER(hh, symbolTable, s, tmp) {
        HASH_DEL(symbolTable, s);
        free(s);
    }
}

void addEntry(const char *symbol, int address){
    Symbol *s;
    HASH_FIND_STR(symbolTable, symbol, s); /* id already in the hash? */
    if (s == NULL) {
        s = (Symbol*)malloc(sizeof(Symbol));
        strncpy(s->name, symbol, sizeof(s->name) - 1);
        s->name[sizeof(s->name) - 1] = '\0';
        s->address = address;
        HASH_ADD_STR(symbolTable, name, s); /* id: name of key field */
    }
}

int findSymbol(const char *symbol) {
    Symbol *s;
    HASH_FIND_STR(symbolTable, symbol, s);
    if (s == NULL) {
        return -1;
    }
    return s->address;
}

void printTable() {
    Symbol *s;
    for(s=symbolTable; s != NULL; s=(Symbol*)(s->hh.next)) {
        printf("Symbol: %s, Address: %d\n", s->name, s->address);
    }
}
