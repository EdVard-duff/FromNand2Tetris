#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "compilerEngine.h"
#include "tokenizer.h"
#include "symbolTable.h"
#include "codeWriter.h"

extern Tokenizer tokenizer;

extern FILE *outputXMLFile, *outputVMFile;

extern GHashTable *classTable, *subroutineTable;

static int indentLevel = 0; 

static char className[VAR_BUF_SIZE], subroutineType[VAR_BUF_SIZE], subroutineName[VAR_BUF_SIZE];
static char varKind[VAR_BUF_SIZE], varType[VAR_BUF_SIZE], varName[VAR_BUF_SIZE];

static char *terminalTag[] = {
    [KEYWORD] = "keyword",
    [SYMBOL] = "symbol",
    [IDENTIFIER] = "identifier",
    [INT_CONST] = "integerConstant",
    [STRING_CONST] = "stringConstant"
}; 

static int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<' || c == '>' || c == '=';
};

static const char* getCommand(char* op) {
    if (strcmp(op, "+") == 0) {
        return "add";
    } else if (strcmp(op, "-") == 0) {
        return "sub";
    } else if (strcmp(op, "*") == 0) {
        return "call Math.multiply 2";
    } else if (strcmp(op, "/") == 0) {
        return "call Math.divide 2";
    } else if (strcmp(op, "&") == 0) {
        return "and";
    } else if (strcmp(op, "|") == 0) {
        return "or";
    } else if (strcmp(op, "<") == 0) {
        return "lt";
    } else if (strcmp(op, ">") == 0) {
        return "gt";
    } else if (strcmp(op, "=") == 0) {
        return "eq";
    } else if (strcmp(op, "~") == 0) {
        return "not";
    } 
    return NULL;
};

static int isUnaryOp(char c) {
    return c == '-' || c == '~';
};

static int isKeywordConstant(const char *str) {
    return strcmp(str, "true") == 0 || strcmp(str, "false") == 0 || strcmp(str, "null") == 0 || strcmp(str, "this") == 0;
};

static void printIndent() {
    for (int i = 0; i < indentLevel; i++)
    {
        fprintf(outputXMLFile, "  ");
    }
};

void compileTokenList() {
    fprintf(outputXMLFile, "<tokens>\n");
    indentLevel++;

    while (advance(&tokenizer) != -1)
    {
        printTerminal();
    }
    
    indentLevel--;
    fprintf(outputXMLFile, "</tokens>\n");
}

void compileClass() {
    symbolTableInit();
    advance(&tokenizer);

    openNonTerminal("class");
    process(NULL, -1, 1, "class");
    process(className, VAR_BUF_SIZE, 1, tokenizer.curToken); // className
    process(NULL, -1, 1, "{");

    while (strcmp(tokenizer.curToken, "static") == 0 || strcmp(tokenizer.curToken, "field") == 0)
    {
        compileClassVarDec();
    }
    // printTable(classTable);

    while (strcmp(tokenizer.curToken, "constructor") == 0 || strcmp(tokenizer.curToken, "function") == 0 || strcmp(tokenizer.curToken, "method") == 0)
    {
        compileSubroutine();
    }
    
    process(NULL, -1, 1, "}");
    closeNonTerminal("class");

    symbolTableDestroy();
};

void compileClassVarDec() {
    openNonTerminal("classVarDec");

    if (process(varKind, VAR_BUF_SIZE, 2, "static", "field") == -1) return;
    process(varType, VAR_BUF_SIZE, 4, "int", "char", "boolean", tokenizer.curToken); // type
    process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken); // varName

    // 注意内存管理, g_strdup 确保 key 和 value 在 hash table 中是独立的
    g_hash_table_insert(classTable, g_strdup(varName), createVariable(varKind, varType));

    while (strcmp(tokenizer.curToken, ",") == 0)
    {
        process(NULL, -1, 1, ",");
        process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken); // varName
        g_hash_table_insert(classTable, g_strdup(varName), createVariable(varKind, varType));
    }
    process(NULL, -1, 1, ";");

    closeNonTerminal("classVarDec");
};

void compileSubroutine() {
    openNonTerminal("subroutineDec");
    
    process(subroutineType, VAR_BUF_SIZE, 3, "constructor", "function", "method");
    process(NULL, -1, 5, "void", "int", "char", "boolean", tokenizer.curToken); // Returntype
    process(subroutineName, VAR_BUF_SIZE, 1, tokenizer.curToken); // subroutineName
    // printf("\nSubroutine: %s\n", subroutineName);

    // Parameter list
    process(NULL, -1, 1, "(");
    compileParameterList();
    process(NULL, -1, 1, ")");

    compileSubroutineBody();

    closeNonTerminal("subroutineDec");
};

