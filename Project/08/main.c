#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h> 

#include "parser.h"
#include "utility.h"
#include "codeWriter.h"

const char *suffix = "vm";
const char *out_dir = "out";
const char *out_suffix = "asm";

// Utility function for safe memory allocation
void *safeMalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Helper function to generate output file path
void generateOutputFilepath(char *output_filepath, const char *input_path, int is_dir)
{
    char filenameCopy[MAX_PATH_LENGTH];
    strcpy(filenameCopy, input_path);

    if (is_dir)
    {
        snprintf(output_filepath, MAX_PATH_LENGTH, "%s%c%s.%s", out_dir, PATH_SEPARATOR, basename(filenameCopy), out_suffix);
    }
    else
    {
        snprintf(output_filepath, MAX_PATH_LENGTH, "%s%c%s", out_dir, PATH_SEPARATOR, basename(filenameCopy));
        char *dot = strrchr(output_filepath, '.');
        if (dot != NULL)
        {
            strcpy(dot + 1, out_suffix);
        }
    }
}

int processFile(CodeWriter *writer, const char *input_file);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: VMTranslator <file.vm>/<directory>\n");
        return EXIT_FAILURE;
    }

    struct stat st;
    if (stat(argv[1], &st) != 0)
    {
        fprintf(stderr, "Failed to get file status\n");
        return EXIT_FAILURE;
    }

    // Generate output file path
    char output_filepath[MAX_PATH_LENGTH];
    generateOutputFilepath(output_filepath, argv[1], S_ISDIR(st.st_mode));
    printf("Output file: %s\n", output_filepath);

    // Initialize code writer
    CodeWriter *writer = safeMalloc(sizeof(CodeWriter));
    if (codeWriterInit(writer, output_filepath) != 0)
    {
        fprintf(stderr, "Failed to initialize code writer\n");
        free(writer);
        return EXIT_FAILURE;
    }

    if (S_ISREG(st.st_mode))
    {
        if (!has_suffix(argv[1], suffix))
        {
            fprintf(stderr, "Invalid input file: %s\n", argv[1]);
            codeWriterFree(writer);
            free(writer);
            return EXIT_FAILURE;
        }
        
        if (processFile(writer, argv[1]) != 0)
        {
            codeWriterFree(writer);
            free(writer);
            return EXIT_FAILURE;
        }
    }
    else if (S_ISDIR(st.st_mode))
    {
        // Try to open the Sys.vm file first
        char sys_vm[MAX_PATH_LENGTH];
        snprintf(sys_vm, MAX_PATH_LENGTH, "%s%cSys.vm", argv[1], PATH_SEPARATOR);
        if (stat(sys_vm, &st) == 0 && S_ISREG(st.st_mode)) {
            writeBootstrap(writer);
            processFile(writer, sys_vm);
        }
        
         
        DIR *dir = opendir(argv[1]);
        if (dir == NULL)
        {
            perror("Failed to open directory");
            codeWriterFree(writer);
            free(writer);
            return EXIT_FAILURE;
        }

        struct dirent *entry;
        char filename[MAX_PATH_LENGTH];
        while ((entry = readdir(dir)) != NULL)
        {
            snprintf(filename, MAX_PATH_LENGTH, "%s%c%s", argv[1], PATH_SEPARATOR, entry->d_name);
            if (stat(filename, &st) == 0 && S_ISREG(st.st_mode) && has_suffix(entry->d_name, suffix))
            {
                if (strcmp(entry->d_name, "Sys.vm") == 0)
                {
                    continue;
                }
                
                if (processFile(writer, filename) != 0)
                {
                    closedir(dir);
                    codeWriterFree(writer);
                    free(writer);
                    return EXIT_FAILURE;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        fprintf(stderr, "Invalid input\n");
        codeWriterFree(writer);
        free(writer);
        return EXIT_FAILURE;
    }
    
    // Free resources
    codeWriterFree(writer);
    free(writer);
    return EXIT_SUCCESS;
}

int processFile(CodeWriter *writer, const char *input_file)
{
    printf("Processing file: %s\n", input_file);
    
    // Initialize parser
    Parser *parser = safeMalloc(sizeof(Parser));
    if (parserInit(parser, input_file) != 0)
    {
        fprintf(stderr, "Failed to initialize parser for file: %s\n", input_file);
        free(parser);
        return -1;
    }

    // Process each command
    while (advance(parser) != -1)
    {
        if (writeCommand(writer, parser) != 0)
        {
            fprintf(stderr, "Failed to write command for file: %s\n", input_file);
            parserFree(parser);
            free(parser);
            return -1;
        }
    }

    // Clean up resources
    parserFree(parser);
    free(parser);

    return 0;
}
