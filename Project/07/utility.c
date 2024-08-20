#include "utility.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

char *trimWhitespace(char *str)
{
    // Trim leading whitespace
    while (isspace(*str))
    {
        str++;
    }
    if (*str == '\0') // All spaces
    { 
        return str;
    }

    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
    {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

char *removeComments(char *str)
{
    char *comment = strstr(str, "//");
    if (comment != NULL)
    {
        *comment = '\0';
    }
    return str;
}

void generate_output_path(const char* input_path, const char* output_dir, const char* suffix, char* output_path) {
    // Extract the base name (file name without directory)
    char input_copy[MAX_PATH_LENGTH];
    strncpy(input_copy, input_path, MAX_PATH_LENGTH);
    input_copy[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null-termination
    char* base_name = basename(input_copy);
    
    // Extract the file name without the extension
    char file_name[MAX_PATH_LENGTH];
    strncpy(file_name, base_name, MAX_PATH_LENGTH);
    file_name[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null-termination
    char* dot = strrchr(file_name, '.');
    if (dot) {
        *dot = '\0'; // Remove the extension
    }

    // Generate the output file path
    snprintf(output_path, MAX_PATH_LENGTH, "%s%c%s.%s", output_dir, PATH_SEPARATOR, file_name, suffix);
}