void compileParameterList() {
    openNonTerminal("parameterList");

    // Method has an implicit argument 'this'
    if (strcmp(subroutineType, "method") == 0)
    {
        g_hash_table_insert(subroutineTable, g_strdup("this"), createVariable("argument", className));
    } 

    if (strcmp(tokenizer.curToken, ")") != 0)
    {
        process(varType, VAR_BUF_SIZE, 4, "int", "char", "boolean", tokenizer.curToken); // type
        process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken); // varName
        g_hash_table_insert(subroutineTable, g_strdup(varName), createVariable("argument", varType));

        while (strcmp(tokenizer.curToken, ",") == 0)
        {
            process(NULL, -1, 1, ",");
            process(varType, VAR_BUF_SIZE, 4, "int", "char", "boolean", tokenizer.curToken); // type
            process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken); // varName
            g_hash_table_insert(subroutineTable, g_strdup(varName), createVariable("argument", varType));
        }
    }

    closeNonTerminal("parameterList");
};

void compileSubroutineBody() {
    openNonTerminal("subroutineBody");

    process(NULL, -1, 1, "{");

    int varCount = 0;
    while (strcmp(tokenizer.curToken, "var") == 0)
    {
        varCount += compileVarDec();
    }
    writeFunction(className, subroutineName, varCount);

    if (strcmp(subroutineType, "constructor") == 0)
    {
        writePush("constant", numsOf("field"));
        writeCall("Memory.alloc", 1);
        writePop("pointer", 0);
    } else if (strcmp(subroutineType, "method") == 0)
    {
        writePush("argument", 0);
        writePop("pointer", 0);
    }

    compileStatements();

    process(NULL, -1, 1, "}");

    // printTable(subroutineTable);
    resetSubroutineTable();
    // printVarCount();

    closeNonTerminal("subroutineBody");
};

int compileVarDec() {
    openNonTerminal("varDec");
    int varCount = 0;

    process(varKind, VAR_BUF_SIZE, 1, "var");
    process(varType, VAR_BUF_SIZE, 4, "int", "char", "boolean", tokenizer.curToken); // type
    process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken); // varName
    g_hash_table_insert(subroutineTable, g_strdup(varName), createVariable(varKind, varType));
    varCount++;

    while (strcmp(tokenizer.curToken, ",") == 0)
    {
        process(NULL, -1, 1, ",");
        process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken);
        g_hash_table_insert(subroutineTable, g_strdup(varName), createVariable(varKind, varType));
        varCount++;
    }

    process(NULL, -1, 1, ";");

    closeNonTerminal("varDec");

    return varCount;
};

void compileStatements() {
    openNonTerminal("statements");

    while (strcmp(tokenizer.curToken, "}") != 0) // Statements end with '}'
    {
        if (strcmp(tokenizer.curToken, "let") == 0)
        {
            compileLet();
        }
        else if (strcmp(tokenizer.curToken, "if") == 0)
        {
            compileIf();
        }
        else if (strcmp(tokenizer.curToken, "while") == 0)
        {
            compileWhile();
        }
        else if (strcmp(tokenizer.curToken, "do") == 0)
        {
            compileDo();
        }
        else if (strcmp(tokenizer.curToken, "return") == 0)
        {
            compileReturn();
        }
    }

    closeNonTerminal("statements");
};

void compileLet() {
    openNonTerminal("letStatement");

    process(NULL, -1, 1, "let");

    process(varName, VAR_BUF_SIZE, 1, tokenizer.curToken); // varName

    if (strcmp(tokenizer.curToken, "=") == 0)
    {
        process(NULL, -1, 1, "=");
        compileExpression();
        
        variableInfo *info = lookup(varName);
        if (info != NULL)
        {
            writePop(info->segment, info->index);
        }
    } else if (strcmp(tokenizer.curToken, "[") == 0)
    {
        variableInfo *info = lookup(varName);
        if (info != NULL)
        {
            writePush(info->segment, info->index);
        }
        
        process(NULL, -1, 1, "[");
        compileExpression();
        process(NULL, -1, 1, "]");
        writeArithmetic("add");

        process(NULL, -1, 1, "=");
        compileExpression();

        writePop("temp", 0);
        writePop("pointer", 1);
        writePush("temp", 0);
        writePop("that", 0);
    }
    process(NULL, -1, 1, ";");

    closeNonTerminal("letStatement");
};

