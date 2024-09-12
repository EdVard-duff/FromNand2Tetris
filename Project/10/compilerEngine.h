#ifndef COMPILER_ENGINE_H
#define COMPILER_ENGINE_H

#include "tokenizer.h"

extern Tokenizer tokenizer;

extern FILE *outputFile;

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
void compileVarDec();   

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
void compileExpressionList();

/**
 * This function checks if the current token is one of the expected tokens.
 *
 * @param num The number of possible parsing options
 * @param ... The parsing option list
 */
void process(int num, ...);

void printTerminal();

void openNonTerminal(const char *nonTerminal);

void closeNonTerminal(const char *nonTerminal);

int isOperator(char c);

int isUnaryOp(char c);

int isKeywordConstant(const char *str);

#endif // COMPILER_ENGINE_H