#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "compilerEngine.h"
#include "tokenizer.h"

static int indentLevel = 0; 

static char *terminalTag[] = {
    [KEYWORD] = "keyword",
    [SYMBOL] = "symbol",
    [IDENTIFIER] = "identifier",
    [INT_CONST] = "integerConstant",
    [STRING_CONST] = "stringConstant"
}; 

static inline void printIndent() {
    for (int i = 0; i < indentLevel; i++)
    {
        fprintf(outputFile, "  ");
    }
};

void compileTokenList() {
    fprintf(outputFile, "<tokens>\n");
    indentLevel++;

    while (advance(&tokenizer) != -1)
    {
        printTerminal();
    }
    
    indentLevel--;
    fprintf(outputFile, "</tokens>\n");
}

void compileClass() {
    advance(&tokenizer);

    openNonTerminal("class");

    process(1, "class");
    process(1, tokenizer.curToken); // className
    process(1, "{");

    while (strcmp(tokenizer.curToken, "static") == 0 || strcmp(tokenizer.curToken, "field") == 0)
    {
        compileClassVarDec();
    }

    while (strcmp(tokenizer.curToken, "constructor") == 0 || strcmp(tokenizer.curToken, "function") == 0 || strcmp(tokenizer.curToken, "method") == 0)
    {
        compileSubroutine();
    }
    
    process(1, "}");
    
    closeNonTerminal("class");
};

void compileClassVarDec() {
    openNonTerminal("classVarDec");

    process(2, "static", "field");
    process(4, "int", "char", "boolean", tokenizer.curToken); // type
    process(1, tokenizer.curToken); // varName

    while (strcmp(tokenizer.curToken, ",") == 0)
    {
        process(1, ",");
        process(1, tokenizer.curToken); // varName
    }

    process(1, ";");

    closeNonTerminal("classVarDec");
};

void compileSubroutine() {
    openNonTerminal("subroutineDec");
    
    process(3, "constructor", "function", "method");
    process(5, "void", "int", "char", "boolean", tokenizer.curToken); // Returntype
    process(1, tokenizer.curToken); // subroutineName

    // Parameter list
    process(1, "(");
    compileParameterList();
    process(1, ")");

    compileSubroutineBody();

    closeNonTerminal("subroutineDec");
};

void compileParameterList() {
    openNonTerminal("parameterList");

    if (strcmp(tokenizer.curToken, ")") != 0)
    {
        process(4, "int", "char", "boolean", tokenizer.curToken); // type
        process(1, tokenizer.curToken); // varName

        while (strcmp(tokenizer.curToken, ",") == 0)
        {
            process(1, ",");
            process(4, "int", "char", "boolean", tokenizer.curToken); // type
            process(1, tokenizer.curToken); // varName
        }
    }

    closeNonTerminal("parameterList");
};

void compileSubroutineBody() {
    openNonTerminal("subroutineBody");

    process(1, "{");

    while (strcmp(tokenizer.curToken, "var") == 0)
    {
        compileVarDec();
    }
    compileStatements();

    process(1, "}");

    closeNonTerminal("subroutineBody");
};

void compileVarDec() {
    openNonTerminal("varDec");

    process(1, "var");
    process(4, "int", "char", "boolean", tokenizer.curToken); // type
    process(1, tokenizer.curToken); // varName

    while (strcmp(tokenizer.curToken, ",") == 0)
    {
        process(1, ",");
        process(1, tokenizer.curToken);
    }

    process(1, ";");

    closeNonTerminal("varDec");
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

    process(1, "let");
    process(1, tokenizer.curToken); // varName

    if (strcmp(tokenizer.curToken, "[") == 0)
    {
        process(1, "[");
        compileExpression();
        process(1, "]");
    }

    process(1, "=");
    compileExpression();
    process(1, ";");

    closeNonTerminal("letStatement");
};

void compileIf() {
    openNonTerminal("ifStatement");

    process(1, "if");
    process(1, "(");
    compileExpression();
    process(1, ")");

    process(1, "{");
    compileStatements();
    process(1, "}");

    if (strcmp(tokenizer.curToken, "else") == 0)
    {
        process(1, "else");
        process(1, "{");
        compileStatements();
        process(1, "}");
    }

    closeNonTerminal("ifStatement");
};