void compileIf() {
    static int ifCount = 0;

    int curIfCount = ifCount++;
    char endLabel[VAR_BUF_SIZE], elseLabel[VAR_BUF_SIZE];
    snprintf(endLabel, VAR_BUF_SIZE, "IF_END_%d", curIfCount);
    snprintf(elseLabel, VAR_BUF_SIZE, "IF_FALSE_%d", curIfCount);

    openNonTerminal("ifStatement");

    process(NULL, -1, 1, "if");
    process(NULL, -1, 1, "(");
    compileExpression();
    process(NULL, -1, 1, ")");

    writeArithmetic("not");
    writeIf(elseLabel);

    process(NULL, -1, 1, "{");
    compileStatements();
    writeGoto(endLabel);
    process(NULL, -1, 1, "}");

    writeLabel(elseLabel);
    if (strcmp(tokenizer.curToken, "else") == 0)
    {
        process(NULL, -1, 1, "else");
        process(NULL, -1, 1, "{");
        compileStatements();
        process(NULL, -1, 1, "}");
    }
    writeLabel(endLabel);

    closeNonTerminal("ifStatement");
};

void compileWhile() {
    static int whileCount = 0;
    whileCount++;
    char startLabel[VAR_BUF_SIZE], endLabel[VAR_BUF_SIZE];
    snprintf(startLabel, VAR_BUF_SIZE, "WHILE_START_%d", whileCount);
    snprintf(endLabel, VAR_BUF_SIZE, "WHILE_END_%d", whileCount);
    
    openNonTerminal("whileStatement");

    process(NULL, -1, 1, "while");
    writeLabel(startLabel);
    process(NULL, -1, 1, "(");
    compileExpression();
    process(NULL, -1, 1, ")");

    writeArithmetic("not");
    writeIf(endLabel);

    process(NULL, -1, 1, "{");
    compileStatements();
    process(NULL, -1, 1, "}");
    writeGoto(startLabel);

    writeLabel(endLabel);

    closeNonTerminal("whileStatement");
};

void compileDo() {
    openNonTerminal("doStatement");
    
    process(NULL, -1, 1, "do");
    // subroutineCall
    compileExpression();

    writePop("temp", 0);

    process(NULL, -1, 1, ";");

    closeNonTerminal("doStatement");
};

void compileReturn() {
    openNonTerminal("returnStatement");

    process(NULL, -1, 1, "return");
    if (strcmp(tokenizer.curToken, ";") != 0)
    {
        compileExpression();
    } else {
        writePush("constant", 0);
    }
    writeReturn();
    process(NULL, -1, 1, ";");
    
    closeNonTerminal("returnStatement");
};

void compileExpression() {
    openNonTerminal("expression");

    compileTerm();

    char op; 
    while (isOperator(tokenizer.curToken[0]))
    {
        process(&op, 1, 1, tokenizer.curToken);
        compileTerm();
        writeArithmetic(getCommand(&op));
    }

    closeNonTerminal("expression");
};

