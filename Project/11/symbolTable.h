#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

typedef struct {
    char *segment; // static, this, argument, local
    char *type; // int, char, boolean, className
    int index;
} variableInfo;

int symbolTableInit(); // Initialize symbol tables

void symbolTableDestroy(); // Destroy symbol tables

variableInfo *createVariable(const char *segment, const char *type); // Create a new variable

void freeVariable(gpointer data); // Free a variable 

void resetSubroutineTable(); // Reset subroutine table

variableInfo* lookup(const char *varName) ; // Look up a variable

int numsOf(const char *segment); // Get the count of variables in a segment

void printVariableInfo(variableInfo *info); // Print variable info

void printTable(GHashTable *table); // Print symbol table

void printVarCount(); // Print variable count

#endif // SYMBOLTABLE_H