void compileWhile() {
    openNonTerminal("whileStatement");

    process(1, "while");
    process(1, "(");
    compileExpression();
    process(1, ")");

    process(1, "{");
    compileStatements();
    process(1, "}");

    closeNonTerminal("whileStatement");
};

void compileDo() {
    openNonTerminal("doStatement");
    
    process(1, "do");
    // subroutineCall
    process(1, tokenizer.curToken);
    if (strcmp(tokenizer.curToken, ".") == 0)
    {
        process(1, ".");
        process(1, tokenizer.curToken); // subroutineName
    }
    process(1, "(");
    compileExpressionList();
    process(1, ")");

    process(1, ";");
    closeNonTerminal("doStatement");
};

void compileReturn() {
    openNonTerminal("returnStatement");

    process(1, "return");
    if (strcmp(tokenizer.curToken, ";") != 0)
    {
        compileExpression();
    }
    process(1, ";");

    closeNonTerminal("returnStatement");
};

void compileExpression() {
    openNonTerminal("expression");

    compileTerm();

    while (isOperator(tokenizer.curToken[0]))
    {
        process(1, tokenizer.curToken);
        compileTerm();
    }

    closeNonTerminal("expression");
};

void compileTerm() {
    openNonTerminal("term");

    if (tokenizer.tokenType == INT_CONST || tokenizer.tokenType == STRING_CONST || isKeywordConstant(tokenizer.curToken))
    {
        process(1, tokenizer.curToken);
    }
    else if (isUnaryOp(tokenizer.curToken[0]))
    {
        process(1, tokenizer.curToken);
        compileTerm();
    }
    else if (strcmp(tokenizer.curToken, "(") == 0)
    {
        process(1, "(");
        compileExpression();
        process(1, ")");
    }
    else if (tokenizer.tokenType == IDENTIFIER)
    {
        process(1, tokenizer.curToken);

        if (strcmp(tokenizer.curToken, "[") == 0)
        {
            process(1, "[");
            compileExpression();
            process(1, "]");
        }
        else if (strcmp(tokenizer.curToken, "(") == 0)
        {
            process(1, "(");
            compileExpressionList();
            process(1, ")");
        }
        else if (strcmp(tokenizer.curToken, ".") == 0)
        {
            process(1, ".");
            process(1, tokenizer.curToken); // subroutineName
            process(1, "(");
            compileExpressionList();
            process(1, ")");
        }
    }

    closeNonTerminal("term");
};

void compileExpressionList() {
    openNonTerminal("expressionList");

    if (strcmp(tokenizer.curToken, ")") != 0)
    {
        compileExpression();

        while (strcmp(tokenizer.curToken, ",") == 0)
        {
            process(1, ",");
            compileExpression();
        }
    }

    closeNonTerminal("expressionList");
};

void process(int num, ...) {
    char *argList[num];
    va_list args;
    va_start(args, num);

    for (int i = 0; i < num; i++)
    {   
        const char *str = va_arg(args, const char *);
        argList[i] = (char *)str;
        if (strcmp(tokenizer.curToken, str) == 0)
        {
            printTerminal();
            advance(&tokenizer);
            va_end(args);
            return;
        }
    }   
    va_end(args);

    fprintf(stderr, "Expected token: ");
    for (int i = 0; i < num; i++)
    {
        fprintf(stderr, "%s ", argList[i]);
    }
    fprintf(stderr, ", got: %s\n", tokenizer.curToken);
    exit(-1);
};

void printTerminal() {
    if (tokenizer.tokenType == INVALID_TOKEN) {
        fprintf(stderr, "Invalid token\n");
    }
    else {
        printIndent();
        fprintf(outputFile, "<%s> %s </%s>\n", terminalTag[tokenizer.tokenType], tokenizer.curToken, terminalTag[tokenizer.tokenType]);
    }
};

void openNonTerminal(const char *nonTerminal) {
    printIndent();
    fprintf(outputFile, "<%s>\n", nonTerminal);
    indentLevel++;
};

void closeNonTerminal(const char *nonTerminal) {
    indentLevel--;
    printIndent();
    fprintf(outputFile, "</%s>\n", nonTerminal);
};

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<' || c == '>' || c == '=';
};

int isUnaryOp(char c) {
    return c == '-' || c == '~';
};

int isKeywordConstant(const char *str) {
    return strcmp(str, "true") == 0 || strcmp(str, "false") == 0 || strcmp(str, "null") == 0 || strcmp(str, "this") == 0;
};