void compileTerm() {
    openNonTerminal("term");

    if (tokenizer.tokenType == INT_CONST) {
        writePush("constant", atoi(tokenizer.curToken));
        process(NULL, -1, 1, tokenizer.curToken);
    } else if (tokenizer.tokenType == STRING_CONST) {
        int len = strlen(tokenizer.curToken);
        writePush("constant", len);
        writeCall("String.new", 1);
        for (int i = 0; i < len; i++)
        {
            writePush("constant", tokenizer.curToken[i]);
            writeCall("String.appendChar", 2);
        }
        process(NULL, -1, 1, tokenizer.curToken);
    } else if (isKeywordConstant(tokenizer.curToken)) {
        if (strcmp(tokenizer.curToken, "true") == 0) {
            writePush("constant", 0);
            writeArithmetic("not");
        } else if (strcmp(tokenizer.curToken, "false") == 0) {
            writePush("constant", 0);
        } else if (strcmp(tokenizer.curToken, "null") == 0) {
            writePush("constant", 0);
        } else if (strcmp(tokenizer.curToken, "this") == 0) {
            writePush("pointer", 0);
        }
        process(NULL, -1, 1, tokenizer.curToken);
    } else if (isUnaryOp(tokenizer.curToken[0])) {
        char op = tokenizer.curToken[0];
        process(NULL, -1, 1, tokenizer.curToken);
        compileTerm();
        if (op == '-') {
            writeArithmetic("neg");
        } else if (op == '~') {
            writeArithmetic("not");
        }
    } else if (strcmp(tokenizer.curToken, "(") == 0) {
        process(NULL, -1, 1, "(");
        compileExpression();
        process(NULL, -1, 1, ")");
    } else if (tokenizer.tokenType == IDENTIFIER) {
        char token_1[VAR_BUF_SIZE], token_2[VAR_BUF_SIZE], funcName[2 * VAR_BUF_SIZE];  
        process(token_1, VAR_BUF_SIZE, 1, tokenizer.curToken);

        if (strcmp(tokenizer.curToken, "[") == 0) { // Array access
            variableInfo *info = lookup(token_1);
            if (info != NULL)
            {
                writePush(info->segment, info->index);
            }
        
            process(NULL, -1, 1, "[");
            compileExpression();
            process(NULL, -1, 1, "]");
            writeArithmetic("add");

            writePop("pointer", 1);
            writePush("that", 0);
        } else if (strcmp(tokenizer.curToken, "(") == 0) { // Function call
            // 暂时假设一定是一个 method call
            writePush("pointer", 0);

            process(NULL, -1, 1, "(");
            int expCount = compileExpressionList();
            process(NULL, -1, 1, ")");

            snprintf(funcName, 2 * VAR_BUF_SIZE, "%s.%s", className, token_1);
            writeCall(funcName, expCount + 1);
        } else if (strcmp(tokenizer.curToken, ".") == 0) { 
            process(NULL, -1, 1, ".");
            process(token_2, VAR_BUF_SIZE, 1, tokenizer.curToken);

            int expCount = 0;
            variableInfo *info = lookup(token_1);
            if (info != NULL) { // Method call
                snprintf(funcName, 2 * VAR_BUF_SIZE, "%s.%s", info->type, token_2);
                writePush(info->segment, info->index); // Push object reference
                expCount++;
            } else { // Function call
                snprintf(funcName, 2 * VAR_BUF_SIZE, "%s.%s", token_1, token_2);
            }

            process(NULL, -1, 1, "(");
            expCount += compileExpressionList();
            process(NULL, -1, 1, ")");
            
            writeCall(funcName, expCount);
        } else {
            variableInfo *info = lookup(token_1);
            if (info != NULL) {
                writePush(info->segment, info->index);
            }
        }
    }

    closeNonTerminal("term");
};

int compileExpressionList() {
    openNonTerminal("expressionList");

    int expCount = 0;
    if (strcmp(tokenizer.curToken, ")") != 0)
    {
        expCount++;
        compileExpression();

        while (strcmp(tokenizer.curToken, ",") == 0)
        {
            expCount++;
            process(NULL, -1, 1, ",");
            compileExpression();
        }
    }

    closeNonTerminal("expressionList");

    return expCount;
};

void printTerminal() {
    if (tokenizer.tokenType == INVALID_TOKEN) {
        fprintf(stderr, "Invalid token\n");
        return; 
    } 
    
    printIndent();
    if (tokenizer.curToken[0] == '<') {
        fprintf(outputXMLFile, "<%s> &lt; </%s>\n", terminalTag[tokenizer.tokenType], terminalTag[tokenizer.tokenType]);
    } else if (tokenizer.curToken[0] == '>') {
        fprintf(outputXMLFile, "<%s> &gt; </%s>\n", terminalTag[tokenizer.tokenType], terminalTag[tokenizer.tokenType]);
    } else if (tokenizer.curToken[0] == '&') {
        fprintf(outputXMLFile, "<%s> &amp; </%s>\n", terminalTag[tokenizer.tokenType], terminalTag[tokenizer.tokenType]);
    } else {
        fprintf(outputXMLFile, "<%s> %s </%s>\n", terminalTag[tokenizer.tokenType], tokenizer.curToken, terminalTag[tokenizer.tokenType]);
    }
};

void openNonTerminal(const char *nonTerminal) {
    printIndent();
    fprintf(outputXMLFile, "<%s>\n", nonTerminal);
    indentLevel++;
};

void closeNonTerminal(const char *nonTerminal) {
    indentLevel--;
    printIndent();
    fprintf(outputXMLFile, "</%s>\n", nonTerminal);
};

int process(char* dest, int buf_size, int num, ...) {
    va_list args;
    va_start(args, num);

    for (int i = 0; i < num; i++)
    {   
        char *str = va_arg(args, char *);
        if (strcmp(tokenizer.curToken, str) == 0)
        {
            if (dest != NULL)
            {
                strncpy(dest, tokenizer.curToken, buf_size);
            }
            printTerminal();
            advance(&tokenizer);
            va_end(args);
            return 0; 
        }
    }   
    va_end(args);

    return -1;
};

