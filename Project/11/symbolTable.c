#include <stdio.h>
#include <glib.h>
#include <errno.h>
#include "symbolTable.h"

GHashTable *classTable; // Class symbol table
GHashTable *subroutineTable; // Subroutine symbol table
static GHashTable *varCount; // Variable count

static const char *getVarSegment(const char *varKind) {
    if (strcmp(varKind, "static") == 0) {
        return "static";
    } else if (strcmp(varKind, "field") == 0) {
        return "this";
    } else if (strcmp(varKind, "argument") == 0) {
        return "argument";
    } else if (strcmp(varKind, "var") == 0) {
        return "local";
    }
    return NULL;
}

int symbolTableInit()
{   
    // g_hash_table_new_full 和 g_hash_table_new 的区别是，前者可以指定 key 和 value 的释放函数, 后者需要手动释放
    classTable = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, freeVariable);
    if (classTable == NULL)
    {
        perror("Failed to create class symbol table");
        return -1;
    }

    subroutineTable = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, freeVariable);
    if (subroutineTable == NULL)
    {
        perror("Failed to create subroutine symbol table");
        g_hash_table_destroy(classTable);
        return -1;
    }

    varCount = g_hash_table_new(g_str_hash, g_str_equal);
    if (varCount == NULL)
    {
        perror("Failed to create variable count table");
        g_hash_table_destroy(classTable);
        g_hash_table_destroy(subroutineTable);
        return -1;
    }

    // 插入静态字符串作为 varCount 的 key
    // GINT_TO_POINTER 将 int 转换为 gpointer, gpointer 是一个 void* 类型, 实际上只是把 int 解释成一个内存地址, 它的作用是将一个整数值存储在指针类型中，而不进行任何内存分配操作.
    g_hash_table_insert(varCount, (gpointer)"static", GINT_TO_POINTER(0));
    g_hash_table_insert(varCount, (gpointer)"this", GINT_TO_POINTER(0));
    g_hash_table_insert(varCount, (gpointer)"argument", GINT_TO_POINTER(0));
    g_hash_table_insert(varCount, (gpointer)"local", GINT_TO_POINTER(0));

    return 0;
}

void symbolTableDestroy()
{
    g_hash_table_destroy(classTable);
    g_hash_table_destroy(subroutineTable);
    g_hash_table_destroy(varCount);
}

variableInfo *createVariable(const char *varKind, const char *type)
{
    variableInfo *info = malloc(sizeof(variableInfo));
    if (info == NULL)
    {
        perror("Failed to allocate memory for variableInfo");
        return NULL;
    }

    info->type = g_strdup(type);
    info->segment = (char *)getVarSegment(varKind); 
    if (info->type == NULL || info->segment == NULL)
    {
        perror("Failed to allocate memory for variableInfo");
        freeVariable(info);
        return NULL;
    }

    gpointer value = NULL;
    if (g_hash_table_lookup_extended(varCount, info->segment, NULL, &value))
    {
        info->index = GPOINTER_TO_INT(value);
        g_hash_table_insert(varCount, (gpointer)info->segment, GINT_TO_POINTER(info->index + 1));
    } else {
        perror("Failed to get variable count");
        freeVariable(info);
        return NULL;
    }

    return info;
}

void freeVariable(gpointer data)
{
    variableInfo *info = (variableInfo *)data;
    if (info == NULL)
    {
        return;
    }

    g_free(info->type);
    free(info);
}

void resetSubroutineTable()
{
    g_hash_table_remove_all(subroutineTable);
    g_hash_table_replace(varCount, (gpointer)"argument", GINT_TO_POINTER(0));
    g_hash_table_replace(varCount, (gpointer)"local", GINT_TO_POINTER(0));
}

variableInfo* lookup(const char *varName) {
    gpointer *info = g_hash_table_lookup(subroutineTable, varName);
    if (info == NULL) {
        info = g_hash_table_lookup(classTable, varName);
    }
    return (variableInfo *)info;
}

int numsOf(const char *varKind) {
    gpointer value = g_hash_table_lookup(varCount, getVarSegment(varKind));
    if (value == NULL) {
        return -1;
    }
    return GPOINTER_TO_INT(value);
}

void printVariableInfo(variableInfo *info)
{
    // printf("Segment: %s, Type: %s, Index: %d\n", info->segment, info->type, info->index);
    printf("%s %s %d\n", info->segment, info->type, info->index);
}

void printTable(GHashTable *table)
{
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {   
        printf("Name: %s, ", (char *)key);
        printVariableInfo((variableInfo *)value);
    }
}

void printVarCount()
{
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, varCount);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {   
        printf("%s: %d\n", (char *)key, GPOINTER_TO_INT(value));
    }
}