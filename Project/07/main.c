#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "utility.h"
#include "codeWriter.h"

// Utility function for safe memory allocation
void* safeMalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: VMTranslator <filename.vm>\n");
        return EXIT_FAILURE;
    } 
    printf("Input file: %s\n", argv[1]);

    // Initialize parser
    Parser* parser = safeMalloc(sizeof(Parser));
    parserInit(parser, argv[1]);

    // Generate output file path (using a stack-allocated buffer)
    char output_filepath[MAX_PATH_LENGTH];
    generate_output_path(argv[1], "out", "asm", output_filepath);
    printf("Output file: %s\n", output_filepath);

    // Initialize code writer
    CodeWriter* writer = safeMalloc(sizeof(CodeWriter));
    codeWriterInit(writer, output_filepath);

    // Process each command
    while (advance(parser) != -1) {
        writeCommand(writer, parser);
    }

    // Clean up resources
    parserFree(parser);
    free(parser);
    codeWriterFree(writer);
    free(writer);

    return EXIT_SUCCESS;
}



