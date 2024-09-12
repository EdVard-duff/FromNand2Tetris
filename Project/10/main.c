#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>

#include "utility.h"
#include "tokenizer.h"
#include "compilerEngine.h"

const char *fileSuffix = "jack";
const char *outDir = "out";
const char *outSuffix = "xml";

FILE *outputFile;

Tokenizer tokenizer;

int processFile(const char *input_file);

void generateOutputFilepath(char *dest, const char *input_path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: JackAnalyzer <file.jack>/<directory>\n");
        return -1;
    }

    struct stat st;
    if (stat(argv[1], &st) != 0)
    {
        fprintf(stderr, "Failed to get file status\n");
        return -1;
    }

    if (S_ISREG(st.st_mode))
    {
        if (!has_suffix(argv[1], fileSuffix))
        {
            fprintf(stderr, "Invalid input file: %s\n", argv[1]);
            return -1;
        }

        if (processFile(argv[1]) != 0)
        {
            return -1;
        }
    }
    else if (S_ISDIR(st.st_mode))
    {
        DIR *dir = opendir(argv[1]);
        if (dir == NULL)
        {
            fprintf(stderr, "Failed to open directory: %s\n", argv[1]);
            return -1;
        }

        struct dirent *entry;
        char filepath[MAX_PATH_LENGTH];
        while ((entry = readdir(dir)) != NULL)
        {
            snprintf(filepath, MAX_PATH_LENGTH, "%s%c%s", argv[1], PATH_SEPARATOR, entry->d_name);
            if (stat(filepath, &st) != 0)
            {
                fprintf(stderr, "Failed to get file status\n");
                closedir(dir);
                return -1;
            }

            if (S_ISREG(st.st_mode) && has_suffix(entry->d_name, fileSuffix))
            {
                if (processFile(filepath) != 0)
                {
                    closedir(dir);
                    return -1;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        fprintf(stderr, "Invalid input file: %s\n", argv[1]);
        return -1;
    }

    return 0;
}

int processFile(const char *input_file)
{
    printf("Processing file: %s\n", input_file);
    if (tokenizerInit(&tokenizer, input_file) == -1)
    {
        return -1;
    }
    
    char outputFilepath[MAX_PATH_LENGTH];
    generateOutputFilepath(outputFilepath, input_file);
    outputFile = fopen(outputFilepath, "w");
    // outputFile = stdout;
    if (outputFile == NULL)
    {
        perror("Failed to open output file");
        
        return -1;
    }
    printf("Output file: %s\n", outputFilepath);

    compileClass();

    // Free resources
    freeTokenizer(&tokenizer);
    fclose(outputFile);
    return 0;
}

void generateOutputFilepath(char *dest, const char *input_path)
{
    char filenameCopy[MAX_PATH_LENGTH];
    strcpy(filenameCopy, input_path);

    snprintf(dest, MAX_PATH_LENGTH, "%s%c%s", outDir, PATH_SEPARATOR, basename(filenameCopy));
    char *dot = strrchr(dest, '.');
    if (dot != NULL)
    {
        strcpy(dot + 1, outSuffix);
    }
}