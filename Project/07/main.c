#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: VMTranslator <filename.vm>\n");
    } 

    Parser* parser = (Parser *)malloc(sizeof(Parser));
    if (parser == NULL) {
        fprintf(stderr, "Failed to allocate memory for parser\n");
        exit(EXIT_FAILURE);
    }
    parserInit(parser, argv[1]);

    while (advance(parser) != -1) {
    
    }
    return 0;
}