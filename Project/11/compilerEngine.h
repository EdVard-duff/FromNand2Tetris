#ifndef COMPILER_ENGINE_H
#define COMPILER_ENGINE_H

#include "tokenizer.h"

#define VAR_BUF_SIZE 32

// Simply print the token list
void compileTokenList();

// 'class' className '{' classVarDec* subroutineDec* '}'
void compileClass();

// ('static' | 'field') type varName (, varName)* ;
void compileClassVarDec();

// ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
void compileSubroutine();

// ((type varName) (',' type varName)*)?
void compileParameterList();

// '{' varDec* statements '}'
void compileSubroutineBody();

// 'var' type varName (, varName)* ';'
int compileVarDec();   

// statement*
void compileStatements();

// 'let' VarName ('[' expression ']')? '=' expression ';'
void compileLet();

// 'if' '(' expression ')' '{' statements '}' ( 'else' '{' statements '}' )?
void compileIf();

// 'while' '(' expression ')' '{' statements '}'
void compileWhile();

// 'do' subroutineCall ';'
void compileDo();

// 'return' expression? ';'
void compileReturn();

// term (op term)*
void compileExpression();

// integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
// subroutineCall: subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
void compileTerm();

// (expression (, expression)*)?
int compileExpressionList();

/**
 * This function checks if the current token is one of the expected tokens. If true, the current token will be consumed, otherwise the function will return -1 and the current token will not be consumed.
 * 
 * @param dest The destination string. If dest is not NULL, current token will be copied to dest.
 * @param buf_size The size of the destination string
 * @param num The number of possible parsing options
 * @param ... The parsing option list
 * 
 * @return -1 if the current token is not one of the expected tokens, 0 otherwise.
 */
int process(char* dest, int buf_size, int num, ...);

void printTerminal();

void openNonTerminal(const char *nonTerminal);

void closeNonTerminal(const char *nonTerminal);

#endif // COMPILER_ENGINE_H