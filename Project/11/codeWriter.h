#ifndef CODEWRITER_H
#define CODEWRITER_H

int codeWriterInit(const char *outputFile);

void FreeCodeWriter();

void writePush(const char *segment, int index); 

void writePop(const char *segment, int index);

void writeArithmetic(const char *command);

void writeLabel(const char *label);

void writeGoto(const char *label);

void writeIf(const char *label);

void writeCall(const char *name, int nArgs);

void writeFunction(const char *className, const char *subroutineName, int nLocals);

void writeReturn();

#endif // CODEWRITER_H